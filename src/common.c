#include "cnwn.h"

const char CNWN_PATH_SEPARATOR = '/';
const char CNWN_PATH_ESCAPE = '\\';

static char CNWN_ERROR_MESSAGE[8192] = {0};

const char * cnwn_get_error(void)
{
    return CNWN_ERROR_MESSAGE;
}

void cnwn_set_error_va(const char * format, va_list args)
{
    char tmps[8192];
    int len = vsnprintf(tmps, sizeof(tmps), format, args);
    if (len > 0)
        memcpy(CNWN_ERROR_MESSAGE, tmps, sizeof(char) * len);
    CNWN_ERROR_MESSAGE[len] = 0;
}

void cnwn_set_error(const char * format, ...)
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


int64_t cnwn_seek(int fd, int64_t offset)
{
    off_t ret = lseek(fd, offset, SEEK_SET);
    if (ret < 0) {
        cnwn_set_error("%s", strerror(errno));
        return -1;
    }
    return ret;
}

int64_t cnwn_seek_delta(int fd, int64_t delta_offset)
{
    off_t ret = lseek(fd, delta_offset, SEEK_CUR);
    if (ret < 0) {
        cnwn_set_error("%s", strerror(errno));
        return -1;
    }
    return ret;
}

int64_t cnwn_seek_end(int fd)
{
    off_t ret = lseek(fd, 0, SEEK_END);
    if (ret < 0) {
        cnwn_set_error("%s", strerror(errno));
        return -1;
    }
    return ret;
}

int64_t cnwn_seek_cur(int fd)
{
    off_t ret = lseek(fd, 0, SEEK_CUR);
    if (ret < 0) {
        cnwn_set_error("%s", strerror(errno));
        return -1;
    }
    return ret;
}

int64_t cnwn_read_uint32(int fd, uint32_t * ret_i)
{
    uint8_t buffer[4];
    int64_t ret = read(fd, buffer, 4);
    if (ret < 0) {
        cnwn_set_error("%s", strerror(errno));
        return -1;
    }
    if (ret < 4) {
        cnwn_set_error("not enough data (%"PRId64")", ret);
        return -1;
    }
    if (ret_i != NULL)
        *ret_i = cnwn_swap32(*((uint32_t *)buffer));
    return ret;
}


int64_t cnwn_write_uint32(int fd, uint32_t i);

int64_t cnwn_read_uint16(int fd, uint16_t * ret_i)
{
    uint8_t buffer[2];
    int64_t ret = read(fd, buffer, 2);
    if (ret < 0) {
        cnwn_set_error("%s", strerror(errno));
        return -1;
    }
    if (ret < 2) {
        cnwn_set_error("not enough data (%"PRId64")", ret);
        return -1;
    }
    if (ret_i != NULL)
        *ret_i = cnwn_swap16(*((uint16_t *)buffer));
    return ret;
}


int64_t cnwn_write_uint16(int fd, uint16_t i);


int64_t cnwn_read_bytes(int fd, int64_t size, uint8_t * ret_data)
{
    if (size > 0) {
        if (ret_data != NULL) {
            int64_t ret = read(fd, ret_data, size);
            if (ret < 0) {
                cnwn_set_error("%s", strerror(errno));
                return -1;
            }
            return ret;
        }
        int64_t ret = 0;
        uint8_t tmp[CNWN_READ_WRITE_BUFFER_SIZE];
        for (int i = 0; i < size / CNWN_READ_WRITE_BUFFER_SIZE; i++) {
            int64_t tret = read(fd, tmp, CNWN_READ_WRITE_BUFFER_SIZE);
            if (tret < 0) {
                cnwn_set_error("%s", strerror(errno));
                return -1;
            }
            ret += tret;
        }
        if (size % CNWN_READ_WRITE_BUFFER_SIZE) {
            int64_t tret = read(fd, tmp, size % CNWN_READ_WRITE_BUFFER_SIZE);
            if (tret < 0) {
                cnwn_set_error("%s", strerror(errno));
                return -1;
            }
            ret += tret;
        }
        return ret;
    }
    return 0;
}

int64_t cnwn_write_bytes(int fd, int64_t size, const uint8_t * data)
{
    if (size > 0) {
        if (data != NULL) {
            int64_t ret = write(fd, data, size);
            if (ret < 0) {
                cnwn_set_error("%s", strerror(errno));
                return -1;
            }
            return ret;
        }
        int64_t ret = 0;
        uint8_t tmp[CNWN_READ_WRITE_BUFFER_SIZE];
        memset(tmp, 0, sizeof(tmp));
        for (int i = 0; i < size / CNWN_READ_WRITE_BUFFER_SIZE; i++) {
            int64_t tret = write(fd, tmp, CNWN_READ_WRITE_BUFFER_SIZE);
            if (tret < 0) {
                cnwn_set_error("%s", strerror(errno));
                return -1;
            }
            ret += tret;
        }
        if (size % CNWN_READ_WRITE_BUFFER_SIZE) {
            int64_t tret = write(fd, tmp, size % CNWN_READ_WRITE_BUFFER_SIZE);
            if (tret < 0) {
                cnwn_set_error("%s", strerror(errno));
                return -1;
            }
            ret += tret;
        }
        return ret;
    }
    return 0;
}

int cnwn_clean_string(const char * s, int max_size, char * ret_s)
{
    int offset = 0;
    if (s != NULL) {
        for (int i = 0; s[i] != 0 && (offset < max_size - 1 || (max_size <= 0 && ret_s == NULL)); i++) {
            if (s[i] > 32) {
                if (max_size > 0 && ret_s != NULL)
                    ret_s[offset++] = s[i];
                else
                    offset++;
            }
        }
    }
    if (ret_s != NULL)
        ret_s[offset] = 0;
    return offset;
}
