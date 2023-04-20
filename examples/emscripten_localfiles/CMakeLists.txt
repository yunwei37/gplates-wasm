project(file4test VERSION 2.3.0 LANGUAGES C CXX)
set(CMAKE_CXX_STANDARD 11)

add_executable(file4test main.cpp)
target_compile_options(file4test PUBLIC -O3 -std=c++11 --bind)
target_link_libraries(file4test PUBLIC embind)

if (EMSCRIPTEN)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -s EXPORTED_RUNTIME_METHODS=['ccall']")
endif()

install(TARGETS file4test
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})

