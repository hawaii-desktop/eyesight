/****************************************************************************
 * This file is part of EyeSight.
 *
 * Copyright (C) 2012-2013 Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
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

#include "zoomwidget.h"
#include <QtWidgets>

ZoomWidget::ZoomWidget(QWidget *parent) :
    QWidget(parent)
{
    this->setDefaults();
    adjustedSize = true;
    zoomIncrement = 25;
    maxZoomValue = 500;
    minZoomValue = 10;
}

bool ZoomWidget::bestZoomValue() const
{
    if (!needToMakeZoom && (zoomValue == 100))
        return true;

    else if (needToMakeZoom && (zoomValue == zoomValueForBestFit))
        return true;

    else
        return false;
}

void ZoomWidget::setDefaults()
{
    maxSize = QSize(-1, -1);
    picSize = QSize(-1, -1);
    zoomValue = 100;
    zoomRestoreValue = 100;
    zoomValueForBestFit = 100;
    needToMakeZoom = false;
    canMakeZoom = false;
    mSizeChanged = false;
    //fTime = true;
    emit sliderSignal(zoomValue);
}

void ZoomWidget::makeZoom(int d)
{
    if (d < maxZoomValue && d > minZoomValue) {
        zoomValue = d;
        emit sliderSignal(d);
        emit zoom(d * 0.01);
    }
}

void ZoomWidget::setAdjustedSize(bool d)
{
    adjustedSize = d;
    if (adjustedSize) {
        zoomRestoreValue = zoomValue;
        this->adjustPicSize();
    } else {
        //qDebug()<<"zom value:"<<zoomValue;
        zoomValue = zoomRestoreValue;
        makeZoom(zoomValue);
    }
}

void ZoomWidget::setMaxSize(const QSize d)
{
    maxSize = d;
    calculateZoom();
    if (canMakeZoom) {
        configure();
    }
}

void ZoomWidget::setPicSize(const QSize d)
{
    picSize = d;
    calculateZoom();
    if (canMakeZoom) {
        configure();
    }
}

void ZoomWidget::maxSizeChanged(QSize d)
{
    maxSize = d;
    mSizeChanged = true;
    calculateZoom();
    configure();
}

void ZoomWidget::configure()
{
    if (!canMakeZoom)
        return;

    if (needToMakeZoom) {
        if (zoomValue < minZoomValue)
            minZoomValue = zoomValueForBestFit;
    }

    if (adjustedSize) {
        this->adjustPicSize();
        //fTime = false;
        return;
    } else {
        if (mSizeChanged) {
            mSizeChanged = false;
            return;
        } else
            this->normalSize();
    }
}

void ZoomWidget::calculateZoom()
{
    if (!maxSize.isValid() || !picSize.isValid()) {
        canMakeZoom = false;
        return;
    }

    needToMakeZoom = false;
    canMakeZoom = true;

    double w, h, mw, mh, rw, rh, g = 1;
    w = picSize.width();
    h = picSize.height();
    mw = maxSize.width();
    mh = maxSize.height();
    rw = mw / w;
    rh = mh / h;

    if (rw < 1 && rh > 1) {
        g = rw;
        needToMakeZoom = true;
    }
    if (rw > 1 && rh < 1) {
        g = rh;
        needToMakeZoom = true;
    }
    if (rw < 1 && rh < 1) {
        if (rw < rh) {
            g = rw;
            needToMakeZoom = true;
        }
        if (rh < rw) {
            g = rh;
            needToMakeZoom = true;
        }
    }

    if (canMakeZoom) {
        zoomValueForBestFit = g * 100;
    }
}
