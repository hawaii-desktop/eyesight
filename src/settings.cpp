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

#include "settings.h"

Settings::Settings(QObject *parent)
    : QObject(parent)
{
    m_settings = new QSettings();
    loadSettings();
}

Settings::~Settings()
{
    delete m_settings;
}

void Settings::getHour(QString time, int &hour, int &min)
{
    //qDebug()<<time;
    if (time.indexOf("@Time(") == -1)
        return;

    time.remove("@Time(");
    time.remove(")");

    QStringList g = time.split(" ");
    hour = g.at(0).toInt();
    min = g.at(1).toInt();
    //qDebug()<<g;
    //qDebug()<<QString("%1:%2").arg(h).arg(m);

    //check min and hour are both valids
    while (min > 59) {
        hour++;
        min -= 60;
    }

    while (hour > 23) {
        hour -= 24;
    }

}

QString Settings::setHour(int hour, int min)
{
    return QString("@Time(%1 %2)").arg(hour).arg(min);
}

bool Settings::isTime()
{
    int bh = 0, bm = 0, eh = 0, em = 0;
    //qDebug()<<QString("Begin: %1:%2").arg(bh).arg(bm);
    //qDebug()<<QString("End: %1:%2").arg(eh).arg(em);

    if (QTime(bh, bm) <= QTime::currentTime() && QTime::currentTime() <= QTime(eh, em))
        return true;

    else
        return false;
}

QStringList Settings::defaultActions() const
{
    QStringList defaultActions;
    defaultActions << "_previous" << "_next" << "_separator" << "_zoomIn" << "_zoomOut" << "_normalSize" << "_adjustSize"
                   << "_separator" << "_rotateLeft" << "_rotateRight";
    return defaultActions;
}

QStringList Settings::defaultColor() const
{
    QStringList defColor;
    defColor << "104" << "102" << "101";
    return defColor;
}

void Settings::loadSettings()
{
    m_windowSize = QSize(m_settings->value("interface/window-width", 500).toInt(),
                         m_settings->value("interface/window-height", 400).toInt());
    m_showMenuBar = m_settings->value("interface/show-menubar", true).toBool();
    m_tbArea = (Qt::ToolBarArea)m_settings->value("interface/toolbar-area", Qt::BottomToolBarArea).toInt();
    m_showZoomSlider = m_settings->value("interface/show-zoom-slider", false).toBool();
    m_maxRecentFiles = m_settings->value("settings/max-recent-files", 5).toInt();
    m_recentFilesList = m_settings->value("behavior/recent-files").toStringList();
    m_restartWhenZooming = m_settings->value("behavior/restart-animation-when-zooming", true).toBool();
    m_movieSpeed = m_settings->value("behavior/animation-playback-speed", 100).toInt();
    m_stopMovieWhenFinish = m_settings->value("behavior/stop-animation-when-finished", false).toBool();
    m_zoomMultiplier = m_settings->value("behavior/zoom-multiplier", 25).toInt();
    m_compressLevel = m_settings->value("behavior/compression-value",-1).toInt();
    m_sorting = m_settings->value("behavior/sort-by", 0).toInt();
#if 0
    m_loadFixedSize       = settings.value("loadFixedSize",        true).toBool();
    m_squaresSize         = settings.value("squaresSize",          8).toInt();
    m_precision           = settings.value("fileSizePrecision",    2).toInt();
    m_backgroundToShow    = settings.value("bgToShow",             "squares").toString();
    m_backgroundColor     = settings.value("backgroundColor",      defaultColor()).toStringList();
    //toolbar settings
    m_actionsLoaded  = settings.value("actionsLoadedFromSettigns", defaultActions()).toStringList();
    //last dir used
    settings.beginGroup("lastDir");
    m_pathToUse   = settings.value("pathToUse",    int(0)).toInt();
    m_lastDirUsed = settings.value("last_dir",     QDir().homePath()).toString();
    m_defaultPath = settings.value("user_default", QDir().homePath()).toString();
    settings.endGroup();
#else
    m_backgroundColor = QStringList();
    m_backgroundColor.append("0");
    m_backgroundColor.append("0");
    m_backgroundColor.append("0");
    m_actionsLoaded = defaultActions();
    m_defaultPath = QDir().homePath();
#endif
}

void Settings::saveSettings()
{
    m_settings->setValue("interface/window-width", m_windowSize.width());
    m_settings->setValue("interface/window-height", m_windowSize.height());
    m_settings->setValue("interface/show-menubar", m_showMenuBar);
    m_settings->setValue("interface/toolbar-area", m_tbArea);
    m_settings->setValue("interface/show-zoom-slider", m_showZoomSlider);
    m_settings->setValue("behavior/max-recent-files", m_maxRecentFiles);
    m_settings->setValue("behavior/recent-files", m_recentFilesList);
    m_settings->setValue("behavior/restart-animation-when-zooming", m_restartWhenZooming);
    m_settings->setValue("behavior/animation-playback-speed", m_movieSpeed);
    m_settings->setValue("behavior/stop-animation-when-finished", m_stopMovieWhenFinish);
    m_settings->setValue("behavior/zoom-multiplier", m_zoomMultiplier);
    m_settings->setValue("behavior/compression-value", m_compressLevel);
    m_settings->setValue("behavior/sort-by", m_sorting);

#if 0
    settings.setValue("fileSizePrecision", m_precision);
    settings.setValue("loadFixedSize", m_loadFixedSize);
    settings.setValue("squaresSize", m_squaresSize);
    settings.setValue("bgToShow", m_backgroundToShow);
    settings.setValue("backgroundColor", m_backgroundColor);
    settings.setValue("actionsLoadedFromSettigns", m_actionsLoaded);
    settings.beginGroup("lastDir");
    settings.setValue("user_default", m_defaultPath);
    settings.setValue("last_dir", m_lastDirUsed);
    settings.setValue("pathToUse", m_pathToUse);
    settings.endGroup();
#endif
}
