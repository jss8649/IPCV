imgs_add_executable(histogram_enhance
  SOURCES
    histogram_enhance.cpp
)

target_link_libraries(histogram_enhance
  Boost::filesystem 
  Boost::program_options 
  imgs::ipcv_histogram_enhancement 
  imgs::ipcv_utils
  imgs::plot
  opencv_core
  opencv_highgui
  opencv_imgcodecs
  
)
