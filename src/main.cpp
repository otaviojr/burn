#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QSettings>
#include <QQuickStyle>
#include <QIcon>

#include "gerber_renderer.h"

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

    QQmlApplicationEngine engine;

    engine.load(QUrl("qrc:/resources/main.qml"));
    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}
