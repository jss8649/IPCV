imgs_add_executable(plot2d
  SOURCES
    plot2d.cpp
)

target_link_libraries(plot2d
  imgs::plot 
  opencv_core
)
