include_directories(${VibeWidgets_INCLUDE_DIRS})

add_definitions(${VibeWidgets_DEFINITIONS})

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/config.h.in ${CMAKE_CURRENT_BINARY_DIR}/config.h)

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


set(QM_SOURCES
    translations/ar.ts
    translations/de.ts
    translations/el.ts
    translations/es.ts
    translations/es_AR.ts
    translations/fr.ts
    translations/it.ts
    translations/ja_JP.ts
    translations/nl.ts
    translations/pt.ts
    translations/zh_CN.ts
)

set(QRC_SOURCES
    eyesight.qrc
)

qt5_wrap_ui(UI_FILES ${UI_SOURCES})
qt5_add_resources(QRC_FILES ${QRC_SOURCES})
qt5_add_translation(QM_FILES ${QM_SOURCES})

add_executable(eyesight ${SOURCES} ${UI_FILES} ${QRC_FILES} ${QM_FILES})
qt5_use_modules(eyesight PrintSupport)
target_link_libraries(eyesight ${VibeWidgets_LIBRARIES})

install(TARGETS eyesight DESTINATION ${CMAKE_INSTALL_BINDIR})
install(FILES ${QM_FILES} DESTINATION ${CMAKE_INSTALL_DATADIR}/eyesight/translations)
