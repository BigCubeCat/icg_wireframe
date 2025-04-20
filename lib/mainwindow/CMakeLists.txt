# Важно: включаем .ui файл в список исходников.
qt_add_library(mainwindow STATIC mainwindow.cpp mainwindow.hpp mainwindow.ui)

target_link_libraries(
  mainwindow
  PUBLIC Qt6::Core
         Qt6::Widgets
         Qt6::Gui
         Qt6::Qml
         Qt6::Quick
         Qt6::QuickWidgets
         ${EIGEN3_INCLUDE_DIR}
         model
         editor
         splinelib)

target_include_directories(mainwindow PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
