set(DATA vk-gtk.glade chrome.png konqueror.png ff.png opera.png vk.png vk-logout.png vk-message.png)
install(FILES ${DATA} DESTINATION ${PKGDATADIR})
install(FILES vk-desktop.png DESTINATION ${CMAKE_INSTALL_PREFIX}/share/pixmaps)
install(FILES vk-gtk.desktop DESTINATION ${CMAKE_INSTALL_PREFIX}/share/applications)
