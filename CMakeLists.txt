cmake_minimum_required(VERSION 3.19)
project(icg_wireframe LANGUAGES CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_FLAGS "-Wall -Wextra")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

find_package(Eigen3 REQUIRED)
find_package(
  Qt6 6.5 REQUIRED
  COMPONENTS Core
             Widgets
             Gui
             Qml
             Quick
             QuickWidgets
             QuickControls2
             WebEngineWidgets)
find_package(OpenMP)
if(OPENMP_FOUND)
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
  set(CMAKE_EXE_LINKER_FLAGS
      "${CMAKE_EXE_LINKER_FLAGS} ${OpenMP_EXE_LINKER_FLAGS}")
endif()

message(STATUS "Using Qt version: ${Qt6_VERSION}")

qt6_add_resources(RESOURCES resources.qrc)

qt_standard_project_setup()

qt_add_executable(icg_wireframe WIN32 MACOSX_BUNDLE src/main.cpp ${RESOURCES})

target_link_libraries(
  icg_wireframe
  PRIVATE Qt6::Core
          Qt6::Widgets
          Qt6::Gui
          Qt6::Qml
          Qt6::Quick
          Qt6::QuickWidgets
          Qt6::QuickControls2
          splinelib
          model
          mainwindow)

include(GNUInstallDirs)

install(
  TARGETS icg_wireframe
  BUNDLE DESTINATION .
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})

qt_generate_deploy_app_script(TARGET icg_wireframe OUTPUT_SCRIPT deploy_script
                              NO_UNSUPPORTED_PLATFORM_ERROR)
install(SCRIPT ${deploy_script})

add_subdirectory(lib/editor)
add_subdirectory(lib/model)
add_subdirectory(lib/mainwindow)
add_subdirectory(lib/spline)
