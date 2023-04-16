/* $Id: Presenter.h 2226 2007-12-20 11:03:30Z jboyden $ */

/**
 * \file 
 * $Revision: 2226 $
 * $Date: 2007-12-20 03:03:30 -0800 (Thu, 20 Dec 2007) $ 
 * 
 * Copyright (C) 2006 The University of Sydney, Australia
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

namespace GPlatesPresenter
{
    class Presenter
    {
    public:
        Presenter &get_presenter()
        {
            static Presenter presenter;
            return presenter;
        }
    private:
        // This is a Singleton so we need to hide the usual suspects.
        Presenter() {  }
        Presenter(const Presenter &);
        Presenter &operator=(const Presenter &);
        ~Presenter();
    };
}
