imgs_add_executable(threshold 
  SOURCES
    threshold.cpp
)

target_link_libraries(threshold 
  Boost::filesystem 
  Boost::program_options 
  imgs::ipcv_otsus_threshold 
  imgs::ipcv_utils 
  opencv_core
  opencv_highgui
  opencv_imgcodecs
)
