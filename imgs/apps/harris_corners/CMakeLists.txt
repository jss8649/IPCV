imgs_add_executable(harris_corners
  SOURCES
    harris_corners.cpp
)

target_link_libraries(harris_corners
  Boost::filesystem 
  Boost::program_options 
  imgs::ipcv_corners 
  opencv_core
  opencv_highgui
  opencv_imgcodecs
  opencv_imgproc
)
