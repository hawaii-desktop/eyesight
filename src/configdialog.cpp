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

#include "configdialog.h"
#include "toolbaredit.h"
#include "settings.h"
#include "fileutils.h"

ConfigDialog::ConfigDialog(QWidget *parent, Qt::WFlags fl) : QDialog(parent, fl)
{
    //configure the ui
    ui.setupUi(this);

    //configure settings
    settings = new Settings;
    settings->loadSettings();

    //add icons to the list items
    ui.listWidget->item(0)->setIcon(QIcon::fromTheme("image-x-generic"));
    ui.listWidget->item(1)->setIcon(QIcon::fromTheme("edit-rename"));
    ui.listWidget->item(2)->setIcon(QIcon::fromTheme("configure-toolbars"));

    //set names to the labels
    ui.toolbarLabel->setText(QString("<h4>%1</h>").arg(tr("Toolbar")));
    ui.generalLabel->setText(QString("<h4>%1</h>").arg(tr("General")));
    ui.locationLabel->setText(QString("<h4>%1</h>").arg(tr("Location")));

    //set icons to the apply, ok and cancel button
    ui.applyButton->setIcon(QIcon::fromTheme("dialog-ok-apply"));
    ui.acceptButton->setIcon(QIcon::fromTheme("dialog-ok"));
    ui.cancelButton->setIcon(QIcon::fromTheme("dialog-cancel"));

    //make buttons connection
    connect(ui.applyButton, SIGNAL(pressed()), this, SLOT(saveSettings()));
    connect(ui.acceptButton, SIGNAL(pressed()), this, SLOT(saveSettings()));

    /**
      * stuff for location page
      */
    //connections, use just one button, becouse when its unselected the other one is selected, and the signal is emited
    connect(ui.selectFolderButton, SIGNAL(pressed()), this, SLOT(openDirSlot()));
    connect(ui.locationOption1, SIGNAL(toggled(bool)), this, SLOT(settingsChangedSlot()));

    //sets last dir option tooltip
    ui.locationOption1->setToolTip(settings->getLastDirUsed());

    //sets option prefered
    switch (settings->getPathToUse()) {
        case 0:
            ui.locationOption1->setChecked(true);
            break;
        case 1:
            ui.locationOption2->setChecked(true);
            break;
    }

    //sets the select folder button icon and tooltip, then make the connection
    ui.selectFolderButton->setIcon(QIcon::fromTheme("document-open-folder"));
    ui.selectFolderButton->setToolTip(tr("Change default location"));

    //sets the line edit text
    ui.defaultPathLineEdit->setText(settings->getDefaultPath());

    /**
      *toolbar stuff
      */
    //edit actions
    tbe = new ToolBarEdit;
    ui.editActions->addWidget(tbe);

    //conections
    connect(ui.leftRadioButtom, SIGNAL(toggled(bool)), this, SLOT(settingsChangedSlot()));
    connect(ui.rightRadioButtom, SIGNAL(toggled(bool)), this, SLOT(settingsChangedSlot()));
    connect(ui.topRadioButtom, SIGNAL(toggled(bool)), this, SLOT(settingsChangedSlot()));
    connect(ui.bottomRadioButtom, SIGNAL(toggled(bool)), this, SLOT(settingsChangedSlot()));
    connect(ui.lockToolbar, SIGNAL(toggled(bool)), this, SLOT(settingsChangedSlot()));
    connect(ui.toolbarVisible, SIGNAL(toggled(bool)), this, SLOT(settingsChangedSlot()));
    connect(ui.onlyIconsRadioButtom, SIGNAL(toggled(bool)), this, SLOT(settingsChangedSlot()));
    connect(ui.onlyTextRadioButtom, SIGNAL(toggled(bool)), this, SLOT(settingsChangedSlot()));
    connect(ui.tbiRadioButtom, SIGNAL(toggled(bool)), this, SLOT(settingsChangedSlot()));
    connect(ui.tuiRadioButtom, SIGNAL(toggled(bool)), this, SLOT(settingsChangedSlot()));
    connect(ui.fsRadioButtom, SIGNAL(toggled(bool)), this, SLOT(settingsChangedSlot()));
    connect(tbe, SIGNAL(actionsListChanged()), this, SLOT(settingsChangedSlot()));

    //toolbar area
    switch (settings->getTBArea()) {
        case 1:
            ui.leftRadioButtom->setChecked(true);
            break;
        case 2:
            ui.rightRadioButtom->setChecked(true);
            break;
        case 4:
            ui.topRadioButtom->setChecked(true);
            break;
        case 8:
            ui.bottomRadioButtom->setChecked(true);
            break;
    }

    //toolbar button style
    switch (settings->getTBButtomStyle()) {
        case 0:
            ui.onlyIconsRadioButtom->setChecked(true);
            break;
        case 1:
            ui.onlyTextRadioButtom->setChecked(true);
            break;
        case 2:
            ui.tbiRadioButtom->setChecked(true);
            break;
        case 3:
            ui.tuiRadioButtom->setChecked(true);
            break;
        case 4:
            ui.fsRadioButtom->setChecked(true);
            break;
    }

    //toolbar visible
    ui.toolbarVisible->setChecked(settings->getTBVisible());

    //toolbar movable
    ui.lockToolbar->setChecked(!settings->getTBMovable());
    ui.lockToolbar->setEnabled(settings->getTBVisible());

    /**
      *general options stuff
      */

    //webp support
    //ui.webpSupport->setChecked(settings->useWebp());
    //files sorting
    ui.orderByComboBox->addItem(tr("Name"), FileUtils::Name);
    ui.orderByComboBox->addItem(tr("Type"), FileUtils::Type);
    ui.orderByComboBox->addItem(tr("Size"), FileUtils::Size);
    ui.orderByComboBox->addItem(tr("Time"), FileUtils::Time);
    ui.orderByComboBox->setCurrentIndex(settings->getSorting());

    //zoom increment
    ui.zoomIncrementSPB->setValue(settings->getZoomIncrement());

    //configure image saving compress level spinbox
    ui.compressLevelSPB->setValue(settings->getCompressLevel());

    //load fixed size
    //ui.loadFixedSizeButton->setChecked(settings->getLoadFixedSize());

    //animated images speed spinbox
    ui.movieSpeedSPB->setValue(settings->getMovieSpeed());

    //show menu bar
    ui.showMenuBarButton->setChecked(settings->getShowMenuBar());

    //stop animated image when reachs the last frame
    ui.stopWhenFinishButton->setChecked(settings->getStopMovieWhenFinish());

    //restart animated image reproduccion when zooming
    ui.restartWhenZoomingButton->setChecked(settings->getRestartWhenZooming());

    //set max recent files list number
    ui.recentFilesNumberSPB->setValue(settings->getMaxRecentFiles());

    //squares size spin box number
    ui.squaresSizeSPB->setValue(settings->getSquaresSize());

    //show zoom slider
    ui.showZoomSliderButton->setChecked(settings->getShowZoomSlider());

    //color background
    getColorFromSettings(settings->getBackgroundColor());

    //making the tabs a little bit more nicer
    ui.tabs->setTabIcon(0, QIcon::fromTheme("preferences-other"));
    ui.tabs->setTabIcon(1, QIcon::fromTheme("image-x-generic"));
    ui.tabs->setTabIcon(2, QIcon::fromTheme("video-x-generic"));

    //sets the combobox
    ui.BGTypeComboBox->setCurrentIndex(getBGSelection(0));
    ui.BGTypeComboBox->setToolTip(tr("With this you select the kind of background you want \n"
                                     "to use for images with alpha channel"));

    //configuring the color button
    ui.colorButton->setIcon(QIcon(iconForButton()));

    //the squares size button
    ui.squaresSampleLabel->setScaledContents(true);
    connect(ui.squaresSizeSPB, SIGNAL(valueChanged(int)), this, SLOT(paintSquaresSample(int)));
    paintSquaresSample(ui.squaresSizeSPB->value());

    //enabling the buttons
    if (settings->getBGToShow() == "squares") {
        ui.colorButton->setEnabled(false);
        ui.colorLabel->setEnabled(false);
        ui.squaresSizeSPB->setEnabled(true);
        ui.squaresSizeLabel->setEnabled(true);
        ui.squaresSampleLabel->setEnabled(true);
    }
    if (settings->getBGToShow() == "solidColor") {
        ui.colorButton->setEnabled(true);
        ui.colorLabel->setEnabled(true);
        ui.squaresSizeSPB->setEnabled(false);
        ui.squaresSizeLabel->setEnabled(false);
        ui.squaresSampleLabel->setEnabled(false);
    }
    if (settings->getBGToShow() == "none") {
        ui.colorButton->setEnabled(false);
        ui.colorLabel->setEnabled(false);
        ui.squaresSizeSPB->setEnabled(false);
        ui.squaresSizeLabel->setEnabled(false);
        ui.squaresSampleLabel->setEnabled(false);
    }

    //delete recent files list
    if (settings->getRecentFilesList().count() == 0) {
        ui.deletRecentFiles->setVisible(false);
    }

    else {
        QString drfl;
        drfl.append("<p><a href=\"delete\">");
        drfl.append(tr("Delete list"));
        drfl.append("</a></p>");
        ui.deletRecentFiles->setText(drfl);
    }

    /*conection to save changes*/
    //connect(ui.loadFixedSizeButton, SIGNAL(toggled(bool)), this, SLOT(settingsChangedSlot()));
    //connect(ui.webpSupport, SIGNAL(toggled(bool)), this, SLOT(settingsChangedSlot()));
    connect(ui.compressLevelSPB, SIGNAL(valueChanged(int)), this, SLOT(settingsChangedSlot()));
    connect(ui.showMenuBarButton, SIGNAL(toggled(bool)), this, SLOT(settingsChangedSlot()));
    connect(ui.showZoomSliderButton, SIGNAL(toggled(bool)), this, SLOT(settingsChangedSlot()));
    connect(ui.stopWhenFinishButton, SIGNAL(toggled(bool)), this, SLOT(settingsChangedSlot()));
    connect(ui.restartWhenZoomingButton, SIGNAL(toggled(bool)), this, SLOT(settingsChangedSlot()));
    connect(ui.BGTypeComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(settingsChangedSlot()));
    connect(ui.orderByComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(settingsChangedSlot()));
    connect(ui.colorButton, SIGNAL(clicked(bool)), this, SLOT(settingsChangedSlot()));
    connect(ui.BGTypeComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(bgComboBoxSlot(int)));
    connect(ui.colorButton, SIGNAL(clicked(bool)), this, SLOT(selectColorSlot()));
    connect(ui.recentFilesNumberSPB, SIGNAL(valueChanged(int)), this, SLOT(settingsChangedSlot()));
    connect(ui.movieSpeedSPB, SIGNAL(valueChanged(int)), this, SLOT(settingsChangedSlot()));
    connect(ui.squaresSizeSPB, SIGNAL(valueChanged(int)), this, SLOT(settingsChangedSlot()));
    connect(ui.zoomIncrementSPB, SIGNAL(valueChanged(int)), this, SLOT(settingsChangedSlot()));
    connect(ui.deletRecentFiles, SIGNAL(linkActivated(QString)), this, SLOT(deleteRecentFilesList(QString)));
    connect(ui.deletRecentFiles, SIGNAL(linkActivated(QString)), this, SLOT(settingsChangedSlot()));
    settingsChanged = false;
    ui.applyButton->setEnabled(false);
    ui.acceptButton->setEnabled(false);
}

