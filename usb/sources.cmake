# USB class sources
set(USB_SOURCE
    ${CMAKE_CURRENT_LIST_DIR}/usbd.c
    ${CMAKE_CURRENT_LIST_DIR}/audio/audio_class.c
    ${CMAKE_CURRENT_LIST_DIR}/audio/audio_desc.c
    ${CMAKE_CURRENT_LIST_DIR}/audio/usbd_audio.c
    ${CMAKE_CURRENT_LIST_DIR}/cdc/cdc_class.c
    ${CMAKE_CURRENT_LIST_DIR}/cdc/cdc_desc.c
    ${CMAKE_CURRENT_LIST_DIR}/cdc/usbd_cdc.c
)
