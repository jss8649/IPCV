cmake_minimum_required(VERSION 3.1)

project(imgs VERSION 0.0.0)

include(cmake/CMakeUtils.cmake)

imgs_set_library_prefix("libimgs_")

find_package(Boost REQUIRED 
  COMPONENTS 
    program_options 
    iostreams 
    filesystem 
    system
)
find_package(Eigen3 REQUIRED NO_MODULE)
find_package(OpenCV REQUIRED)
#find_package(Tesseract 5.0.0 REQUIRED)
find_package(Leptonica 1.79.0 REQUIRED)

#set(${Tesseract_INCLUDE_DIRS} /usr/local/include)
#set(${Tesseract_LIBRARIES} /usr/local/lib)
#add_library(tesseract SHARED IMPORTED)
#set_target_properties(tesseract PROPERTIES
#IMPORTED_LOCATION "/usr/local/lib/libtesseract.5.dylib"
#INTERFACE_INCLUDE_DIRECTORIES "/usr/local/include/tesseract"
#)

find_package( PkgConfig REQUIRED)
pkg_search_module( TESSERACT REQUIRED tesseract )
pkg_search_module( TESSERACT REQUIRED libraw )
include_directories( ${TESSERACT_INCLUDE_DIRS} )
link_directories( ${TESSERACT_LIBRARY_DIRS} )

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(imgs)