/**
  *general options stuff
  */
void ConfigDialog::paintSquaresSample(int d)
{
    QPixmap m = QPixmap(d * 2, d * 2);
    QPainter p(&m);
    p.fillRect(m.rect(), QColor(128, 128, 128));
    QColor light = QColor(192, 192, 192);
    p.fillRect(0, 0, d, d, light);
    p.fillRect(d, d, d, d, light);
    p.end();
    QPixmap n = QPixmap(34, 34);
    QPainter q(&n);
    q.drawTiledPixmap(0, 0, 34, 34, m);
    q.end();
    ui.squaresSampleLabel->setPixmap(n);
}

void ConfigDialog::bgComboBoxSlot(int d)
{
    switch (d) {
        case 0:
            //qDebug()<<"Chesboard.";
            ui.colorButton->setEnabled(false);
            ui.colorLabel->setEnabled(false);
            ui.squaresSizeSPB->setEnabled(true);
            ui.squaresSizeLabel->setEnabled(true);
            ui.squaresSampleLabel->setEnabled(true);
            break;
        case 1:
            //qDebug()<<"Color.";
            ui.colorButton->setEnabled(true);
            ui.colorLabel->setEnabled(true);
            ui.squaresSizeSPB->setEnabled(false);
            ui.squaresSizeLabel->setEnabled(false);
            ui.squaresSampleLabel->setEnabled(false);
            break;
        case 2:
            //qDebug()<<"none.";
            ui.colorButton->setEnabled(false);
            ui.colorLabel->setEnabled(false);
            ui.squaresSizeSPB->setEnabled(false);
            ui.squaresSizeLabel->setEnabled(false);
            ui.squaresSampleLabel->setEnabled(false);
            break;
    }
}

