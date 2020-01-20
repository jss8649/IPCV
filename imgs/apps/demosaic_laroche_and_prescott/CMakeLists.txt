imgs_add_executable(demosaic_laroche_and_prescott 
  SOURCES
    demosaic_laroche_and_prescott.cpp
)

target_link_libraries(demosaic_laroche_and_prescott 
  Boost::filesystem 
  Boost::program_options 
  imgs::ipcv_demosaicing
  opencv_core
  opencv_highgui
  opencv_imgcodecs
)
