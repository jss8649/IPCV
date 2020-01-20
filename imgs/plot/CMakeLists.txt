set(SRCS
)

set(HDRS
  plot.h
)

add_subdirectory(plot2d)

imgs_add_library(plot SOURCES ${SRCS} HEADERS ${HDRS})

target_link_libraries(plot
  PUBLIC
    opencv_core
  PRIVATE
    imgs::gnuplot-iostream
)
