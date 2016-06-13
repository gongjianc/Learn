CI_DECLARE_MOD_DATA ci_service_module_t service = {
    "echo",
    "Echo demo service",
    ICAP_RESPMOD | ICAP_REQMOD,
    echo_init_service,
    NULL,
    echo_close_service,
    echo_init_request_data,
    echo_release_requese_data,
    echo_check_preview_handler,
    echo_end_of_data_handler,
    echo_io,
    NULL,
    NULL
};

struct echo_req_data{
    ci_ring_buf_t *body;
    int eof;
};

int echo_init_service(ci_service_xdata_t *srv_xdata,
        struct ci_server_conf *server_conf)
{
    ci_debug_printf(5, "initialization of echo module....\n");

    ci_service_set_preview(srv_xdata, 1024);

    ci_service_enable_204
