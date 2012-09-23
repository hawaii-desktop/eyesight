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
#include <QPrintDialog>

#include <VAboutDialog>

#include "mainwindow.h"
#include "imagewidget.h"
#include "zoomwidget.h"
#include "fileproperties.h"
#include "configdialog.h"
#include "settings.h"
#include "fileutils.h"
#include "gotodialog.h"
#include "edittoolbar.h"

MainWindow::MainWindow()
{
    settings = new Settings;
    settings->loadSettings();

    //actions manager
    actionsManager = new ActionsManager(this);

    //configure the scrollarea
    scrollArea = new QScrollArea;
    scrollArea->setBackgroundRole(QPalette::Mid);
    /*scrollArea->setBackgroundRole(QPalette(QColor(settings->getBackgroundColor().at(0).toInt(),
                                                  settings->getBackgroundColor().at(1).toInt(),
                                                  settings->getBackgroundColor().at(2).toInt())));*/
    scrollArea->setAlignment(Qt::AlignCenter);
    scrollArea->setFrameShape(QFrame::NoFrame);
    setCentralWidget(scrollArea);

    //create the image widget and make the conections
    imageWidget = new ImageWidget;
    scrollArea->setWidget(imageWidget);
    connect(imageWidget, SIGNAL(couldOpen(bool)), this, SLOT(couldOpen(bool)));
    connect(imageWidget, SIGNAL(pixmapChanged()), this, SLOT(pixmapChangedSlot()));
    connect(imageWidget, SIGNAL(moveWidget(QPoint, QPoint)), this, SLOT(moveWidget(QPoint, QPoint)));

    //the zoom widget
    zoomWidget = new ZoomWidget;
    connect(imageWidget, SIGNAL(wheelZoom(int)), this, SLOT(wheelZoom(int)));
    connect(imageWidget, SIGNAL(picSizeChanged()), this, SLOT(imageTranformated()));
    connect(zoomWidget, SIGNAL(zoom(double)), imageWidget, SLOT(makeZoom(double)));
    connect(zoomWidget, SIGNAL(zoom(double)), this, SLOT(updateWindowTitle()));

    setNameFilters();
    createActions();
    createMenus();
    createToolBar();

    imageSetted = false;
    pixmapChanged = false;

    //file utils
    fileUtils = new FileUtils;
    fileUtils->setNameFilters(nameFilters);
    connect(fileUtils, SIGNAL(filePathChanged(QString)), this, SLOT(setLastPathUsed(QString)));

    //add actions to imageWidget, zoomin, zoomout, separator, etc
    imageWidget->addAction(zoomInAct);
    imageWidget->addAction(zoomOutAct);
    //a separator
    QAction *action1 = new QAction(this);
    action1->setSeparator(true);
    imageWidget->addAction(action1);
    imageWidget->addAction(normalSizeAct);
    imageWidget->addAction(adjustSizeAct);
    //another separator
    QAction *action2 = new QAction(this);
    action2->setSeparator(true);
    imageWidget->addAction(action2);
    imageWidget->addAction(filePropertiesAct);
    imageWidget->setContextMenuPolicy(Qt::ActionsContextMenu);

    this->loadSettings();
    this->setWindowTitle(tr("EyeSight"));
    this->setWindowIcon(QIcon::fromTheme("eyesight"));
    this->showMenuBar();
    this->addAction(this->showMenuBarAct);
    this->addAction(this->configureToolBarAct);
    this->setContextMenuPolicy(Qt::ActionsContextMenu);
}

void MainWindow::print()
{
    //copy/paste from qt exaples :-P
    Q_ASSERT(imageWidget->getPixmap());
#ifndef QT_NO_PRINTER
    QPrintDialog dialog(&printer, this);
    if (dialog.exec()) {
        QPainter painter(&printer);
        QRect rect = painter.viewport();
        QSize size = imageWidget->getPictureSize();
        size.scale(rect.size(), Qt::KeepAspectRatio);
        painter.setViewport(rect.x(), rect.y(), size.width(), size.height());
        painter.setWindow(imageWidget->getPixmap().rect());
        painter.drawPixmap(0, 0, imageWidget->getPixmap());
    }
#endif
}

void MainWindow::moveWidget(QPoint d, QPoint e)
{
    //don't ask, i just toke this method code from gwenview (lib/scrolltool) and i don't know how it works
    //of course, i had to make some tweaks
    int width = scrollArea->width();
    int height = scrollArea->height();

    QPoint mousePos = d;

    int deltaX = e.x() - mousePos.x();
    int deltaY = e.y() - mousePos.y();

    if (mousePos.y() <= 4 && scrollArea->verticalScrollBar()->value() < scrollArea->verticalScrollBar()->maximum() - 10) {
        // wrap mouse from top to bottom
        mousePos.setY(height - 5);
    } else if (mousePos.y() >= height - 4 && scrollArea->verticalScrollBar()->value() > 10) {
        // wrap mouse from bottom to top
        mousePos.setY(5);
    }

    if (mousePos.x() <= 4 && scrollArea->horizontalScrollBar()->value() < scrollArea->horizontalScrollBar()->maximum() - 10) {
        // wrap mouse from left to right
        mousePos.setX(width - 5);
    } else if (mousePos.x() >= width - 4 && scrollArea->horizontalScrollBar()->value() > 10) {
        // wrap mouse from right to left
        mousePos.setX(5);
    }

    scrollArea->horizontalScrollBar()->setValue(scrollArea->horizontalScrollBar()->value() + deltaX);
    scrollArea->verticalScrollBar()->setValue(scrollArea->verticalScrollBar()->value() + deltaY);
}

