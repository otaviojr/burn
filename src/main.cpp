#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QCommandLineParser>
#include <QQmlContext>
#include <QSettings>
#include <QQuickStyle>
#include <QIcon>

#include "gerber_renderer.h"
#include "gerber_renderer_mirror.h"

#include "wifi_model.h"

struct Options {
   bool cli;
};

static Options parseOptions() {
   Options opts = {};
   QCommandLineParser parser;
   QCommandLineOption cliOption("cli", "Start in client mode.");
   parser.addOption(cliOption);
   parser.process(*qApp);
   opts.cli = parser.isSet(cliOption);
   return opts;
}

int main(int argc, char *argv[])
{
    qputenv("QT_IM_MODULE", QByteArray("qtvirtualkeyboard"));

    QGuiApplication::setApplicationName("Burn");
    QGuiApplication::setOrganizationName("Delta3D");

    QGuiApplication app(argc, argv);

    QIcon::setThemeName("burn");
    QIcon::setThemeSearchPaths(QStringList() << ":" << ":/resources" << ":/resources/icons");

    QQuickStyle::setStyle("material");

    qmlRegisterType<GerberRenderer>("GerberRenderer", 1, 0, "Gerber");
    qmlRegisterType<GerberRendererMirror>("GerberRendererMirror", 1, 0, "GerberMirror");
    qmlRegisterType<WifiModel>("WifiModel", 1, 0, "WifiModel");

    QQmlApplicationEngine engine;

    auto options = parseOptions();
    if (options.cli) {
      engine.load(QUrl("qrc:/resources/main_cli.qml"));
    } else {
      engine.load(QUrl("qrc:/resources/BurnButton.qml"));
      engine.load(QUrl("qrc:/resources/DoubleSpinBox.qml"));
      engine.load(QUrl("qrc:/resources/pages/StartDialog.qml"));
      engine.load(QUrl("qrc:/resources/pages/FileSelection.qml"));
      engine.load(QUrl("qrc:/resources/pages/FolderSelection.qml"));
      engine.load(QUrl("qrc:/resources/pages/ConfigDialog.qml"));
      engine.load(QUrl("qrc:/resources/pages/GerberConfigDialog.qml"));
      engine.load(QUrl("qrc:/resources/pages/ExecutingDialog.qml"));
      engine.load(QUrl("qrc:/resources/main.qml"));
    }
    if (engine.rootObjects().isEmpty())
        return -1;

    //QQmlComponent component(&engine, QUrl::fromLocalFile("MyItem.qml"));
    //QObject *object = component.create();
    //...
    //delete object;

    return app.exec();
}
