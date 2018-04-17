#include "cnwn/file.h"

const char CNWN_PATH_SEPARATOR = '/';
const char CNWN_PATH_ESCAPE = '\\';

int cnwn_open_read(const char * path)
{
    int fd = open(path, O_RDONLY, 0);
    if (fd < 0) {
        cnwn_set_error("%s", strerror(errno));
        return -1;
    }
    return fd;
}

int cnwn_open_write(const char * path)
{
    int fd = open(path, O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP);
    if (fd < 0) {
        cnwn_set_error("%s", strerror(errno));
        return -1;
    }
    return fd;
}

void cnwn_close(int fd)
{
    close(fd);
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
        cnwn_set_error("unexpected EOF");
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
        cnwn_set_error("unexpected EOF");
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

int cnwn_path_exists(const char * path)
{
    struct stat st = {0};
    if (stat(path, &st) < 0) {
        if (errno != ENOENT)
            return 0;
        cnwn_set_error("%s", strerror(errno));
        return -1;
    }
    return 1;
}

int64_t cnwn_copy_bytes(int fd, int out_fd, int64_t size)
{
    if (size <= 0)
        return 0;
    int64_t retbytes = 0;
    uint8_t buffer[CNWN_READ_WRITE_BUFFER_SIZE];
    for (int i = 0; i < size / CNWN_READ_WRITE_BUFFER_SIZE; i++) {
        int ret = read(fd, buffer, CNWN_READ_WRITE_BUFFER_SIZE);
        if (ret < 0) 
            return -1;
        ret = write(out_fd, buffer, ret);
        if (ret < 0)
            return -1;
        retbytes += ret;
    }
    if (size % CNWN_READ_WRITE_BUFFER_SIZE) {
        int ret = read(fd, buffer, CNWN_READ_WRITE_BUFFER_SIZE);
        if (ret < 0) 
            return -1;
        ret = write(out_fd, buffer, ret);
        if (ret < 0)
            return -1;
        retbytes += ret;
    }
    return retbytes;
}

int cnwn_directory_exists(const char * path)
{
    struct stat st = {0};
    if (stat(path, &st) < 0 && errno != ENOENT) {
        cnwn_set_error("%s", strerror(errno));
        return -1;
    }
    if (S_ISDIR(st.st_mode))
        return 1;
    return 0;
}

int cnwn_file_exists(const char * path)
{
    struct stat st = {0};
    if (stat(path, &st) < 0 && errno != ENOENT) {
        cnwn_set_error("%s", strerror(errno));
        return -1;
    }
    if (S_ISREG(st.st_mode))
        return 1;
    return 0;
}

static int cnwn_setup_path_mkdir(const char * path, int len, int max_size, char * ret_path)
{
    int plen = path != NULL ? strlen(path) : 0;
    if (len < 0)
        len = plen;
    len = CNWN_MIN(len, plen);
    len = CNWN_MIN(len, CNWN_PATH_MAX_SIZE - 1);
    char tmps[CNWN_PATH_MAX_SIZE];
    if (len > 0) {
        memcpy(tmps, path, sizeof(char) * len);
        if (max_size > 0)
            len = CNWN_MIN(len, max_size - 1);
        tmps[len] = 0;
        cnwn_path_unescape(tmps, sizeof(tmps), tmps);
        if (ret_path != NULL) {
            memcpy(ret_path, tmps, sizeof(char) * len);
            ret_path[len] = 0;
        }
    }
    return len;
}

static int cnwn_mkdir(const char * path) {
    int exists = cnwn_directory_exists(path);
    if (exists < 0) 
        return -1;
    if (exists == 0) {
        int mkret = mkdir(path, S_IRUSR | S_IWUSR | S_IXUSR | S_IRGRP | S_IWGRP | S_IXGRP);
        if (mkret < 0) {
            cnwn_set_error("%s\n", strerror(errno));
            return -1;
        }
        return 1;
    }
    return 0;
}

int cnwn_mkdirs(const char * path)
{
    int ret = 0;
    if (path != NULL && path[0] != 0) {
        int i = 0;
        while (path[i] != 0 && i < CNWN_PATH_MAX_SIZE - 1) {
            if (path[i] == CNWN_PATH_SEPARATOR) {
                bool escaped = false;
                for (int j = i - 1; j > 0 && path[j] == CNWN_PATH_ESCAPE; j--)
                    escaped = (escaped ? false : true);
                if (!escaped) {
                    char tmppath[CNWN_PATH_MAX_SIZE];
                    cnwn_setup_path_mkdir(path, i, sizeof(tmppath), tmppath);
                    int mret = cnwn_mkdir(tmppath);
                    if (mret < 0) {
                        cnwn_set_error("mkdir %s, %s\n", tmppath, strerror(errno));
                        return -1;
                    }
                    ret += mret;
                }
            }
            i++;
        }
        char tmppath[CNWN_PATH_MAX_SIZE];
        cnwn_setup_path_mkdir(path, i, sizeof(tmppath), tmppath);
        int mret = cnwn_mkdir(tmppath);
        if (mret < 0) {
            cnwn_set_error("mkdir %s, %s\n", tmppath, strerror(errno));
            return -1;
        }
        ret += mret;
    }
    return ret;
}

int cnwn_path_unescape(const char * path, int max_size, char * ret_path)
{
    char tmps[CNWN_PATH_MAX_SIZE];
    int offset = 0;
    if (path != NULL) {
        for (int i = 0; path[i] != 0 && offset < sizeof(tmps) - 1; i++) {
            if (path[i] != CNWN_PATH_ESCAPE || (path[i + 1] != CNWN_PATH_SEPARATOR && path[i + 1] == CNWN_PATH_ESCAPE)) 
                tmps[offset++] = path[i];
        }
    }
    tmps[offset] = 0;
    int copylen = strlen(tmps);
    if (max_size > 0) {
        copylen = CNWN_MIN(copylen, max_size - 1);
        if (ret_path != NULL) {
            if (copylen > 0)
                memcpy(ret_path, tmps, sizeof(char) * copylen);
            ret_path[copylen] = 0;
        }
    }
    return copylen;
}

int cnwn_path_filename(const char * path, int max_size, char * ret_filename)
{
    char tmps[CNWN_PATH_MAX_SIZE];
    tmps[0] = 0;
    int len = path != NULL ? strlen(path) : 0;
    if (len > 0) {
        int offset = len - 1;
        while (offset >= 0) {
            if (path[offset] == CNWN_PATH_SEPARATOR) {
                bool escaped = false;
                for (int i = offset - 1; i > 0 && path[i] == CNWN_PATH_ESCAPE; i--)
                    escaped = (escaped ? false : true);
                if (!escaped) {
                    offset++;
                    break;
                }
            }
            offset--;
        }
        offset = CNWN_MAX(0, offset);
        int copylen = len - offset;
        copylen = CNWN_MINMAX(copylen, 0, sizeof(tmps) - 1);
        if (copylen > 0)
            memcpy(tmps, path + offset, sizeof(char) * copylen);
        tmps[copylen] = 0;
    }
    int copylen = strlen(tmps);
    if (max_size > 0) {
        copylen = CNWN_MIN(copylen, max_size - 1);
        if (ret_filename != NULL) {
            if (copylen > 0)
                memcpy(ret_filename, tmps, sizeof(char) * copylen);
            ret_filename[copylen] = 0;
        }
    }
    return copylen;
}

int cnwn_path_directory(const char * path, int max_size, char * ret_directory)
{
    char tmps[CNWN_PATH_MAX_SIZE];
    tmps[0] = 0;
    int len = path != NULL ? strlen(path) : 0;
    if (len > 0) {
        int offset = len - 1;
        while (offset >= 0) {
            if (path[offset] == CNWN_PATH_SEPARATOR) {
                bool escaped = false;
                for (int i = offset - 1; i > 0 && path[i] == CNWN_PATH_ESCAPE; i--)
                    escaped = (escaped ? false : true);
                if (!escaped) {
                    offset++;
                    break;
                }
            }
            offset--;
        }
        int copylen = offset;
        copylen = CNWN_MINMAX(copylen, 0, sizeof(tmps) - 1);
        if (copylen > 0)
            memcpy(tmps, path, sizeof(char) * copylen);
        tmps[copylen] = 0;
    }
    int copylen = strlen(tmps);
    if (max_size > 0) {
        copylen = CNWN_MIN(copylen, max_size - 1);
        if (ret_directory != NULL) {
            if (copylen > 0)
                memcpy(ret_directory, tmps, sizeof(char) * copylen);
            ret_directory[copylen] = 0;
        }
    }
    return copylen;
}

int cnwn_path_extension(const char * path, int max_size, char * ret_extension)
{
    char tmps[CNWN_PATH_MAX_SIZE];
    tmps[0] = 0;
    int len = path != NULL ? strlen(path) : 0;
    if (len > 0) {
        int offset = len - 1;
        while (offset >= 0 && path[offset] != '.') {
            if (path[offset] == CNWN_PATH_SEPARATOR) {
                bool escaped = false;
                for (int i = offset - 1; i > 0 && path[i] == CNWN_PATH_ESCAPE; i--)
                    escaped = (escaped ? false : true);
                if (!escaped) {
                    if (ret_extension != NULL && max_size > 0)
                        ret_extension[0] = 0;
                    return 0;
                }
            }
            offset--;
        }
        if (offset > 0) {
            int copylen = len - offset - 1;
            copylen = CNWN_MINMAX(copylen, 0, sizeof(tmps) - 1);
            if (copylen > 0)
                memcpy(tmps, path + offset + 1, sizeof(char) * copylen);
            tmps[copylen] = 0;
        }
    }
    int copylen = strlen(tmps);
    if (max_size > 0) {
        copylen = CNWN_MIN(copylen, max_size - 1);
        if (ret_extension != NULL) {
            if (copylen > 0)
                memcpy(ret_extension, tmps, sizeof(char) * copylen);
            ret_extension[copylen] = 0;
        }
    }
    return copylen;
}

int cnwn_path_concat(int num_paths, const char * paths[], int max_size, char * ret_path)
{
    char tmps[CNWN_PATH_MAX_SIZE];
    int tmpsoffset = 0;
    if (paths != NULL) {
        for (int i = 0; i < num_paths && tmpsoffset < sizeof(tmps) - 1; i++) {
            if (paths[i] != NULL) {
                int plen = strlen(paths[i]);
                plen = CNWN_MIN(plen, sizeof(tmps) - tmpsoffset - 1);
                plen = CNWN_MAX(0, plen);
                if (plen > 0)
                    memcpy(tmps + tmpsoffset, paths[i], sizeof(char) * plen);
                tmpsoffset += plen;
                if (i < num_paths - 1 && tmpsoffset < sizeof(tmps) - 1)
                    tmps[tmpsoffset++] = CNWN_PATH_SEPARATOR;
            }
        }
    }
    tmps[tmpsoffset] = 0;
    int copylen = strlen(tmps);
    if (max_size > 0) {
        copylen = CNWN_MIN(copylen, max_size - 1);
        if (ret_path != NULL) {
            if (copylen > 0)
                memcpy(ret_path, tmps, sizeof(char) * copylen);
            ret_path[copylen] = 0;
        }
    }
    return copylen;
}

int cnwn_path_append(const char * path, const char * append, int max_size, char * ret_path)
{
    const char * paths[2] = {path, append};
    return cnwn_path_concat(2, paths, max_size, ret_path);
}