void MainWindow::imageTranformated()
{
    if (!adjustSizeAct->isChecked()) {
        return;
    }

    zoomWidget->clearSizes();
    zoomWidget->setPicSize(imageWidget->getPictureSize());
    zoomWidget->setMaxSize(centralWidget()->size() - QSize(20, 20));
    if (showZoomSlider) {
        zoomSlider->setValue(zoomWidget->getZoomValue());
    }

    updateActions();
}

void MainWindow::setLastPathUsed(QString newPath)
{
    qDebug() << newPath;
    lastDirUsed = newPath;
}

void MainWindow::openImageFromCommandLine(const QStringList d)
{
    //sets the file
    QFileInfo currentFile;
    currentFile.setFile(d.at(1));

    bool open;
    int i, j;
    i = 0;
    j = nameFilters.size();
    open = false;

    //check if the file format is included in the supported formats
    //in case it is, open=true, but open=false
    QString g;
    while (!open && i < j) {
        g = nameFilters.at(i);
        g.remove('*');
        g.remove('"');
        if (currentFile.absoluteFilePath().indexOf(g) != -1) {
            open = true;
            i = j;
        }
        i++;
    }

    //if open=true, the app explore the folder and open the image
    if (open && fileUtils->openFile(currentFile.absoluteFilePath())) {
        openImage();
    }

    //if open=false, show a message
    else {
        QMessageBox notOpen;
        notOpen.setIcon(QMessageBox::Information);
        notOpen.setText(tr("EyeSight couldn't open the image, what do you want to do?"));
        notOpen.setStandardButtons(QMessageBox::Ok | QMessageBox::Open);
        notOpen.setDefaultButton(QMessageBox::Ok);
        int ret = notOpen.exec();
        switch (ret) {
            case QMessageBox::Ok:
                break;
            case QMessageBox::Open:
                this->open();
                break;
            default:
                // should never be reached
                break;
        }
    }
}

void MainWindow::goToSlot()
{
    GoToDialog gt(this);
    gt.setWindowIcon(QIcon::fromTheme("go-jump"));
    gt.setList(fileUtils->getFilesList());
    gt.setRange(1, fileUtils->getFilesAmount(), fileUtils->getCurrentPosition());
    connect(&gt, SIGNAL(goTo(QString, int)), this, SLOT(goToSlot(QString, int)));
    gt.exec();
}

void MainWindow::goToSlot(QString s, int g)
{
    bool d = fileUtils->goToFile(s, g);
    if (!d) {
        qDebug() << "the file doesn't exists";
    }

    else {
        this->openImage();
    }
}

void MainWindow::configureToolBarSlot()
{
    qDebug() << "configure toolbars";
    EditToolBar etb;
    etb.setActionsList(actionsManager->getActions(), settings->getActionsLoaded());
    connect(&etb, SIGNAL(actionsChanged(QStringList)), this, SLOT(configureToolBarSlot2(QStringList)));
    etb.exec();
}

void MainWindow::configureToolBarSlot2(QStringList l)
{
    settings->setActionsLoaded(l);
    setUpToolBar(l, settings->getTBArea(), showZoomSlider);
}

void MainWindow::configureProgram()
{
    saveSettings();
    ConfigDialog con(this);
    con.setWindowIcon(QIcon::fromTheme("configure"));
    con.setWindowTitle(tr("Configuration"));
    con.setActionsList(actionsManager->getActions());
    connect(&con, SIGNAL(settingsSaved()), this, SLOT(loadSettings()));
    con.exec();
}

void MainWindow::resizeEvent(QResizeEvent *)
{
    if (imageSetted) {
        zoomWidget->maxSizeChanged(centralWidget()->size() - QSize(20, 20));
    }
}

void MainWindow::setUpToolBar(QStringList sl, Qt::ToolBarArea a, bool zl)
{
    //clear tool bar
    mainToolBar->clear();

    //create a list with all the ids
    QStringList actList = actionsManager->getIds();

    //add the actions
    for (int i = 0; i < sl.size(); i++) {
        //add a separator
        if (sl.at(i) == "_separator") {
            mainToolBar->addSeparator();
        }

        //add an action
        for (int j = 0; j < actList.size(); j++) {
            QString id = actList.at(j);
            if (id == sl.at(i)) {
                mainToolBar->addAction(actionsManager->getAction(id));
            }
        }
    }

    //add the toolbar to the mainWindow
    addToolBar(a, mainToolBar);

    //zoom slider stuff
    if (zl) {
        zoomSlider = new QSlider(Qt::Horizontal);
        zoomSlider->setMaximumSize(QSize(75, 25));
        zoomSlider->setRange(zoomWidget->getMinZoomValue(),
                             zoomWidget->getMaxZoomValue());
        connect(zoomWidget, SIGNAL(sliderSignal(int)), zoomSlider, SLOT(setValue(int)));
        connect(zoomSlider, SIGNAL(valueChanged(int)), zoomWidget, SLOT(sliderSlot(int)));
        mainToolBar->addSeparator();
        mainToolBar->addWidget(zoomSlider);
        zoomSlider->setEnabled(false);
    }
}

