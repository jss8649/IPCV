imgs_add_executable(quantize 
  SOURCES
    quantize.cpp
)

target_link_libraries(quantize 
  Boost::filesystem 
  Boost::program_options 
  imgs::ipcv_quantization
  opencv_core
  opencv_highgui
  opencv_imgcodecs
)
