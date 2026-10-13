# Startup sources
# C files shared between ARM and GCC
set(STARTUP_SOURCE
    ${CMAKE_CURRENT_LIST_DIR}/x.c
)

# Compiler-specific startup assembly
if(COMPILER_TYPE STREQUAL "GCC")
    set(STARTUP_ASM ${CMAKE_CURRENT_LIST_DIR}/GCC/startup_x.s)
    list(APPEND STARTUP_SOURCE
        ${CMAKE_CURRENT_LIST_DIR}/GCC/syscalls.c
        ${CMAKE_CURRENT_LIST_DIR}/GCC/sysmem.c
    )
else()
    set(STARTUP_ASM ${CMAKE_CURRENT_LIST_DIR}/ARM/startup_x.s)
endif()
