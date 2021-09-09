#ifndef __GERBER_RENDERER_H__
#define __GERBER_RENDERER_H__

#include <QtQuick/QQuickItem>
#include <QColor>

#include "gerber/gerbv.h"

class GerberRenderer : public QQuickItem
{
    Q_OBJECT
    Q_PROPERTY(QColor color READ color WRITE setColor)
    Q_PROPERTY(QColor backgroundColor READ backgroundColor WRITE setBackgroundColor)
    Q_PROPERTY(bool hasProject READ hasProject NOTIFY projectChanged)
    QML_ELEMENT

public:
    GerberRenderer(QQuickItem *parent = 0);

    QColor color() const;
    void setColor(const QColor &color);
    QColor backgroundColor() const;
    void setBackgroundColor(const QColor &color);

    QSGNode *updatePaintNode(QSGNode *, UpdatePaintNodeData *) override;

    Q_INVOKABLE bool openProject(const QString &fileName);
    Q_INVOKABLE void closeProject();
    Q_INVOKABLE bool hasProject();
private:
    QColor m_color;
    QColor m_backgroundColor;
    gerbv_project_t * gerbv_project;

signals:
    void projectChanged(const bool value);
};

#endif

