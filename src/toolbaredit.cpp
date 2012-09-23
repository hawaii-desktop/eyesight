/*********************************************************************
* Copyright (C) 2010 by Dario Ignacio Aguilera                       *
* dario_21_06@hotmail.com                                            *
*                                                                    *
* This program is free software; you can redistribute it and/or      *
* modify it under the terms of the GNU General Public License        *
* as published by the Free Software Foundation; either version 2     *
* of the License, or (at your option) any later version.             *
*                                                                    *
* This program is distributed in the hope that it will be useful,    *
* but WITHOUT ANY WARRANTY; without even the implied warranty of     *
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the      *
* GNU General Public License for more details.                       *
*                                                                    *
* You should have received a copy of the GNU General Public License  *
* along with this program; if not, write to the                      *
* Free Software Foundation, Inc.                                     *
* 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.      *
**********************************************************************/

#include <QtWidgets>
#include "toolbaredit.h"

ToolBarEdit::ToolBarEdit(QWidget *parent)
    : QWidget(parent)
{
    ui.setupUi(this);

    //create separator item
    separatorItem = new QListWidgetItem(QString("--- %1 ---").arg(tr("Separator")));
    separatorItem->setData(32, "_separator");

    connect(this, SIGNAL(actionListAdded()), this, SLOT(configureActionList()));
    connect(ui.availableActions, SIGNAL(itemSelectionChanged()), this, SLOT(enableActionsA()));
    connect(ui.currentActions, SIGNAL(itemSelectionChanged()), this, SLOT(enableActionsB()));

    //configure buttons
    ui.addButton->setIcon(QIcon::fromTheme("go-next"));
    connect(ui.addButton, SIGNAL(clicked()), this, SLOT(addAction()));
    ui.removeButton->setIcon(QIcon::fromTheme("go-previous"));
    connect(ui.removeButton, SIGNAL(clicked()), this, SLOT(removeAction()));
    ui.moveDownButton->setIcon(QIcon::fromTheme("go-down"));
    connect(ui.moveDownButton, SIGNAL(clicked()), this, SLOT(moveDownAction()));
    ui.moveUpButton->setIcon(QIcon::fromTheme("go-up"));
    connect(ui.moveUpButton, SIGNAL(clicked()), this, SLOT(moveUpAction()));
    ui.addSeparatorButton->setIcon(QIcon::fromTheme("list-add"));
    connect(ui.addSeparatorButton, SIGNAL(clicked()), this, SLOT(addSeparatorAction()));

    connect(ui.currentFilter, SIGNAL(textChanged(QString)), this, SLOT(currentActionsSlot(QString)));
    connect(ui.availableFilter, SIGNAL(textChanged(QString)), this, SLOT(availableActionsSlot(QString)));

}

QStringList ToolBarEdit::getActionsList()
{
    QStringList actions;
    for (int i = 0; i < ui.currentActions->count(); i++) {
        actions << ui.currentActions->item(i)->data(32).toString();
    }

    return actions;
}

void ToolBarEdit::addSeparatorAction()
{
    int i = ui.currentActions->currentRow();
    switch (changeSeparatorButton()) {
        case 0:
            if (i == -1) {
                i = ui.currentActions->count();
            }
            ui.currentActions->insertItem(i + 1, separatorItem->clone());
            ui.currentActions->setCurrentRow(i + 1);
            break;
        case 1:
            ui.currentActions->takeItem(i);
            break;
    }
    emit actionsListChanged();
}

void ToolBarEdit::addAction()
{
    /*qDebug()<<"Agregar accion"<<ui.availableActions->currentRow()+1
            <<":"<<ui.availableActions->currentItem()->text();*/
    int i = ui.currentActions->currentRow() + 1;
    if (i == 0) {
        i = ui.currentActions->count();
    }

    for (int i = 0; i < ui.currentActions->count(); i++) {
        if (ui.currentActions->item(i)->text() ==
                ui.availableActions->item(ui.availableActions->currentRow())->text()) {
            return;
        }
    }

    ui.currentActions->insertItem(i, ui.availableActions->item(ui.availableActions->currentRow())->clone());
    ui.currentActions->setCurrentRow(i);
    ui.removeButton->setEnabled(true);
    emit actionsListChanged();
}

void ToolBarEdit::removeAction()
{
    if (ui.currentActions->count() == 1) {
        ui.currentActions->clear();
        ui.removeButton->setEnabled(false);
        return;
    }
    QListWidgetItem *g = ui.currentActions->takeItem(ui.currentActions->currentRow());
    g->~QListWidgetItem();
    emit actionsListChanged();
    //ui.currentActions->removeItemWidget(ui.currentActions->item(ui.currentActions->currentRow()));
    /*qDebug()<<"Quitar accion"<<ui.currentActions->currentRow()+1
            <<":"<<ui.currentActions->currentItem()->text();*/
    /*bool add=true;
    for(int i=0; i<ui.availableActions->count(); i++){
        if(ui.availableActions->item(i)->text() == ui.currentActions->item(ui.currentActions->currentRow())->text()){
            add = false;
            i = ui.availableActions->count();
        }
    }
    if(add){
        ui.availableActions->addItem(ui.currentActions->takeItem(ui.currentActions->currentRow()));
        emit actionsListChanged();
    }*/
}

