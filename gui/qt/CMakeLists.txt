cmake_minimum_required(VERSION 3.16)

project(GUI VERSION 1.0.0 LANGUAGES CXX)

# Enable automatic Qt features
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

# C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_PREFIX_PATH "/opt/homebrew/Cellar/qt/6.9.1" ${CMAKE_PREFIX_PATH})
# Find Qt6
find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets LinguistTools)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets LinguistTools)

# Source files
set(PROJECT_SOURCES
    main.cpp
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui
)

# Create executable
add_executable(GUI ${PROJECT_SOURCES})

target_link_libraries(GUI PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)


# # Link Qt and the backend library
# target_link_libraries(GUI
#     Qt6::Widgets
#     SensitivityStudy  # backend library target
# )

# === INSTALL RULES ===

# Install the binary to bin/
install(TARGETS GUI
    RUNTIME DESTINATION bin
)

# Optional: install resources if you have them
# install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/resources/
#     DESTINATION share/SensitivityApp/resources)

# === CPack CONFIGURATION FOR PACKAGING ===

include(InstallRequiredSystemLibraries)

# Basic metadata for packaging
set(CPACK_PACKAGE_NAME "SensitivityApp")
set(CPACK_PACKAGE_VENDOR "YourNameOrOrg")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "GUI for Sensitivity Study")
set(CPACK_PACKAGE_VERSION_MAJOR ${PROJECT_VERSION_MAJOR})
set(CPACK_PACKAGE_VERSION_MINOR ${PROJECT_VERSION_MINOR})
set(CPACK_PACKAGE_VERSION_PATCH 0)
set(CPACK_PACKAGE_CONTACT "your.email@example.com")

# Format: you can add TGZ, ZIP, DEB, RPM, etc.
set(CPACK_GENERATOR "TGZ")

# Platform-specific settings (optional, expand later)
# set(CPACK_DEBIAN_PACKAGE_MAINTAINER "YourName")
# set(CPACK_PACKAGE_INSTALL_DIRECTORY "SensitivityApp")

include(CPack)




