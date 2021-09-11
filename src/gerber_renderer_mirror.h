#ifndef __GERBER_RENDERER_MIRROR_H__
#define __GERBER_RENDERER_MIRROR_H__

#include <QQmlEngine>

class GerberRendererMirror : public QObject
{
    Q_OBJECT

    public:
        GerberRendererMirror(QObject *parent = nullptr);
        ~GerberRendererMirror();

        enum class GerberMirror : int
        {
            NONE,
            X,
            Y,
            XY
        };
        Q_ENUM(GerberMirror)
};

#endif //__GERBER_RENDERER_MIRROR_H__
