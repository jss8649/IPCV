 imgs_add_executable(transform_rst
  SOURCES
    transform_rst.cpp
)

 imgs_add_executable(transform_test
  SOURCES
    transform_test.cpp
)

target_link_libraries(transform_rst
  Boost::filesystem 
  Boost::program_options 
  imgs::ipcv_geometric_transformation
  opencv_core
  opencv_calib3d
  opencv_highgui
  opencv_imgcodecs
)

target_link_libraries(transform_test
  Boost::filesystem
  Boost::program_options
  imgs::ipcv_geometric_transformation
  opencv_core
  opencv_calib3d
  opencv_highgui
  opencv_imgcodecs
)
