#include <iostream>
#include <QGuiApplication>
#include <QPainter>
#include <QScreen>
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
        emit negativeChanged(false);
        emit mirrorChanged(GerberRendererMirror::GerberMirror::NONE);
        emit rotateChanged(0);
    }
}

bool GerberRenderer::openProject(const QString &fileName)
{
    if(this->gerbv_project)
        gerbv_destroy_project(this->gerbv_project);


    this->gerbv_project = gerbv_create_project();
    this->gerbv_project->background = (GerbvColor){1.0, 1.0, 1.0, 1.0};
    gerbv_open_layer_from_filename_with_color(this->gerbv_project, (char*)fileName.toUtf8().constData(), 0.0, 0.0, 0.0, 1.0);
    if(this->gerbv_project->file[0]){
        this->update();
        emit projectChanged(true);
        emit negativeChanged(false);
        emit mirrorChanged(GerberRendererMirror::GerberMirror::NONE);
        emit rotateChanged(0);
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

bool GerberRenderer::negative() const {
    return (this->gerbv_project && this->gerbv_project->file[0] && this->gerbv_project->file[0]->transform.inverted);
}

void GerberRenderer::setNegative(const bool &negative) {
    if(this->gerbv_project && this->gerbv_project->file[0]){
        this->gerbv_project->file[0]->transform.inverted = negative;
        this->update();
        emit negativeChanged(negative);
    }
}

GerberRendererMirror::GerberMirror GerberRenderer::mirror() const {
    if(this->gerbv_project && this->gerbv_project->file[0]){
        if(this->gerbv_project->file[0]->transform.mirrorAroundX && !this->gerbv_project->file[0]->transform.mirrorAroundY){
            return GerberRendererMirror::GerberMirror::X;
        } else if(!this->gerbv_project->file[0]->transform.mirrorAroundX && this->gerbv_project->file[0]->transform.mirrorAroundY){
            return GerberRendererMirror::GerberMirror::Y;
        } else if(this->gerbv_project->file[0]->transform.mirrorAroundX && this->gerbv_project->file[0]->transform.mirrorAroundY){
            return GerberRendererMirror::GerberMirror::XY;
        }
    }

    return GerberRendererMirror::GerberMirror::NONE;
}

void GerberRenderer::setMirror(const GerberRendererMirror::GerberMirror &mirror) {
    if(this->gerbv_project && this->gerbv_project->file[0]){
        if(mirror == GerberRendererMirror::GerberMirror::X){
            this->gerbv_project->file[0]->transform.mirrorAroundX = true;
            this->gerbv_project->file[0]->transform.mirrorAroundY = false;
        } else if(mirror == GerberRendererMirror::GerberMirror::Y){
            this->gerbv_project->file[0]->transform.mirrorAroundX = false;
            this->gerbv_project->file[0]->transform.mirrorAroundY = true;
        } else if(mirror == GerberRendererMirror::GerberMirror::XY){
            this->gerbv_project->file[0]->transform.mirrorAroundX = true;
            this->gerbv_project->file[0]->transform.mirrorAroundY = true;
        } else {
            this->gerbv_project->file[0]->transform.mirrorAroundX = false;
            this->gerbv_project->file[0]->transform.mirrorAroundY = false;
        }
        this->update();
        emit mirrorChanged(mirror);
    }
}

float GerberRenderer::rotate() const {
    if(this->gerbv_project && this->gerbv_project->file[0]){
        return this->gerbv_project->file[0]->transform.rotation / (M_PI / 180);
    }
    return 0;
}
void GerberRenderer::setRotate(const float &rotate){
    if(this->gerbv_project && this->gerbv_project->file[0]){
        this->gerbv_project->file[0]->transform.rotation = rotate * (M_PI / 180);
        this->update();
        emit rotateChanged(rotate);
    }
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

        /*
         * TODO: Will be used on the second display to display the real scale version of the greber
         * QScreen *screen = QGuiApplication::primaryScreen();
         * gerbv_render_zoom_real_size (this->gerbv_project, &renderInfo, screen->physicalDotsPerInchX(), screen->physicalDotsPerInchY());
         * gerbv_render_translate_to_fit_display(this->gerbv_project, &renderInfo);
         */

         gerbv_render_zoom_to_fit_display (this->gerbv_project, &renderInfo);
         gerbv_render_all_layers_to_cairo_target(this->gerbv_project, cr, &renderInfo);
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
