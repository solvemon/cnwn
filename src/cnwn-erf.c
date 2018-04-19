#include "cnwn/erf_util.h"

void print_version(const cnwn_ERFUtilOptions * options)
{
    printf("%s%d.%d.%d\n",
           CNWN_COLORIZE_NORMAL(options->color),
           VERSION_MAJOR,
           VERSION_MINOR,
           VERSION_PATCH);
}

void print_help(const cnwn_ERFUtilOptions * options, char command)
{
    printf("%scnwn-erf %s[general options] <ERF file> %s<list|extract|create|add> %s[command options] [command args]\n",
           CNWN_COLORIZE_EMPHASIS(options->color),
           CNWN_COLORIZE_NORMAL(options->color),
           CNWN_COLORIZE_EMPHASIS(options->color),
           CNWN_COLORIZE_NORMAL(options->color));
    printf("%sVersion: ", CNWN_COLORIZE_NORMAL(options->color));
    print_version(options);
    printf("\n");
    printf("%sGeneral options:\n", CNWN_COLORIZE_EMPHASIS(options->color));
    printf("%s", CNWN_COLORIZE_NORMAL(options->color));
    cnwn_cli_options_print_help(CNWN_ERF_UTIL_OPTIONS_GENERAL, "  ");
    printf("\n");
    printf("%scnwn-erf %s<ERF file> %slist|ls|l %s[options] [regexp filters ...]:\n",
           CNWN_COLORIZE_EMPHASIS(options->color),
           CNWN_COLORIZE_NORMAL(options->color),
           CNWN_COLORIZE_EMPHASIS(options->color),
           CNWN_COLORIZE_NORMAL(options->color));
    printf("%s", CNWN_COLORIZE_NORMAL(options->color));
    cnwn_cli_options_print_help(CNWN_ERF_UTIL_OPTIONS_LIST, "  ");
    printf("\n");
    printf("%scnwn-erf %s<ERF file> %sextract|e|x %s[options] [regexp filters ...]:\n",
           CNWN_COLORIZE_EMPHASIS(options->color),
           CNWN_COLORIZE_NORMAL(options->color),
           CNWN_COLORIZE_EMPHASIS(options->color),
           CNWN_COLORIZE_NORMAL(options->color));
    printf("%s", CNWN_COLORIZE_NORMAL(options->color));
    cnwn_cli_options_print_help(CNWN_ERF_UTIL_OPTIONS_EXTRACT, "  ");
    printf("\n");
    printf("%scnwn-erf %s<ERF file> %screate|c %s[options] [input files ...]:\n",
           CNWN_COLORIZE_EMPHASIS(options->color),
           CNWN_COLORIZE_NORMAL(options->color),
           CNWN_COLORIZE_EMPHASIS(options->color),
           CNWN_COLORIZE_NORMAL(options->color));
    printf("%s", CNWN_COLORIZE_NORMAL(options->color));
    cnwn_cli_options_print_help(CNWN_ERF_UTIL_OPTIONS_CREATE, "  ");
    printf("\n");
    printf("%scnwn-erf %s<ERF file> %sadd|a %s[options] [regexp filters ...]:\n",
           CNWN_COLORIZE_EMPHASIS(options->color),
           CNWN_COLORIZE_NORMAL(options->color),
           CNWN_COLORIZE_EMPHASIS(options->color),
           CNWN_COLORIZE_NORMAL(options->color));
    printf("%s", CNWN_COLORIZE_NORMAL(options->color));
    cnwn_cli_options_print_help(CNWN_ERF_UTIL_OPTIONS_ADD, "  ");
    printf("\n");
    printf("%scnwn-erf %s<ERF file> %sremove|r %s[options] [regexp filters ...]:\n",
           CNWN_COLORIZE_EMPHASIS(options->color),
           CNWN_COLORIZE_NORMAL(options->color),
           CNWN_COLORIZE_EMPHASIS(options->color),
           CNWN_COLORIZE_NORMAL(options->color));
    printf("%s", CNWN_COLORIZE_NORMAL(options->color));
    cnwn_cli_options_print_help(CNWN_ERF_UTIL_OPTIONS_REMOVE, "  ");
    printf("\n");
    printf("%scnwn-erf %s<ERF file> %sdiff|d %s<other ERF file>\n",
           CNWN_COLORIZE_EMPHASIS(options->color),
           CNWN_COLORIZE_NORMAL(options->color),
           CNWN_COLORIZE_EMPHASIS(options->color),
           CNWN_COLORIZE_NORMAL(options->color));
    printf("\n");
}

