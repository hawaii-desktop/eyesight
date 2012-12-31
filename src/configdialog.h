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

#ifndef CONFIGDIALOG2_H
#define CONFIGDIALOG2_H

#include <QDialog>
#include "ui_configdialog.h"
#include "actiondata.h"

class Settings;
class ToolBarEdit;

class ConfigDialog : public QDialog
{
    Q_OBJECT

public:
    ConfigDialog(QWidget *parent = 0);

    /**
     * Sends all the actions available with their shortcuts and icons
     * name to the dialog
     */
    void setActionsList(QList<ActionData *> actionData);//actions's icons name

signals:
    /**
     * signal sent when the dialog saves the configuration
     */
    void settingsSaved();

protected:
    void closeEvent(QCloseEvent *event);

private:
    Ui_ConfigDialog ui;
    Settings *settings;
    int closeDialog();
    bool settingsChanged;

    //loaction page settings stuff
    int getLastDirOption();

    //toolbar stuff
    ToolBarEdit *tbe;
    Qt::ToolBarArea gettbpos();

    //general options stuff
    QPixmap iconForButton();
    QStringList rflBack;
    QColor color;
    QStringList setColorSettings();
    void getColorFromSettings(QStringList);
    int getBGSelection(int);//0->returns the settings selectins
    //1->returns the combobox selecteditem

private slots:
    void saveSettings();
    void settingsChangedSlot();

    //loaction page settings stuff
    void openDirSlot();

    //general options stuff
    void paintSquaresSample(int);
    void bgComboBoxSlot(int);
    void selectColorSlot();
    void deleteRecentFilesList(QString);

};

#endif
