#include "cnwn/erf.h"
#include "cnwn/cliopt.h"

#include <getopt.h>

const cnwn_CliOption CNWN_ERF_OPTIONS_GENERAL[] = {
    {"h", "help", NULL, "Show this help."},
    {"V", "version", NULL, "Show the version."},
    {"v", "verbose", NULL, "Verbose output."},
    {"e", "extended", NULL, "Use extended regular expressions."},
    {0}
};

const cnwn_CliOption CNWN_ERF_OPTIONS_LIST[] = {
    {"h", "help", NULL, "Show command specific help."},
    {"l", "long-format", NULL, "Long format."},
    {0}
};

const cnwn_CliOption CNWN_ERF_OPTIONS_EXTRACT[] = {
    {"h", "help", NULL, "Show command specific help."},
    {"f", "force", NULL, "Force overwrites."},
    {"q", "no-quit", NULL, "Don't quit on errors."},
    {"d", "output-dir", "path", "Specify an output directory."},
    {0}
};

const cnwn_CliOption CNWN_ERF_OPTIONS_ARCHIVE[] = {
    {"h", "help", NULL, "Show command specific help."},
    {"q", "no-quit", NULL, "Don't quit on errors."},
    {"d", "input-dir", "path", "Specify an input directory."},
    {0}
};

void print_help(const cnwn_CliOption * options)
{
    if (options == CNWN_ERF_OPTIONS_GENERAL) {
        printf("cnwn-erf [GENERAL OPTIONS] <ERF FILE> <list|extract|archive> [COMMAND OPTIONS] [REGULAR EXPRESSIONS ...]\n");
        printf("Version: %d.%d.%d", VERSION_MAJOR, VERSION_MINOR, VERSION_PATCH);
        printf("\n");
        cnwn_cli_options_print_help(options, NULL);
    }
    // printf("\n");
    // printf("  'list', 'l' or 'ls':\n");
    // printf("    FILE is the ERF file to list the contents of.\n");
    // printf("    REGEXP will filter the entries to list.\n");
    // cnwn_print_cli_options(LIST_OPTIONS, "    ");
    // printf("\n");
    // printf("  'extract', 'e' or 'x':\n");
    // printf("    FILE is the ERF file to extract.\n");
    // printf("    REGEXP will filter the entries to extract.\n");
    // cnwn_print_cli_options(EXTRACT_OPTIONS, "    ");
    // printf("\n");
    // printf("  'archive' or 'a':\n");
    // printf("    FILE is the ERF file to archive.\n");
    // printf("    REGEXP will filter the files to archive.\n");
    // cnwn_print_cli_options(ARCHIVE_OPTIONS, "    ");
    // printf("\n");
    // printf("  Multiple REGEXP's are allowed (OR operator).\n");
    // printf("\n");
}


int main(int argc, char * argv[])
{
    int retparse = 0, argindex = 1, optindex;
    const char * optarg;
    const cnwn_CliOption * options = CNWN_ERF_OPTIONS_GENERAL;
    print_help(options);
    while ((retparse = cnwn_cli_options_parse(options, argindex, argc, argv, &optindex, &optarg)) != 0) {
        if (retparse < 0) {
            if (retparse == CNWN_CLI_OPTION_INVALID) 
                fprintf(stderr, "ERROR: Invalid option (argindex=%d): %s\n", argindex, argv[argindex]);
            else if (retparse == CNWN_CLI_OPTION_MISSING) 
                fprintf(stderr, "ERROR: Missing argument for option (argindex=%d): %s\n", argindex, argv[argindex]);
            else
                fprintf(stderr, "Unknown error for option (argindex=%d): %s\n", argindex, argv[argindex]);
            return 1;
        } 
        if (optindex >= 0) {
            
            printf("Valid option: %s = %s\n", argv[argindex], optarg != NULL ? optarg : "");
        } else {
            printf("Valid argument: %s\n", argv[argindex]);
        }
        argindex += retparse;
    }
    return 0;
}
