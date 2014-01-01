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

#ifndef WEBPDECODER_H
#define WEBPDECODER_H

#include <QWidget>

class WebpDecoder : public QWidget
{
    Q_OBJECT

public:
    /**
      *constructor
      */
    WebpDecoder(QWidget *parent = 0);

    /**
      *pass the file \a name to decode, returns true if sucess, otherwise return false
      */
    bool setFile(const QString name);

    /**
      * @return the image passed as an QPixmap object
      */
    inline QPixmap getPixmap() const {
        return pixmap;
    }

private:
    QPixmap pixmap;
    QString name;

    bool decodeWebpImage(const QString fileName);
    bool readImage(unsigned char *rgb, int width, int height, QPixmap &pixmap);

};

#endif // WEBPDECODER_H