int main(int argc, char * argv[])
{
    cnwn_ERFUtilOptionError option_errors[1024];
    cnwn_ERFUtilOptions options = {0};
    const char * erf_path = NULL;
    char command = 0;
    int errors = cnwn_erf_util_parse_options(argc, argv, &options, &erf_path, &command, 1024, option_errors);
    if (options.help) {
        print_help(&options, command);
        printf("%s", CNWN_COLORIZE_NORMAL(options.color));
        return 0;
    }
    if (options.version) {
        print_version(&options);
        printf("%s", CNWN_COLORIZE_NORMAL(options.color));
        return 0;
    }
    if (erf_path == NULL) {
        fprintf(stderr, "%sERROR:%s argument %s1%s ERF path is missing\n",
                CNWN_COLORIZE_ERROR(options.color),
                CNWN_COLORIZE_NORMAL(options.color),
                CNWN_COLORIZE_NUMBER(options.color),
                CNWN_COLORIZE_NORMAL(options.color));
        printf("%s", CNWN_COLORIZE_NORMAL(options.color));
        return 1;
    }
    if (command == 0)
        command = 'i';
    if (command < 0) {
        fprintf(stderr, "%sERROR:%s argument %s2%s invalid command: %s\n",
                CNWN_COLORIZE_ERROR(options.color),
                CNWN_COLORIZE_NORMAL(options.color),
                CNWN_COLORIZE_NUMBER(options.color),
                CNWN_COLORIZE_NORMAL(options.color),
                argv[2]);
        printf("%s", CNWN_COLORIZE_NORMAL(options.color));
        return 1;
    }
    if (errors > 0) {
        for (int i = 0; i < errors; i++) {
            if (option_errors[i].error == CNWN_CLI_OPTION_INVALID)
                fprintf(stderr, "%sERROR:%s argument %s%d%s has invalid option: %s\n",
                        CNWN_COLORIZE_ERROR(options.color),
                        CNWN_COLORIZE_NORMAL(options.color),
                        CNWN_COLORIZE_NUMBER(options.color),
                        option_errors[i].argindex,
                        CNWN_COLORIZE_NORMAL(options.color),
                        argv[option_errors[i].argindex]);
            else if (option_errors[i].error == CNWN_CLI_OPTION_MISSING)
                fprintf(stderr, "%sERROR:%s argument %s%d%s missing option argument: %s\n",
                        CNWN_COLORIZE_ERROR(options.color),
                        CNWN_COLORIZE_NORMAL(options.color),
                        CNWN_COLORIZE_NUMBER(options.color),
                        option_errors[i].argindex,
                        CNWN_COLORIZE_NORMAL(options.color),
                        argv[option_errors[i].argindex]);
            else
                fprintf(stderr, "%sERROR:%s argument %s%d%s has unknown error: %s\n",
                        CNWN_COLORIZE_ERROR(options.color),
                        CNWN_COLORIZE_NORMAL(options.color),
                        CNWN_COLORIZE_NUMBER(options.color),
                        option_errors[i].argindex,
                        CNWN_COLORIZE_NORMAL(options.color),
                        argv[option_errors[i].argindex]);
        }
        printf("%s", CNWN_COLORIZE_NORMAL(options.color));
        return 3;
    }
    switch (command) {
    case 'i':
        errors = cnwn_erf_util_info(erf_path, &options);
        break;
    case 'l':
        errors = cnwn_erf_util_list(erf_path, &options);
        break;
    default:
        break;
    }
    if (errors > 0) {
        printf("%sERROR: %s%d %serror%s reported.\n",
               CNWN_COLORIZE_ERROR(options.color),
               CNWN_COLORIZE_NUMBER(options.color),
               errors,
               CNWN_COLORIZE_NORMAL(options.color),
               errors != 1 ? "s" : "");
        return 4;
    }
    printf("%s", CNWN_COLORIZE_NORMAL(options.color));
    return 0;
}
