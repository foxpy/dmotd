add_library(MotdData
    motd_uptime.c
    motd_loadavg.c
    motd_users.c
    motd_kernel.c
    motd_memory.c
    motd_swap.c
    motd_pids.c
    motd_storage.c
)
target_link_libraries(MotdData PUBLIC qc)
target_include_directories(MotdData INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
