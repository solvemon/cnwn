#include "cnwn/tool_erf.h"


#define CNWN_TOOL_ERF_OPTION_HELP 1
#define CNWN_TOOL_ERF_OPTION_VERSION 2
#define CNWN_TOOL_ERF_OPTION_VERBOSE 3
#define CNWN_TOOL_ERF_OPTION_EXCLUDE 4
#define CNWN_TOOL_ERF_OPTION_BINARY 5
#define CNWN_TOOL_ERF_OPTION_YES 6
#define CNWN_TOOL_ERF_OPTION_NO 7
#define CNWN_TOOL_ERF_OPTION_TEMPFILE 8
#define CNWN_TOOL_ERF_OPTION_DIRECTORY 9

const cnwn_Option CNWN_TOOL_ERF_OPTIONS[] = {
    {'h', "help", NULL, "Print help.", CNWN_TOOL_ERF_OPTION_HELP},
    {'V', "version", NULL, "Print version.", CNWN_TOOL_ERF_OPTION_VERSION},
    {'v', "verbose", NULL, "Print stuff to stdout.", CNWN_TOOL_ERF_OPTION_VERBOSE},
    {0}
};

const cnwn_Option CNWN_TOOL_ERF_OPTIONS_LIST[] = {
    {'x', "exclude", NULL, "Regexps exclude files.", CNWN_TOOL_ERF_OPTION_EXCLUDE},
    {0}
};

const cnwn_Option CNWN_TOOL_ERF_OPTIONS_EXTRACT[] = {
    {'x', "exclude", NULL, "Regexps exclude files.", CNWN_TOOL_ERF_OPTION_EXCLUDE},
    {'d', "directory", "path", "Output directory.", CNWN_TOOL_ERF_OPTION_DIRECTORY},
    {'b', "binary", NULL, "Binary output formats.", CNWN_TOOL_ERF_OPTION_BINARY},
    {'Y', "yes", NULL, "Assume YES on all questions.", CNWN_TOOL_ERF_OPTION_YES},
    {'N', "no", NULL, "Assume NO on all questions.", CNWN_TOOL_ERF_OPTION_NO},
    {0}
};

const cnwn_Option CNWN_TOOL_ERF_OPTIONS_CREATE[] = {
    {'Y', "yes", NULL, "Assume YES on all questions.", CNWN_TOOL_ERF_OPTION_YES},
    {'N', "no", NULL, "Assume NO on all questions.", CNWN_TOOL_ERF_OPTION_NO},
    {0}
};

const cnwn_Option CNWN_TOOL_ERF_OPTIONS_ADD[] = {
    {'T', "tempfile", "path", "Override default temporary file.", CNWN_TOOL_ERF_OPTION_TEMPFILE},
    {'Y', "yes", NULL, "Assume YES on all questions.", CNWN_TOOL_ERF_OPTION_YES},
    {'N', "no", NULL, "Assume NO on all questions.", CNWN_TOOL_ERF_OPTION_NO},
    {0}
};

const cnwn_Option CNWN_TOOL_ERF_OPTIONS_REMOVE[] = {
    {'T', "tempfile", "path", "Override default temporary file.", CNWN_TOOL_ERF_OPTION_TEMPFILE},
    {'x', "exclude", NULL, "Regexps exclude files.", CNWN_TOOL_ERF_OPTION_EXCLUDE},
    {'Y', "yes", NULL, "Assume YES on all questions.", CNWN_TOOL_ERF_OPTION_YES},
    {'N', "no", NULL, "Assume NO on all questions.", CNWN_TOOL_ERF_OPTION_NO},
    {0}
};

const cnwn_Option CNWN_TOOL_ERF_OPTIONS_DIFF[] = {
    {'x', "exclude", NULL, "Regexps exclude files.", CNWN_TOOL_ERF_OPTION_EXCLUDE},
    {0}
};

void print_options_help(const cnwn_Option * options, const char * prefix)
{
    for (int i = 0; !CNWN_OPTION_SENTINEL(options + i); i++) {
        char tmps[4096];
        cnwn_option_to_string(options + i, sizeof(tmps), tmps);
        printf("%s%s", prefix != NULL ? prefix : "", tmps);
        if (!cnwn_strisblank(options[i].help))
            printf(" : %s", options[i].help);
        printf("\n");
    }
}

char match_command(const char * arg)
{
    if (cnwn_strstartswithi("info", arg))
        return 'i';
    if (cnwn_strstartswithi("list", arg))
        return 'l';
    if (cnwn_strstartswithi("extract", arg) || cnwn_strcmpi(arg, "x") == 0)
        return 'e';
    if (cnwn_strstartswithi("create", arg))
        return 'c';
    if (cnwn_strstartswithi("add", arg))
        return 'a';
    if (cnwn_strstartswithi("remove", arg))
        return 'r';
    if (cnwn_strstartswithi("diff", arg))
        return 'd';
    return 0;
}

