
string(
  APPEND _opts
  "$<IF:$<CXX_COMPILER_ID:MSVC>,"
  "/W4;$<$<CONFIG:RELEASE>:/Ox>,"
  "-Wall;-Wextra;-Werror;"
  "$<$<CONFIG:RELEASE>:-O3>"
  "$<$<CXX_COMPILER_ID:Clang>:-stdlib=libc++>"
  ">"
  )
add_compile_options("${_opts}")


add_executable(main main.cpp test_float.cpp mytime.c half_test.cpp half.cpp float_test_vector.cpp half_vector_test.cpp half_print.cpp)
target_include_directories(main PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../include)
