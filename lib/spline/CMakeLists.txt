add_library(splinelib STATIC spline.hpp spline.cpp point.hpp)
target_link_libraries(splinelib PRIVATE m)

target_include_directories(splinelib PUBLIC ${EIGEN3_INCLUDE_DIR})
