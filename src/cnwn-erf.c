#include "cnwn/erf.h"

#include <getopt.h>

const cnwn_CliOption GENERAL_OPTIONS[] = {
    {"h", false, "Show this help."},
    {"v", false, "Show version."},
    {"q", false, "Supress output to stdout."},
    {0}
};

const cnwn_CliOption LIST_OPTIONS[] = {
    {"l", false, "Long format."},
    {0}
};

const cnwn_CliOption EXTRACT_OPTIONS[] = {
    {"o", false, "Output directory."},
    {0}
};

const cnwn_CliOption ARCHIVE_OPTIONS[] = {
    {0}
};

void print_help(void)
{
    printf("cnwn-erf [options] <list|extract|archive> <ERF file> [file spec]\n");
    printf("Version: %d.%d.%d", VERSION_MAJOR, VERSION_MINOR, VERSION_PATCH);
    printf("\n");
    printf("  General options:\n");
    cnwn_print_cli_options(GENERAL_OPTIONS, "    ");
    printf("\n");
    printf("  List command options (file spec will only list matched files):\n");
    cnwn_print_cli_options(LIST_OPTIONS, "    ");
    printf("\n");
    printf("  Extract command options (file spec will only extract matched files):\n");
    cnwn_print_cli_options(EXTRACT_OPTIONS, "    ");
    printf("\n");
    printf("  Archive command options (file spec will add files):\n");
    cnwn_print_cli_options(ARCHIVE_OPTIONS, "    ");
    printf("\n");
}


int main(int argc, char * argv[])
{
    const cnwn_CliOption * options = GENERAL_OPTIONS;
    for (int i = 1; i < argc; i++) {
        const char * arg;
        int optindex = cnwn_parse_cli_option(options, argv[i], i < argc - 1 ? argv[i + 1] : NULL, &arg);
        if (optindex >= 0) {
            if (options == GENERAL_OPTIONS) {
                if (optindex == 0) {
                    print_help();
                    return 0;
                } else if (optindex == 1) {
                    printf("%d.%d.%d\n", VERSION_MAJOR, VERSION_MINOR, VERSION_PATCH);
                    return 0;
                }
            }
            if (CNWN_CLI_OPTION_IS_SHORT(argv[i]) && arg != NULL)
                i++;
        } else if (optindex == CNWN_CLI_OPTION_ARG) {
            if (options == GENERAL_OPTIONS) {
                if (arg[0] == 'l')
                    options = LIST_OPTIONS;
                else if (arg[0] == 'e')
                    options = EXTRACT_OPTIONS;
                else if (arg[0] == 'a')
                    options = ARCHIVE_OPTIONS;
            } else
                printf("INVALID ARGUMENT\n");
        } else if (optindex == CNWN_CLI_OPTION_INVALID) {
            fprintf(stderr, "ERROR: invalid option '%s' for argument %d\n", argv[i], i);
            return 1;
        } else if (optindex == CNWN_CLI_OPTION_INCOMPLETE) {
            fprintf(stderr, "ERROR: option '%s' for argument %d requires a value\n", argv[i], i);
            return 1;
        }
    }
    return 0;
}
