# STM32F429 platform configuration
# Exports to parent scope: MCU_FLAGS, LINK_FLAGS, PROJECT_DEFINES

# --- MCU compile flags ---
if(COMPILER_TYPE STREQUAL "GCC")
    set(MCU_FLAGS
        -mcpu=cortex-m4
        -mthumb
        -mfpu=fpv4-sp-d16
        -mfloat-abi=hard
    )
else()
    set(MCU_FLAGS
        --target=arm-arm-none-eabi
        -mcpu=cortex-m4
        -mthumb
        -mfpu=fpv4-sp-d16
        -mfloat-abi=hard
    )
endif()
set(MCU_FLAGS ${MCU_FLAGS} PARENT_SCOPE)

# --- Linker flags ---
if(COMPILER_TYPE STREQUAL "GCC")
    set(LINK_FLAGS
        -mcpu=cortex-m4
        -mthumb
        -mfpu=fpv4-sp-d16
        -mfloat-abi=hard
        -T${CMAKE_CURRENT_LIST_DIR}/linker/gcc.ld
        -Wl,--gc-sections
        -Wl,-Map=${CMAKE_BINARY_DIR}/output/Project.map
        -Wl,--print-memory-usage
    )
    if(USE_MICROLIB)
        list(APPEND LINK_FLAGS --specs=nano.specs)
    else()
        list(APPEND LINK_FLAGS --specs=nosys.specs)
    endif()
else()
    set(LINK_FLAGS
        --cpu Cortex-M4.fp.sp
        --scatter=${CMAKE_CURRENT_LIST_DIR}/linker/armclang.sct
        --entry=Reset_Handler
        --map
        --list=${CMAKE_BINARY_DIR}/output/Project.map
        --info=summarysizes,sizes,totals
    )
    if(USE_MICROLIB)
        list(APPEND LINK_FLAGS --library_type=microlib)
    endif()
endif()
set(LINK_FLAGS ${LINK_FLAGS} PARENT_SCOPE)

# --- Platform defines ---
set(PROJECT_DEFINES
    USE_HAL_DRIVER
    STM32F429xx
)
set(PROJECT_DEFINES ${PROJECT_DEFINES} PARENT_SCOPE)

# --- Debug / Flash configuration (for .vscode template generation) ---
set(OPENOCD_INTERFACE "interface/cmsis-dap.cfg" PARENT_SCOPE)
set(OPENOCD_TARGET "target/stm32f4x.cfg" PARENT_SCOPE)
set(DEBUG_DEVICE "STM32F429" PARENT_SCOPE)
set(SVD_FILE "platform/${PLATFORM}/STM32F429.svd" PARENT_SCOPE)

# --- Vendor std lib ---
include(${CMAKE_CURRENT_LIST_DIR}/vendor/sources.cmake)
add_library(vendor OBJECT ${VENDOR_SOURCE})
target_compile_options(vendor PRIVATE ${MCU_FLAGS})
target_compile_definitions(vendor PRIVATE $<$<COMPILE_LANGUAGE:C>:${PROJECT_DEFINES}>)

# --- Startup (assembly + system + interrupts) ---
include(${CMAKE_CURRENT_LIST_DIR}/startup/sources.cmake)
add_library(startup OBJECT ${STARTUP_SOURCE})
target_compile_options(startup PRIVATE
    $<$<COMPILE_LANGUAGE:C>:${MCU_FLAGS}>
)
target_compile_definitions(startup PRIVATE
    $<$<COMPILE_LANGUAGE:C>:${PROJECT_DEFINES}>
)
if(COMPILER_TYPE STREQUAL "GCC")
    set_source_files_properties(${STARTUP_ASM} PROPERTIES LANGUAGE ASM)
else()
    set(ASM_FLAGS "--cpu Cortex-M4.fp")
    if(USE_MICROLIB)
        set(ASM_FLAGS "${ASM_FLAGS} --pd \"__MICROLIB SETA 1\"")
    endif()
    set_source_files_properties(${STARTUP_ASM} PROPERTIES
        COMPILE_FLAGS "${ASM_FLAGS}"
    )
endif()
target_sources(startup PRIVATE ${STARTUP_ASM})

# --- HAL ---
include(${CMAKE_CURRENT_LIST_DIR}/hal/sources.cmake)
add_library(hal OBJECT ${HAL_SOURCE})
target_compile_options(hal PRIVATE ${MCU_FLAGS})
target_compile_definitions(hal PRIVATE $<$<COMPILE_LANGUAGE:C>:${PROJECT_DEFINES}>)

# --- Optional: USB driver library ---
set(HAS_PLATFORM_USB FALSE PARENT_SCOPE)
if(EXISTS "${CMAKE_CURRENT_LIST_DIR}/usb_lib")
    set(HAS_PLATFORM_USB TRUE PARENT_SCOPE)
endif()
