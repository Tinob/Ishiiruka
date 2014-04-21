set(SRCS	FramebufferManager.cpp
			GLUtil.cpp
			main.cpp
			NativeVertexFormat.cpp
			PerfQuery.cpp
			PixelShaderCache.cpp
			PostProcessing.cpp
			ProgramShaderCache.cpp
			RasterFont.cpp
			Render.cpp
			SamplerCache.cpp
			StreamBuffer.cpp
			TextureCache.cpp
			TextureConverter.cpp
			VertexShaderCache.cpp
			VertexManager.cpp)

set(LIBS	videocommon
			SOIL
			common
			${X11_LIBRARIES})
if(USE_EGL)
	set(LIBS ${LIBS}
		EGL)
endif()

if(USE_GLES3)
	set(LIBS ${LIBS}
		GLESv2)
	set(SRCS ${SRCS} GLFunctions.cpp)
else()
	set(LIBS ${LIBS}
		GLEW
		${OPENGL_LIBRARIES})
endif()

if(wxWidgets_FOUND)
	set(LIBS	${LIBS} ${wxWidgets_LIBRARIES})
endif(wxWidgets_FOUND)

if(NOT ${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
	set(LIBS	${LIBS} clrun)
endif()

if(${CMAKE_SYSTEM_NAME} MATCHES "FreeBSD" OR
	${CMAKE_SYSTEM_NAME} MATCHES "NetBSD")
	set(LIBS	${LIBS} usbhid)
endif()

add_library(videoogl STATIC ${SRCS})
target_link_libraries(videoogl ${LIBS})
