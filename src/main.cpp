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

#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QDir>
#include <QLibraryInfo>
#include <QStandardPaths>
#include <QTranslator>

#include <config.h>

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
    app.setApplicationName(QStringLiteral("EyeSight"));
    app.setApplicationVersion(EYESIGHT_VERSION);
    app.setOrganizationDomain(QStringLiteral("maui-project.org"));
    app.setOrganizationName(QStringLiteral("Hawaii"));

    // Translations
    const QString locale = QLocale::system().name();
    QTranslator qtTranslator;
    qtTranslator.load("qt_" + locale,
                      QLibraryInfo::location(QLibraryInfo::TranslationsPath));
    app.installTranslator(&qtTranslator);

    QTranslator translator;
    QString localeDir = QStandardPaths::locate(
                            QStandardPaths::GenericDataLocation,
                            QStringLiteral("eyesight/translations"),
                            QStandardPaths::LocateDirectory);
    translator.load(locale, localeDir);
    app.installTranslator(&translator);

    // QML application
    QQmlApplicationEngine engine(QUrl("qrc:///qml/MainWindow.qml"));
    return app.exec();
}
