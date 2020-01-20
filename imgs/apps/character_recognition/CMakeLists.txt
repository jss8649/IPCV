imgs_add_executable(character_recognition 
  SOURCES
    character_recognition.cpp
)

imgs_add_executable(change_char_quality
  SOURCES
    change_char_quality.cpp
)

target_link_libraries(character_recognition 
  Boost::filesystem 
  Boost::program_options 
  imgs::ipcv_character_recognizing
  opencv_core
  opencv_highgui
  opencv_imgcodecs
)

target_link_libraries(change_char_quality
  Boost::filesystem
  Boost::program_options
  imgs::ipcv_character_recognizing
  imgs::ipcv_geometric_transformation
  imgs::ipcv_utils
  opencv_core
  opencv_highgui
  opencv_imgcodecs
  tesseract
  leptonica
)
