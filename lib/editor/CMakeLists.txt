# Важно: включаем .ui файл в список исходников.
qt_add_library(
  editor
  STATIC
  editor.cpp
  editor.hpp
  editor.ui
  point_item.hpp
  point_item.cpp)

target_link_libraries(
  editor
  PUBLIC Qt6::Core
         Qt6::Widgets
         Qt6::Gui
         Qt6::Qml
         Qt6::Quick
         Qt6::QuickWidgets
         Qt6::QuickControls2
         model)

target_include_directories(editor PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
