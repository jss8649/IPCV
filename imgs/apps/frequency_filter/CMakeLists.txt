imgs_add_executable(frequency_filter 
  SOURCES
    frequency_filter.cpp
)

target_link_libraries(frequency_filter 
  Boost::filesystem 
  Boost::program_options 
  imgs::ipcv_frequency_filtering
  opencv_core
  opencv_highgui
  opencv_imgcodecs
)
