/****************************************************************************
 * This file is part of EyeSight.
 *
 * Copyright (c) 2012 Pier Luigi Fiorini
 * Copyright (c) 2010-2011 Dario Ignacio Aguilera <dario_21_06@hotmail.com>
 *
 * Author(s):
 *    Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
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

#ifndef EDITTOOLBAR_H
#define EDITTOOLBAR_H

#include <QDialog>
#include <QList>
#include "actiondata.h"
#include "ui_edittoolbar.h"

class ToolBarEdit;

class EditToolBar : public QDialog
{
    Q_OBJECT
public:
    EditToolBar(QWidget *parent = 0);
    void setActionsList(QList<ActionData *>, QStringList);

signals:
    void actionsChanged(QStringList);

public slots:

private:
    Ui_Dialog ui;
    ToolBarEdit *tbe;

private slots:
    void applySlot();
    void okSlot();
    void actionsChangedSlot();

};

#endif // EDITTOOLBAR_H
