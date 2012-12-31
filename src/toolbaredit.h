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

#ifndef TOOLBAREDIT_H
#define TOOLBAREDIT_H

#include <QMainWindow>
#include "actiondata.h"
#include "ui_toolbaredit.h"

class QListWidgetItem;

class ToolBarEdit : public QWidget
{
    Q_OBJECT

public:
    ToolBarEdit(QWidget *parent = 0);

    QStringList getActionsList();
    void setActionsList(QList<ActionData *>, QStringList);

signals:
    void actionsListChanged();

private:
    Ui_toolBarEdit ui;
    void auCurrentItems();

    QList<ActionData *> allActions;
    QStringList actionsUsed;

    int changeSeparatorButton();
    QListWidgetItem *separatorItem;

private slots:
    void configureActionList();
    void enableActionsA();
    void enableActionsB();

    void addAction();
    void removeAction();
    void moveUpAction();
    void moveDownAction();
    void addSeparatorAction();
    void availableActionsSlot(QString);
    void currentActionsSlot(QString);

signals:
    void actionListAdded();

};

#endif
