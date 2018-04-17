#include "cnwn/erf.h"

int main(int argc, char * argv[])
{
    if (cnwn_erf_list(argc > 1 ? argv[1] : "../tests/test-01.mod", true, stdout) <0)
        printf("ERROR: %s\n", cnwn_get_error());
    return 0;
}
