#include "cnwn/options.h"

int main(int argc, char * argv[])
{

    const cnwn_Option options[] = {
        {'a', "alpha", NULL, "This is the alpha option.", 1},
        {'b', "beta", "B", "This is the beta option.", 1},
        {'c', "gamma", NULL, "This is the gamma option.", 1},
        {'d', "delta", "C", "This is the delta option.", 1},
        {0}
    };

    int index = 1;
    cnwn_OptionResult result;
    int ret;
    while ((ret = cnwn_options_parse_argument(options, index, argc, argv, &result)) != 0) {
        printf("Argument '%s' %d\n", result.arg, ret);
        if (ret > 0)
            index += ret;
        else
            index++;
    }    
    return 0;
}
