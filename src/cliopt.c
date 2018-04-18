#include "cnwn/cliopt.h"

int cnwn_cli_option_to_string(const cnwn_CliOption * option, int max_size, char * ret_s)
{
    char tmps[8192];
    if (option != NULL) {
        int offset = 0;
        if (offset < sizeof(tmps) - 1 && !cnwn_string_isempty(option->skey)) {
            if (option->argname != NULL)
                offset += snprintf(tmps + offset, sizeof(tmps) - offset, "-%s <%s>", option->skey, option->argname);
            else
                offset += snprintf(tmps + offset, sizeof(tmps) - offset, "-%s", option->skey);
        }
        if (offset < sizeof(tmps) - 1 && !cnwn_string_isempty(option->lkey)) {
            if (offset > 0) 
                offset += snprintf(tmps + offset, sizeof(tmps) - offset, ", ");
            if (offset < sizeof(tmps) - 1) {
                if (option->argname != NULL)
                    offset += snprintf(tmps + offset, sizeof(tmps) - offset, "--%s=<%s>", option->lkey, option->argname);
                else
                    offset += snprintf(tmps + offset, sizeof(tmps) - offset, "--%s", option->lkey);
            }
        }
        if (offset < sizeof(tmps) - 1 && !cnwn_string_isempty(option->help)) {
            if (offset > 0) 
                offset += snprintf(tmps + offset, sizeof(tmps) - offset, " : ");
            if (offset < sizeof(tmps) - 1) 
                offset += snprintf(tmps + offset, sizeof(tmps) - offset, "%s", option->help);
        }
        tmps[offset] = 0;
    } else
        tmps[0] = 0;
    return cnwn_copy_string(ret_s, max_size, tmps, -1);
}

int cnwn_cli_option_find(const cnwn_CliOption * options, const char * arg)
{
    if (options != NULL) {
        if (CNWN_CLI_OPTION_ARG_IS_SHORT(arg)) {
            for (int i = 0; !cnwn_string_isempty(options[i].skey) || !cnwn_string_isempty(options[i].lkey); i++)
                if (!cnwn_string_isempty(options[i].skey) && strcmp(options[i].skey, arg + 1) == 0)
                    return i;
        } else if (CNWN_CLI_OPTION_ARG_IS_LONG(arg)) {
            for (int i = 0; !cnwn_string_isempty(options[i].lkey) || !cnwn_string_isempty(options[i].lkey); i++) {
                int nlen = 0;
                while (arg[2 + nlen] != '=' && arg[2 + nlen] != 0)
                    nlen++;
                if (!cnwn_string_isempty(options[i].lkey) && strncmp(options[i].lkey, arg + 2, nlen) == 0)
                    return i;
            }
        }
    }
    return -1;
}

const cnwn_CliOption * cnwn_cli_option_get(const cnwn_CliOption * options, const char * arg)
{
    int find = cnwn_cli_option_find(options, arg);
    if (find >= 0)
        return options + find;
    return NULL;
}

int cnwn_cli_options_parse(const cnwn_CliOption * options, int argindex, int argc, char * argv[], int * ret_optindex, const char ** ret_optarg)
{
    if (argindex >= 0 && argindex < argc) {
        if (CNWN_CLI_OPTION_ARG_IS_OPTION(argv[argindex])) {
            int optindex = cnwn_cli_option_find(options, argv[argindex]);
            if (optindex >= 0) {
                if (options[optindex].argname != NULL) {
                    if (ret_optindex != NULL)
                        *ret_optindex = optindex;
                    if (CNWN_CLI_OPTION_ARG_IS_LONG(argv[argindex])) {
                        for (int i = 2; argv[argindex][i] != 0; i++)
                            if (argv[argindex][i] == '=') {
                                if (ret_optarg != NULL)
                                    *ret_optarg = argv[argindex] + i + 1;
                                return 1;
                            }
                    } else if (argindex < argc - 1) {
                        if (ret_optarg != NULL)
                            *ret_optarg = argv[argindex + 1];
                        return 2;
                    }
                    if (ret_optarg != NULL)
                        *ret_optarg = NULL;
                    return CNWN_CLI_OPTION_MISSING;
                }
                if (ret_optindex != NULL)
                    *ret_optindex = optindex;
                if (ret_optarg != NULL)
                    *ret_optarg = NULL;
                return 1;
            }
            if (ret_optindex != NULL)
                *ret_optindex = -1;
            if (ret_optarg != NULL)
                *ret_optarg = NULL;
            return CNWN_CLI_OPTION_INVALID;
        }
        if (ret_optindex != NULL)
            *ret_optindex = -1;
        if (ret_optarg != NULL)
            *ret_optarg = NULL;
        return 1;
    }
    if (ret_optindex != NULL)
        *ret_optindex = -1;
    if (ret_optarg != NULL)
        *ret_optarg = NULL;
    return 0;

}

void cnwn_cli_options_print_help(const cnwn_CliOption * options, const char * prefix)
{
    if (options != NULL) {
        char tmps[8192];
        for (int i = 0; !cnwn_string_isempty(options[i].skey) || !cnwn_string_isempty(options[i].lkey); i++) {
            cnwn_cli_option_to_string(options + i, sizeof(tmps), tmps);
            printf("%s%s\n", prefix != NULL ? prefix : "", tmps);
        }
    }
}
