cmake_minimum_required(VERSION 3.16)
project(Memoneando VERSION 1.0.8)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find Qt6
find_package(Qt6 REQUIRED COMPONENTS Core Widgets Gui Network)

# Set Qt policies
cmake_policy(SET CMP0071 NEW)

# Enable automoc for Qt
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# Set compile and link flags to avoid snap conflicts
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,--exclude-libs,ALL")
set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -Wl,--exclude-libs,ALL")

# Source files
set(SOURCES
    src/main.cpp
    src/mainwindow.cpp
    src/memecanvas.cpp
    src/layeritem.cpp
    src/settingsdialog.cpp
    src/systemtrayicon.cpp
    src/imagedownloader.cpp
    src/imagesearchwidget.cpp
    src/imageprovider.cpp
    src/pixabayprovider.cpp
    src/unsplashprovider.cpp
    src/pexelsprovider.cpp
    src/wikimediaprovider.cpp
    src/lorempicsumprovider.cpp
    src/colorpickerwidget.cpp
)

# Header files
set(HEADERS
    src/mainwindow.h
    src/memecanvas.h
    src/layeritem.h
    src/settingsdialog.h
    src/systemtrayicon.h
    src/imagedownloader.h
    src/imagesearchwidget.h
    src/imageprovider.h
    src/pixabayprovider.h
    src/unsplashprovider.h
    src/pexelsprovider.h
    src/wikimediaprovider.h
    src/lorempicsumprovider.h
    src/colorpickerwidget.h
)

# Resource files
qt6_add_resources(RESOURCE_FILES resources/resources.qrc)

# Create executable
add_executable(${PROJECT_NAME} 
    ${SOURCES} 
    ${HEADERS}
    ${RESOURCE_FILES}
)

# Link Qt libraries
target_link_libraries(${PROJECT_NAME} Qt6::Core Qt6::Widgets Qt6::Gui Qt6::Network)

# Force static linking to avoid snap conflicts
set_target_properties(${PROJECT_NAME} PROPERTIES
    LINK_FLAGS "-static-libgcc -static-libstdc++"
)

# Set output directory
set_target_properties(${PROJECT_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)

# Install target
install(TARGETS ${PROJECT_NAME}
    RUNTIME DESTINATION bin
)
