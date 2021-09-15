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
    Q_PROPERTY(bool hasProject READ hasProject NOTIFY projectChanged)
    Q_PROPERTY(bool negative READ negative WRITE setNegative NOTIFY negativeChanged)
    Q_PROPERTY(bool realSize READ realSize WRITE setRealSize NOTIFY realSizeChanged)
    Q_PROPERTY(float rotate READ rotate WRITE setRotate NOTIFY rotateChanged)
    Q_PROPERTY(float dpix READ dpix WRITE setDpix NOTIFY dpixChanged)
    Q_PROPERTY(float dpiy READ dpiy WRITE setDpiy NOTIFY dpiyChanged)
    Q_PROPERTY(GerberRendererMirror::GerberMirror mirror READ mirror WRITE setMirror NOTIFY mirrorChanged)
    QML_ELEMENT

public:
    GerberRenderer(QQuickItem *parent = nullptr);
    ~GerberRenderer();

    QStringList fileNames() const;
    bool negative() const;
    void setNegative(const bool &negative);
    GerberRendererMirror::GerberMirror mirror() const;
    void setMirror(const GerberRendererMirror::GerberMirror &mirror);
    float rotate() const;
    void setRotate(const float &rotate);

    float dpix() const;
    void setDpix(const float dpix);
    float dpiy() const;
    void setDpiy(const float dpiy);

    bool realSize() const;
    void setRealSize(const bool &negative);

    QSGNode *updatePaintNode(QSGNode *, UpdatePaintNodeData *) override;

    Q_INVOKABLE bool newProject();
    Q_INVOKABLE void closeProject();
    Q_INVOKABLE bool hasProject();
    Q_INVOKABLE bool addFileToProject(const QString &fileName);
private:
    bool m_realSize;
    gerbv_project_t * gerbv_project;
    float m_dpix;
    float m_dpiy;

signals:
    void fileNamesChanged(const QStringList fileName);
    void realSizeChanged(const bool value);
    void projectChanged(const bool value);
    void rotateChanged(const float rotate);
    void mirrorChanged(const GerberRendererMirror::GerberMirror mirror);
    void negativeChanged(const bool negative);
    void dpixChanged(const float dpix);
    void dpiyChanged(const float dpiy);
};

#endif
