idf_component_register( SRCS "bt_app_hf.c" "bt_app_core.c" "app_hf_msg_set.c"
                        REQUIRES typedefs bt sgtl5000 console esp_ringbuf
                        INCLUDE_DIRS "include")