void ConfigDialog::deleteRecentFilesList(QString d)
{
    //qDebug()<<"delete files";
    QString drfl;
    if (d == "delete") {
        drfl.append("<p><a href=\"restore\">");
        drfl.append(tr("Restore list"));
        rflBack = settings->getRecentFilesList();
        settings->setRecentFilesList(QStringList());
    }
    if (d == "restore") {
        drfl.append("<p><a href=\"delete\">");
        drfl.append(tr("Delete list"));
        settings->setRecentFilesList(rflBack);
    }
    drfl.append("</a></p>");
    ui.deletRecentFiles->setText(drfl);
}

void ConfigDialog::selectColorSlot()
{
    color = QColorDialog::getColor(color);
    ui.colorButton->setIcon(QIcon(iconForButton()));
}

QStringList ConfigDialog::setColorSettings()
{
    QStringList g;
    g << QString("%1").arg(color.red());
    g << QString("%1").arg(color.green());
    g << QString("%1").arg(color.blue());
    return g;
}

void ConfigDialog::getColorFromSettings(QStringList d)
{
    color = QColor(d.at(0).toInt(),
                   d.at(1).toInt(),
                   d.at(2).toInt());
}

QPixmap ConfigDialog::iconForButton()
{
    QPixmap pi(ui.colorButton->sizeHint());
    pi.fill(color);
    return pi;
}

