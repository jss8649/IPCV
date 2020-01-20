imgs_add_executable(gamma_correct 
  SOURCES
    gamma_correct.cpp
)

target_link_libraries(gamma_correct 
  Boost::filesystem 
  Boost::program_options 
  imgs::ipcv_utils
  opencv_core
  opencv_highgui
  opencv_imgcodecs
)
