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

#include <QtWidgets/QCompleter>
#include "gotodialog.h"

GoToDialog::GoToDialog(QWidget *parent) :
    QDialog(parent)
{
    ui.setupUi(this);
    ui.nameLine->setEnabled(false);
    ui.positionLine->setEnabled(false);

    ui.cancelButton->setIcon(QIcon::fromTheme("dialog-cancel"));
    ui.goButton->setIcon(QIcon::fromTheme("dialog-ok"));

    connect(ui.goButton, SIGNAL(clicked()), this, SLOT(goSlot()));
    connect(ui.cancelButton, SIGNAL(clicked()), this, SLOT(cancelSlot()));
    connect(ui.positionLine, SIGNAL(valueChanged(int)), this, SLOT(spinSlot(int)));
    connect(ui.nameLine, SIGNAL(textChanged(QString)), this, SLOT(nameLineSlot(QString)));
}

void GoToDialog::setList(QStringList list)
{
    completer = new QCompleter(list);
    this->list = list;
    completer->setCompletionMode(QCompleter::PopupCompletion);
    ui.nameLine->setEnabled(true);
    ui.nameLine->setCompleter(completer);
}

void GoToDialog::setRange(int min, int max, int pos)
{
    oldPos = pos;
    ui.positionLine->setRange(min, max);
    ui.positionLine->setValue(pos + 1);
    ui.positionLine->setEnabled(true);
    ui.nameLine->setText(list.at(pos));
}

void GoToDialog::cancelSlot()
{
    emit goTo("", oldPos);
    this->close();
}

void GoToDialog::goSlot()
{
    emit goTo(ui.nameLine->text(), ui.positionLine->value() - 1);
    this->close();
}

void GoToDialog::nameLineSlot(QString d)
{
    Q_UNUSED(d);
    ui.positionLine->setValue(list.lastIndexOf(ui.nameLine->text()) + 1);
}

void GoToDialog::spinSlot(int d)
{
    ui.nameLine->setText(list.at(d - 1));
    emit goTo(list.at(d - 1), d - 1);
}
