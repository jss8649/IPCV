imgs_add_executable(plot_histogram 
  SOURCES
    plot_histogram.cpp
)

target_link_libraries(plot_histogram 
  Boost::filesystem 
  Boost::program_options 
  imgs::ipcv_utils 
  imgs::plot 
  opencv_core
  opencv_imgcodecs
)
