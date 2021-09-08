#include "gerber_renderer.h"
#include <QPainter>

GerberRenderer::GerberRenderer(QQuickItem *parent)
    : QQuickPaintedItem(parent)
{
    this->m_color = QColor(QLatin1String("red"));
}

QString GerberRenderer::fileName() const
{
    return m_fileName;
}

void GerberRenderer::setFileName(const QString &name)
{
    m_fileName = name;
}

QColor GerberRenderer::color() const
{
    return m_color;
}

void GerberRenderer::setColor(const QColor &color)
{
    m_color = color;
}

QColor GerberRenderer::backgroundColor() const
{
    return m_color;
}

void GerberRenderer::setBackgroundColor(const QColor &color)
{
    m_backgroundColor = color;
}

void GerberRenderer::paint(QPainter *painter)
{
    QPen pen(m_color, 2);
    painter->setPen(pen);
    painter->setRenderHints(QPainter::Antialiasing, true);
    painter->drawPie(boundingRect().adjusted(1, 1, -1, -1), 90 * 16, 290 * 16);
}

