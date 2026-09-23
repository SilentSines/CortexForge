cmake_minimum_required(VERSION 3.21)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(CortexForge C ASM)

set(PLATFORM "" CACHE STRING "Target platform")

if(NOT PLATFORM)
    message(FATAL_ERROR "PLATFORM not set, use -DPLATFORM=at32f403a")
endif()

set(PROJECT_ROOT "${CMAKE_CURRENT_LIST_DIR}")
set(PLATFORM_DIR "${PROJECT_ROOT}/platform/${PLATFORM}")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/output")

set(PROJECT_OPT_LEVEL 0 CACHE STRING "Optimization level (0-3)")
option(USE_MICROLIB "Link with ARMCLANG microlib instead of the standard C library" ON)
if(WIN32)
    set(_openocd_scripts_default "D:/OpenOCD/scripts")
    set(_openocd_exe_default "D:/OpenOCD/bin-windows_x86/openocd.exe")
else()
    set(_openocd_scripts_default "")
    set(_openocd_exe_default "openocd")
endif()
set(OPENOCD_SCRIPTS_DIR "${_openocd_scripts_default}" CACHE PATH "OpenOCD scripts directory")
set(OPENOCD_EXECUTABLE "${_openocd_exe_default}" CACHE FILEPATH "OpenOCD executable")

# --- Detect compiler type ---
if(CMAKE_C_COMPILER_ID STREQUAL "Clang" OR CMAKE_C_COMPILER_ID STREQUAL "ARMClang")
    set(COMPILER_TYPE "ARM" CACHE STRING "Compiler type" FORCE)
    set(COMPILER_DEFINE "ARM")
elseif(CMAKE_C_COMPILER_ID STREQUAL "GNU")
    set(COMPILER_TYPE "GCC" CACHE STRING "Compiler type" FORCE)
    set(COMPILER_DEFINE "GCC")
else()
    set(COMPILER_TYPE "UNKNOWN" CACHE STRING "Compiler type" FORCE)
    set(COMPILER_DEFINE "UNKNOWN")
endif()

message(STATUS "Platform: ${PLATFORM}")
message(STATUS "Compiler: ${CMAKE_C_COMPILER_ID} (${COMPILER_TYPE})")
message(STATUS "C compiler: ${CMAKE_C_COMPILER}")
message(STATUS "ASM compiler: ${CMAKE_ASM_COMPILER}")
message(STATUS "Linker: ${CMAKE_LINKER}")
message(STATUS "Image converter: ${CMAKE_OBJCOPY}")

# --- Include directories (visible to all subdirectories) ---
include_directories(
    "${PROJECT_ROOT}/platform/cmsis"
    "${PLATFORM_DIR}/startup"
    "${PLATFORM_DIR}/vendor/inc"
    "${PLATFORM_DIR}/hal"
    "${PROJECT_ROOT}/bsp"
    "${PROJECT_ROOT}/utils"
)

# Optional: usb include
if(EXISTS "${PROJECT_ROOT}/usb")
    include_directories("${PROJECT_ROOT}/usb")
endif()

# Optional: platform usb_lib include
if(EXISTS "${PLATFORM_DIR}/usb_lib/inc")
    include_directories("${PLATFORM_DIR}/usb_lib/inc")
endif()

# --- Common compile options ---
add_compile_options(
    $<$<COMPILE_LANGUAGE:C>:-fshort-enums>
    $<$<COMPILE_LANGUAGE:C>:-fshort-wchar>
    $<$<COMPILE_LANGUAGE:C>:-O${PROJECT_OPT_LEVEL}>
    $<$<COMPILE_LANGUAGE:C>:-g>
)

# GCC: also pass -fshort-wchar to linker to silence wchar_t size mismatch
if(COMPILER_TYPE STREQUAL "GCC")
    add_link_options(-Wl,--no-wchar-size-warning)
endif()

# --- Global compiler define (C only, armasm uses --pd) ---
add_compile_definitions($<$<COMPILE_LANGUAGE:C>:${COMPILER_DEFINE}>)

# --- Output suffix ---
if(COMPILER_TYPE STREQUAL "GCC")
    set(PROJECT_SUFFIX ".elf")
else()
    set(PROJECT_SUFFIX ".axf")
endif()

# --- Subdirectories ---
# Platform exports: MCU_FLAGS, LINK_FLAGS, PROJECT_DEFINES,
#                   OPENOCD_INTERFACE, OPENOCD_TARGET, DEBUG_DEVICE, SVD_FILE
add_subdirectory(platform/${PLATFORM})
add_subdirectory(bsp)
add_subdirectory(utils)

# Optional USB: only if platform has usb_lib directory
set(USB_OBJ "")
if(HAS_PLATFORM_USB)
    set(USB_BASE_DIRS
        "${PLATFORM_DIR}/usb_lib"
        "${PROJECT_ROOT}/usb"
    )
    foreach(_dir ${USB_BASE_DIRS})
        if(EXISTS "${_dir}/sources.cmake")
            include("${_dir}/sources.cmake")
        endif()
    endforeach()
    foreach(_lib USB_LIB USB)
        if(DEFINED ${_lib}_SOURCE)
            string(TOLOWER ${_lib} _lib_lower)
            add_library(${_lib_lower} OBJECT ${${_lib}_SOURCE})
            target_compile_options(${_lib_lower} PRIVATE ${MCU_FLAGS})
            target_compile_definitions(${_lib_lower} PRIVATE $<$<COMPILE_LANGUAGE:C>:${PROJECT_DEFINES}>)
            set(USB_OBJ ${USB_OBJ} $<TARGET_OBJECTS:${_lib_lower}>)
        endif()
    endforeach()
