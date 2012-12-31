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

#include <QtCore/QDebug>
#include "edittoolbar.h"
#include "toolbaredit.h"

EditToolBar::EditToolBar(QWidget *parent) :
    QDialog(parent)
{
    ui.setupUi(this);
    ui.okButton->setIcon(QIcon::fromTheme("dialog-ok"));
    ui.applyButton->setIcon(QIcon::fromTheme("dialog-ok-apply"));
    ui.cancelButton->setIcon(QIcon::fromTheme("dialog-cancel"));

    tbe = new ToolBarEdit(this);
    tbe->setMaximumWidth(ui.widget->maximumWidth());
    tbe->setMaximumHeight(ui.widget->maximumHeight());

    connect(tbe, SIGNAL(actionsListChanged()), this, SLOT(actionsChangedSlot()));
    connect(ui.okButton, SIGNAL(clicked()), this, SLOT(okSlot()));
    connect(ui.applyButton, SIGNAL(clicked()), this, SLOT(applySlot()));

    ui.widget = tbe;

    this->setWindowTitle(tr("Configure toolbar"));
}

void EditToolBar::setActionsList(QList<ActionData *> actions, QStringList lActions)
{
    tbe->setActionsList(actions, lActions);
}

void EditToolBar::actionsChangedSlot()
{
    ui.applyButton->setEnabled(true);
    ui.okButton->setEnabled(true);
}

void EditToolBar::applySlot()
{
    qDebug() << "apply slot";
    emit actionsChanged(tbe->getActionsList());
}

void EditToolBar::okSlot()
{
    qDebug() << "ok slot";
    emit actionsChanged(tbe->getActionsList());
    this->close();
}

