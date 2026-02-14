cmake_minimum_required(VERSION 4.1)
project(NTA)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(CMAKE_PREFIX_PATH ${CMAKE_PREFIX_PATH} "${VCPKG_INSTALLED_DIR}/${VCPKG_TARGET_TRIPLET}")

find_package(Qt6 COMPONENTS
        Core
        Gui
        Widgets
        REQUIRED)
find_package(spdlog CONFIG REQUIRED)

add_executable(NTA main.cpp)
target_link_libraries(NTA
        Qt::Core
        Qt::Gui
        Qt::Widgets
)

target_link_libraries(NTA spdlog::spdlog)