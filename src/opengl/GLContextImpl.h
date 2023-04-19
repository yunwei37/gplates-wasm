/* $Id: GLContextImpl.h 19478 2020-06-12 08:12:57Z jcannon $ */

/**
 * \file 
 * $Revision: 19478 $
 * $Date: 2020-06-12 01:12:57 -0700 (Fri, 12 Jun 2020) $
 * 
 * Copyright (C) 2013 The University of Sydney, Australia
 *
 * This file is part of GPlates.
 *
 * GPlates is free software; you can redistribute it and/or modify it under
 * the terms of the GNU General Public License, version 2, as published by
 * the Free Software Foundation.
 *
 * GPlates is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
 * for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

#ifndef GPLATES_OPENGL_GLCONTEXTIMPL_H
#define GPLATES_OPENGL_GLCONTEXTIMPL_H

#include <QOpenGLWidget>
#include <QOpenGLContext>
#include <QOpenGLPaintDevice>
#include <QOffscreenSurface>
#include <QOpenGLFramebufferObject>
#include "GLContext.h"


namespace GPlatesOpenGL
{
	namespace GLContextImpl
	{
		/**
		 * A derivation of GLContext::Impl for QOpenGLWidget.
		 */
		class QOpenGLWidgetImpl :
				public GLContext::Impl
		{
		public:
			explicit
			QOpenGLWidgetImpl(
					QOpenGLWidget &qgl_widget) :
				d_qgl_widget(qgl_widget)
			{  }

			virtual
			void
			make_current()
			{
				d_qgl_widget.makeCurrent();
			}

			virtual
			unsigned int
			get_width() const
			{
				// Dimensions, in OpenGL, are in device pixels.
				return d_qgl_widget.width() * d_qgl_widget.devicePixelRatio();
			}

			virtual
			unsigned int
			get_height() const
			{
				// Dimensions, in OpenGL, are in device pixels.
				return d_qgl_widget.height() * d_qgl_widget.devicePixelRatio();
			}

		private:
			QOpenGLWidget &d_qgl_widget;
		};


		/**
		 * A derivation of GLContext::Impl for QOpenGLFramebufferObject.
		 */
		class QOpenGLFramebufferObjectImpl :
				public GLContext::Impl
		{
		public:
			explicit
			QOpenGLFramebufferObjectImpl(
					QOpenGLFramebufferObject &qgl_pixel_buffer) :
				d_qgl_pixel_buffer(&qgl_pixel_buffer)
			{  }

			void
			set_pixel_buffer(
					QOpenGLFramebufferObject &qgl_pixel_buffer)
			{
				d_qgl_pixel_buffer = &qgl_pixel_buffer;
			}

			QOpenGLFramebufferObject &
			get_pixel_buffer() const
			{
				return *d_qgl_pixel_buffer;
			}

			virtual
			void
			make_current()
			{
                // d_qgl_pixel_buffer->makeCurrent();
                QOpenGLContext *context = QOpenGLContext::currentContext();
                QSurface *surface = context->surface(); // Store the current surface to restore it later
                QOffscreenSurface offscreenSurface;
                offscreenSurface.create(); // Create the offscreen surface

                context->makeCurrent(&offscreenSurface); // Make the context current for the offscreen surface
			}

			virtual
			unsigned int
			get_width() const
			{
                // Dimensions, in OpenGL, are in device pixels.
                return d_qgl_pixel_buffer->width(); //* d_qgl_pixel_buffer->devicePixelRatio();
			}

			virtual
			unsigned int
			get_height() const
			{
                // Dimensions, in OpenGL, are in device pixels.
                return d_qgl_pixel_buffer->height(); // * d_qgl_pixel_buffer->devicePixelRatio();
			}

		private:
			QOpenGLFramebufferObject *d_qgl_pixel_buffer;
		};
	}
}

#endif // GPLATES_OPENGL_GLCONTEXTIMPL_H
