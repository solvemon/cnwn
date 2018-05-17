#include "cnwn/cnwna.h"
#include "cnwn/erf.h"

int main(int argc, char * argv[])
{
    if (cnwn_cnwna_has_version(argc, argv)) {
        cnwn_cnwna_print_version();
        return 0;
    } else if (cnwn_cnwna_has_help(argc, argv)) {
        cnwn_cnwna_print_help();
        return 0;
    }
    cnwn_CNWNASettings settings;
    int ret = cnwn_cnwna_settings_init(&settings, argc, argv);
    if (ret >= 0) {
        CNWN_RESOURCE_HANDLERS[CNWN_RESOURCE_TYPE_ERF] = CNWN_RESOURCE_HANDLER_ERF;
        CNWN_RESOURCE_HANDLERS[CNWN_RESOURCE_TYPE_MOD] = CNWN_RESOURCE_HANDLER_ERF;
        CNWN_RESOURCE_HANDLERS[CNWN_RESOURCE_TYPE_HAK] = CNWN_RESOURCE_HANDLER_ERF;
        CNWN_RESOURCE_HANDLERS[CNWN_RESOURCE_TYPE_NWM] = CNWN_RESOURCE_HANDLER_ERF;
        ret = cnwn_cnwna_execute(&settings);
        if (ret < 0)
            fprintf(stderr, "ERROR: %s\n", cnwn_get_error());
        cnwn_cnwna_settings_deinit(&settings);
        return 0;
    } 
    fprintf(stderr, "ERROR: %s\n", cnwn_get_error());
    return 2;
}
