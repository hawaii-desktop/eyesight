#install(FILES org.hawaii.eyesight.json
#    DESTINATION ${CMAKE_INSTALL_SETTINGSDIR})
install(FILES eyesight.appdata.xml
    DESTINATION ${CMAKE_INSTALL_DATADIR}/appdata)
install(FILES eyesight.desktop
    DESTINATION ${CMAKE_INSTALL_DATADIR}/applications)
