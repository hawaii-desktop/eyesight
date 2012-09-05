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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMap>
#include <QPrinter>
#include "actionsmanager.h"

class QAction;
class QMenu;
class QScrollArea;
class QAbstractScrollArea;
class QToolBar;
class QSlider;
class QMovie;
class QFileInfo;
class QPaintEvent;
class QMouseEvent;
class QShortcut;
class ZoomWidget;
class ImageWidget;
class Settings;
class FileUtils;

enum ImageType {
    Static, //this is for static images, such as jpeg's or not animated gif ones
    Dynamic, //animated images, such as animated gif or png
    Webp //needed becouse qt doesn't handle this yet
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    /**
     * constructor
     */
    MainWindow();

    /**
      * Load app command from command line
      * @param d is the list of the parameters passed
      */
    void openImageFromCommandLine(const QStringList);

protected:
    void resizeEvent(QResizeEvent *);
    void closeEvent(QCloseEvent *event);

private:
    //variables
    FileUtils *fileUtils;
    Settings *settings;
    ActionsManager *actionsManager;
    ImageType imageType;
    ImageWidget *imageWidget;
    ZoomWidget *zoomWidget;
    QSlider *zoomSlider;
    bool showZoomSlider;
    bool imageSetted;
    bool pixmapChanged;
    int compressLevel;
    QStringList nameFilters;
    QScrollArea *scrollArea;
    QStringList actionsLoaded;
    QString lastDirUsed;//could i delete it and just use archivo->absoluteFilePath()? nop
    int pathToUse;//0=lastDir, 1=default
    QString defaultPath;
    int maxRecentFiles;
    QList<QAction *> recentFilesAct;
    QStringList recentFilesPath;

    //toolbar and menus
    QToolBar *mainToolBar;
    QMenu *fileMenu;
    QMenu *viewMenu;
    QMenu *editMenu;
    QMenu *goMenu;
    QMenu *imageMenu;
    QMenu *helpMenu;
    QMenu *recentFilesMenu;

    //actions
    QAction *printAct;
    QAction *openAct;
    QAction *saveAct;
    QAction *exitAct;
    QAction *aboutQtAct;
    QAction *filePropertiesAct;
    QAction *zoomInAct;
    QAction *zoomOutAct;
    QAction *normalSizeAct;
    QAction *adjustSizeAct;
    QAction *rotateLeftAct;
    QAction *rotateRightAct;
    QAction *flipHorizontallyAct;
    QAction *flipVerticallyAct;
    QAction *nextAct;
    QAction *prevAct;
    QAction *goFirstAct;
    QAction *goLastAct;
    QAction *openDirAct;
    QAction *showMenuBarAct;
    QAction *setTBMovableAct;
    QAction *showToolBarAct;
    QAction *configAct;
    QAction *deleteRecentFilesAct;
    QAction *deleteFileAct;
    QAction *moveToAct;
    QAction *goToAct;
    QAction *configureToolBarAct;

#ifndef QT_NO_PRINTER
    QPrinter printer;
#endif

    //functions
    QString getLastDir() const;
    void createActions();
    void createMenus();
    void createToolBar();
    void setNameFilters();
    void determinateImageType();
    void openImage();
    void updateActions();
    void getColorFromSettings(QStringList);
    void canCloseImage();

    /**
      *creating actions for the recent files
      */
    void createRFActs();

    /**
      *this method is called when i open an image, to add it to the recent files
      */
    void pushNewFile();

    /**
      * configure toolbar
      *
      * @param sl is the list of the actions to add to the toolbar
      * @param a is the toolbar area
      * @param s the toolbar buttom style
      * @param v toolbar visible or not
      * @param zl show zoom slider
      */
    void setUpToolBar(QStringList, Qt::ToolBarArea, Qt::ToolButtonStyle, bool, bool);

private slots:
    void open();
    void save();
    void print();
    void next();
    void previous();
    void goFirst();
    void goLast();
    void openDir();
    void openRecentFile();
    void openStatic();
    void openDynamic();
    void fileProperties();
    void zoomIn();
    void zoomOut();
    void wheelZoom(int);
    void normalSize();
    void adjustSizeSlot();
    void rotateLeft();
    void rotateRight();
    void flipHorizontally();
    void flipVertically();
    void couldOpen(bool);
    void loadSettings();
    void saveSettings();
    void showMenuBar();
    void setToolBarMovable();
    void setToolBarVisible(bool);
    void deleteRecentFiles();
    void deleteFileSlot();
    void moveToSlot();
    void goToSlot();
    void goToSlot(QString name, int pos);
    void updateWindowTitle();
    void setLastPathUsed(QString newPath);
    void configureToolBarSlot();
    void configureToolBarSlot2(QStringList);
    inline void pixmapChangedSlot() {
        pixmapChanged = true;
    }

    /**
      *this methos is for moving the image with the mouse
      * @param e is the QPoint value from mousePressEvent
      * @param d is the QPoint value from mouseMoveEvent
      */
    void moveWidget(QPoint, QPoint);

    /**
      * configuration dialog
      */
    void configureProgram();

    /**
      *this is for when the image is rotated and the button fixed size
      *is checked
      */
    void imageTranformated();

};

#endif
