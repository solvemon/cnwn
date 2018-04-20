#include "cnwn/cliopt.h"

int main(int argc, char * argv[])
{
    cnwn_CliOption options[] = {
        {"a", "alpha", NULL, "This is the alpha option."},
        {"b", "beta", NULL, "This is the beta option."},
        {"x", "exxa", "exxaarg", "This is the exxa option."},
        {"z", "zeta", "zetaarg", "This is the zeta option."},
        {0}
    };
    cnwn_cli_options_print_help(options, NULL);
    int retparse = 0, argindex = 0, optindex;
    const char * optarg;
    while ((retparse = cnwn_cli_options_parse(options, argindex, argc, argv, &optindex, &optarg)) != 0) {
        if (retparse >= 0) {
            if (optindex >= 0) 
                printf("Valid option: %s = %s\n", argv[argindex], optarg != NULL ? optarg : "");
            else
                printf("Valid argument: %s\n", argv[argindex]);
        } else if (retparse == CNWN_CLI_OPTION_INVALID)
            printf("Invalid option: %s\n", argv[argindex]);
        else if (retparse == CNWN_CLI_OPTION_NOARG)
            printf("Noarg argument for option: %s\n", argv[argindex]);
        else
            printf("Unknown error for option: %s\n", argv[argindex]);
        argindex += (retparse < 0 ? 1 : retparse);
    }
    return 0;
}
