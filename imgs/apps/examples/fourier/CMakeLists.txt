imgs_add_executable(test_dft
  SOURCES
    test_dft.cpp
)

target_link_libraries(test_dft
  imgs::ipcv_utils 
  opencv_core
  opencv_highgui
  opencv_imgcodecs
)