void MainWindow::createMenus()
{
    recentFilesMenu = new QMenu(tr("&Recent images"), this);
    recentFilesMenu->setIcon(QIcon::fromTheme("document-open-recent"));

    fileMenu = new QMenu(tr("&File"), this);
    fileMenu->addAction(openAct);
    fileMenu->addAction(openDirAct);
    fileMenu->addMenu(recentFilesMenu);
    fileMenu->addAction(printAct);
    fileMenu->addSeparator();
    fileMenu->addAction(filePropertiesAct);
    fileMenu->addSeparator();
    fileMenu->addAction(saveAct);
    fileMenu->addAction(moveToAct);
    fileMenu->addAction(deleteFileAct);
    fileMenu->addSeparator();
    fileMenu->addAction(exitAct);

    editMenu = new QMenu(tr("&Edit"), this);
    editMenu->addAction(showMenuBarAct);
    editMenu->addAction(configAct);

    viewMenu = new QMenu(tr("&View"), this);
    viewMenu->addAction(zoomInAct);
    viewMenu->addAction(zoomOutAct);
    viewMenu->addSeparator();
    viewMenu->addAction(normalSizeAct);
    viewMenu->addAction(adjustSizeAct);

    imageMenu = new QMenu(tr("&Image"), this);
    imageMenu->addAction(rotateLeftAct);
    imageMenu->addAction(rotateRightAct);
    imageMenu->addSeparator();
    imageMenu->addAction(flipVerticallyAct);
    imageMenu->addAction(flipHorizontallyAct);

    helpMenu = new QMenu(tr("&Help"), this);
    helpMenu->addAction(aboutAct);

    goMenu = new QMenu(tr("&Go"), this);
    goMenu->addAction(nextAct);
    goMenu->addAction(prevAct);
    goMenu->addSeparator();
    goMenu->addAction(goFirstAct);
    goMenu->addAction(goLastAct);
    goMenu->addSeparator(),
           goMenu->addAction(goToAct);

    menuBar()->addMenu(fileMenu);
    menuBar()->addMenu(editMenu);
    menuBar()->addMenu(viewMenu);
    menuBar()->addMenu(imageMenu);
    menuBar()->addMenu(goMenu);
    menuBar()->addMenu(helpMenu);
}

void MainWindow::createToolBar()
{
    mainToolBar = new QToolBar(tr("Main toolbar"));
    mainToolBar->setFloatable(false);
}

void MainWindow::updateActions()
{
    actionsManager->setEnabled("_zoomIn" , imageSetted);
    actionsManager->setEnabled("_zoomOut" , imageSetted);
    actionsManager->setEnabled("_moveTo" , imageSetted);
    actionsManager->setEnabled("_print" , imageSetted);
    actionsManager->setEnabled("_deleteFile" , imageSetted);
    actionsManager->setEnabled("_save" , imageSetted && imageType != Dynamic);
    actionsManager->setEnabled("_normalSize" , imageSetted);
    actionsManager->setEnabled("_adjustSize" , imageSetted);
    actionsManager->setEnabled("_rotateLeft" , imageSetted);
    actionsManager->setEnabled("_rotateRight" , imageSetted);
    actionsManager->setEnabled("_flipVertically" , imageSetted);
    actionsManager->setEnabled("_flipHorizontally" , imageSetted);
    if (showZoomSlider) {
        zoomSlider->setEnabled(imageSetted);
    }
    actionsManager->setEnabled("_fileProperties" , imageSetted);

    bool g = (fileUtils->getFilesAmount() > 1) && imageSetted;
    actionsManager->setEnabled("_next" , g);
    actionsManager->setEnabled("_previous" , g);
    actionsManager->setEnabled("_goLast" , g);
    actionsManager->setEnabled("_goFirst" , g);
    actionsManager->setEnabled("_goTo" , g);
}

