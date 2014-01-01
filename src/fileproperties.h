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

#ifndef FILEPROPERTIES_H
#define FILEPROPERTIES_H

#include <QDialog>
#include "ui_fileproperties.h"

class FileProperties : public QDialog
{
    Q_OBJECT

public:
    /**
     * constructor
     */
    FileProperties(QWidget *parent = 0);

    /**
     * Sets \a fileName as the file to be processed
     * @param useBinarySystem enable/disable the binary sistem notation to show the file size
     * @param sizePrecision sets the file's size precision to use
     */
    void setFile(const QString fileName, bool useBinarySystem = false, int sizePrecision = 2);

    /**
     * Sets \a prevPixmap as the preview image to show in the dialog
     */
    void setPreviewPixmap(const QPixmap prevPixmap);

    /**
     * Sets \a size as the picture size (height and width) to show
     */
    void setPictureSize(QSize size);

private:
    Ui_PropertiesDialog ui;
    QString getFromResource(const QString &res_file);

};
#endif
