qt_add_library(
  model
  STATIC
  fileprocessor.hpp
  fileprocessor.cpp
  utils.hpp
  model.hpp
  model.cpp)

target_link_libraries(
  model
  PUBLIC Qt6::Core Qt6::Widgets Qt6::Gui
  PRIVATE splinelib)

target_include_directories(model PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
                                        ${OpenCV_INCLUDE_DIRS})
target_link_libraries(model PUBLIC ${OpenCV_LIBRARIES})