void MainWindow::createActions()
{
    openAct = new QAction(tr("&Open"), this);
    openAct->setToolTip(tr("Open an image"));
    actionsManager->addAction(openAct, "_open", this, this, SLOT(open()), QKeySequence::Open);

    saveAct = new QAction(tr("&Save"), this);
    saveAct->setEnabled(false);
    actionsManager->addAction(saveAct, "_save", this, this, SLOT(save()), QKeySequence::Save);

    exitAct = new QAction(tr("&Exit"), this);
    actionsManager->addAction(exitAct, "_exit", this, this, SLOT(close()), QKeySequence::Quit);

    filePropertiesAct = new QAction(tr("Properties"), this);
    filePropertiesAct->setEnabled(false);
    actionsManager->addAction(filePropertiesAct, "_fileProperties", this, this,
                              SLOT(fileProperties()), QKeySequence("Ctrl+."));

    zoomInAct = new QAction(tr("Zoom In"), this);
    zoomInAct->setEnabled(false);
    actionsManager->addAction(zoomInAct, "_zoomIn", this, this, SLOT(zoomIn()), QKeySequence("+"));

    zoomOutAct = new QAction(tr("Zoom Out"), this);
    zoomOutAct->setEnabled(false);
    actionsManager->addAction(zoomOutAct, "_zoomOut", this, this, SLOT(zoomOut()), QKeySequence("-"));

    normalSizeAct = new QAction(tr("Normal Size"), this);
    normalSizeAct->setEnabled(false);
    actionsManager->addAction(normalSizeAct, "_normalSize", this, this, SLOT(normalSize()), QKeySequence("1"));

    adjustSizeAct = new QAction(tr("Best Fit"), this);
    adjustSizeAct->setEnabled(false);
    adjustSizeAct->setCheckable(true);
    actionsManager->addAction(adjustSizeAct, "_adjustSize", this, this, SLOT(adjustSizeSlot()), QKeySequence("0"));

    rotateRightAct = new QAction(tr("Rotate to right"), this);
    rotateRightAct->setToolTip(tr("Rotate image in the clockwise clock"));
    rotateRightAct->setEnabled(false);
    actionsManager->addAction(rotateRightAct, "_rotateRight", this, this, SLOT(rotateRight()));

    rotateLeftAct = new QAction(tr("Rotate to Left"), this);
    rotateLeftAct->setToolTip(tr("Rotate image counter-clockwise to clockwise"));
    rotateLeftAct->setEnabled(false);
    actionsManager->addAction(rotateLeftAct, "_rotateLeft", this, this, SLOT(rotateLeft()));

    flipVerticallyAct = new QAction(tr("Flip vertically"), this);
    flipVerticallyAct->setToolTip(tr("Turns vertically the image"));
    flipVerticallyAct->setEnabled(false);
    actionsManager->addAction(flipVerticallyAct, "_flipVertically", this, this, SLOT(flipVertically()));

    flipHorizontallyAct = new QAction(tr("Flip horizontally"), this);
    flipHorizontallyAct->setToolTip(tr("Reflects the image"));
    flipHorizontallyAct->setEnabled(false);
    actionsManager->addAction(flipHorizontallyAct, "_flipHorizontally", this, this, SLOT(flipHorizontally()));

    aboutAct = new QAction(tr("A&bout"), this);
    actionsManager->addAction(aboutAct, "_about", this, this, SLOT(about()), QKeySequence::HelpContents);

    nextAct = new QAction(tr("Ne&xt"), this);
    nextAct->setStatusTip(tr("Loads next image"));
    nextAct->setEnabled(false);
    actionsManager->addAction(nextAct, "_next", this, this, SLOT(next()), Qt::Key_Right);

    goFirstAct = new QAction(tr("Go to the first"), this);
    goFirstAct->setStatusTip(tr("Loads the first image in the folder"));
    goFirstAct->setEnabled(false);
    actionsManager->addAction(goFirstAct, "_goFirst", this, this, SLOT(goFirst()), Qt::Key_Home);

    prevAct = new QAction(tr("Pre&vious"), this);
    prevAct->setStatusTip(tr("Loads previous image"));
    prevAct->setEnabled(false);
    actionsManager->addAction(prevAct, "_previous", this, this, SLOT(previous()), Qt::Key_Left);

    goLastAct = new QAction(tr("Go to the last"), this);
    goLastAct->setStatusTip(tr("Loads the last image in the folder"));
    goLastAct->setEnabled(false);
    actionsManager->addAction(goLastAct, "_goLast", this, this, SLOT(goLast()), Qt::Key_End);

    openDirAct = new QAction(tr("Open &Folder"), this);
    openDirAct->setStatusTip("Open a folder to explore images inside it");
    actionsManager->addAction(openDirAct, "_openFolder", this, this, SLOT(openDir()), QKeySequence("Ctrl+Shift+O"));

    showMenuBarAct = new QAction(tr("Show Menu Bar"), this);
    showMenuBarAct->setCheckable(true);
    actionsManager->addAction(showMenuBarAct, "_showMenuBar", this, this, SLOT(showMenuBar()), QKeySequence("Ctrl+M"));

    configAct = new QAction(tr("Configuration"), this);
    configAct->setEnabled(true);
    actionsManager->addAction(configAct, "_configuration", this, this, SLOT(configureProgram()), QKeySequence("Ctrl+C"));

    deleteRecentFilesAct = new QAction(tr("Delete list"), this);
    deleteRecentFilesAct->setIcon(QIcon::fromTheme("edit-clear"));
    connect(deleteRecentFilesAct, SIGNAL(triggered()), this, SLOT(deleteRecentFiles()));

    printAct = new QAction(tr("Print"), this);
    printAct->setEnabled(false);
    actionsManager->addAction(printAct, "_print", this, this, SLOT(print()), QKeySequence::Print);

    deleteFileAct = new QAction(tr("Delete"), this);
    deleteFileAct->setEnabled(false);
    deleteFileAct->setToolTip(tr("This deletes completly the file from the disk, doesn't move it to the trash"));
    actionsManager->addAction(deleteFileAct, "_deleteFile", this, this, SLOT(deleteFileSlot()), QKeySequence::Delete);

    moveToAct = new QAction(tr("Move to..."), this);
    moveToAct->setEnabled(false);
    actionsManager->addAction(moveToAct, "_moveTo", this, this, SLOT(moveToSlot()));

    goToAct = new QAction(tr("Go to"), this);
    goToAct->setEnabled(false);
    actionsManager->addAction(goToAct, "_goTo", this, this, SLOT(goToSlot()), QKeySequence("Ctrl+J"));

    configureToolBarAct = new QAction(tr("Configure toolbar"), this);
    actionsManager->addAction(configureToolBarAct, "_configureToolBar", this, this, SLOT(configureToolBarSlot()));

    //set the icons, becouse QIcon::name() was included in Qt4.7
    actionsManager->setActionIcon("_about", "help-about");
    actionsManager->setActionIcon("_adjustSize", "zoom-fit-best");
    actionsManager->setActionIcon("_configuration", "configure");
    actionsManager->setActionIcon("_deleteFile", "edit-delete");
    actionsManager->setActionIcon("_exit", "application-exit");
    actionsManager->setActionIcon("_fileProperties", "document-properties");
    actionsManager->setActionIcon("_flipHorizontally", "object-flip-horizontal");
    actionsManager->setActionIcon("_flipVertically", "object-flip-vertical");
    actionsManager->setActionIcon("_goFirst", "go-first");
    actionsManager->setActionIcon("_goLast", "go-last");
    actionsManager->setActionIcon("_goTo", "go-jump");
    actionsManager->setActionIcon("_moveTo", "none");
    actionsManager->setActionIcon("_next", "go-next");
    actionsManager->setActionIcon("_normalSize", "zoom-original");
    actionsManager->setActionIcon("_open", "document-open");
    actionsManager->setActionIcon("_openFolder", "folder-open");
    actionsManager->setActionIcon("_previous", "go-previous");
    actionsManager->setActionIcon("_print", "document-print");
    actionsManager->setActionIcon("_rotateLeft", "object-rotate-left");
    actionsManager->setActionIcon("_rotateRight", "object-rotate-right");
    actionsManager->setActionIcon("_save", "document-save");
    actionsManager->setActionIcon("_showMenuBar", "show-menu");
    actionsManager->setActionIcon("_showToolBar", "configure-toolbars");
    actionsManager->setActionIcon("_tbMovable", "configure-toolbars");
    actionsManager->setActionIcon("_configureToolBar", "configure-toolbars");
    actionsManager->setActionIcon("_zoomIn", "zoom-in");
    actionsManager->setActionIcon("_zoomOut", "zoom-out");
}

