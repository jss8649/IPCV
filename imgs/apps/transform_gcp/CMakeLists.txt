 imgs_add_executable(transform_gcp
  SOURCES
    transform_gcp.cpp
)

target_link_libraries(transform_gcp
  Boost::filesystem 
  Boost::program_options 
  imgs::ipcv_geometric_transformation 
  opencv_core
  opencv_highgui
  opencv_imgcodecs
)
