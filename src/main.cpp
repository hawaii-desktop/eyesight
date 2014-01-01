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

#include <QApplication>
#include <QDir>
#include <QLibraryInfo>
#include <QStandardPaths>
#include <QTranslator>

#include "mainwindow.h"
#include "configdialog.h"

#include <getopt.h>
#include <iostream>

#include <config.h>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    app.setApplicationName(QLatin1String("EyeSight"));
    app.setApplicationVersion(EYESIGHT_VERSION);
    app.setOrganizationDomain(QLatin1String("maui-project.org"));
    app.setOrganizationName(QLatin1String("Hawaii"));
    app.setWindowIcon(QIcon::fromTheme("eyesight"));

    //all the translation stuff was taken from minitube
    const QString locale = QLocale::system().name();

    // Translations
    QTranslator qtTranslator;
    qtTranslator.load("qt_" + locale,
                      QLibraryInfo::location(QLibraryInfo::TranslationsPath));
    app.installTranslator(&qtTranslator);

    QTranslator translator;
    QString localeDir = QStandardPaths::locate(
                            QStandardPaths::GenericDataLocation,
                            QLatin1String("eyesight/translations"),
                            QStandardPaths::LocateDirectory);
    translator.load(locale, localeDir);
    app.installTranslator(&translator);

    /**
      *command line stuff
      */
    int next_option;
    int re = 0;
    const char *const short_options = "hev";
    const struct option long_options[] = {
        {"help",     0, NULL, 'h'},
        {"version",  0, NULL, 'v'},
        {NULL,       0, NULL, 0}
    };

    next_option = getopt_long(argc, argv, short_options, long_options, NULL);

    if (next_option == 'h') {
        std::cout << QString("How to use: eyesight [OPTION/FILE]\n"
                             "Avaible options:\n"
                             "    %1 shows this help and finish\n"
                             "    %2 shows eyesight version\n"
                            ).arg("-h --help").arg("-v --version").toStdString();
        re = 0;
    }

    else if (next_option == '?') {
        std::cout << QObject::tr("Try 'eyesight --help' for more information\n").toStdString();
        re = 0;
    }

    else if (next_option == 'v') {
        std::cout << QObject::tr("EyeSight %1\n"
                                 "Copyright (C) 2011 Aguilera Dario.\n"
                                 "Copyright (C) 2012-2014 Pier Luigi Fiorini.\n"
                                 "License GPLv2+.\n"
                                 "<http://gnu.org/licenses/gpl.html>.\n"
                                 "This is free software: you are free to change it and redistribute.\n"
                                 "There is NO WARRANTY.\n"
                                ).arg(QLatin1String(EYESIGHT_VERSION)).toStdString();
        re = 0;
    }

    else if (next_option == -1) {
        MainWindow imageViewer;
        if (QApplication::arguments().size() > 1)
            imageViewer.openImageFromCommandLine(QApplication::arguments());
        imageViewer.show();
        re = app.exec();
    }

    return re;
}
