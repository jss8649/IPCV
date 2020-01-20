imgs_add_executable(awb_grayworld 
  SOURCES
    awb_grayworld.cpp
)

target_link_libraries(awb_grayworld 
  Boost::filesystem 
  Boost::program_options 
  imgs::ipcv_utils
  opencv_core
  opencv_highgui
  opencv_imgcodecs
)
