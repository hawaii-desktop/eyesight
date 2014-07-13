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

QtControls.ApplicationWindow {
    title: "EyeSight"
    minimumWidth: 640
    minimumHeight: 480
    width: 800
    height: 480
    visible: true
    statusBar: QtControls.ToolBar {
        ColumnLayout {
            anchors.fill: parent

            RowLayout {
                QtControls.Slider {
                    id: slider
                    minimumValue: viewer.minZoom
                    maximumValue: viewer.maxZoom
                    stepSize: viewer.zoomStep

                    Layout.minimumWidth: units.gridUnit * 12
                    Layout.alignment: Qt.AlignLeft

                    Component.onCompleted: value = 1.0
                }

                Controls {
                    Layout.alignment: Qt.AlignHCenter
                }

                Options {
                    Layout.alignment: Qt.AlignRight
                }

                Layout.fillWidth: true
            }

            PicturesList {
                onPictureSelected: viewer.pictureUrl = url

                Layout.fillWidth: true
                //Layout.fillHeight: true
                Layout.minimumHeight: units.gridUnit * 5
                Layout.maximumHeight: units.gridUnit * 15
            }
        }
    }

    ImageView {
        id: viewer
        anchors.fill: parent
        zoom: slider.value
    }

/*
    QtControls.StackView {
        id: stackView
        anchors.fill: parent
        initialItem: FoldersView {}
    }

    function selectImage(path) {
    }
*/
}
