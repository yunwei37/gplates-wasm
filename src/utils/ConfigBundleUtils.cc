/* $Id: ConfigBundleUtils.cc 12626 2011-12-09 06:51:06Z mchin $ */

/**
 * \file 
 * $Revision: 12626 $
 * $Date: 2011-12-08 22:51:06 -0800 (Thu, 08 Dec 2011) $
 * 
 * Copyright (C) 2011 The University of Sydney, Australia
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

#include <QRegularExpression>

#include "ConfigBundleUtils.h"

QString
GPlatesUtils::sanitise_key(
		const QString &key_with_slashes)
{
	QString sane = key_with_slashes;
	sane.replace('/', '_');
	return sane;
}


QStringList
GPlatesUtils::match_prefix(
		const QStringList &keys,
		const QString &prefix)
{
	QRegularExpression rx("^" + QRegularExpression::escape(prefix) + "/*");
	return keys.filter(rx);
}


void
GPlatesUtils::strip_prefix(
		QStringList &keys,
		const QString &prefix)
{
	QRegularExpression rx("^" + QRegularExpression::escape(prefix) + "/*");
	keys.replaceInStrings(rx, "");
}


void
GPlatesUtils::strip_all_except_root(
		QStringList &keys)
{
	QRegularExpression rx("/*$");
	keys.replaceInStrings(rx, "");
}


QString
GPlatesUtils::compose_keyname(
		const QString &prefix,
		const QString &subkey)
{
	if (prefix.isEmpty()) {
		return subkey;
	} else if (prefix.endsWith('/')) {
		// You shouldn't have a trailing slash in your key names / prefixes but just in case...
		return QString("%1%2").arg(prefix, subkey);
	} else {
		return QString("%1/%2").arg(prefix, subkey);
	}
}

