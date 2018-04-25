#include "cnwn/options.h"

int cnwn_option_to_string(const cnwn_Option * option, int max_size, char * ret_s)
{
    if (!CNWN_OPTION_SENTINEL(option)) {
        char argtmps[1024];
        if (cnwn_strisblank(option->arg))
            argtmps[0] = 0;
        else
            snprintf(argtmps, sizeof(argtmps), "<%s>", option->arg);
        char shorttmps[1024];
        if (option->shorthand == 0)
            shorttmps[0] = 0;
        else
            snprintf(shorttmps, sizeof(shorttmps), "-%c%s%s", option->shorthand, argtmps[0] != 0 ? " " : "", argtmps);
        char longtmps[1024];
        if (cnwn_strisblank(option->longhand))
            longtmps[0] = 0;
        else
            snprintf(longtmps, sizeof(longtmps), "--%s%s%s", option->longhand, argtmps[0] != 0 ? "=" : "", argtmps);
        char tmps[4096];
        snprintf(tmps, sizeof(tmps), "%s%s%s", shorttmps, shorttmps[0] != 0 && longtmps[0] != 0 ? ", " : "", longtmps);
        return cnwn_strcpy(ret_s, max_size, tmps, -1);
    }
    if (max_size > 0 && ret_s != NULL)
        ret_s[0] = 0;
    return 0;
}

int cnwn_options_find(const cnwn_Option * options, const char * opt)
{
    if (options != NULL && !cnwn_strisblank(opt)) 
        for (int i = 0; !CNWN_OPTION_SENTINEL(options + i); i++)
            if ((options[i].shorthand != 0 && options[i].shorthand == opt[0])
                || (!cnwn_strisblank(options[i].longhand) && cnwn_strcmp(options[i].longhand, opt) == 0))
                return i;
    return -1;
}

int cnwn_options_parse(const cnwn_Option * options, int index, int argc, char * argv[], int * ret_optindex, const char ** ret_optarg)
{
    if (index < 0)
        index += argc;
    if (index >= 0 && index < argc) {
        if (CNWN_OPTION_ARG_IS_SHORTHAND(argv[index])) {
            int optindex = cnwn_options_find(options, argv[index] + 1);
            if (optindex < 0) {
                cnwn_set_error("invalid option");
                if (ret_optindex != NULL)
                    *ret_optindex = -1;
                if (ret_optarg != NULL)
                    *ret_optarg = NULL;
                return -1;
            }
            if (!cnwn_strisblank(options[optindex].arg) && index >= argc - 1) {
                cnwn_set_error("missing option argument");
                if (ret_optindex != NULL)
                    *ret_optindex = optindex;
                if (ret_optarg != NULL)
                    *ret_optarg = NULL;
                return -1;
            }
            if (ret_optindex != NULL)
                *ret_optindex = optindex;
            if (!cnwn_strisblank(options[optindex].arg)) {
                if (ret_optarg != NULL)
                    *ret_optarg = argv[index + 1];
                return 2;
            } else if (ret_optarg != NULL)
                *ret_optarg = NULL;
            return 1;
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
