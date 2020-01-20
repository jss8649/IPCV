imgs_add_executable(dist
  SOURCES
    dist.cpp
)

target_link_libraries(dist
  imgs::ipcv_utils 
  opencv_core
  opencv_highgui
  opencv_imgcodecs
)
