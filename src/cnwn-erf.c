#include "cnwn/erf.h"
#include "cnwn/cliopt.h"

#include <getopt.h>

typedef struct {
    const char * erf_path;
    char command;
    int regexp_start;
    bool help;
    bool version;
    bool verbose;
    bool extended;
    bool no_quit;
    bool recurse;
    bool no_color;
    bool force;
    bool info;
    bool truncate;
    const char * dir;
} cnwn_ERFOptions;

const cnwn_CliOption CNWN_ERF_OPTIONS_GENERAL[] = {
    {"h", "help", NULL, "Show this help."},
    {"V", "version", NULL, "Show the version."},
    {"v", "verbose", NULL, "Verbose output."},
    {"e", "extended", NULL, "Use extended regular expressions."},
    {"q", "no-quit", NULL, "Don't quit on errors."},
    {"r", "recurse", NULL, "Recurse into sub-ERF/directory."},
    {"c", "no-color", NULL, "Disable color output for stdout."},
    {0}
};

const cnwn_CliOption CNWN_ERF_OPTIONS_LIST[] = {
    {"i", "info", NULL, "Only show ERF file info."},
    {0}
};

const cnwn_CliOption CNWN_ERF_OPTIONS_EXTRACT[] = {
    {"f", "force", NULL, "Force overwrites."},
    {"d", "output-dir", "path", "Specify an output directory."},
    {0}
};

const cnwn_CliOption CNWN_ERF_OPTIONS_ARCHIVE[] = {
    {"f", "force", NULL, "Force overwrites."},
    {"t", "truncate", NULL, "Truncate existing ERF file."},
    {"d", "input-dir", "path", "Specify an input directory."},
    {0}
};

void print_help(const cnwn_CliOption * options)
{
    if (options == CNWN_ERF_OPTIONS_GENERAL) {
        printf("cnwn-erf [GENERAL OPTIONS] <list|extract|archive> <ERF FILE> [COMMAND OPTIONS] [REGULAR EXPRESSIONS ...]\n");
        printf("Version: %d.%d.%d", VERSION_MAJOR, VERSION_MINOR, VERSION_PATCH);
        printf("\n");
        cnwn_cli_options_print_help(options, NULL);
        printf("Use -h/--help after a command to get more help.\n");
        printf("\n");
    } else if (options == CNWN_ERF_OPTIONS_LIST) {
        printf("cnwn-erf [GENERAL OPTIONS] <list|ls|l> <ERF FILE> [COMMAND OPTIONS] [REGULAR EXPRESSIONS ...]\n");
        printf("List contents of an ERF file.\n");
        printf("\n");
        cnwn_cli_options_print_help(options, NULL);
        printf("\n");
    } else if (options == CNWN_ERF_OPTIONS_EXTRACT) {
        printf("cnwn-erf [GENERAL OPTIONS] <extract|x|e> <ERF FILE> [COMMAND OPTIONS] [REGULAR EXPRESSIONS ...]\n");
        printf("Extract contents of an ERF file.\n");
        printf("\n");
        cnwn_cli_options_print_help(options, NULL);
        printf("\n");
    } else if (options == CNWN_ERF_OPTIONS_ARCHIVE) {
        printf("cnwn-erf [GENERAL OPTIONS] <archive|add|a> <ERF FILE> [COMMAND OPTIONS] [REGULAR EXPRESSIONS ...]\n");
        printf("Archive contents to an ERF file.\n");
        printf("\n");
        cnwn_cli_options_print_help(options, NULL);
        printf("\n");
    }
}

int parse_arguments(int argc, char * argv[], cnwn_ERFOptions * ret_parsed_options)
{
    cnwn_ERFOptions parsed_options = {0};
    int retparse = 0, argindex = 1, optindex;
    const char * optarg;
    const cnwn_CliOption * options = CNWN_ERF_OPTIONS_GENERAL;
    while ((retparse = cnwn_cli_options_parse(options, argindex, argc, argv, &optindex, &optarg)) != 0) {
        bool general_used = options == CNWN_ERF_OPTIONS_GENERAL;
        if (retparse < 0) {
            retparse = cnwn_cli_options_parse(CNWN_ERF_OPTIONS_GENERAL, argindex, argc, argv, &optindex, &optarg);
            general_used = true;
        }
        if (retparse < 0) {
            if (retparse == CNWN_CLI_OPTION_INVALID) 
                fprintf(stderr, "ERROR: Invalid option (argindex=%d): %s\n", argindex, argv[argindex]);
            else if (retparse == CNWN_CLI_OPTION_MISSING) 
                fprintf(stderr, "ERROR: Missing argument for option (argindex=%d): %s\n", argindex, argv[argindex]);
            else
                fprintf(stderr, "ERROR: Unknown error for option (argindex=%d): %s\n", argindex, argv[argindex]);
            return -1;
        } 
        if (optindex >= 0) {
            if (general_used) {
                if (optindex == 0) {
                    parsed_options.help = true;
                    break;
                } else if (optindex == 1) {
                    parsed_options.version = true;
                    break;
                } else if (optindex == 2) 
                    parsed_options.verbose = true;
                else if (optindex == 3) 
                    parsed_options.extended = true;
                else if (optindex == 4) 
                    parsed_options.no_quit = true;
                else if (optindex == 5) 
                    parsed_options.recurse = true;
                else if (optindex == 6) 
                    parsed_options.no_color = true;
            } else if (options == CNWN_ERF_OPTIONS_LIST) {
                if (optindex == 0)
                    parsed_options.info = true;
            }
        } else {
            if (parsed_options.command == 0) {
                if (cnwn_string_cmpicase(argv[argindex], "list") == 0
                    || cnwn_string_cmpicase(argv[argindex], "ls") == 0
                    || cnwn_string_cmpicase(argv[argindex], "l") == 0) {
                    parsed_options.command = 'l';
                    options = CNWN_ERF_OPTIONS_LIST;
                } else if (cnwn_string_cmpicase(argv[argindex], "extract") == 0
                    || cnwn_string_cmpicase(argv[argindex], "x") == 0
                    || cnwn_string_cmpicase(argv[argindex], "e") == 0) {
                    parsed_options.command = 'e';
                    options = CNWN_ERF_OPTIONS_EXTRACT;
                } else if (cnwn_string_cmpicase(argv[argindex], "archive") == 0
                    || cnwn_string_cmpicase(argv[argindex], "add") == 0
                    || cnwn_string_cmpicase(argv[argindex], "a") == 0) {
                    parsed_options.command = 'a';
                    options = CNWN_ERF_OPTIONS_ARCHIVE;
                } else {
                    fprintf(stderr, "ERROR: invalid command: %s\n", argv[argindex]);
                    return -1;
                }
            } else if (parsed_options.erf_path == NULL)
                parsed_options.erf_path = argv[argindex];
            else if (parsed_options.regexp_start <= 0)
                parsed_options.regexp_start = argindex;
        }
        argindex += retparse;
    }
    if (!parsed_options.help) {
        if (parsed_options.command == 0) {
            fprintf(stderr, "ERROR: no command specified.\n");
            return -1;
        }
        if (parsed_options.erf_path == NULL) {
            fprintf(stderr, "ERROR: no ERF file specified.\n");
            return -1;
        }
    }
    if (ret_parsed_options != NULL)
        *ret_parsed_options = parsed_options;
    return argindex;
}

