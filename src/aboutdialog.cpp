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
#include "aboutdialog.h"

QString AboutDialog::getFromResource(const QString& res_file)
{
  QString ret_string;
  QFile file(res_file);
  if (file.open(QIODevice::ReadOnly))
  {
      QTextStream ts(&file);
      ts.setCodec("UTF-8");
      ret_string = ts.readAll();
      file.close();
  }
  return ret_string;
}

AboutDialog::AboutDialog(QWidget* parent, Qt::WFlags fl) : QDialog( parent, fl )
{
    ui.setupUi(this);
#ifdef WEBP_SUPPORT
    qDebug()<<"experimental webp support enabled";
#endif

#ifdef PKGVERSION
    ui.versionLabel->setText(tr("Version %1").arg(QLatin1String(PKGVERSION)));
#else
    ui.versionLabel->setText(tr("Version ") + "0.3.*");
#endif
    
    QString descString;
    descString.append("<p>");
    descString.append(tr("A simple and lightweight image viewer, written totally in Qt."));
    descString.append("</p><p>");
    descString.append("<a href=\"http://code.google.com/p/qiviewer\">");
    descString.append(tr("Home page"));
    descString.append("</a>");
    descString.append(" - ");
    descString.append("<a href=\"http://twitter.com/qiviewer\">");
    descString.append(tr("Twitter account"));
    descString.append("</a></p>");
    ui.descriptionLabel->setText(descString);
    ui.descriptionLabel->setTextFormat(Qt::RichText);

    QString authorString;
    authorString.append("<p>");
    authorString.append(tr("Use %1Google code project page%2 to report bugs, patches or ideas.").arg("<a href=\"http://code.google.com/p/qiviewer/issues/list\">", "</a>"));
    authorString.append("</p><p><b>");
    authorString.append("Aguilera Dario");
    authorString.append("</b><br />");
    authorString.append(tr("Developer."));
    authorString.append("</p>");
    authorString.append("<p><a href=\"mailto:dario_21_06@hotmail.com?Subject=");
    authorString.append(tr("Hello"));
    authorString.append("\">");
    authorString.append(tr("Email"));
    authorString.append("</a> - ");
    authorString.append("<a href=\"http://sammyie.wordpress.com\">");
    authorString.append(tr("Personal blog"));
    authorString.append("</a></p>");
    ui.authorLabel->setText(authorString);

    QString thanksTo;
    thanksTo.append("<p><b>Salvador Parra Camacho</b><ul><li>");
    thanksTo.append(tr("Icons on systems without icon themes"));
    thanksTo.append("</li><li>");
    thanksTo.append(tr("OS/2 support"));
    thanksTo.append("</li></ul></p>");
    ui.thaksToLabel->setText(thanksTo);

    QString translators;
    translators.append("<p><b>Salvador Parra Camacho</b><ul><li>");
    translators.append(tr("Spain-spanish translation"));
    translators.append("</li><li></p>");
    translators.append("<p><b>Neo Kolokotronis (tetris4)</b><ul><li>");
    translators.append(tr("Greek translation"));
    translators.append("</li></ul></p>");
    ui.translatorLabel->setText(translators);

    ui.licenseText->setText(getFromResource(":license.txt"));
    ui.okButton->setIcon(QIcon::fromTheme("dialog-ok"));

    this->resize(QSize(470, 270));
}
