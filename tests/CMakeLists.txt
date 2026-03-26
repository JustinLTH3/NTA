cmake_minimum_required(VERSION 4.1)
project(NTA_Test)

enable_testing()
find_package(Qt6 COMPONENTS
        Test
        REQUIRED)

add_executable(NTA_Test
        ../src/space.cpp
        ../src/space.h
        createspacetest.cpp
        main.cpp
        createspacetest.h
        openspacetest.cpp
        openspacetest.h
        ../src/Note.h
        testcreatenote.cpp
        testcreatenote.h
        testgetnotes.cpp
        testgetnotes.h
        testaddlink.cpp
        testaddlink.h
        ../src/nstartwindow.cpp
        ../src/nstartwindow.h
        ../src/nstartwindow.ui
        ../src/ncreatespacedialog.cpp
        ../src/ncreatespacedialog.h
        ../src/ncreatespacedialog.ui
        createspacedialogtest.cpp
        createspacedialogtest.h
        startwindowtest.cpp
        startwindowtest.h
        ../src/nmainwindow.cpp
        ../src/nmainwindow.h
        ../src/nmainwindow.ui
        ../src/nwidget.cpp
        ../src/nwidget.h
        ../src/nwidget.ui
        testnwidget.cpp
        testnwidget.h
        NTestWidget.cpp
        NTestWidget.h
        ../src/nspacemanager.cpp
        ../src/nspacemanager.h
        spacemanagertest.cpp
        spacemanagertest.h
        ../src/nnoteexplorer.cpp
        ../src/nnoteexplorer.h
        ../src/nwidgetmanager.cpp
        ../src/nwidgetmanager.h
        nwidgetmanagertest.cpp
        nwidgetmanagertest.h
        ../src/neditorwidget.cpp
        ../src/neditorwidget.h
        ../src/nlinkwidget.cpp
        ../src/nlinkwidget.h
        ../src/nlinkmanager.cpp
        ../src/nlinkmanager.h
        ../src/nselectnotedialog.cpp
        ../src/nselectnotedialog.h
        ../src/nselectnotedialog.ui
        ../src/nbacklinkwidget.cpp
        ../src/nbacklinkwidget.h
        ../src/nrenderwidget.cpp
        ../src/nrenderwidget.h
        ../src/nmd.cpp
        ../src/nmd.h
)
target_link_libraries(NTA_Test
        Qt::Core
        Qt::Gui
        Qt::Widgets
        Qt::Test
)
target_link_libraries(NTA_Test SQLiteCpp)
target_link_libraries(NTA_Test spdlog::spdlog)
target_link_libraries(NTA_Test SQLite::SQLite3)
target_link_libraries(NTA_Test fts5_icu)
target_link_libraries(NTA_Test qtadvanceddocking-qt6)
add_test(NAME SpaceTest COMMAND SpaceTest)