int command_list(const cnwn_ERFOptions * parsed_options, int argc, char * argv[])
{
    cnwn_File * f = cnwn_file_open_r(parsed_options->erf_path);
    if (f == NULL)
        return -1;
    cnwn_ERFHeader header = {0};
    cnwn_ERFEntry * entries = NULL;
    int erfret = cnwn_erf_read_header(f, &header, &entries);
    cnwn_file_close(f);
    if (erfret < 0) 
        return erfret;
    int64_t total_size = 0;
    for (int i = 0; i < erfret; i++) 
        total_size += entries[i].resource_size;
    if (parsed_options->verbose || parsed_options->info) 
        printf("%s%s %s(%s) %s%d.%d %s(%s)\n",
               parsed_options->no_color ? "": CNWN_ANSI_COLOR_GREEN,
               CNWN_RESOURCE_TYPE_EXTENSION(header.type),
               parsed_options->no_color ? "": CNWN_ANSI_COLOR_GREY,
               header.type_str,
               parsed_options->no_color ? "": CNWN_ANSI_COLOR_GREEN,
               header.version.major,
               header.version.minor,
               parsed_options->no_color ? "": CNWN_ANSI_COLOR_GREY,
               header.version_str);
    if (!parsed_options->info) {
        for (int i = 0; i < erfret; i++) {
            char filename[CNWN_PATH_MAX_SIZE];
            cnwn_resource_type_and_key_to_filename(entries[i].type, entries[i].key, sizeof(filename), filename);
            if (parsed_options->verbose) 
                printf("%s%s%s%s%u %sbytes (@0x%x, erftype=%u, key=%d@0x%x, id=%u)\n",
                       parsed_options->no_color ? "": CNWN_ANSI_COLOR_WHITE,
                       filename,
                       cnwn_string_isempty(filename) ? "": " ",
                       parsed_options->no_color ? "": CNWN_ANSI_COLOR_YELLOW,
                       entries[i].resource_size,
                       parsed_options->no_color ? "": CNWN_ANSI_COLOR_GREY,
                       entries[i].resource_offset,
                       entries[i].erf_type,
                       entries[i].index,
                       entries[i].key_offset,
                       entries[i].resource_id
                       );
            else
                printf("%s%s\n", parsed_options->no_color ? "": CNWN_ANSI_COLOR_WHITE, filename);
        }
    }
    if (parsed_options->verbose || parsed_options->info) 
        printf("%sTotal: %s%u %sentr%s and %s%"PRId64" %sbyte%s of resources.\n",
               parsed_options->no_color ? "": CNWN_ANSI_COLOR_WHITE,
               parsed_options->no_color ? "": CNWN_ANSI_COLOR_YELLOW,
               header.num_entries,
               parsed_options->no_color ? "": CNWN_ANSI_COLOR_WHITE,
               header.num_entries == 1 ? "y" : "ies",
               parsed_options->no_color ? "": CNWN_ANSI_COLOR_YELLOW,
               total_size,
               parsed_options->no_color ? "": CNWN_ANSI_COLOR_WHITE,
               total_size == 1 ? "" : "s"
               );
    free(entries);
    if (!parsed_options->no_color)
        printf("%s\n", CNWN_ANSI_COLOR_RESET);
    return 0;
}

int main(int argc, char * argv[])
{
    cnwn_ERFOptions parsed_options = {0};
    if (parse_arguments(argc, argv, &parsed_options) < 0)
        return 1;
    if (parsed_options.help) {        
        if (parsed_options.command == 'l')
            print_help(CNWN_ERF_OPTIONS_LIST);
        else if (parsed_options.command == 'e')
            print_help(CNWN_ERF_OPTIONS_EXTRACT);
        else if (parsed_options.command == 'a')
            print_help(CNWN_ERF_OPTIONS_ARCHIVE);
        else
            print_help(CNWN_ERF_OPTIONS_GENERAL);
    } else if (parsed_options.command == 'l') {
        if (command_list(&parsed_options, argc, argv) < 0) {
            fprintf(stderr, "ERROR: %s\n", cnwn_get_error());
            return 2;
        }
    }
    return 0;
}
