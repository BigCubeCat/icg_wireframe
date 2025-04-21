qt_add_library(canvas STATIC canvas.cpp canvas.hpp)

target_link_libraries(
  canvas
  PUBLIC Qt6::Core
         Qt6::Widgets
         Qt6::Gui
         Qt6::Qml
         Qt6::Quick
         Qt6::QuickWidgets
         Qt6::QuickControls2
         model
         splinelib)

target_include_directories(canvas PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
