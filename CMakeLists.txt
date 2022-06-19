cmake_minimum_required(VERSION 3.5)

project(Morphing LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_compile_options(-Wall -Wextra -Werror)

find_package(Qt5 COMPONENTS Widgets OpenGL REQUIRED)

add_executable(Morphing
	main.cpp
        mainwindow.h mainwindow.ui mainwindow.cpp 
	openglpicture.h openglpicture.cpp
	geometryengine.h geometryengine.cpp
	shaders.qrc
)

target_link_libraries(Morphing PRIVATE
    Qt5::Core Qt5::Widgets Qt5::OpenGL)