void MainWindow::showMenuBar()
{
    if (showMenuBarAct->isChecked())
        menuBar()->show();
    else
        menuBar()->hide();
}

void MainWindow::fileProperties()
{
    FileProperties fp;
    fp.setFile(fileUtils->absoluteFilePath(), settings->getFileSizePrecision());
    fp.setPreviewPixmap(imageWidget->getPixmap());
    fp.setPictureSize(imageWidget->getPictureSize());
    fp.exec();
}

void MainWindow::about()
{
    QStringList authors;
    authors << "Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>";

    VAboutDialog dialog(this);
    dialog.setAuthors(authors);
    dialog.setCopyright("Pier Luigi Fiorini");
    dialog.setDescription("Simple image visualization application.");
    dialog.setLink(QUrl("http://www.maui-project.org/"));

    QFile licenseFile(":/COPYING");
    if (licenseFile.open(QIODevice::ReadOnly)) {
        dialog.setLicenseText(licenseFile.readAll());
        licenseFile.close();
    }

    dialog.exec();
}

void MainWindow::flipHorizontally()
{
    imageWidget->transformImage(ImageWidget::horizontally);
}

void MainWindow::flipVertically()
{
    imageWidget->transformImage(ImageWidget::vertically);
}

void MainWindow::rotateRight()
{
    imageWidget->transformImage(ImageWidget::toRight);
}

void MainWindow::rotateLeft()
{
    imageWidget->transformImage(ImageWidget::toLeft);
}

void MainWindow::deleteRecentFiles()
{
    //qDebug()<<"delete";
    //clean menu and actions
    recentFilesMenu->clear();
    for (int i = 0; i < recentFilesAct.size(); i++) {
        recentFilesAct.at(i)->~QAction();
    }
    recentFilesAct.clear();

    //add action to clear list
    recentFilesMenu->addAction(deleteRecentFilesAct);
    recentFilesMenu->addSeparator();

    //remove any previous coincidence
    recentFilesPath.clear();

    createRFActs();
}

