cmake_minimum_required(VERSION 3.20)

list(APPEND CMAKE_PREFIX_PATH "$ENV{QT_DIR}/lib/cmake/Qt6/")

project(mygl VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS
  Core
  Gui
  Widgets
  OpenGLWidgets
  OpenGL
  )

find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS
  Core
  Gui
  Widgets
  OpenGLWidgets
  OpenGL
  )

add_subdirectory(src)