void print_help(void)
{
    printf("cnwn-erf <command> [options] [command arguments]\n");
    printf("Version: %d.%d.%d\n", BUILD_VERSION_MAJOR, BUILD_VERSION_MINOR, BUILD_VERSION_PATCH);
    printf("\n");
    printf("General options:\n");
    print_options_help(CNWN_TOOL_ERF_OPTIONS, "    ");
    printf("\n");
    printf("cnwn-erf info [options] <ERF file>\n");
    printf("\n");
    printf("cnwn-erf list [options] <ERF file> [regular expressions]\n");
    print_options_help(CNWN_TOOL_ERF_OPTIONS_LIST, "    ");
    printf("\n");
    printf("cnwn-erf extract [options] <ERF file> [regular expressions]\n");
    print_options_help(CNWN_TOOL_ERF_OPTIONS_EXTRACT, "    ");
    printf("\n");
    printf("cnwn-erf create [options] <ERF file> [files or directories]\n");
    print_options_help(CNWN_TOOL_ERF_OPTIONS_CREATE, "    ");
    printf("\n");
    printf("cnwn-erf add [options] <ERF file> [files or a single directory]\n");
    print_options_help(CNWN_TOOL_ERF_OPTIONS_ADD, "    ");
    printf("\n");
    printf("cnwn-erf remove [options] <ERF file> [regular expressions]\n");
    print_options_help(CNWN_TOOL_ERF_OPTIONS_REMOVE, "    ");
    printf("\n");
    printf("cnwn-erf diff [options] <ERF file> <other ERF file>\n");
    print_options_help(CNWN_TOOL_ERF_OPTIONS_DIFF, "    ");
    printf("\n");
}

int main(int argc, char * argv[])
{
    cnwn_ToolErfSettings settings = {0};
    int index = 1;
    int ret = 0;
    const cnwn_Option * current_options = CNWN_TOOL_ERF_OPTIONS;
    int optindex;
    const char * optarg;
    char command = 0;
    const char * erf_path = NULL;
    int command_args_start = -1;
    while ((ret = cnwn_options_parse(current_options, index, argc, argv, &optindex, &optarg)) != 0) {
        const cnwn_Option * use_options = current_options;
        if (ret < 0 && optindex < 0) {
            ret = cnwn_options_parse(CNWN_TOOL_ERF_OPTIONS, index, argc, argv, &optindex, &optarg);
            use_options = CNWN_TOOL_ERF_OPTIONS;
        }
        if (ret < 0) {
            fprintf(stderr, "ERROR: %s (%s)\n", cnwn_get_error(), argv[index]);
            return 1;
        }
        if (optindex >= 0) {
            switch (use_options[optindex].optvalue) {
            case CNWN_TOOL_ERF_OPTION_HELP:
                print_help();
                return 0;
            case CNWN_TOOL_ERF_OPTION_VERSION:
                printf("Version: %d.%d.%d\n", BUILD_VERSION_MAJOR, BUILD_VERSION_MINOR, BUILD_VERSION_PATCH);
                return 0;
            case CNWN_TOOL_ERF_OPTION_VERBOSE:
                settings.verbose = true;
                break;
            case CNWN_TOOL_ERF_OPTION_EXCLUDE:
                settings.exclude = true;
                break;
            case CNWN_TOOL_ERF_OPTION_BINARY:
                settings.binary = true;
                break;
            case CNWN_TOOL_ERF_OPTION_YES:
                settings.yes = true;
                break;
            case CNWN_TOOL_ERF_OPTION_NO:
                settings.no = true;
                break;
            case CNWN_TOOL_ERF_OPTION_TEMPFILE:
                settings.tempfile = optarg;
                break;
            case CNWN_TOOL_ERF_OPTION_DIRECTORY:
                settings.directory = optarg;
                break;
            default:
                break;
            }
        } else if (command == 0) {
            command = match_command(argv[index]);
            switch (command) {
            case 'i':
                break;
            case 'l':
                current_options = CNWN_TOOL_ERF_OPTIONS_LIST;
                break;
            case 'e':
                current_options = CNWN_TOOL_ERF_OPTIONS_EXTRACT;
                break;
            case 'c':
                current_options = CNWN_TOOL_ERF_OPTIONS_CREATE;
                break;
            case 'a':
                current_options = CNWN_TOOL_ERF_OPTIONS_ADD;
                break;
            case 'r':
                current_options = CNWN_TOOL_ERF_OPTIONS_REMOVE;
                break;
            case 'd':
                current_options = CNWN_TOOL_ERF_OPTIONS_DIFF;
                break;
            default:
                fprintf(stderr, "ERROR: invalid command '%s'\n", argv[index]);
                return 1;
            }
        } else if (erf_path == NULL) {
            erf_path = argv[index];
        } else if (command_args_start < 0)
            command_args_start = index;
        index += ret;
    }
    if (cnwn_strisblank(erf_path)) {
        fprintf(stderr, "ERROR: missing ERF file path\n");
        return 3;
    }
    int errors = 0;
    switch (command) {
    case 'i':
        errors += cnwn_tool_erf_command_info(&settings, erf_path);
        break;
    case 'l':
        errors += cnwn_tool_erf_command_list(&settings, erf_path,
                                             command_args_start >= 0 ? argc - command_args_start : 0,
                                             command_args_start >= 0 ? argv + command_args_start : NULL);
        break;
    case 'e':
        errors += cnwn_tool_erf_command_extract(&settings, erf_path,
                                                command_args_start >= 0 ? argc - command_args_start : 0,
                                                command_args_start >= 0 ? argv + command_args_start : NULL);
        break;
    case 'c':
        break;
    case 'a':
        break;
    case 'r':
        break;
    case 'd':
        break;
    default:
        break;
    }
    return 0;    
}
