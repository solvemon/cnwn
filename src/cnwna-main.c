#include "cnwn/cnwna.h"

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
        ret = cnwn_cnwna_execute(&settings);
        if (ret < 0)
            fprintf(stderr, "ERROR: %s\n", cnwn_get_error());
        cnwn_cnwna_settings_deinit(&settings);
        return 0;
    } 
    fprintf(stderr, "ERROR: %s\n", cnwn_get_error());
    return 2;
}
