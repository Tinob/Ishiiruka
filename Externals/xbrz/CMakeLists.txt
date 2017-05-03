

set(SRCS
	xbrz.cpp
)

if(${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
	set_source_files_properties(${SRCS} PROPERTIES COMPILE_FLAGS -std=gnu++14)
endif()

add_library(xbrz STATIC ${SRCS})
if(APPLE OR ${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
endif()
