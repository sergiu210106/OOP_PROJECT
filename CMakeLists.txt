cmake_minimum_required(VERSION 3.20) # You can use 3.20 or a more recent version
project(finalProjectOOP)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(CMAKE_PREFIX_PATH "C:/Qt/6.9.0/mingw_64") # Make sure this is the correct path to your Qt installation

find_package(Qt6 COMPONENTS
        Core
        Gui
        Widgets
        REQUIRED
)

add_executable(finalProjectOOP
        main.cpp
        mainwindow.cpp
        Controller.cpp
        AddEventCommand.cpp
        AddVolunteerCommand.cpp
        CSVEventRepository.cpp
        CSVVolunteerRepository.cpp
        JSONEventRepository.cpp
        JSONVolunteerRepository.cpp
        RemoveEventCommand.cpp
        UpdateEventCommand.cpp
        UpdateVolunteerCommand.cpp
        Volunteer.cpp
        Event.cpp
)

target_link_libraries(finalProjectOOP
        Qt::Core
        Qt::Gui
        Qt::Widgets
)

# Deployment (Windows - MinGW)
if (WIN32 AND NOT DEFINED CMAKE_TOOLCHAIN_FILE)
    set(QT_INSTALL_PATH "${CMAKE_PREFIX_PATH}")
    if (NOT EXISTS "${QT_INSTALL_PATH}/bin")
        set(QT_INSTALL_PATH "${CMAKE_PREFIX_PATH}/..")
        if (NOT EXISTS "${QT_INSTALL_PATH}/bin")
            set(QT_INSTALL_PATH "${CMAKE_PREFIX_PATH}/../..")
        endif()
    endif()

    if (EXISTS "${QT_INSTALL_PATH}/plugins/platforms/qwindows.dll")
        add_custom_command(TARGET finalProjectOOP POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E make_directory "$<TARGET_FILE_DIR:finalProjectOOP>/plugins/platforms")
        add_custom_command(TARGET finalProjectOOP POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy "${QT_INSTALL_PATH}/plugins/platforms/qwindows.dll"
                "$<TARGET_FILE_DIR:finalProjectOOP>/plugins/platforms/")
    endif()

    foreach(QT_LIB Core Gui Widgets)
        if (EXISTS "${QT_INSTALL_PATH}/bin/Qt6${QT_LIB}.dll")
            add_custom_command(TARGET finalProjectOOP POST_BUILD
                    COMMAND ${CMAKE_COMMAND} -E copy "${QT_INSTALL_PATH}/bin/Qt6${QT_LIB}.dll"
                    "$<TARGET_FILE_DIR:finalProjectOOP>")
        endif()
    endforeach()
endif()