/*
 * if d == 0-> returns the string with the type of selected background
 * if d == 1-> sets bgToShow to the selected item in the comboBox widget
 */
int ConfigDialog::getBGSelection(int d)
{
    switch (d) {
        case 0:
            if (settings->getBGToShow() == "squares") {
                return 0;
            }
            if (settings->getBGToShow() == "solidColor") {
                return 1;
            }
            if (settings->getBGToShow() == "none") {
                return 2;
            }
            break;

        case 1:
            switch (ui.BGTypeComboBox->currentIndex()) {
                case 0:
                    settings->setBGTosShow(QString("squares"));
                    break;
                case 1:
                    settings->setBGTosShow(QString("solidColor"));
                    break;
                case 2:
                    settings->setBGTosShow(QString("none"));
                    break;
            }
    }
    return 0;
}

/**
  *location stuff
  */
int ConfigDialog::getLastDirOption()
{
    if (ui.locationOption1->isChecked()) {
        return 0;
    }
    if (ui.locationOption2->isChecked()) {
        return 1;
    }
    return 0;
}

void ConfigDialog::openDirSlot()
{
    QString dirName = QFileDialog::getExistingDirectory(this, tr("Select folder"),
                                                        settings->getDefaultPath(), QFileDialog::ShowDirsOnly
                                                        | QFileDialog::DontResolveSymlinks);
    if (dirName.size() != 0) {
        settings->setDefaultPath(dirName);
        ui.defaultPathLineEdit->setText(dirName);
        settingsChangedSlot();
    }
}

