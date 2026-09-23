# BSP (Board Support Package)

include(${CMAKE_CURRENT_LIST_DIR}/sources.cmake)

add_library(bsp OBJECT ${BSP_SOURCE})
target_compile_options(bsp PRIVATE ${MCU_FLAGS})
target_compile_definitions(bsp PRIVATE $<$<COMPILE_LANGUAGE:C>:${PROJECT_DEFINES}>)
