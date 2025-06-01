cmake_minimum_required(VERSION 3.20)
project(finalProjectOOP)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(CMAKE_PREFIX_PATH "C:/Qt/6.9.0/mingw_64") # Make sure this is correct

enable_testing()

find_package(Qt6 COMPONENTS
        Core
        Gui
        Widgets
        Test
        REQUIRED
)

# Create a library with all the core classes
add_library(volunteer_core
        Controller/Controller.cpp
        core/Command/AddEventCommand.cpp
        core/Command/AddVolunteerCommand.cpp
        Repository/CSV/CSVEventRepository.cpp
        Repository/CSV/CSVVolunteerRepository.cpp
        Repository/JSON/JSONEventRepository.cpp
        Repository/JSON/JSONVolunteerRepository.cpp
        core/Command/RemoveEventCommand.cpp
        core/Command/RemoveVolunteerCommand.cpp
        core/Command/UpdateEventCommand.cpp
        core/Command/UpdateVolunteerCommand.cpp
        Model/Volunteer.cpp
        Model/Event.cpp
)

target_link_libraries(volunteer_core
        Qt::Core
)

# Main application executable
add_executable(finalProjectOOP
        ui/main.cpp
        ui/mainwindow.cpp
)

target_link_libraries(finalProjectOOP
        volunteer_core
        Qt::Core
        Qt::Gui
        Qt::Widgets
)

# Test executable
add_executable(volunteer_tests
        tests/tests.cpp
)

target_link_libraries(volunteer_tests
        volunteer_core
        Qt::Core
        Qt::Test
)

# Add the test to CTest
add_test(NAME VolunteerManagementTests COMMAND volunteer_tests)

# Set working directory for tests
set_tests_properties(VolunteerManagementTests PROPERTIES
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
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

    foreach(QT_LIB Core Gui Widgets Test)
        if (EXISTS "${QT_INSTALL_PATH}/bin/Qt6${QT_LIB}.dll")
            add_custom_command(TARGET finalProjectOOP POST_BUILD
                    COMMAND ${CMAKE_COMMAND} -E copy "${QT_INSTALL_PATH}/bin/Qt6${QT_LIB}.dll"
                    "$<TARGET_FILE_DIR:finalProjectOOP>")
            # Also copy DLLs for test executable
            add_custom_command(TARGET volunteer_tests POST_BUILD
                    COMMAND ${CMAKE_COMMAND} -E copy "${QT_INSTALL_PATH}/bin/Qt6${QT_LIB}.dll"
                    "$<TARGET_FILE_DIR:volunteer_tests>")
        endif()
    endforeach()
endif()