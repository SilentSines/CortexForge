# Startup sources
# C files shared between ARM and GCC
set(STARTUP_SOURCE
    ${CMAKE_CURRENT_LIST_DIR}/system_stm32f4xx.c
    ${CMAKE_CURRENT_LIST_DIR}/stm32f4xx_it.c
)

# Compiler-specific startup assembly
if(COMPILER_TYPE STREQUAL "GCC")
    set(STARTUP_ASM ${CMAKE_CURRENT_LIST_DIR}/GCC/startup_stm32f429xx.s)
    list(APPEND STARTUP_SOURCE
        ${CMAKE_CURRENT_LIST_DIR}/GCC/syscalls.c
        ${CMAKE_CURRENT_LIST_DIR}/GCC/sysmem.c
    )
else()
    set(STARTUP_ASM ${CMAKE_CURRENT_LIST_DIR}/ARM/startup_stm32f429xx.s)
endif()
