/****************************************************************************
 * This file is part of EyeSight.
 *
 * Copyright (C) 2012-2013 Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
 * Copyright (C) 2010-2011 Dario Ignacio Aguilera <dario_21_06@hotmail.com>
 *
 * Author(s):
 *    Pier Luigi Fiorini
 *
 * $BEGIN_LICENSE:GPL2+$
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * $END_LICENSE$
 ***************************************************************************/

#ifndef ACTIONDATA_H
#define ACTIONDATA_H

#include <QObject>

class QString;

class ActionData : public QObject
{
    Q_OBJECT
public:
    explicit ActionData(QString name, QString id, QString shortcut,
                        QString iconName, QObject *parent = 0);
    inline QString getName() const {
        return name;
    }
    inline QString getId() const {
        return id;
    }
    inline QString getShortcut() const {
        return shortcut;
    }
    inline QString getIconName() const {
        return iconName;
    }

    inline void setName(QString name) {
        this->name = name;
    }
    inline void setIconName(QString iconName) {
        this->iconName = iconName;
    }
    inline void setId(QString id) {
        this->id = id;
    }
    inline void setShortcut(QString shortcut) {
        this->shortcut = shortcut;
    }

private:
    QString name;
    QString id;
    QString shortcut;
    QString iconName;

};

#endif // ACTIONDATA_H