void MainWindow::moveToSlot()
{
    QString newFileName = QFileDialog::getExistingDirectory(this, tr("Move to"), getLastDir());
    if (fileUtils->moveFile(newFileName)) {
        qDebug() << "moved";
        recentFilesPath.removeAt(0);
        createRFActs();
        openImage();
    }
}

void MainWindow::deleteFileSlot()
{
    //ask if user really wants to delete the file
    int result = QMessageBox::question(this, tr("Delete"), tr("Do you want to delete %1?").arg(fileUtils->getFileName()),
                                       QMessageBox::Yes | QMessageBox::No);

    if (result == QMessageBox::No) {
        return;
    }

    if (fileUtils->deleteFile()) {
        qDebug() << tr("File deleted");
    } else {
        QMessageBox::information(this, tr("EyeSight"), tr("The file couldn't be deleted"), QMessageBox::Ok);
    }

    if (fileUtils->getFilesAmount() != 0) {
        //removes the file from the list of recent files, and since it was the last one
        //opened, then remove the first in the list.
        recentFilesPath.removeAt(0);
        createRFActs();
        openImage();
    }

    else {
        imageWidget->clear();
    }
}

void MainWindow::pushNewFile()
{
    //clean menu and actions
    recentFilesMenu->clear();
    for (int i = 0; i < recentFilesAct.size(); i++) {
        recentFilesAct.at(i)->~QAction();
    }
    recentFilesAct.clear();

    //remove any previous coincidence
    recentFilesPath.removeAll(fileUtils->absoluteFilePath());

    //add the file to the top of the list
    recentFilesPath.prepend(fileUtils->absoluteFilePath());

    //make sure the list doesn't have more files than the allowed
    if (recentFilesPath.size() > maxRecentFiles) {
        recentFilesPath.removeLast();
    }

    createRFActs();
}

void MainWindow::openRecentFile()
{
    /*i don't have a clue of how the next stuff wokrs,
     *i just copy/pasted it from the qt examples :-P*/
    QString temp;
    QAction *action = qobject_cast<QAction *>(sender());
    if (action) {
        temp = action->data().toString();
    }

    //check if the file exists
    if (!fileUtils->openFile(temp)) {
        QMessageBox::information(this, tr("EyeSight"), tr("The file doesn't exist, may be it's been moved or deleted."),
                                 QMessageBox::Ok);
        recentFilesPath.removeOne(temp);
        createRFActs();
        return;
    }

    else {
        openImage();
    }
}

void MainWindow::createRFActs()
{
    while (recentFilesPath.size() > maxRecentFiles) {
        recentFilesPath.removeLast();
    }

    //remove the files moved or deleted from the list
    QStringList temp;
    foreach(QString flag, recentFilesPath) {
        if (QFileInfo(flag).exists()) {
            temp << flag;
        }
    }
    recentFilesPath = temp;

    //clean menu and actions
    recentFilesMenu->clear();
    for (int i = 0; i < recentFilesAct.size(); i++) {
        recentFilesAct.at(i)->~QAction();
    }

    //add action to clear list
    recentFilesMenu->addAction(deleteRecentFilesAct);
    recentFilesMenu->addSeparator();

    //enable/disable the menu
    recentFilesMenu->setEnabled(recentFilesPath.size() > 0);

    recentFilesAct.clear();
    if (recentFilesPath.size() > 0) {
        for (int i = 0; i < recentFilesPath.size(); i++) {
            //creo y agrego un objeto QAction con un icono generico y el nombre del archivo
            recentFilesAct << new QAction(QIcon::fromTheme("image-x-generic"), QFileInfo(recentFilesPath.at(i)).fileName(), this);

            //agrego la direccion completa a la data del QAction correspondiente
            recentFilesAct.at(i)->setData(recentFilesPath.at(i));

            //connecto la accion con el slot encargado de abrir las imagenes recientes
            connect(recentFilesAct.at(i), SIGNAL(triggered()), this, SLOT(openRecentFile()));

            //agrego la accion al menu
            recentFilesMenu->addAction(recentFilesAct.at(i));
        }
    }
}

void MainWindow::setNameFilters()
{
    nameFilters << "*.png" << "*.pbm"
                << "*.jpg" << "*.jpeg"
                << "*.gif" << "*.tiff"
                << "*.xbm" << "*.JPG"
                << "*.bmp" << "*.ppm"
                << "*.pgm" << "*.mng"
                << "*.xpm"
#ifdef WEBP_SUPPORT
                << "*.webp"
#endif
                << "*.svg" << "*.svgz";
    nameFilters.removeDuplicates();
}

