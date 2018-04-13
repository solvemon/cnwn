#include "cnwn.h"

int main(int argc, char * argv[])
{
    int fd = open("../tests/test-01.mod", O_RDONLY, 0);
    cnwn_ERFHeader header = {0};
    int ret = cnwn_erf_read(fd, &header, 0, NULL);
    printf("Returned: %d (%s)\n", ret, ret < 0 ? cnwn_get_error() : "");
    printf("Header type: %s (%s)\n", CNWN_RESOURCE_TYPE_EXTENSION(header.type), header.type_str);
    printf("Header version: %d.%d (%s)\n", header.version.major, header.version.minor, header.version_str);
    close(fd);
    return 0;
}
