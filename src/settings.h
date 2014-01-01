/****************************************************************************
 * This file is part of EyeSight.
 *
 * Copyright (C) 2012-2014 Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
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

#ifndef SETTINGS_H
#define SETTINGS_H

#include <QObject>
#include <QtCore>
#include <QDebug>
#include <QSize>

class Settings : public QObject
{
    Q_OBJECT
public:
    Settings(QObject *parent = 0);
    ~Settings();

    inline QSize getWindowSize() const {
        return m_windowSize;
    }

    inline void setWindowSize(const QSize d) {
        m_windowSize = d;
    }

    inline bool getShowMenuBar() const {
        return m_showMenuBar;
    }

    inline void setShowMenuBar(const bool d) {
        m_showMenuBar = d;
    }

    inline Qt::ToolBarArea getTBArea() const {
        return m_tbArea;
    }

    inline void setTBArea(const Qt::ToolBarArea d) {
        m_tbArea = d;
    }

    inline bool getShowZoomSlider() const {
        return m_showZoomSlider;
    }

    inline void setShowZoomSlider(const bool d) {
        m_showZoomSlider = d;
    }

    inline int getMaxRecentFiles() const {
        return m_maxRecentFiles;
    }

    inline void setMaxRecentFiles(const int d) {
        m_maxRecentFiles = d;
    }

    inline QStringList getRecentFilesList() const {
        return m_recentFilesList;
    }

    inline void setRecentFilesList(const QStringList d) {
        m_recentFilesList = d;
    }

    inline bool getRestartWhenZooming() const {
        return m_restartWhenZooming;
    }

    inline void setRestartWhenZooming(const bool d) {
        m_restartWhenZooming = d;
    }

    inline int getMovieSpeed() const {
        return m_movieSpeed;
    }

    inline void setMovieSpeed(const int d) {
        m_movieSpeed = d;
    }

    inline bool getStopMovieWhenFinish() const {
        return m_stopMovieWhenFinish;
    }

    inline void setStopMovieWhenFinish(const bool d) {
        m_stopMovieWhenFinish = d;
    }

    inline int getZoomIncrement() const {
        return m_zoomMultiplier;
    }

    inline void setZoomIncrement(const int d) {
        m_zoomMultiplier = d;
    }

    inline int getCompressLevel() const {
        return m_compressLevel;
    }

    inline void setCompressLevel(const int d) {
        m_compressLevel = d;
    }

    inline int getSorting() const {
        return m_sorting;
    }

    inline void setSorting(const int sort) {
        m_sorting = sort;
    }




    inline QStringList getActionsLoaded() const {
        return m_actionsLoaded;
    }
    inline QStringList getBackgroundColor() const {
        return m_backgroundColor;
    }

    inline QStringList getShortCutsActs() const {
        return m_shortCutsOwner;
    }
    inline QStringList getShortCuts() const {
        return m_shortCuts;
    }
    inline QString getBGToShow() const {
        return m_backgroundToShow;
    }
    inline QString getLastDirUsed() const {
        return m_lastDirUsed;
    }
    inline QString getDefaultPath() const {
        return m_defaultPath;
    }

    inline int getPathToUse() const {
        return m_pathToUse;
    }
    inline int getSquaresSize() const {
        return m_squaresSize;
    }
    inline int getFileSizePrecision() const {
        return m_precision;
    }
    inline bool getCreateNewShortCuts() const {
        return createNewShortCuts;
    }
    inline void setActionsLoaded(const QStringList d) {
        m_actionsLoaded = d;
    }
    inline void setBackgroundColor(const QStringList d) {
        m_backgroundColor = d;
    }
    inline void setShortCuts(const QStringList d, const QStringList e) {
        m_shortCuts = d;
        m_shortCutsOwner = e;
    }
    inline void setBGTosShow(const QString d) {
        m_backgroundToShow = d;
    }
    inline void setLastDirUsed(const QString d) {
        m_lastDirUsed = d;
    }
    inline void setDefaultPath(const QString d) {
        m_defaultPath = d;
    }
    inline void setPathToUse(const int d) {
        m_pathToUse = d;
    }
    inline void setSquaresSize(const int d) {
        m_squaresSize = d;
    }
    inline void setFileSizePrecision(const int fSizePrecision) {
        m_precision = fSizePrecision;
    }
    inline void setCreateNewShortCuts(const bool d) {
        createNewShortCuts = d;
    }

public slots:
    void saveSettings();
    void loadSettings();

private:
    QSettings *m_settings;
    QStringList m_actionsLoaded;
    QStringList m_backgroundColor;
    QStringList m_recentFilesList;
    QStringList m_shortCutsOwner;
    QStringList m_shortCuts;
    QString m_backgroundToShow;
    QString m_lastDirUsed;
    QString m_defaultPath;
    QSize m_windowSize;
    int m_sorting;
    int m_pathToUse;
    int m_squaresSize;
    int m_zoomMultiplier;
    int m_maxRecentFiles;
    int m_precision;
    int m_movieSpeed;
    int m_compressLevel;
    Qt::ToolBarArea m_tbArea;
    bool m_restartWhenZooming;
    bool m_stopMovieWhenFinish;
    bool m_showZoomSlider;
    bool m_showMenuBar;
    bool createNewShortCuts;
    bool m_previousSettings;

    bool isTime();

    /**
      * @return hour and min from the given string
      * @param time string that contains the time
      * @param hour returns the hour
      * @param min returns the min
      */
    void getHour(QString time, int &hour, int &min);

    /**
      * @return a string containing the hour and min
      * @param hour pass the hour
      * @param min pass the minutes
      */
    QString setHour(int hour, int min);
    QStringList defaultActions() const;
    QStringList defaultColor() const;
};

#endif // SETTINGS_H
