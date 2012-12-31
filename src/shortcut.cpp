/****************************************************************************
 * This file is part of EyeSight.
 *
 * Copyright (C) 2012 Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
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

#include "shortcut.h"
#include <QShortcut>
#include <QDebug>

Shortcut::Shortcut(QWidget *parent) :
    QObject(parent)
{
    //define the posibles id's
    avaibleIds = QStringList(QString("_open, _openDir, _save, _print, _fileProperties, _exit,"
                                     " _showMenuBar, _toolBarMovable, _configure, _zoomIn, _zoomOut, _normalSize, _bestFit,"
                                     " _toLeft, _toRight, _horizontally, _vertically, _next, _previous, _last, _first, _about,"
                                     " _aboutQt").split(", "));

    avaibleSignals = QStringList() << SIGNAL(open()) << SIGNAL(openDir()) << SIGNAL(save()) << SIGNAL(print())
                     << SIGNAL(fileProperties()) << SIGNAL(exit()) << SIGNAL(showMenuBar())
                     << SIGNAL(setToolBarMovable()) << SIGNAL(configureProgram())
                     << SIGNAL(zoomIn()) << SIGNAL(zoomOut()) << SIGNAL(normalSize())
                     << SIGNAL(adjustSize()) << SIGNAL(rotateLeft()) << SIGNAL(rotateRight())
                     << SIGNAL(flipHorizontally()) << SIGNAL(flipVertically()) << SIGNAL(next())
                     << SIGNAL(previous()) << SIGNAL(goLast()) << SIGNAL(goFirst())
                     << SIGNAL(about()) << SIGNAL(aboutQt());

    //create a shorcut for each id and store it in shortcuts
    for (int i = 0; i < avaibleIds.count(); i++) {
        shortcuts[avaibleIds.at(i)].shortcut = new QShortcut(parent);
        shortcuts[avaibleIds.at(i)].signal = avaibleSignals.at(i).toStdString();
    }

    //create connections
    this->MakeConnections(avaibleIds);
}

void Shortcut::MakeConnections(QStringList li)
{
    for (int i = 0; i < li.count(); i++) {
        connect(shortcuts[li.at(i)].shortcut, SIGNAL(activated()), this, shortcuts[li.at(i)].signal.data());
    }
}

void Shortcut::disconnectShortcuts()
{
    for (int i = 0; i < avaibleIds.count(); i++) {
        connect(shortcuts[avaibleIds.at(i)].shortcut, SIGNAL(activated()), this, shortcuts[avaibleIds.at(i)].signal.data());
    }
}

bool Shortcut::addShortcut(QString id, QString shortcut, std::string member)
{
    //check if the id is valid
    if (!shortcuts.contains(id)) {
        qDebug() << "no es valido";
        return false;
    } else {
        shortcuts[id].shortcut->setKey(QKeySequence(shortcut));
        shortcuts[id].member = member;
        return true;
    }
}

bool Shortcut::isEmpty(QString id)
{
    if (!shortcuts.contains(id)) {
        return false;
    } else {
        return shortcuts[id].shortcut->key().isEmpty();
    }
}

std::string Shortcut::getSignal(QString id)
{
    return shortcuts[id].signal;
}

std::string Shortcut::getMember(QString id)
{
    return shortcuts[id].member;
}