/**
  *toolbar stuff
  */
int ConfigDialog::gettbpos()
{
    if (ui.leftRadioButtom->isChecked()) {
        return 1;
    }
    if (ui.rightRadioButtom->isChecked()) {
        return 2;
    }
    if (ui.topRadioButtom->isChecked()) {
        return 4;
    }
    if (ui.bottomRadioButtom->isChecked()) {
        return 8;
    }
    return 4;
}

int ConfigDialog::getButtonStyle()
{
    if (ui.onlyIconsRadioButtom->isChecked()) {
        return 0;
    }
    if (ui.onlyTextRadioButtom->isChecked()) {
        return 1;
    }
    if (ui.tbiRadioButtom->isChecked()) {
        return 2;
    }
    if (ui.tuiRadioButtom->isChecked()) {
        return 3;
    }
    if (ui.fsRadioButtom->isChecked()) {
        return 4;
    }
    return 0;
}

/**
  *config dialog stuff
  */
void ConfigDialog::setActionsList(QList<ActionData *> actions)
{
    tbe->setActionsList(actions, settings->getActionsLoaded());
}

int ConfigDialog::closeDialog()
{
    if (settingsChanged)
        return 1;
    else
        return 0;
}

void ConfigDialog::closeEvent(QCloseEvent *event)
{
    Q_UNUSED(event);
    switch (closeDialog()) {
        case 0:
            break;
        case 1:
            qDebug() << "the changes have not been saved";
            break;
    }
}

void ConfigDialog::saveSettings()
{
    //location stuff
    settings->setDefaultPath(ui.defaultPathLineEdit->text());
    settings->setPathToUse(this->getLastDirOption());

    //toolbar stuff
    settings->setTBMovable(!ui.lockToolbar->isChecked());
    settings->setTBVisible(ui.toolbarVisible->isChecked());
    settings->setTBArea(this->gettbpos());
    settings->setTBButtomStyle(this->getButtonStyle());
    settings->setActionsLoaded(tbe->getActionsList());

    //general options stuff
    //settings->setUseWebp(ui.webpSupport->isChecked());
    settings->setBackgroundColor(setColorSettings());
    settings->setCompressLevel(ui.compressLevelSPB->value());
    //settings->setLoadFixedSize(ui.loadFixedSizeButton->isChecked());
    settings->setSorting(ui.orderByComboBox->currentIndex());
    settings->setMaxRecentFiles(ui.recentFilesNumberSPB->value());
    settings->setMovieSpeed(ui.movieSpeedSPB->value());
    settings->setRestartWhenZooming(ui.restartWhenZoomingButton->isChecked());
    settings->setShowMenuBar(ui.showMenuBarButton->isChecked());
    settings->setShowZoomSlider(ui.showZoomSliderButton->isChecked());
    settings->setSquaresSize(ui.squaresSizeSPB->value());
    settings->setStopMovieWhenFinish(ui.stopWhenFinishButton->isChecked());
    settings->setZoomIncrement(ui.zoomIncrementSPB->value());
    getBGSelection(1);

    settings->saveSettings();
    ui.applyButton->setEnabled(false);
    emit settingsSaved();
}

void ConfigDialog::settingsChangedSlot()
{
    settingsChanged = true;
    ui.applyButton->setEnabled(true);
    ui.acceptButton->setEnabled(true);
    //qDebug()<<"settings changed"<<settingsChanged;
}