void MainWindow::save()
{
    //create a list with the formas that can be used as output
    QStringList g = nameFilters;
    g.removeAll("*.gif");
    g.removeAll("*.mng");
    g.removeAll("*.pbm");
    g.removeAll("*.pgm");
    g.removeAll("*.svg");
    g.removeAll("*.svgz");
#ifdef WEBP_SUPPORT
    g.removeAll("*.webp");
#endif

    //call the save dialog
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save File"),
                                                    fileUtils->getFilePath(),
                                                    tr("Image Files (%1)").arg(g.join(" ")));

    //check if the name isn't empty and imageWidget can save
    if (!(fileName.isEmpty()) && imageWidget->canSave()) {
        //try to save the image
        bool couldsave = imageWidget->getPixmap().save(fileName, 0, compressLevel);

        //if succed, then open ot
        if (couldsave && fileUtils->openFile(fileName)) {
            this->openImage();
        }

        //else, show a messege informing the user
        else {
            QMessageBox::information(this, tr("EyeSight"), tr("I couldn't save the image"), QMessageBox::Ok);
        }
    }
}

void MainWindow::openDir()
{
    this->canCloseImage();
    QString dirName = QFileDialog::getExistingDirectory(this, tr("Open folder"),
                                                        getLastDir(), QFileDialog::ShowDirsOnly
                                                        | QFileDialog::DontResolveSymlinks);
    qDebug() << "folder" << dirName;
    if (dirName.isEmpty()) {
        qDebug() << tr("No folder selected");
        return;
    }

    if (fileUtils->openFolder(dirName)) {
        pixmapChanged = false;
        openImage();
    } else {
        QMessageBox::information(this, tr("EyeSight"), tr("No folder selected, i can't do anything"),
                                 QMessageBox::Ok);
    }
}

void MainWindow::canCloseImage()
{
    if (pixmapChanged) {
        int g = QMessageBox::information(this, tr("Image changed"),
                                         tr("I've touched the image so, do you want to "
                                            "save it before exit?"),
                                         QMessageBox::Save, QMessageBox::Discard, QMessageBox::Cancel);
        switch (g) {
            case QMessageBox::Save:
                this->save();
                pixmapChanged = false;
                break;
            case QMessageBox::Cancel:
                return;
        }
    }
}

QString MainWindow::getLastDir() const
{
    QString d;
    switch (pathToUse) {
        case 0:
            d = lastDirUsed;
            break;
        case 1:
            d = defaultPath;
            break;
        default:
            d = defaultPath;
            break;
    }
    return d;
}

void MainWindow::open()
{
    //check if the current image has been modificated (i.e turned to left)
    this->canCloseImage();

    //get a filename from a open dialog
    QString fileName = QFileDialog::getOpenFileName(this,
                                                    tr("Open file", "dialog to open file"),
                                                    getLastDir(),
                                                    tr("Image Files (%1)").arg(nameFilters.join(" ")));

    //if the filename name isn't empty, open the file
    if (!(fileName.isEmpty())) {
        if (fileUtils->openFile(fileName)) {
            pixmapChanged = false;
            openImage();
        } else {
            QMessageBox::information(this, tr("EyeSight"), tr("EyeSight couldn't open the image, what do you want to do?"),
                                     QMessageBox::Ok);
        }
    }
}

void MainWindow::goFirst()
{
    if (fileUtils->goToFile(FileUtils::First)) {
        openImage();
    }
}

void MainWindow::goLast()
{
    if (fileUtils->goToFile(FileUtils::Last)) {
        openImage();
    }
}

void MainWindow::next()
{
    if (fileUtils->goToFile(FileUtils::Next)) {
        openImage();
    }
}

void MainWindow::previous()
{
    if (fileUtils->goToFile(FileUtils::Previous)) {
        openImage();
    }
}

void MainWindow::openImage()
{
    determinateImageType();

    this->setCursor(Qt::BusyCursor);
    switch (imageType) {
        case Static:
            openStatic();
            break;
        case Dynamic:
            openDynamic();
            break;
        case Webp:
            openStatic();
    }
    this->setCursor(Qt::ArrowCursor);
    this->pushNewFile();
    this->updateWindowTitle();
}

void MainWindow::determinateImageType()
{
#ifdef WEBP_SUPPORT
    if (fileUtils->getFileName().indexOf(".webp") != -1) {
        imageType = Webp;
        return;
    }
#endif

    QMovie m(fileUtils->absoluteFilePath());
    if (m.isValid()) {
        if (m.frameCount() > 1) {
            imageType = Dynamic;
        } else {
            imageType = Static;
        }
    }
}

void MainWindow::updateWindowTitle()
{
    if (fileUtils->getFilesAmount() == 0) {
        this->setWindowTitle(tr("EyeSight"));
    } else {
        QString windowTitleFormat = QString("appName :: pos/amount - zoomLevel - imageName");
        windowTitleFormat.replace("appName", "EyeSight");
        windowTitleFormat.replace("imageName", fileUtils->getFileName());
        windowTitleFormat.replace("pos", QString("%1").arg(fileUtils->getCurrentPosition() + 1));
        windowTitleFormat.replace("amount", QString("%1").arg(fileUtils->getFilesAmount()));
        windowTitleFormat.replace("zoomLevel", QString("%1%").arg(zoomWidget->getZoomValue()));
        this->setWindowTitle(windowTitleFormat);
    }

}

