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
    bool disable_xml;
    const char * dir;
    const char * erf_version;
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
    {"x", "disable-xml", NULL, "Disable all XML conversions."},
    {"d", "output-dir", "path", "Specify an output directory."},
    {0}
};

const cnwn_CliOption CNWN_ERF_OPTIONS_CREATE[] = {
    {"f", "force", NULL, "Force overwrites."},
    {"d", "input-dir", "path", "Specify an input directory."},
    {"t", "erf-version", "version", "Specify an ERF version: \"1.0\" (default) or \"1.1\"."},
    {0}
};

const cnwn_CliOption CNWN_ERF_OPTIONS_ADD[] = {
    {"f", "force", NULL, "Force overwrites."},
    {"d", "input-dir", "path", "Specify an input directory."},
    {0}
};

void print_help(const cnwn_CliOption * options, bool verbose)
{
    if (options == CNWN_ERF_OPTIONS_GENERAL || verbose) {
        printf("cnwn-erf [GENERAL OPTIONS] <list|extract|create|add> <ERF FILE> [COMMAND OPTIONS] [REGULAR EXPRESSIONS ...]\n");
        printf("Version: %d.%d.%d", VERSION_MAJOR, VERSION_MINOR, VERSION_PATCH);
        printf("\n");
        cnwn_cli_options_print_help(options, NULL);
        printf("Use -h/--help after a command to get more help.\n");
        printf("\n");
    } else if (options == CNWN_ERF_OPTIONS_LIST || verbose) {
        printf("cnwn-erf [GENERAL OPTIONS] <list|ls|l> <ERF FILE> [COMMAND OPTIONS] [REGULAR EXPRESSIONS ...]\n");
        printf("List contents of an ERF file.\n");
        printf("\n");
        cnwn_cli_options_print_help(options, NULL);
        printf("\n");
    } else if (options == CNWN_ERF_OPTIONS_EXTRACT || verbose) {
        printf("cnwn-erf [GENERAL OPTIONS] <extract|x|e> <ERF FILE> [COMMAND OPTIONS] [REGULAR EXPRESSIONS ...]\n");
        printf("Extract contents of an ERF file.\n");
        printf("\n");
        cnwn_cli_options_print_help(options, NULL);
        printf("\n");
    } else if (options == CNWN_ERF_OPTIONS_CREATE || verbose) {
        printf("cnwn-erf [GENERAL OPTIONS] <create|c> <ERF FILE> [COMMAND OPTIONS] [REGULAR EXPRESSIONS ...]\n");
        printf("Create a new ERF file and add contents to it.\n");
        printf("\n");
        cnwn_cli_options_print_help(options, NULL);
        printf("\n");
    } else if (options == CNWN_ERF_OPTIONS_ADD || verbose) {
        printf("cnwn-erf [GENERAL OPTIONS] <add|a> <ERF FILE> [COMMAND OPTIONS] [REGULAR EXPRESSIONS ...]\n");
        printf("Add contents to an ERF file.\n");
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
            } else if (options == CNWN_ERF_OPTIONS_EXTRACT) {
                if (optindex == 0)
                    parsed_options.force = true;
                else if (optindex == 1)
                    parsed_options.dir = optarg;
                else if (optindex == 2)
                    parsed_options.disable_xml = true;
            } else if (options == CNWN_ERF_OPTIONS_CREATE) {
                if (optindex == 0)
                    parsed_options.force = true;
                else if (optindex == 1)
                    parsed_options.dir = optarg;
                else if (optindex == 2)
                    parsed_options.erf_version = optarg;
            } else if (options == CNWN_ERF_OPTIONS_ADD) {
                if (optindex == 0)
                    parsed_options.force = true;
                else if (optindex == 1)
                    parsed_options.dir = optarg;
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
                } else if (cnwn_string_cmpicase(argv[argindex], "create") == 0
                           || cnwn_string_cmpicase(argv[argindex], "c") == 0) {
                    parsed_options.command = 'c';
                    options = CNWN_ERF_OPTIONS_CREATE;
                } else if (cnwn_string_cmpicase(argv[argindex], "add") == 0
                           || cnwn_string_cmpicase(argv[argindex], "a") == 0) {
                    parsed_options.command = 'a';
                    options = CNWN_ERF_OPTIONS_ADD;
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
    if (!parsed_options.help && !parsed_options.version) {
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

int command_list_entries(const cnwn_ERFOptions * parsed_options, const char * prefix_path, int num_entries, const cnwn_ERFEntry * entries)
{
    for (int i = 0; i < num_entries; i++) {
        char filename[CNWN_PATH_MAX_SIZE];
        cnwn_resource_type_and_key_to_filename(entries[i].type, entries[i].key, sizeof(filename), filename);
        char path[CNWN_PATH_MAX_SIZE];
        cnwn_path_concat(path, sizeof(path), prefix_path, filename, NULL);
        char number[128];
        snprintf(number, sizeof(number), "%u", entries[i].resource_size);
        if (parsed_options->verbose) 
            printf("%s%-10s %s%-20s %s(index=%d, id=%u, key=0x%x, res=0x%x, itype=%u)\n",
                   parsed_options->no_color ? "": CNWN_CLI_COLOR(CNWN_CLI_COLOR_NUMBER),
                   number,
                   parsed_options->no_color ? "": CNWN_CLI_COLOR(CNWN_CLI_COLOR_GREEN),
                   path,
                   parsed_options->no_color ? "": CNWN_CLI_COLOR(CNWN_CLI_COLOR_NONE),
                   entries[i].index,
                   entries[i].resource_id,
                   entries[i].key_offset,
                   entries[i].resource_offset,
                   entries[i].itype
                   );
        else
            printf("%s%s\n", parsed_options->no_color ? "": CNWN_CLI_COLOR(CNWN_CLI_COLOR_NONE), filename);
    }
    return 0;
}

int command_list(const cnwn_ERFOptions * parsed_options, int argc, char * argv[])
{
    cnwn_File * f = cnwn_file_open_r(parsed_options->erf_path);
    if (f == NULL) {
        fprintf(stderr, "%sERROR: %serror opening file (%s): %s%s\n",
                parsed_options->no_color ? "": CNWN_CLI_COLOR(CNWN_CLI_COLOR_ALERT),
                parsed_options->no_color ? "": CNWN_CLI_COLOR(CNWN_CLI_COLOR_NONE),
                cnwn_get_error(),
                parsed_options->no_color ? "": CNWN_CLI_COLOR(CNWN_CLI_COLOR_PATH),
                parsed_options->erf_path);
        return -1;
    }
    cnwn_ERFHeader header = {0};
    cnwn_ERFEntry * entries = NULL;
    int erfret = cnwn_erf_read_header(f, &header, &entries);
    cnwn_file_close(f);
    if (erfret < 0) {
        fprintf(stderr, "%sERROR: %serror reading entries (%s): %s%s\n",
                parsed_options->no_color ? "": CNWN_CLI_COLOR(CNWN_CLI_COLOR_ALERT),
                parsed_options->no_color ? "": CNWN_CLI_COLOR(CNWN_CLI_COLOR_NONE),
                cnwn_get_error(),
                parsed_options->no_color ? "": CNWN_CLI_COLOR(CNWN_CLI_COLOR_PATH),
                parsed_options->erf_path);
        return -1;
    }
    int64_t total_size = 0;
    for (int i = 0; i < erfret; i++) 
        total_size += entries[i].resource_size;
    if (parsed_options->verbose || parsed_options->info) 
        printf("%s%s (%s) %d.%d (%s)\n",
               parsed_options->no_color ? "": CNWN_CLI_COLOR(CNWN_CLI_COLOR_NONE),
               CNWN_RESOURCE_TYPE_EXTENSION(header.type),
               header.type_str,
               header.version.major,
               header.version.minor,
               header.version_str);
    int listerr = 0;
    if (!parsed_options->info) 
        listerr = command_list_entries(parsed_options, "", header.num_entries, entries);
    if (listerr < 0 && !parsed_options->no_quit) {
        free(entries);
        if (!parsed_options->no_color)
            printf("%s", CNWN_CLI_COLOR(CNWN_CLI_COLOR_NONE));
        return -1;
    }
    if (parsed_options->verbose || parsed_options->info) 
        printf("%sTotal: %s%u %sentr%s and %s%"PRId64" %sbyte%s of resources.\n",
               parsed_options->no_color ? "": CNWN_CLI_COLOR(CNWN_CLI_COLOR_NONE),
               parsed_options->no_color ? "": CNWN_CLI_COLOR(CNWN_CLI_COLOR_NUMBER),
               header.num_entries,
               parsed_options->no_color ? "": CNWN_CLI_COLOR(CNWN_CLI_COLOR_NONE),
               header.num_entries == 1 ? "y" : "ies",
               parsed_options->no_color ? "": CNWN_CLI_COLOR(CNWN_CLI_COLOR_NUMBER),
               total_size,
               parsed_options->no_color ? "": CNWN_CLI_COLOR(CNWN_CLI_COLOR_NONE),
               total_size == 1 ? "" : "s"
               );
    free(entries);
    return 0;
}


int command_extract_entries(cnwn_File * erf_f, const cnwn_ERFOptions * parsed_options, const char * prefix_path, int num_entries, const cnwn_ERFEntry * entries)
{
    char outpath[CNWN_PATH_MAX_SIZE];
    cnwn_path_concat(outpath, sizeof(outpath), parsed_options->dir, NULL);
    if (!cnwn_string_isempty(outpath)) {
        // FIXME: check existance and ask permission mkdir unless no-quit.
        int mkret = cnwn_mkdirs(outpath);
        if (mkret < 0) {
            fprintf(stderr, "%sERROR: %serror creating directory (%s): %s%s\n",
                    parsed_options->no_color ? "": CNWN_CLI_COLOR(CNWN_CLI_COLOR_ALERT),
                    parsed_options->no_color ? "": CNWN_CLI_COLOR(CNWN_CLI_COLOR_NONE),
                    cnwn_get_error(),
                    parsed_options->no_color ? "": CNWN_CLI_COLOR(CNWN_CLI_COLOR_PATH),
                    outpath);
            if (!parsed_options->no_quit)
                return -1;
        } else if (mkret > 0 && parsed_options->verbose)
            printf("%sCreated path: %s%s\n",
                   parsed_options->no_color ? "": CNWN_CLI_COLOR(CNWN_CLI_COLOR_NONE),
                   parsed_options->no_color ? "": CNWN_CLI_COLOR(CNWN_CLI_COLOR_PATH),
                   outpath);
    }
    for (int i = 0; i < num_entries; i++) {
        char filename[CNWN_PATH_MAX_SIZE];
        cnwn_resource_type_and_key_to_filename(entries[i].type, entries[i].key, sizeof(filename), filename);
        char path[CNWN_PATH_MAX_SIZE];
        cnwn_path_concat(path, sizeof(path), prefix_path, filename, NULL);
        char outpathf[CNWN_PATH_MAX_SIZE];
        cnwn_path_concat(outpathf, sizeof(outpathf), outpath, path, NULL);
        char number[128];
        snprintf(number, sizeof(number), "%u", entries[i].resource_size);
        // FIXME: check existance and ask permission to overwrite unless no-quit.
        int seekret = cnwn_file_seek(erf_f, entries[i].resource_offset);
        if (seekret < 0) {
            fprintf(stderr, "%sERROR: %serror seeking file (%s): %s%s\n",
                    parsed_options->no_color ? "": CNWN_CLI_COLOR(CNWN_CLI_COLOR_ALERT),
                    parsed_options->no_color ? "": CNWN_CLI_COLOR(CNWN_CLI_COLOR_NONE),
                    cnwn_get_error(),
                    parsed_options->no_color ? "": CNWN_CLI_COLOR(CNWN_CLI_COLOR_PATH),
                    parsed_options->erf_path);
            if (!parsed_options->no_quit) 
                return -1;
        } else {
            cnwn_File * outf = cnwn_file_open_w(outpathf);
            if (outf != NULL) {
                int copyret = cnwn_file_copy_bytes(erf_f, outf, entries[i].resource_size);
                if (copyret < 0) {
                    fprintf(stderr, "%sERROR: %serror writing file (%s): %s%s\n",
                            parsed_options->no_color ? "": CNWN_CLI_COLOR(CNWN_CLI_COLOR_ALERT),
                            parsed_options->no_color ? "": CNWN_CLI_COLOR(CNWN_CLI_COLOR_NONE),
                            cnwn_get_error(),
                            parsed_options->no_color ? "": CNWN_CLI_COLOR(CNWN_CLI_COLOR_PATH),
                            parsed_options->erf_path);
                    if (!parsed_options->no_quit) {
                        cnwn_file_close(outf);
                        return -1;
                    }
                } else if (parsed_options->verbose) 
                    printf("%s%s %s=> %s%s %s%s\n",
                           parsed_options->no_color ? "": CNWN_CLI_COLOR(CNWN_CLI_COLOR_GREEN),
                           path,
                           parsed_options->no_color ? "": CNWN_CLI_COLOR(CNWN_CLI_COLOR_NONE),
                           parsed_options->no_color ? "": CNWN_CLI_COLOR(CNWN_CLI_COLOR_PATH),
                           outpathf,
                           parsed_options->no_color ? "": CNWN_CLI_COLOR(CNWN_CLI_COLOR_NUMBER),
                           number
                           );
                cnwn_file_close(outf);
            } else {
                fprintf(stderr, "%sERROR: %serror creating file (%s): %s%s\n",
                        parsed_options->no_color ? "": CNWN_CLI_COLOR(CNWN_CLI_COLOR_ALERT),
                        parsed_options->no_color ? "": CNWN_CLI_COLOR(CNWN_CLI_COLOR_NONE),
                        cnwn_get_error(),
                        parsed_options->no_color ? "": CNWN_CLI_COLOR(CNWN_CLI_COLOR_PATH),
                        outpathf);
                if (!parsed_options->no_quit)
                    return -1;
            }
        }
    }
    return 0;
}

int command_extract(const cnwn_ERFOptions * parsed_options, int argc, char * argv[])
{
    cnwn_File * f = cnwn_file_open_r(parsed_options->erf_path);
    if (f == NULL) {
        fprintf(stderr, "%sERROR: %serror opening file (%s): %s%s\n",
                parsed_options->no_color ? "": CNWN_CLI_COLOR(CNWN_CLI_COLOR_ALERT),
                parsed_options->no_color ? "": CNWN_CLI_COLOR(CNWN_CLI_COLOR_NONE),
                cnwn_get_error(),
                parsed_options->no_color ? "": CNWN_CLI_COLOR(CNWN_CLI_COLOR_PATH),
                parsed_options->erf_path);
        return -1;
    }
    cnwn_ERFHeader header = {0};
    cnwn_ERFEntry * entries = NULL;
    int erfret = cnwn_erf_read_header(f, &header, &entries);
    if (erfret < 0) {
        fprintf(stderr, "%sERROR: %serror reading entries (%s): %s%s\n",
                parsed_options->no_color ? "": CNWN_CLI_COLOR(CNWN_CLI_COLOR_ALERT),
                parsed_options->no_color ? "": CNWN_CLI_COLOR(CNWN_CLI_COLOR_NONE),
                cnwn_get_error(),
                parsed_options->no_color ? "": CNWN_CLI_COLOR(CNWN_CLI_COLOR_PATH),
                parsed_options->erf_path);
        cnwn_file_close(f);
        return -1;
    }
    int64_t total_size = 0;
    for (int i = 0; i < erfret; i++) 
        total_size += entries[i].resource_size;
    if (parsed_options->verbose) 
        printf("%s%s (%s) %d.%d (%s)\n",
               parsed_options->no_color ? "": CNWN_CLI_COLOR(CNWN_CLI_COLOR_NONE),
               CNWN_RESOURCE_TYPE_EXTENSION(header.type),
               header.type_str,
               header.version.major,
               header.version.minor,
               header.version_str);
    int err_extract = command_extract_entries(f, parsed_options, "", header.num_entries, entries);
    cnwn_file_close(f);
    if (err_extract < 0 && !parsed_options->no_quit) {
        free(entries);
        if (!parsed_options->no_color)
            printf("%s", CNWN_CLI_COLOR(CNWN_CLI_COLOR_NONE));
        return -1;
    }
    if (parsed_options->verbose) 
        printf("%sTotal: %s%u %sentr%s and %s%"PRId64" %sbyte%s of resources.\n",
               parsed_options->no_color ? "": CNWN_CLI_COLOR(CNWN_CLI_COLOR_NONE),
               parsed_options->no_color ? "": CNWN_CLI_COLOR(CNWN_CLI_COLOR_NUMBER),
               header.num_entries,
               parsed_options->no_color ? "": CNWN_CLI_COLOR(CNWN_CLI_COLOR_NONE),
               header.num_entries == 1 ? "y" : "ies",
               parsed_options->no_color ? "": CNWN_CLI_COLOR(CNWN_CLI_COLOR_NUMBER),
               total_size,
               parsed_options->no_color ? "": CNWN_CLI_COLOR(CNWN_CLI_COLOR_NONE),
               total_size == 1 ? "" : "s"
               );
    free(entries);
    return 0;
}

int main(int argc, char * argv[])
{
    cnwn_ERFOptions parsed_options = {0};
    if (parse_arguments(argc, argv, &parsed_options) < 0)
        return 1;
    if (parsed_options.help) {        
        if (parsed_options.command == 'l')
            print_help(CNWN_ERF_OPTIONS_LIST, false);
        else if (parsed_options.command == 'e')
            print_help(CNWN_ERF_OPTIONS_EXTRACT, false);
        else if (parsed_options.command == 'c')
            print_help(CNWN_ERF_OPTIONS_CREATE, false);
        else if (parsed_options.command == 'a')
            print_help(CNWN_ERF_OPTIONS_ADD, false);
        else
            print_help(CNWN_ERF_OPTIONS_GENERAL, false);
    } else if (parsed_options.version) {
        printf("%s%d.%d.%d\n", parsed_options.no_color ? "": CNWN_CLI_COLOR(CNWN_CLI_COLOR_NONE), VERSION_MAJOR, VERSION_MINOR, VERSION_PATCH);
    } else if (parsed_options.command == 'l') {
        if (command_list(&parsed_options, argc, argv) < 0) {
            if (!parsed_options.no_color)
                printf("%s", CNWN_CLI_COLOR(CNWN_CLI_COLOR_NONE));
            return 2;
        }
    } else if (parsed_options.command == 'e') {
        if (command_extract(&parsed_options, argc, argv) < 0) {
            if (!parsed_options.no_color)
                printf("%s", CNWN_CLI_COLOR(CNWN_CLI_COLOR_NONE));
            return 2;
        }
    }
    if (!parsed_options.no_color)
        printf("%s", CNWN_CLI_COLOR(CNWN_CLI_COLOR_NONE));
    return 0;
}
