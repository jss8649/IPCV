imgs_add_library(ipcv_character_recognizing
  SOURCES
    CharacterRecognition.cpp
  HEADERS
    CharacterRecognition.h
)

target_link_libraries(ipcv_character_recognizing 
  PUBLIC 
    opencv_core
    opencv_imgproc
    opencv_highgui
    ipcv_utils
)
