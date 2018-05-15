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

int cnwn_options_parse_argument(const cnwn_Option * options, int index, int argc, char * argv[], cnwn_OptionResult * ret_result)
{
    if (index < 0)
        index += argc;
    if (index >= 0 && index < argc) {
        if (CNWN_OPTION_ARG_IS_SHORTHAND(argv[index])) {
            int optindex = cnwn_options_find(options, argv[index] + 1);
            if (optindex < 0) {
                cnwn_set_error("invalid option");
                if (ret_result != NULL) {
                    ret_result->index = index;
                    ret_result->arg = argv[index];
                    ret_result->optindex = -1;
                    ret_result->option = NULL;
                    ret_result->optarg = NULL;
                    ret_result->optvalue = 0;
                }
                return CNWN_OPTION_ERROR_INVALID;
            }
            if (!cnwn_strisblank(options[optindex].arg) && index >= argc - 1) {
                cnwn_set_error("missing option argument");
                if (ret_result != NULL) {
                    ret_result->index = index;
                    ret_result->arg = argv[index];
                    ret_result->optindex = optindex;
                    ret_result->option = options + optindex;
                    ret_result->optarg = NULL;
                    ret_result->optvalue = 0;
                }
                return CNWN_OPTION_ERROR_NOARG;
            }
            if (!cnwn_strisblank(options[optindex].arg)) {
                if (ret_result != NULL) {
                    ret_result->index = index;
                    ret_result->arg = argv[index];
                    ret_result->optindex = optindex;
                    ret_result->option = options + optindex;
                    ret_result->optarg = argv[index + 1];
                    ret_result->optvalue = options[optindex].optvalue;
                }
                return 2;
            } 
            if (ret_result != NULL) {
                ret_result->index = index;
                ret_result->arg = argv[index];
                ret_result->optindex = optindex;
                ret_result->option = options + optindex;
                ret_result->optarg = NULL;
                ret_result->optvalue = options[optindex].optvalue;
            }
            return 1;
        } else if (CNWN_OPTION_ARG_IS_LONGHAND(argv[index])) {
            int optindex = cnwn_options_find(options, argv[index] + 2);
            if (optindex < 0) {
                cnwn_set_error("invalid option");
                if (ret_result != NULL) {
                    ret_result->index = index;
                    ret_result->arg = argv[index];
                    ret_result->optindex = -1;
                    ret_result->option = NULL;
                    ret_result->optarg = NULL;
                    ret_result->optvalue = 0;
                }
                return CNWN_OPTION_ERROR_INVALID;
            }
            if (!cnwn_strisblank(options[optindex].arg)) {
                // FIND VALUE
                int finder = cnwn_strfind(argv[index], 0, "=", NULL);
                if (finder < 3) {
                    cnwn_set_error("missing option value");
                    if (ret_result != NULL) {
                        ret_result->index = index;
                        ret_result->arg = argv[index];
                        ret_result->optindex = optindex;
                        ret_result->option = options + optindex;
                        ret_result->optarg = NULL;
                        ret_result->optvalue = 0;
                    }
                    return CNWN_OPTION_ERROR_NOARG;
                }
                if (ret_result != NULL) {
                    ret_result->index = index;
                    ret_result->arg = argv[index];
                    ret_result->optindex = optindex;
                    ret_result->option = options + optindex;
                    ret_result->optarg = argv[index + 1] + finder + 1;
                    ret_result->optvalue = options[optindex].optvalue;
                }
                return 1;
            } 
            if (ret_result != NULL) {
                ret_result->index = index;
                ret_result->arg = argv[index];
                ret_result->optindex = optindex;
                ret_result->option = options + optindex;
                ret_result->optarg = NULL;
                ret_result->optvalue = options[optindex].optvalue;
            }
            return 1;
        } else if (ret_result != NULL) {
            ret_result->index = index;
            ret_result->arg = argv[index];
            ret_result->optindex = -1;
            ret_result->option = NULL;
            ret_result->optarg = NULL;
            ret_result->optvalue = 0;
        }
        return 1;
    }
    if (ret_result != NULL) {
        ret_result->index = index;
        ret_result->arg = argv[index];
        ret_result->optindex = -1;
        ret_result->option = NULL;
        ret_result->optarg = NULL;
        ret_result->optvalue = 0;
    }
    return 0;
}

cnwn_OptionResult * cnwn_options_parse(const cnwn_Option * options, int argc, char * argv[])
{
    if (argc < 0)
        argc = 0;
    int index = 0;
    cnwn_OptionResult * results = malloc(sizeof(cnwn_OptionResult) * (argc + 1));
    memset(results, 0, sizeof(cnwn_OptionResult) * (argc + 1));
    int ret;
    while (index < argc && (ret = cnwn_options_parse_argument(options, index, argc, argv, results + index)) != 0) {
        if (ret > 0)
            index += ret;
        else
            index++;
    }
    results[index].index = -1;
    return results;
}
