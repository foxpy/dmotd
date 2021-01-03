cmake_minimum_required(VERSION 3.1)
project(Dmotd)

add_subdirectory(qc)
add_subdirectory(motd_data)
add_executable(dmotd dmotd.c)
target_link_libraries(dmotd PUBLIC qc MotdData)
