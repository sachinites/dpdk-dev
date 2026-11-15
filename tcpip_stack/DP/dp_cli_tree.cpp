typedef struct _param_t_ param_t;

extern void dp_mount_show_interface_cli (param_t *show_hook);

void DP_Mount_show_CLI(param_t *mount_point) {

    dp_mount_show_interface_cli (mount_point);
}