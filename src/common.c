#include "cnwn/common.h"

const char CNWN_PATH_SEPARATOR = '/';
const char CNWN_PATH_ESCAPE = '\\';

static char CNWN_ERROR_MESSAGE[8192] = {0};

const char * cnwn_get_error(void)
{
    return CNWN_ERROR_MESSAGE;
}

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

int cnwn_exists(const char * path)
{
    struct stat st = {0};
    if (stat(path, &st) < 0) {
        if (errno != ENOENT)
            return 0;
        cnwn_set_error("invalid path '%s', %s", path, strerror(errno));
        return -1;
    }
    return 1;
}

int cnwn_mkdirs(const char * path)
{
    int ret = 0;
    if (path != NULL && path[0] != 0) {
        for (int i = 0; path[i] != 0 && i < CNWN_PATH_MAX_SIZE - 1; i++) {
            if (path[i] == CNWN_PATH_SEPARATOR) {
                bool escaped = false;
                for (int j = i - 1; j > 0 && path[j] == CNWN_PATH_ESCAPE; j--)
                    escaped = (escaped ? false : true);
                if (!escaped) {
                    char tmppath[CNWN_PATH_MAX_SIZE];
                    memcpy(tmppath, path, sizeof(char) * i);
                    tmppath[i] = 0;
                    cnwn_unescape_path(tmppath);
                    int test = cnwn_exists(tmppath);
                    if (test < 0) 
                        return -1;
                    if (test == 0) {
                        int mkret = mkdir(tmppath, S_IRUSR | S_IWUSR | S_IXUSR | S_IRGRP | S_IWGRP | S_IXGRP);
                        if (mkret < 0) {
                            cnwn_set_error("could not create dir '%s', %s\n", tmppath, strerror(errno));
                            return -1;
                        }
                        ret++;
                    }
                }
            }
        }
        int plen = strlen(path);
        if (plen > 0) {
            char tmppath[CNWN_PATH_MAX_SIZE];
            memcpy(tmppath, path, sizeof(char) * plen);
            tmppath[plen] = 0;
            cnwn_unescape_path(tmppath);
            int test = cnwn_exists(tmppath);
            if (test < 0) 
                return -1;
            if (test == 0) {
                int mkret = mkdir(tmppath, S_IRUSR | S_IWUSR | S_IXUSR | S_IRGRP | S_IWGRP | S_IXGRP);
                if (mkret < 0) {
                    cnwn_set_error("could not create dir '%s', %s\n", tmppath, strerror(errno));
                    return -1;
                }
                ret++;
            }
        }
    }
    return ret;
}

int cnwn_unescape_path(char * path)
{
    if (path != NULL) {
        int len = strlen(path);
        return len;
    }
    return 0;
}
