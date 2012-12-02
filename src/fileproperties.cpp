/****************************************************************************
 * This file is part of EyeSight.
 *
 * Copyright (c) 2012 Pier Luigi Fiorini
 * Copyright (c) 2010-2011 Dario Ignacio Aguilera <dario_21_06@hotmail.com>
 *
 * Author(s):
 *    Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
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

#include <QtWidgets>
#include "fileproperties.h"
#include "ui_fileproperties.h"

QString FileProperties::getFromResource(const QString &res_file)
{
    QString ret_string;
    QFile file(res_file);
    if (file.open(QIODevice::ReadOnly)) {
        QTextStream ts(&file);
        ts.setCodec("UTF-8");
        ret_string = ts.readAll();
        file.close();
    }
    return ret_string;
}

FileProperties::FileProperties(QWidget *parent)
    : QDialog(parent)
{
    ui.setupUi(this);
    ui.okButton->setIcon(QIcon::fromTheme("dialog-ok"));

    setWindowIcon(QIcon::fromTheme("document-properties"));
}

void FileProperties::setFile(const QString fileName, bool binary, int precision)
{
    QFileInfo archivo(fileName);
    this->setWindowTitle(tr("%1's properties").arg(archivo.baseName()));
    ui.nameLabelF->setText(archivo.baseName());
    QString path;
    path.append("<p><a href=\"path\">path</a></p>");
    path.replace("path", archivo.absolutePath());
    ui.pathLabelF->setText(path);
    ui.typeLabelF->setText(tr("%1 Image", "Image type").arg((archivo.completeSuffix()).toUpper()));

    QString size;

    //egg
    if (binary) {
        //qDebug()<<archivo.size();
        size.setNum(archivo.size(), 2);
        ui.sizeLabelF->setText(tr("%1 Bytes").arg(size));
        ui.sizeLabelF->setToolTip(getFromResource(":eastereggs.txt"));
    }

    else if ((archivo.size() / 1.024) * 0.001 > 1000) {
        size.setNum(((((archivo.size() / 1.024) * 0.001)) / 1.024) * 0.001, 'f', precision);
        ui.sizeLabelF->setText(tr("%1 MiB (%2)").arg(size).arg(archivo.size()));
    }

    else {
        size.setNum((archivo.size() / 1.024) * 0.001, 'f', precision);
        ui.sizeLabelF->setText(tr("%1 KiB (%2)").arg(size).arg(archivo.size()));
    }
}

void FileProperties::setPreviewPixmap(const QPixmap prevPixmap)
{
    ui.prevLabel->setPixmap(prevPixmap.scaled(ui.prevLabel->size(), Qt::KeepAspectRatio));
}

void FileProperties::setPictureSize(QSize size)
{
    ui.heightLabelF->setText(tr("%1 Pixels").arg(size.height()));
    ui.widthLabelF->setText(tr("%1 Pixels").arg(size.width()));
}
