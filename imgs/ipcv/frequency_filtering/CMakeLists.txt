imgs_add_library(ipcv_frequency_filtering
  SOURCES
    FrequencyFilter.cpp
  HEADERS
    FrequencyFilter.h
)

target_link_libraries(ipcv_frequency_filtering 
  PUBLIC 
    opencv_core
    opencv_highgui
    ipcv_utils
)
