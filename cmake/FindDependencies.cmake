if(AXR_BUILD_PROTOTYPE)
    ### Qt ###
    find_package(Qt4 4.8 COMPONENTS QtCore QtGui QtXml REQUIRED)
    include(${QT_USE_FILE})
    add_definitions(${QT_DEFINITIONS} -DNOMINMAX)

    ### GTK ###
    find_package(GTK2)
    if(GTK2_FOUND)
        include_directories(${GTK2_INCLUDE_DIRS})
    else()
        message(STATUS "Could not find GTK2; will not build browser plugin on Linux")
    endif()

    ### DPKG ###
    find_program(DPKG NAMES "dpkg-deb" PATHS "/usr/bin")
    if(DPKG)
        get_filename_component(DPKG_PATH ${DPKG} ABSOLUTE)
        message(STATUS "Found dpkg-deb: ${DPKG_PATH}")
        set(DPKG_FOUND TRUE)
    else()
        message(STATUS "Could not find dpkg-deb; will not build Debian packages")
        set(DPKG_FOUND FALSE)
    endif()

    ### RPMBUILD ###
    find_program(RPMBUILD NAMES "rpmbuild" PATHS "/usr/bin")
    if(RPMBUILD)
        get_filename_component(RPMBUILD_PATH ${RPMBUILD} ABSOLUTE)
        message(STATUS "Found rpmbuild: ${RPMBUILD_PATH}")
        set(RPMBUILD_FOUND TRUE)
    else()
        message(STATUS "Could not find rpmbuild; will not build RPM packages")
        set(RPMBUILD_FOUND FALSE)
    endif()
endif()
