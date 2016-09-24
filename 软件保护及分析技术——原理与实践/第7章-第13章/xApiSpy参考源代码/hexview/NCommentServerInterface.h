/*
Copyright (C) 2006 - 2009 Evan Teran
                   eteran@alum.rit.edu

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
*/

#ifndef COMMENTSERVERINTERFACE_20070427_H_
#define COMMENTSERVERINTERFACE_20070427_H_
#include <QObject>
class QString;

class NCommentServerInterface {
public:
    virtual ~NCommentServerInterface() {}
public:
    virtual void set_comment(qintptr address, const QString &comment) = 0;
    virtual QString comment(qintptr address, int size) const = 0;
	virtual void clear() = 0;
};

#endif

