#ifndef __GERBER_RENDERER_H__
#define __GERBER_RENDERER_H__

#include <QtQuick/QQuickItem>
#include <QColor>

#include "gerber_renderer_mirror.h"
#include "gerber/gerbv.h"

class GerberRenderer : public QQuickItem
{
    Q_OBJECT
    Q_PROPERTY(QStringList fileNames READ fileNames NOTIFY fileNamesChanged)
    Q_PROPERTY(QColor color READ color WRITE setColor)
    Q_PROPERTY(QColor backgroundColor READ backgroundColor WRITE setBackgroundColor)
    Q_PROPERTY(bool hasProject READ hasProject NOTIFY projectChanged)
    Q_PROPERTY(bool negative READ negative WRITE setNegative NOTIFY negativeChanged)
    Q_PROPERTY(GerberRendererMirror::GerberMirror mirror READ mirror WRITE setMirror NOTIFY mirrorChanged)
    Q_PROPERTY(float rotate READ rotate WRITE setRotate NOTIFY rotateChanged)
    Q_PROPERTY(bool realSize READ realSize WRITE setRealSize NOTIFY realSizeChanged)
    QML_ELEMENT

public:
    GerberRenderer(QQuickItem *parent = 0);

    QStringList fileNames() const;
    QColor color() const;
    void setColor(const QColor &color);
    QColor backgroundColor() const;
    void setBackgroundColor(const QColor &color);

    bool negative() const;
    void setNegative(const bool &negative);
    GerberRendererMirror::GerberMirror mirror() const;
    void setMirror(const GerberRendererMirror::GerberMirror &mirror);
    float rotate() const;
    void setRotate(const float &rotate);

    bool realSize() const;
    void setRealSize(const bool &negative);

    QSGNode *updatePaintNode(QSGNode *, UpdatePaintNodeData *) override;

    Q_INVOKABLE bool newProject();
    Q_INVOKABLE void closeProject();
    Q_INVOKABLE bool hasProject();
    Q_INVOKABLE bool addFileToProject(const QString &fileName);
private:
    QColor m_color;
    QColor m_backgroundColor;
    bool m_realSize;
    gerbv_project_t * gerbv_project;

signals:
    void fileNamesChanged(const QStringList fileName);
    void realSizeChanged(const bool value);
    void projectChanged(const bool value);
    void rotateChanged(const float rotate);
    void mirrorChanged(const GerberRendererMirror::GerberMirror mirror);
    void negativeChanged(const bool negative);
};

#endif
