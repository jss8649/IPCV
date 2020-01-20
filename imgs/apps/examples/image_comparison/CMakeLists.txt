imgs_add_executable(image_comparison
  SOURCES
    image_comparison.cpp
)

target_link_libraries(image_comparison
  imgs::ipcv_utils 
  opencv_core
  opencv_highgui
  opencv_imgcodecs
  opencv_imgproc
)
