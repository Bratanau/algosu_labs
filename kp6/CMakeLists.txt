add_executable(gen gen.c)
add_executable(query query.c)
target_link_libraries(gen m)
target_link_libraries(query m)