#include "cnwn/options.h"

int cnwn_options_find(const cnwn_Option * options, const char * opt)
{
    if (options != NULL && !cnwn_strisblank(opt)) 
        for (int i = 0; options[i].shorthand != 0 || !cnwn_strisblank(options[i].longhand); i++)
            if ((options[i].shorthand != 0 && options[i].shorthand == opt[0])
                || (!cnwn_strisblank(options[i].longhand) && cnwn_strcmp(options[i].longhand, opt) == 0))
                return i;
    return -1;
}

int cnwn_options_parse(const cnwn_Option * options, int index, int argc, char * argv[], int * ret_optindex)
{
    if (index < 0)
        index += argc;
    if (index >= 0 && index < argc) {
        if (CNWN_OPTION_ARG_IS_SHORTHAND(argv[index])) {
            int optindex = cnwn_options_find(options, argv[index] + 1);
            if (optindex < 0) {
                cnwn_set_error("invalid option");
                return -1;
            }
        }
        if (ret_optindex != NULL)
            *ret_optindex = -1;
        return index + 1;
    }
    return argc;
}
