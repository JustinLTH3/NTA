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
add_test(NAME SpaceTest COMMAND SpaceTest)
