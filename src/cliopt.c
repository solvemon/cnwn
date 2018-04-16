#include "cnwn/cliopt.h"

int cnwn_parse_cli_option(const cnwn_CliOption * options, const char * arg, const char * next, const char ** ret_arg)
{
    if (options != NULL && arg != NULL) {
        if (CNWN_CLI_OPTION_IS_SHORT(arg)) {
            for (int i = 0; options[i].key != NULL; i++) {
                if (strcmp(options[i].key, arg + 1) == 0) {
                    if (options[i].requires_argument) {
                        if (next == NULL) {
                            if (ret_arg != NULL)
                                *ret_arg = NULL;
                            return CNWN_CLI_OPTION_INCOMPLETE;
                        }
                        if (ret_arg != NULL)
                            *ret_arg = next;
                    } else if (ret_arg != NULL)
                        *ret_arg = NULL;
                    return i;
                }
            }
            if (ret_arg != NULL)
                *ret_arg = NULL;
            return CNWN_CLI_OPTION_INVALID;
        } else if (CNWN_CLI_OPTION_IS_LONG(arg)) {
            for (int i = 0; options[i].key != NULL; i++) {
                int keylen = strlen(options[i].key);
                if (strncmp(options[i].key, arg + 2, keylen) == 0) {
                    if (options[i].requires_argument) {
                        int argoffset = 2 + keylen;
                        while (arg[argoffset] != 0 && arg[argoffset] != '=')
                            argoffset++;
                        if (arg[argoffset] == 0) {
                            if (ret_arg != NULL)
                                *ret_arg = NULL;
                            return CNWN_CLI_OPTION_INCOMPLETE;
                        }
                        if (ret_arg != NULL)
                            *ret_arg = arg + argoffset + 1;
                    } else if (ret_arg != NULL)
                        *ret_arg = NULL;
                    return i;
                }
            }
            if (ret_arg != NULL)
                *ret_arg = NULL;
            return CNWN_CLI_OPTION_INVALID;
        }
    }
    if (ret_arg != NULL)
        *ret_arg = arg;
    return CNWN_CLI_OPTION_ARG;
}

void cnwn_print_cli_options(const cnwn_CliOption * options, const char * prefix)
{
    for (int i = 0; options[i].key != NULL; i++) {
        char tmps[1024];
        snprintf(tmps, sizeof(tmps), "-%s%s%s%s%s",
                 options[i].key,
                 options[i].requires_argument ? " " : "",
                 options[i].requires_argument ? "<" : "",
                 options[i].requires_argument ? "arg" : "",
                 options[i].requires_argument ? ">" : "");
        printf("%s%s%s%s\n",
               prefix != NULL ? prefix : "",
               tmps,
               options[i].help != NULL ? "  :" : "",
               options[i].help != NULL ? options[i].help : "");
    }
}
