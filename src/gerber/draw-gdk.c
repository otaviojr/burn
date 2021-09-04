/*
 * gEDA - GNU Electronic Design Automation
 * This file is a part of gerbv.
 *
 *   Copyright (C) 2000-2003 Stefan Petersen (spe@stacken.kth.se)
 *
 * $Id$
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111 USA
 */

/** \file draw-gdk.c
    \brief GDK rendering functions
    \ingroup libgerbv
*/

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <math.h>  /* ceil(), atan2() */

#ifdef HAVE_STRING_H
#include <string.h>
#endif

#include <gtk/gtk.h>
#include "gerbv.h"
#include "draw-gdk.h"
#include "common.h"

#undef round
#define round(x) ceil((double)(x))

#define dprintf if(DEBUG) printf

/*
 * If you want to rotate a
 * column vector v by t degrees using matrix M, use
 *
 *   M = {{cos t, -sin t}, {sin t, cos t}} in M*v.
 *
 * From comp.graphics.algorithms Frequently Asked Questions
 *
 * Due reverse defintion of X-axis in GTK you have to negate
 * angels.
 *
 */
static GerbvPoint 
rotate_point(GerbvPoint point, double angle)
{
    double sint, cost;
    GerbvPoint returned;
    
    if (angle == 0.0)
	return point;

    sint = sin(DEG2RAD(-angle));
    cost = cos(DEG2RAD(-angle));
    
    returned.x = lround(cost*point.x - sint*point.y);
    returned.y = lround(sint*point.x + cost*point.y);
    
    return returned;
}


/*
 * Aperture macro primitive 1 (Circle)
 */
static void
gerbv_gdk_draw_prim1(cairo_t *cr, double *p, 
		     double scale, gint x, gint y)
{
    const int exposure_idx = 0;
    const int diameter_idx = 1;
    const int x_offset_idx = 2;
    const int y_offset_idx = 3;
    const gint full_circle = 23360;

    gint dia    = round(fabs(p[diameter_idx] * scale));
    gint real_x = x - dia / 2;
    gint real_y = y - dia / 2;

    real_x += (int)(p[x_offset_idx] * (double)scale);
    real_y -= (int)(p[y_offset_idx] * (double)scale);

    /* Exposure */
    if (p[exposure_idx] == 0.0) {
	cairo_set_source_rgb (cr, 0, 0, 0);
    }

    cairo_set_line_width (cr, 1.0);
    cairo_arc (cr, real_x, real_y, dia/2, 0, 2*M_PI);
    cairo_fill(cr);
    cairo_stroke(cr);

    return;
} /* gerbv_gdk_draw_prim1 */


/*
 * Aperture macro primitive 4 (outline)
 * - Start point is not included in number of points.
 * - Outline is 1 pixel.
 */
static void
gerbv_gdk_draw_prim4(cairo_t *cr, double *p, 
		     double scale, gint x, gint y)
{
    return;
} /* gerbv_gdk_draw_prim4 */


/*
 * Aperture macro primitive 5 (polygon)
 */
static void
gerbv_gdk_draw_prim5(cairo_t *cr, double *p, 
		     double scale, gint x, gint y)
{
    return;
} /* gerbv_gdk_draw_prim5 */


/*
 * Doesn't handle and explicit x,y yet
 * Questions:
 *  - is "gap" distance between edges of circles or distance between
 *    center of line of circle?
 */
static void
gerbv_gdk_draw_prim6(cairo_t *cr, double *p, 
		 double scale, gint x, gint y)
{
    return;
} /* gerbv_gdk_draw_prim6 */


static void
gerbv_gdk_draw_prim7(cairo_t *cr, double *p, 
		 double scale, gint x, gint y)
{
    return;
} /* gerbv_gdk_draw_prim7 */


/*
 * Doesn't handle and explicit x,y yet
 */
static void
gerbv_gdk_draw_prim20(cairo_t *cr, double *p, 
		  double scale, gint x, gint y)
{
    return;
} /* gerbv_gdk_draw_prim20 */


static void
gerbv_gdk_draw_prim21(cairo_t *cr, double *p, 
		  double scale, gint x, gint y)
{
    return;
} /* gerbv_gdk_draw_prim21 */


/*
 * Doesn't handle explicit x,y yet
 */
static void
gerbv_gdk_draw_prim22(cairo_t *cr, double *p, 
		  double scale, gint x, gint y)
{
    return;
} /* gerbv_gdk_draw_prim22 */

