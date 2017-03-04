

set(SRCS
	xbrz.cpp
)

if(${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
	set_source_files_properties(${SRCS} PROPERTIES COMPILE_FLAGS -std=gnu++14)
endif()

add_library(xbrz STATIC ${SRCS})
