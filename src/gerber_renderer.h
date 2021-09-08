#ifndef __GERBER_RENDERER_H__
#define __GERBER_RENDERER_H__

#include <QtQuick/QQuickItem>
#include <QColor>

class GerberRenderer : public QQuickItem
{
    Q_OBJECT
    Q_PROPERTY(QString fileName READ fileName WRITE setFileName)
    Q_PROPERTY(QColor color READ color WRITE setColor)
    Q_PROPERTY(QColor backgroundColor READ backgroundColor WRITE setBackgroundColor)
    QML_ELEMENT

public:
    GerberRenderer(QQuickItem *parent = 0);

    QString fileName() const;
    void setFileName(const QString &name);
    QColor color() const;
    void setColor(const QColor &color);
    QColor backgroundColor() const;
    void setBackgroundColor(const QColor &color);

    QSGNode *updatePaintNode(QSGNode *, UpdatePaintNodeData *);

private:
    QString m_fileName;
    QColor m_color;
    QColor m_backgroundColor;
};

#endif

