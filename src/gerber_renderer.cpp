#include <iostream>
#include <QPainter>
#include <QSGSimpleTextureNode>
#include <cairo/cairo.h>

#include "gerber/gerbv.h"

#include "gerber_renderer.h"

GerberRenderer::GerberRenderer(QQuickItem *parent)
    : QQuickItem(parent)
{
    setFlag(QQuickItem::ItemHasContents);
    this->m_color = QColor(QLatin1String("red"));
}

void GerberRenderer::closeProject()
{
    if(this->gerbv_project){
        gerbv_destroy_project(this->gerbv_project);
        this->gerbv_project = NULL;
        this->update();
        emit projectChanged(true);
    }
}

bool GerberRenderer::openProject(const QString &fileName)
{
    if(this->gerbv_project)
        gerbv_destroy_project(this->gerbv_project);


    this->gerbv_project = gerbv_create_project();
    this->gerbv_project->background = (GerbvColor){0.5, 0.5, 0.5, 1.0};
    gerbv_open_layer_from_filename_with_color(this->gerbv_project, (char*)fileName.toUtf8().constData(), 0.0, 0.0, 0.0, 1.0);
    if(this->gerbv_project->file[0]){
        this->update();
        emit projectChanged(true);
        return TRUE;
    }
    gerbv_destroy_project(this->gerbv_project);
    this->gerbv_project = NULL;
    return FALSE;
}

bool GerberRenderer::hasProject()
{
    return (this->gerbv_project && this->gerbv_project->file[0]);
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

QSGNode *GerberRenderer::updatePaintNode(QSGNode * oldNode, QQuickItem::UpdatePaintNodeData * nodeData)
{
    auto const w(width()), h(height());

    QImage image(w, h, QImage::Format_RGBA8888);
    image.fill(QColor("transparent"));

    auto const surf(cairo_image_surface_create_for_data(image.bits(),
      CAIRO_FORMAT_RGB24, w, h, image.bytesPerLine()));

    auto const cr(cairo_create(surf));

    if(this->gerbv_project){
        gerbv_render_info_t renderInfo = {1.0, 1.0, 0.0, 0.0, GERBV_RENDER_TYPE_CAIRO_HIGH_QUALITY, (int)w, (int)h};
        gerbv_render_zoom_to_fit_display (this->gerbv_project, &renderInfo);

        //cairo_matrix_t matrix;
        //cairo_matrix_init_translate (&matrix, 0, 0);
        //cairo_transform(cr, &matrix);

        for(int i = gerbv_project->last_loaded; i >= 0; i--) {
            if (gerbv_project->file[i]) {
                 gerbv_render_layer_to_cairo_target (cr, this->gerbv_project->file[i], &renderInfo);
    	    }
        }
    }

    QSGSimpleTextureNode *node = static_cast<QSGSimpleTextureNode *>(oldNode);
    if (!node) {
        node = new QSGSimpleTextureNode();
    } else {
        delete node->texture();
    }

    QSGTexture *texture = window()->createTextureFromImage(image);
    node->setTexture(texture);

    cairo_surface_destroy(surf);
    cairo_destroy(cr);

    node->setRect(boundingRect());
    return node;
}
