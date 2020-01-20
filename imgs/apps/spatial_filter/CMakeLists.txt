imgs_add_executable(spatial_filter 
  SOURCES
    spatial_filter.cpp
)

target_link_libraries(spatial_filter 
  Boost::filesystem 
  Boost::program_options 
  imgs::ipcv_spatial_filtering
  opencv_core
  opencv_highgui
  opencv_imgcodecs
)
