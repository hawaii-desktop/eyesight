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

#ifndef GOTODIALOG_H
#define GOTODIALOG_H

#include <QDialog>
#include "ui_gotodialog.h"

class GoToDialog : public QDialog
{
    Q_OBJECT
public:
    /*!
     * constructor
     */
    GoToDialog(QWidget *parent = 0);

    /*!
     * sets \a list as the list to use for the autocompletion
     */
    void setList(QStringList list);

    /*!
     * sets the range(\a min, \a max) to use in the spin box, and sets it to \a pos
     * */
    void setRange(int min, int max, int pos);

signals:
    /*!
     * signal emited when clicking on go button, it send the \a name selected
     * and the \a postion
     */
    void goTo(QString name, int pos);

private:
    Ui_goToDialog ui;

    //a completer for the QLineEdit object
    QCompleter *completer;

    //a list qith the files so the completer knows what to recomend
    QStringList list;

    //stores old position
    int oldPos;

private slots:
    void goSlot();
    void cancelSlot();
    void spinSlot(int);
    void nameLineSlot(QString);

};

#endif // GOTODIALOG_H