else()
    message(STATUS "USB: platform has no usb_lib, skipping USB")
endif()

# --- Generate VS Code configs from templates ---
if(COMPILER_TYPE STREQUAL "GCC")
    set(BUILD_OUTPUT_DIR ".build/${PLATFORM}-gcc")
    set(BUILD_PRESET "${PLATFORM}-gcc")
else()
    set(BUILD_OUTPUT_DIR ".build/${PLATFORM}-armclang")
    set(BUILD_PRESET "${PLATFORM}-armclang")
endif()
if(OPENOCD_SCRIPTS_DIR)
    set(OPENOCD_S_ARGS "\"-s\",\n                \"${OPENOCD_SCRIPTS_DIR}\",")
else()
    set(OPENOCD_S_ARGS "")
endif()
message(STATUS "OpenOCD: ${OPENOCD_EXECUTABLE} ${OPENOCD_S_ARGS}")

# Format PROJECT_DEFINES as JSON array for c_cpp_properties.json
set(CPP_DEFINES_JSON "")
set(_first TRUE)
foreach(_def ${PROJECT_DEFINES})
    if(NOT _first)
        string(APPEND CPP_DEFINES_JSON ", ")
    endif()
    string(APPEND CPP_DEFINES_JSON "\"${_def}\"")
    set(_first FALSE)
endforeach()

configure_file(
    "${PROJECT_ROOT}/.vscode/launch.json.in"
    "${PROJECT_ROOT}/.vscode/launch.json"
    @ONLY
)
configure_file(
    "${PROJECT_ROOT}/.vscode/tasks.json.in"
    "${PROJECT_ROOT}/.vscode/tasks.json"
    @ONLY
)
configure_file(
    "${PROJECT_ROOT}/.vscode/settings.json.in"
    "${PROJECT_ROOT}/.vscode/settings.json"
    @ONLY
)
configure_file(
    "${PROJECT_ROOT}/.vscode/c_cpp_properties.json.in"
    "${PROJECT_ROOT}/.vscode/c_cpp_properties.json"
    @ONLY
)

# --- Executable ---
add_executable(${PROJECT_NAME}
    main.c
    $<TARGET_OBJECTS:vendor>
    $<TARGET_OBJECTS:startup>
    $<TARGET_OBJECTS:hal>
    $<TARGET_OBJECTS:bsp>
    $<TARGET_OBJECTS:utils>
    ${USB_OBJ}
)

target_compile_definitions(${PROJECT_NAME} PRIVATE
    $<$<COMPILE_LANGUAGE:C>:${PROJECT_DEFINES}>
)

target_compile_options(${PROJECT_NAME} PRIVATE
    ${MCU_FLAGS}
)

target_link_options(${PROJECT_NAME} PRIVATE
    ${LINK_FLAGS}
)

set_target_properties(${PROJECT_NAME} PROPERTIES
    OUTPUT_NAME Project
    SUFFIX ${PROJECT_SUFFIX}
)

# --- Post-build: generate hex and bin ---
if(COMPILER_TYPE STREQUAL "GCC")
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_OBJCOPY} -Oihex $<TARGET_FILE:${PROJECT_NAME}> ${CMAKE_BINARY_DIR}/output/Project.hex
        COMMAND ${CMAKE_OBJCOPY} -Obinary $<TARGET_FILE:${PROJECT_NAME}> ${CMAKE_BINARY_DIR}/output/Project.bin
        COMMAND ${CMAKE_SIZE} $<TARGET_FILE:${PROJECT_NAME}>
        VERBATIM
    )
else()
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_OBJCOPY} --i32combined --output=${CMAKE_BINARY_DIR}/output/Project.hex $<TARGET_FILE:${PROJECT_NAME}>
        COMMAND ${CMAKE_OBJCOPY} --bin --output=${CMAKE_BINARY_DIR}/output/Project.bin $<TARGET_FILE:${PROJECT_NAME}>
        VERBATIM
    )
endif()

# --- Flash target ---
if(OPENOCD_SCRIPTS_DIR)
    set(OPENOCD_SCRIPTS_ARG -s ${OPENOCD_SCRIPTS_DIR})
else()
    set(OPENOCD_SCRIPTS_ARG "")
endif()
add_custom_target(flash
    COMMAND ${OPENOCD_EXECUTABLE}
        ${OPENOCD_SCRIPTS_ARG}
        -f ${OPENOCD_INTERFACE}
        -f ${OPENOCD_TARGET}
        -c "program ${CMAKE_BINARY_DIR}/output/Project.hex verify reset exit"
    DEPENDS ${PROJECT_NAME}
    WORKING_DIRECTORY ${PROJECT_ROOT}
    VERBATIM
)
