/****************************************************************************
 * This file is part of EyeSight.
 *
 * Copyright (C) 2012-2014 Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
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

import QtQuick 2.2
import QtQuick.Controls 1.2 as QtControls
import QtQuick.Layouts 1.1
import org.kde.plasma.core 2.0 as PlasmaCore

BlurBackground {
    id: imageViewer
    source: image

    readonly property real minZoom: 0.1
    readonly property real maxZoom: 3.0
    readonly property real zoomStep: 0.1
    property alias zoom: image.scale
    property alias pictureUrl: image.source

    Flickable {
        id: flick
        anchors.fill: parent
        contentWidth: imageContainer.width
        contentHeight: imageContainer.height
        flickableDirection: Flickable.HorizontalAndVerticalFlick
        interactive: false

        Item {
            id: imageContainer
            width: Math.max(image.width * image.scale, flick.width)
            height: Math.max(image.height * image.scale, flick.height)

            Image {
                property real prevScale

                id: image
                anchors.centerIn: parent
                transformOrigin: Item.Center
                asynchronous: true
                source: "file:///home/plfiorini/Pictures/wallpaper_bg1_2560x1440.png"
                fillMode: Image.PreserveAspectFit
                smooth: !flick.movingVertically
                scale: 1.0
                onScaleChanged: {
                    if ((width * scale) > flick.width) {
                        var offset = (flick.width / 2 + flick.contentX) * scale / prevScale;
                        flick.contentX = offset - flick.width / 2;
                    }
                    if ((height * scale) > flick.height) {
                        var offset = (flick.height / 2 + flick.contentY) * scale / prevScale;
                        flick.contentY = offset - flick.height / 2;
                    }

                    prevScale = scale;
                }
            }

            MouseArea {
                id: mouseArea
                anchors.fill: parent
                onWheel: {
                    var pt = Qt.point(wheel.x, wheel.y);
                    if (wheel.angleDelta.y > 0)
                        zoomIn();
                    else
                        zoomOut();
                }
            }
        }
    }

    function rotateLeft() {
        image.rotation = -90;
    }

    function rotateRight() {
        image.rotation = 90;
    }

    function flipHorizontally() {
        image.mirror = true;
    }

    function flipVertically() {
        image.mirror = false;
    }

    function zoomIn() {
        image.scale += zoomStep;
        if (image.scale > maxZoom)
            image.scale = maxZoom;
    }

    function zoomOut() {
        image.scale -= zoomStep;
        if (image.scale < minZoom)
            image.scale = minZoom;
    }
}
