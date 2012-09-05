include_directories(
    ${Qt5Widgets_INCLUDE_DIRS}
    ${Qt5PrintSupport_INCLUDE_DIRS}
)

add_definitions(
    ${Qt5Widgets_DEFINITIONS}
    ${Qt5PrintSupport_DEFINITIONS}
)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/config.h.in ${CMAKE_CURRENT_BINARY_DIR}/config.h)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/eyesight.desktop.in ${CMAKE_CURRENT_BINARY_DIR}/eyesight.desktop)

set(SOURCES
    main.cpp
    mainwindow.cpp
    actiondata.cpp
    actionsmanager.cpp
    configdialog.cpp
    edittoolbar.cpp
    fileproperties.cpp
    fileutils.cpp
    gotodialog.cpp
    imagewidget.cpp
    settings.cpp
    shortcut.cpp
    toolbaredit.cpp
    zoomwidget.cpp
)

set(UI_SOURCES
    forms/fileproperties.ui
    forms/configdialog.ui
    forms/gotodialog.ui
    forms/toolbaredit.ui
    forms/edittoolbar.ui
)

set(QRC_SOURCES
    eyesight.qrc
)

qt5_wrap_ui(UI_FILES ${UI_SOURCES})
qt5_add_resources(QRC_FILES ${QRC_SOURCES})

add_executable(eyesight ${SOURCES} ${UI_FILES} ${QRC_FILES})
set_target_properties(eyesight PROPERTIES COMPILE_FLAGS ${Qt5Widgets_EXECUTABLE_COMPILE_FLAGS})
target_link_libraries(eyesight
    ${Qt5Widgets_LIBRARIES}
    ${Qt5PrintSupport_LIBRARIES}
)

install(TARGETS eyesight DESTINATION ${CMAKE_INSTALL_PROGSDIR})
#install(FILES ${CMAKE_CURRENT_BINARY_DIR}/eyesight.desktop ${CMAKE_INSTALL_APPSDIR})
