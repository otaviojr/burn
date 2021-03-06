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
    this->gerbv_project = NULL;
    this->m_dpix = 0;
    this->m_dpiy = 0;
    this->m_realSize = false;
    setFlag(QQuickItem::ItemHasContents);
}

GerberRenderer::~GerberRenderer()
{
    this->closeProject();
}

void GerberRenderer::closeProject()
{
    if(this->gerbv_project){
        gerbv_destroy_project(this->gerbv_project);
        this->gerbv_project = NULL;
        this->update();
        emit fileNamesChanged(QStringList());
        emit projectChanged(true);
        emit negativeChanged(false);
        emit mirrorChanged(GerberRendererMirror::GerberMirror::NONE);
        emit rotateChanged(0);
    }
}

bool GerberRenderer::newProject()
{
    if(this->gerbv_project)
        gerbv_destroy_project(this->gerbv_project);

    this->gerbv_project = gerbv_create_project();
    emit negativeChanged(false);
    emit mirrorChanged(GerberRendererMirror::GerberMirror::NONE);
    emit rotateChanged(0);

    return TRUE;
}

bool GerberRenderer::addFileToProject(const QString &fileName)
{
    if(!this->gerbv_project) return FALSE;

    int layers = this->gerbv_project->last_loaded;

    this->gerbv_project->background = (GerbvColor){1.0, 1.0, 1.0, 1.0};
    gerbv_open_layer_from_filename_with_color(this->gerbv_project, (char*)fileName.toUtf8().constData(), 0.0, 0.0, 0.0, 1.0);
    if(layers < this->gerbv_project->last_loaded && this->gerbv_project->file[this->gerbv_project->last_loaded]){
        this->update();
        emit fileNamesChanged(this->fileNames());
        emit projectChanged(true);
        return TRUE;
    }
    return FALSE;
}

QStringList GerberRenderer::fileNames() const{
    QStringList list;
    if(this->gerbv_project->last_loaded > 0){
        for(int i = 0; i <= this->gerbv_project->last_loaded; i++){
            list << QString::fromUtf8(this->gerbv_project->file[i]->fullPathname);
        }
    }
    return list;
}


bool GerberRenderer::hasProject()
{
    return (this->gerbv_project && this->gerbv_project->last_loaded > 0);
}

float GerberRenderer::dpix() const
{
    return this->m_dpix;
}

void GerberRenderer::setDpix(const float dpix)
{
    this->m_dpix = dpix;
    this->update();
    emit dpixChanged(dpix);

}

float GerberRenderer::dpiy() const
{
    return this->m_dpiy;
}

void GerberRenderer::setDpiy(const float dpiy)
{
    this->m_dpiy = dpiy;
    this->update();
    emit dpiyChanged(dpiy);
}

bool GerberRenderer::realSize() const {
    return m_realSize;
}

void GerberRenderer::setRealSize(const bool &realSize) {
    m_realSize = realSize;
    this->update();
    emit realSizeChanged(realSize);
}

bool GerberRenderer::negative() const {
    return (this->gerbv_project && this->gerbv_project->file[0] && this->gerbv_project->file[0]->transform.inverted);
}

void GerberRenderer::setNegative(const bool &negative) {
    if(this->gerbv_project){
        for(int i = 0; i <= this->gerbv_project->last_loaded; i++){
            this->gerbv_project->file[i]->transform.inverted = negative;
        }
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
    if(this->gerbv_project){
        if(mirror == GerberRendererMirror::GerberMirror::X){
            for(int i = 0; i <= this->gerbv_project->last_loaded; i++){
                this->gerbv_project->file[i]->transform.mirrorAroundX = true;
                this->gerbv_project->file[i]->transform.mirrorAroundY = false;
            }
        } else if(mirror == GerberRendererMirror::GerberMirror::Y){
            for(int i = 0; i <= this->gerbv_project->last_loaded; i++){
                this->gerbv_project->file[i]->transform.mirrorAroundX = false;
                this->gerbv_project->file[i]->transform.mirrorAroundY = true;
            }
        } else if(mirror == GerberRendererMirror::GerberMirror::XY){
            for(int i = 0; i <= this->gerbv_project->last_loaded; i++){
                this->gerbv_project->file[i]->transform.mirrorAroundX = true;
                this->gerbv_project->file[i]->transform.mirrorAroundY = true;
            }
        } else {
            for(int i = 0; i <= this->gerbv_project->last_loaded; i++){
                this->gerbv_project->file[i]->transform.mirrorAroundX = false;
                this->gerbv_project->file[i]->transform.mirrorAroundY = false;
            }
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
    if(this->gerbv_project){
        for(int i = 0; i <= this->gerbv_project->last_loaded; i++){
            this->gerbv_project->file[i]->transform.rotation = rotate * (M_PI / 180);
        }
        this->update();
        emit rotateChanged(rotate);
    }
}


QSGNode *GerberRenderer::updatePaintNode(QSGNode * oldNode, QQuickItem::UpdatePaintNodeData * nodeData)
{
    auto const w(width()), h(height());

    QImage image(w, h, QImage::Format_RGBA8888);
    image.fill(QColor("transparent"));

    if(this->gerbv_project && this->gerbv_project->last_loaded > 0){

        auto const surf(cairo_image_surface_create_for_data(image.bits(),
          CAIRO_FORMAT_RGB24, w, h, image.bytesPerLine()));

        auto const cr(cairo_create(surf));

        gerbv_render_info_t renderInfo = {1.0, 1.0, 0.0, 0.0, GERBV_RENDER_TYPE_CAIRO_HIGH_QUALITY, (int)w, (int)h};

        /*
         * TODO: Will be used on the second display to display the real scale version of the greber
         * QScreen *screen = QGuiApplication::primaryScreen();
         * gerbv_render_zoom_real_size (this->gerbv_project, &renderInfo, screen->physicalDotsPerInchX(), screen->physicalDotsPerInchY());
         * gerbv_render_translate_to_fit_display(this->gerbv_project, &renderInfo);
         */

         if(this->m_realSize && this->m_dpix > 0 && this->m_dpiy > 0){
             gerbv_render_zoom_real_size (this->gerbv_project, &renderInfo, this->m_dpix, this->m_dpiy);
             gerbv_render_translate_to_fit_display(this->gerbv_project, &renderInfo);
             if(this->gerbv_project->file[0]) this->gerbv_project->file[0]->isVisible = false;
         } else {
             gerbv_render_zoom_to_fit_display (this->gerbv_project, &renderInfo);
         }

        gerbv_render_all_layers_to_cairo_target_for_vector_output(this->gerbv_project, cr, &renderInfo);

        cairo_surface_destroy(surf);
        cairo_destroy(cr);
    }

    QSGSimpleTextureNode *node = static_cast<QSGSimpleTextureNode *>(oldNode);
    if (!node) {
        node = new QSGSimpleTextureNode();
    } else {
        delete node->texture();
    }

    QSGTexture *texture = window()->createTextureFromImage(image);
    node->setTexture(texture);

    node->setRect(boundingRect());
    return node;
}
