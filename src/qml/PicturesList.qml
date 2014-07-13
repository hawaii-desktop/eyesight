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
import Qt.labs.folderlistmodel 2.1
import org.kde.plasma.core 2.0 as PlasmaCore

QtControls.ScrollView {
    readonly property variant filters: [ "*.png", "*.jpg", "*.jpeg", "*.svg", "*.svgz", "*.bmp", "*.gif" ]

    signal pictureSelected(url url)

    id: picturesScrollList

    ListView {
        orientation: ListView.Horizontal
        model: FolderListModel {
            folder: "file:///home/plfiorini"
            showDirs: false
            showDotAndDotDot: false
            showDirsFirst: true
            showFiles: true
            sortField: FolderListModel.Name
            nameFilters: filters
        }
        spacing: units.smallSpacing
        delegate: Item {
            width: units.gridUnit * 5
            height: width * image.ratio

            Image {
                readonly property real ratio: sourceSize.width / sourceSize.height

                id: image
                anchors.fill: parent
                source: fileURL
                fillMode: Image.PreserveAspectFit
            }

            MouseArea {
                anchors.fill: parent
                acceptedButtons: Qt.LeftButton
                onClicked: picturesScrollList.pictureSelected(fileURL)
            }
        }
    }
}