void ToolBarEdit::moveUpAction()
{
    int g = ui.currentActions->currentRow();
    ui.currentActions->insertItem(g - 1, ui.currentActions->takeItem(g));
    ui.currentActions->setCurrentRow(g - 1);
    emit actionsListChanged();
}

void ToolBarEdit::moveDownAction()
{
    int g = ui.currentActions->currentRow();
    ui.currentActions->insertItem(g + 1, ui.currentActions->takeItem(g));
    ui.currentActions->setCurrentRow(g + 1);
    emit actionsListChanged();
}

void ToolBarEdit::enableActionsA()
{
    //qDebug()<<"Item seleccionado:"<<ui.availableActions->currentRow()+1;
    ui.addButton->setEnabled(ui.availableActions->count() - 1 != 0);
}

int ToolBarEdit::changeSeparatorButton()
{
    //qDebug()<<ui.currentActions->currentItem()->text();
    if (ui.currentActions->currentItem()->text() != separatorItem->text()) {
        return 0;
    }
    if (ui.currentActions->currentItem()->text() == separatorItem->text()) {
        return 1;
    }
    return 2;
}

void ToolBarEdit::enableActionsB()
{
    //qDebug()<<"cantidad:"<<ui.currentActions->count();
    //qDebug()<<"posicopm:"<<ui.currentActions->currentRow();
    //qDebug()<<"Item seleccionado:"<<ui.currentActions->currentRow()+1;
    ui.removeButton->setEnabled(ui.currentActions->count() - 1 != 0);
    ui.moveDownButton->setEnabled(false);
    ui.moveUpButton->setEnabled(false);
    ui.addSeparatorButton->setEnabled(true);
    switch (changeSeparatorButton()) {
        case 0:
            ui.addSeparatorButton->setIcon(QIcon::fromTheme("list-add"));
            ui.addSeparatorButton->setToolTip(tr("Add separator"));
            break;
        case 1:
            ui.addSeparatorButton->setIcon(QIcon::fromTheme("list-remove"));
            ui.addSeparatorButton->setToolTip(tr("Remove separator"));
            break;
    }
    if (ui.currentActions->currentRow() >= 1) {
        ui.moveUpButton->setEnabled(true);
    }
    if (ui.currentActions->currentRow() < ui.currentActions->count() - 1) {
        ui.moveDownButton->setEnabled(true);
    }
}

void ToolBarEdit::setActionsList(QList<ActionData *> d1, QStringList d2)
{
    allActions = d1;
    actionsUsed = d2;
    emit actionListAdded();
}

void ToolBarEdit::configureActionList()
{
    auCurrentItems();
}

void ToolBarEdit::availableActionsSlot(QString d)
{
    //qDebug()<<d;
    ui.availableActions->keyboardSearch(d);
}

void ToolBarEdit::currentActionsSlot(QString d)
{
    //qDebug()<<d;
    ui.currentActions->keyboardSearch(d);
}

void ToolBarEdit::auCurrentItems()
{
    ui.availableActions->clear();
    ui.currentActions->clear();

    QListWidgetItem *item;
    foreach(ActionData * flag, allActions) {
        item = new QListWidgetItem(flag->getName());
        if (flag->getIconName() != "none") {
            //qDebug()<<flag->getIconName();
            item->setIcon(QIcon::fromTheme(flag->getIconName()));
        } else {
            item->setText(QString("---  %1").arg(item->text()));
        }
        item->setData(32, flag->getId());
        ui.availableActions->addItem(item);
    }

    for (int i = 0; i < actionsUsed.size(); i++) {
        if (actionsUsed.at(i) == "_separator") {
            item = separatorItem->clone();
        } else {
            item = new QListWidgetItem(actionsUsed.at(i));
        }
        ui.currentActions->addItem(item);
    }

    for (int j = 0; j < ui.currentActions->count(); j++) {
        bool add = false;
        foreach(ActionData * flag, allActions) {
            if ((flag->getId() == ui.currentActions->item(j)->text()) && !add) {
                ui.currentActions->item(j)->setText(flag->getName());
                ui.currentActions->item(j)->setIcon(QIcon::fromTheme(flag->getIconName()));
                ui.currentActions->item(j)->setData(32, flag->getId());
            }
        }
    }
}
