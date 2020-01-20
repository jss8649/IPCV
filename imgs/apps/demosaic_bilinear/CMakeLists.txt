imgs_add_executable(demosaic_bilinear 
  SOURCES
    demosaic_bilinear.cpp
)

target_link_libraries(demosaic_bilinear 
  Boost::filesystem 
  Boost::program_options 
  imgs::ipcv_demosaicing
  imgs::ipcv_histogram_enhancement
  opencv_core
  opencv_highgui
  opencv_imgcodecs
  opencv_imgproc
  imgs::ipcv_utils 
)
