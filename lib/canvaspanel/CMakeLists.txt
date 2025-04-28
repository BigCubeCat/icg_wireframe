qt_add_library(
  canvaspanel
  STATIC
  canvaspanel.cpp
  canvaspanel.hpp
  canvaspanel.ui
  gradient.hpp
  gradient.cpp)

target_link_libraries(
  canvaspanel
  PUBLIC Qt6::Core
         Qt6::Widgets
         Qt6::Gui
         Qt6::Qml
         Qt6::Quick
         Qt6::QuickWidgets
         ${EIGEN3_INCLUDE_DIR}
         model
         editor
         canvas
         splinelib)

target_include_directories(canvaspanel PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
