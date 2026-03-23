cmake_minimum_required(VERSION 4.1)
project(NTA)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_PREFIX_PATH ${CMAKE_PREFIX_PATH} "${VCPKG_INSTALLED_DIR}/${VCPKG_TARGET_TRIPLET}")

find_package(Qt6 COMPONENTS
        Core
        Gui
        Widgets
        REQUIRED)
find_package(spdlog CONFIG REQUIRED)
find_package(unofficial-sqlite3 CONFIG REQUIRED)
find_package(SQLiteCpp CONFIG REQUIRED)
find_package(ICU REQUIRED COMPONENTS uc i18n)
if (NOT TARGET SQLite::SQLite3)
    add_library(SQLite::SQLite3 ALIAS unofficial::sqlite3::sqlite3)
endif ()
add_library(fts5_icu STATIC
        fts5-icu-tokenizer/src/fts5_icu.c
)

add_executable(NTA src/main.cpp
        src/space.cpp
        src/space.h
        src/Note.h
        src/nstartwindow.cpp
        src/nstartwindow.h
        src/nstartwindow.ui
        src/ncreatespacedialog.cpp
        src/ncreatespacedialog.h
        src/ncreatespacedialog.ui
        src/nmainwindow.cpp
        src/nmainwindow.h
        src/nmainwindow.ui
        src/nwidget.cpp
        src/nwidget.h
        src/nwidget.ui
        src/nspacemanager.cpp
        src/nspacemanager.h
        src/nnoteexplorer.cpp
        src/nnoteexplorer.h
        src/nwidgetmanager.cpp
        src/nwidgetmanager.h
        src/neditorwidget.cpp
        src/neditorwidget.h
        src/nlinkwidget.cpp
        src/nlinkwidget.h
        src/nlinkmanager.cpp
        src/nlinkmanager.h
        src/nselectnotedialog.cpp
        src/nselectnotedialog.h
        src/nselectnotedialog.ui
        src/nbacklinkwidget.cpp
        src/nbacklinkwidget.h
        src/nrenderwidget.cpp
        src/nrenderwidget.h
)
target_link_libraries(NTA
        Qt::Core
        Qt::Gui
        Qt::Widgets
)

target_link_libraries(NTA spdlog::spdlog)
target_link_libraries(NTA SQLite::SQLite3)
target_link_libraries(NTA SQLiteCpp)
target_link_libraries(fts5_icu PRIVATE SQLite::SQLite3 ICU::i18n ICU::uc)
target_link_libraries(NTA fts5_icu)

include_directories(Qt-Advanced-Docking-System)
target_include_directories(NTA PRIVATE "Qt-Advanced-Docking-System/src")
add_subdirectory(Qt-Advanced-Docking-System)
target_link_libraries(NTA qtadvanceddocking-qt6)

include_directories(md4c)
target_include_directories(NTA PRIVATE "md4c/src")
add_subdirectory(md4c)
target_sources(NTA PRIVATE md4c/src/md4c-html.c md4c/src/md4c.c md4c/src/entity.c md4c/src)

target_include_directories(fts5_icu PRIVATE fts5-icu-tokenizer/src)

add_subdirectory(tests)