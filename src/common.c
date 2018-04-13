#include "cnwn.h"

static char CNWN_ERROR_MESSAGE[8192] = {0};

const char * cnwn_get_error(void)
{
    return CNWN_ERROR_MESSAGE;
}

static void cnwn_set_error_va(const char * format, va_list args)
{
    char tmps[8192];
    int len = vsnprintf(tmps, sizeof(tmps), format, args);
    if (len > 0)
        memcpy(CNWN_ERROR_MESSAGE, tmps, sizeof(char) * len);
    CNWN_ERROR_MESSAGE[len] = 0;
}

static void cnwn_set_error(const char * format, ...)
{
    va_list args;
    va_start(args, format);
    cnwn_set_error_va(format, args);
    va_end(args);
}


uint32_t cnwn_swap32(uint32_t i)
{
    #ifdef BIG_ENDIAN
    return ((((uint32_t)(i) & 0xff000000) >> 8) | (((uint32_t)(i) & 0x00ff0000) << 8) | (((uint32_t)(i) & 0x0000ff00) >> 8) | (((uint32_t)(i) & 0x000000ff) << 8));
    #else
    return i;
    #endif
}

uint16_t cnwn_swap16(uint16_t i)
{
    #ifdef BIG_ENDIAN
    return ((((uint16_t)(i) & 0xff00) >> 8) | (((uint16_t)(i) & 0x00ff) << 8))
    #else
    return i;
    #endif
}

int cnwn_read_uint32(int fd, uint32_t * ret_i)
{
    if (ret_i != NULL) {
        uint8_t buffer[4];
        int ret = read(fd, buffer, 4);
        if (ret < 0) {
            cnwn_set_error("%s", strerror(errno));
            return -1;
        }
        if (ret < 4) {
            cnwn_set_error("not enough data");
            return -1;
        }
        *ret_i = cnwn_swap32(*((uint32_t *)buffer));
    } else if (lseek(fd, 4, SEEK_CUR) < 0) {
        cnwn_set_error("seek error (%s)", strerror(errno));
        return -1;
    }
    return 4;
}


int cnwn_write_uint32(int fd, uint32_t i);

int cnwn_read_uint16(int fd, uint16_t * ret_i)
{
    if (ret_i != NULL) {
        uint8_t buffer[2];
        int ret = read(fd, buffer, 2);
        if (ret < 0) {
            cnwn_set_error("%s", strerror(errno));
            return -1;
        }
        if (ret < 2) {
            cnwn_set_error("not enough data");
            return -1;
        }
        *ret_i = cnwn_swap16(*((uint16_t *)buffer));
    } else if (lseek(fd, 2, SEEK_CUR) < 0) {
        cnwn_set_error("seek error (%s)", strerror(errno));
        return -1;
    }
    return 2;
}


int cnwn_write_uint16(int fd, uint16_t i);
