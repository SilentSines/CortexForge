# Utils (task_manager, msg_manager, page_manager, rand)

include(${CMAKE_CURRENT_LIST_DIR}/sources.cmake)

add_library(utils OBJECT ${UTILS_SOURCE})
target_compile_options(utils PRIVATE ${MCU_FLAGS})
target_compile_definitions(utils PRIVATE $<$<COMPILE_LANGUAGE:C>:${PROJECT_DEFINES}>)