/* Keep this array in order and without holes */
static void (*dgk_draw_amacro_funcs[])(cairo_t *cr, double *, 
		     double, gint, gint) = {
	[GERBV_APTYPE_MACRO_CIRCLE]  = &gerbv_gdk_draw_prim1,
	[GERBV_APTYPE_MACRO_OUTLINE] = &gerbv_gdk_draw_prim4,
	[GERBV_APTYPE_MACRO_POLYGON] = &gerbv_gdk_draw_prim5,
	[GERBV_APTYPE_MACRO_MOIRE]   = &gerbv_gdk_draw_prim6,
	[GERBV_APTYPE_MACRO_THERMAL] = &gerbv_gdk_draw_prim7,
	[GERBV_APTYPE_MACRO_LINE20]  = &gerbv_gdk_draw_prim20,
	[GERBV_APTYPE_MACRO_LINE21]  = &gerbv_gdk_draw_prim21,
	[GERBV_APTYPE_MACRO_LINE22]  = &gerbv_gdk_draw_prim22,
};

static inline void 
gerbv_gdk_draw_amacro(cairo_t *cr, 
		      gerbv_simplified_amacro_t *s, double scale, 
		      gint x, gint y)
{
} /* gerbv_gdk_draw_amacro */


/*
 * Draws a circle _centered_ at x,y with diameter dia
 */
static void 
gerbv_gdk_draw_circle(cairo_t *cr, 
		  gint filled, gint x, gint y, gint dia)
{
    return;
}

/*
 * Draws a rectangle _centered_ at x,y with sides x_side, y_side
 */
static void
gerbv_gdk_draw_rectangle(cairo_t *cr,
		int filled, gint x, gint y, gint x_side, gint y_side,
		double angle_deg)
{
	return;
} /* gerbv_gdk_draw_rectangle */


/*
 * Draws an oval _centered_ at x,y with x axis x_axis and y axis y_axis
 */ 
static void
gerbv_gdk_draw_oval(cairo_t *cr,
		int filled, gint x, gint y, gint x_axis, gint y_axis,
		double angle_deg)
{
    return;
} /* gerbv_gdk_draw_oval */


/*
 * Draws an arc 
 * Draws an arc _centered_ at x,y
 * direction:  0 counterclockwise, 1 clockwise
 */
static void
gerbv_gdk_draw_arc(cairo_t *cr,
	       int x, int y,
	       int width, int height,
	       double angle1, double angle2)
{
    return;
}

void
draw_gdk_render_polygon_object (gerbv_net_t *oldNet, gerbv_image_t *image, double sr_x, double sr_y,
			cairo_matrix_t *fullMatrix, cairo_matrix_t *scaleMatrix, cairo_t *cr) {
	return;
}

void
draw_gdk_apply_netstate_transformation  (cairo_matrix_t *fullMatrix, cairo_matrix_t *scaleMatrix,
	gerbv_netstate_t *state) {
	/* apply scale factor */
	cairo_matrix_scale (fullMatrix, state->scaleA, state->scaleB);
	cairo_matrix_scale (scaleMatrix, state->scaleA, state->scaleB);
	/* apply offset */
	cairo_matrix_translate (fullMatrix, state->offsetA, state->offsetB);
	/* apply mirror */
	switch (state->mirrorState) {
		case GERBV_MIRROR_STATE_FLIPA:
			cairo_matrix_scale (fullMatrix, -1, 1);
			cairo_matrix_scale (scaleMatrix, -1, 1);
			break;
		case GERBV_MIRROR_STATE_FLIPB:
			cairo_matrix_scale (fullMatrix, 1, -1);
			cairo_matrix_scale (scaleMatrix, -1, 1);
			break;
		case GERBV_MIRROR_STATE_FLIPAB:
			cairo_matrix_scale (fullMatrix, -1, -1);
			cairo_matrix_scale (scaleMatrix, -1, 1);
			break;
		default:
			break;
	}
	/* finally, apply axis select */
	if (state->axisSelect == GERBV_AXIS_SELECT_SWAPAB) {
		/* we do this by rotating 270 (counterclockwise, then mirroring
		   the Y axis */
		cairo_matrix_rotate (fullMatrix, M_PI + M_PI_2);
		cairo_matrix_scale (fullMatrix, 1, -1);
	}
}

static void
draw_gdk_cross (cairo_t *cr, gint xc, gint yc, gint r)
{
}

/*
 * Convert a gerber image to a GDK clip mask to be used when creating pixmap
 */
int
draw_gdk_image_to_pixmap(cairo_t *cr, gerbv_image_t *image, 
	     double scale, double trans_x, double trans_y,
	     enum draw_mode drawMode,
	     gerbv_selection_info_t *selectionInfo, gerbv_render_info_t *renderInfo,
	     gerbv_user_transformation_t transform)
{
	return 1;

} /* image2pixmap */


