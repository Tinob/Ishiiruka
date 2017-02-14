add_subdirectory(OGL)
add_subdirectory(Software)

if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
  add_subdirectory(DX9)
  add_subdirectory(DX11)
  add_subdirectory(D3D12)
endif()

if(NOT APPLE)
add_subdirectory(Vulkan)
endif()
# TODO: Add other backends here!
