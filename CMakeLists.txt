cmake_minimum_required(VERSION 3.5)

project(QuantProgram VERSION 0.1 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_compile_options(-Wall -Wextra -pedantic -Werror)

find_package(Qt5 COMPONENTS Widgets REQUIRED)
find_package(GTest 1.10 REQUIRED)

set(PROJECT_SOURCES
        main.cpp
        mainwindow.cpp
        mainwindow.h
        table_model.cpp
        table_model.h
        item_delegate.cpp
        item_delegate.h
        mainwindow.ui
)

set(STORAGE_TARGET storage)

add_library(${STORAGE_TARGET} storage.cpp storage.h)
target_link_libraries(${STORAGE_TARGET} PRIVATE Qt5::Widgets)

add_executable(${STORAGE_TARGET}_tests storage_tests.cpp)
target_link_libraries(${STORAGE_TARGET}_tests PRIVATE GTest::GTest Qt5::Widgets ${STORAGE_TARGET})

add_executable(${PROJECT_NAME} ${PROJECT_SOURCES})
target_link_libraries(${PROJECT_NAME} PRIVATE Qt5::Widgets ${STORAGE_TARGET})
