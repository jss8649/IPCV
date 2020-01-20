imgs_add_executable(fast_corners
  SOURCES
    fast_corners.cpp
)

target_link_libraries(fast_corners
  Boost::filesystem 
  Boost::program_options 
  imgs::ipcv_corners 
  opencv_core
  opencv_highgui
  opencv_imgcodecs
  opencv_features2d
)
