configure_file(${CMAKE_CURRENT_SOURCE_DIR}/eyesight.desktop.in ${CMAKE_CURRENT_BINARY_DIR}/eyesight.desktop)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/eyesight.desktop DESTINATION ${CMAKE_INSTALL_APPSDIR})
