# Startup sources
# C files shared between ARM and GCC
set(STARTUP_SOURCE
    ${CMAKE_CURRENT_LIST_DIR}/system_at32f403a_407.c
    ${CMAKE_CURRENT_LIST_DIR}/at32f403a_407_int.c
)

# Compiler-specific startup assembly
if(COMPILER_TYPE STREQUAL "GCC")
    set(STARTUP_ASM ${CMAKE_CURRENT_LIST_DIR}/GCC/startup_at32f403a_407.s)
    list(APPEND STARTUP_SOURCE
        ${CMAKE_CURRENT_LIST_DIR}/GCC/syscalls.c
        ${CMAKE_CURRENT_LIST_DIR}/GCC/sysmem.c
    )
else()
    set(STARTUP_ASM ${CMAKE_CURRENT_LIST_DIR}/ARM/startup_at32f403a_407.s)
endif()