void MainWindow::openStatic()
{
    imageWidget->setPicture(fileUtils->absoluteFilePath());
    zoomWidget->clearSizes();
    zoomWidget->setPicSize(imageWidget->getPictureSize());
    zoomWidget->setMaxSize(centralWidget()->size() - QSize(20, 20));
    if (showZoomSlider) {
        zoomSlider->setValue(zoomWidget->getZoomValue());
    }
    updateActions();
}

void MainWindow::openDynamic()
{
    imageWidget->setMovie(fileUtils->absoluteFilePath());
    zoomWidget->clearSizes();
    zoomWidget->setPicSize(imageWidget->getPictureSize());
    zoomWidget->setMaxSize(centralWidget()->size() - QSize(20, 20));
    if (showZoomSlider) {
        zoomSlider->setValue(zoomWidget->getZoomValue());
    }
    updateActions();
}

void MainWindow::couldOpen(bool d)
{
    if (d) {
        imageSetted = true;
        return;
    }
    int h = QMessageBox::information(this, tr("EyeSight"), tr("I couldn't make it :'("),
                                     QMessageBox::Ok, QMessageBox::Open, QMessageBox::Ignore);
    switch (h) {
        case QMessageBox::Ok:
            break;
        case QMessageBox::Open:
            this->open();
            break;
        case QMessageBox::Ignore:
            fileUtils->openFile(imageWidget->getPrevFile());
            openImage();
            break;
    }
}

void MainWindow::wheelZoom(int d)
{
    if (!imageSetted) {
        return;
    }

    if (d > 0) {
        zoomWidget->zoomIn();
    }

    if (d < 0) {
        zoomWidget->zoomOut();
    }

    adjustSizeAct->setChecked(zoomWidget->bestZoomValue());
}

void MainWindow::zoomOut()
{
    zoomWidget->zoomOut();
    adjustSizeAct->setChecked(zoomWidget->bestZoomValue());
}

void MainWindow::adjustSizeSlot()
{
    zoomWidget->setAdjustedSize(adjustSizeAct->isChecked());
}

void MainWindow::zoomIn()
{
    zoomWidget->zoomIn();
    adjustSizeAct->setChecked(zoomWidget->bestZoomValue());
}

void MainWindow::normalSize()
{
    zoomWidget->normalSize();
    adjustSizeAct->setChecked(zoomWidget->bestZoomValue());
    zoomWidget->setAdjustedSize(false);
}

void MainWindow::getColorFromSettings(QStringList d)
{
    imageWidget->setSCBColor(QColor(d.at(0).toInt(),
                                    d.at(1).toInt(),
                                    d.at(2).toInt()));
}

void MainWindow::loadSettings()
{
    settings->loadSettings();

    /*main app settings*/
    resize(settings->getWindowSize());
    getColorFromSettings(settings->getBackgroundColor());
    showMenuBarAct->setChecked(settings->getShowMenuBar());
    zoomWidget->setZoomIncrement(settings->getZoomIncrement());
    imageWidget->setBGType(settings->getBGToShow());
    imageWidget->setCBSSize(settings->getSquaresSize());
    imageWidget->setRestartWhenZooming(settings->getRestartWhenZooming());
    imageWidget->setMovieSpeed(settings->getMovieSpeed());
    imageWidget->setStopWhenFinish(settings->getStopMovieWhenFinish());
    showZoomSlider  = settings->getShowZoomSlider();
    compressLevel   = settings->getCompressLevel();
    maxRecentFiles  = settings->getMaxRecentFiles();
    recentFilesPath = settings->getRecentFilesList();
    fileUtils->setSorting(FileUtils::Order(settings->getSorting()));
    createRFActs();
    adjustSizeSlot();
    showMenuBar();

    //tool bar settings
    setUpToolBar(settings->getActionsLoaded(),
                 settings->getTBArea(),
                 showZoomSlider);

    //last dir used
    lastDirUsed = settings->getLastDirUsed();
    defaultPath = settings->getDefaultPath();
    pathToUse   = settings->getPathToUse();

    this->updateActions();
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    saveSettings();
    event->accept();
}

void MainWindow::saveSettings()
{
    //much more clear than loadSettings(), don't you think?
    settings->setWindowSize(this->size());
    settings->setRecentFilesList(recentFilesPath);
    settings->setShowMenuBar(showMenuBarAct->isChecked());
    settings->setTBArea(toolBarArea(mainToolBar));
    settings->setLastDirUsed(lastDirUsed);
    /*settings->setCreateNewShortCuts(false);
    QStringList a, b;
    for(int i=0; i<actionsList.size(); i++){
        if(!actionsList.at(i)->shortcut().isEmpty()){
            a<<actionsList.at(i)->text().remove('&');
            b<<actionsList.at(i)->shortcut();
        }
    }
    settings->setShortCuts(a, b);*/
    settings->saveSettings();
}
