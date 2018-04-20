#include "cnwn/file.h"

const char CNWN_PATH_SEPARATOR = '/';
const char CNWN_PATH_ESCAPE = '\\';

#ifdef SOME_PLATFORM
#else
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <dirent.h>
struct cnwn_File_s { int fd; };
#endif

cnwn_File * cnwn_file_open_r(const char * path)
{
#ifdef SOME_PLATFORM
#else
    // POSIX
    int fd = open(path, O_RDONLY, 0);
    if (fd < 0) {
        cnwn_set_error("%s", strerror(errno));
        return NULL;
    }
    cnwn_File * ret = malloc(sizeof(cnwn_File));
    ret->fd = fd;
    return ret;
#endif
}

cnwn_File * cnwn_file_open_w(const char * path)
{
#ifdef SOME_PLATFORM
#else
    int fd = open(path, O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP);
    if (fd < 0) {
        cnwn_set_error("%s", strerror(errno));
        return NULL;
    }
    cnwn_File * ret = malloc(sizeof(cnwn_File));
    ret->fd = fd;
    return ret;
#endif
}

cnwn_File * cnwn_file_open_rw(const char * path)
{
#ifdef SOME_PLATFORM
#else
    int fd = open(path, O_RDWR | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP);
    if (fd < 0) {
        cnwn_set_error("%s", strerror(errno));
        return NULL;
    }
    cnwn_File * ret = malloc(sizeof(cnwn_File));
    ret->fd = fd;
    return ret;
#endif
}

cnwn_File * cnwn_file_open_rw_notrunc(const char * path)
{
#ifdef SOME_PLATFORM
#else
    int fd = open(path, O_RDWR | O_CREAT, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP);
    if (fd < 0) {
        cnwn_set_error("%s", strerror(errno));
        return NULL;
    }
    cnwn_File * ret = malloc(sizeof(cnwn_File));
    ret->fd = fd;
    return ret;
#endif
}

void cnwn_file_close(cnwn_File * f)
{
    if (f != NULL) {
#ifdef SOME_PLATFORM
#else
        close(f->fd);
#endif
        free(f);
    }
}

int64_t cnwn_file_seek(cnwn_File * f, int64_t offset)
{
    if (f == NULL) {
        cnwn_set_error("file is NULL");
        return -1;
    }
#ifdef SOME_PLATFORM
#else
    off_t ret = lseek(f->fd, offset, SEEK_SET);
    if (ret < 0) {
        cnwn_set_error("%s", strerror(errno));
        return -1;
    }
    return ret;
#endif
}

int64_t cnwn_file_seek_delta(cnwn_File * f, int64_t delta_offset)
{
    if (f == NULL) {
        cnwn_set_error("file is NULL");
        return -1;
    }
#ifdef SOME_PLATFORM
#else
    off_t ret = lseek(f->fd, delta_offset, SEEK_CUR);
    if (ret < 0) {
        cnwn_set_error("%s", strerror(errno));
        return -1;
    }
    return ret;
#endif
}

int64_t cnwn_file_seek_start(cnwn_File * f)
{
    if (f == NULL) {
        cnwn_set_error("file is NULL");
        return -1;
    }
#ifdef SOME_PLATFORM
#else
    off_t ret = lseek(f->fd, 0, SEEK_SET);
    if (ret < 0) {
        cnwn_set_error("%s", strerror(errno));
        return -1;
    }
    return ret;
#endif
}

int64_t cnwn_file_seek_end(cnwn_File * f)
{
    if (f == NULL) {
        cnwn_set_error("file is NULL");
        return -1;
    }
#ifdef SOME_PLATFORM
#else
    off_t ret = lseek(f->fd, 0, SEEK_END);
    if (ret < 0) {
        cnwn_set_error("%s", strerror(errno));
        return -1;
    }
    return ret;
#endif
}

int64_t cnwn_file_seek_cur(cnwn_File * f)
{
    if (f == NULL) {
        cnwn_set_error("file is NULL");
        return -1;
    }
#ifdef SOME_PLATFORM
#else
    off_t ret = lseek(f->fd, 0, SEEK_CUR);
    if (ret < 0) {
        cnwn_set_error("%s", strerror(errno));
        return -1;
    }
    return ret;
#endif
}

int64_t cnwn_file_read(cnwn_File * f, int64_t size, uint8_t * ret_data)
{
    if (f == NULL) {
        cnwn_set_error("file is NULL");
        return -1;
    }
    if (size <= 0)
        return 0;
#ifdef SOME_PLATFORM
#else
    if (ret_data != NULL) {
        ssize_t ret = read(f->fd, ret_data, size);
        if (ret < 0) {
            cnwn_set_error("%s", strerror(errno));
            return -1;
        }
        return ret;
    }
    int64_t ret = 0;
    uint8_t buffer[CNWN_READ_WRITE_BUFFER_SIZE];
    for (int i = 0; i < size / CNWN_READ_WRITE_BUFFER_SIZE; i++) {
        ssize_t readret = read(f->fd, buffer, CNWN_READ_WRITE_BUFFER_SIZE);
        if (readret < 0) {
            cnwn_set_error("%s", strerror(errno));
            return -1;
        }
        ret += readret;
    }
    if (size % CNWN_READ_WRITE_BUFFER_SIZE) {
        int readret = read(f->fd, buffer, size % CNWN_READ_WRITE_BUFFER_SIZE);
        if (readret < 0) {
            cnwn_set_error("%s", strerror(errno));
            return -1;
        }
        ret += readret;
    }
    return ret;
#endif
}

int64_t cnwn_file_write(cnwn_File * f, int64_t size, const uint8_t * data)
{
    if (f == NULL) {
        cnwn_set_error("file is NULL");
        return -1;
    }
    if (size <= 0)
        return 0;
#ifdef SOME_PLATFORM
#else
    if (data != NULL) {
        ssize_t ret = write(f->fd, data, size);
        if (ret < 0) {
            cnwn_set_error("%s", strerror(errno));
            return -1;
        }
        return ret;
    }
    int64_t ret = 0;
    uint8_t buffer[CNWN_READ_WRITE_BUFFER_SIZE] = {0};
    for (int i = 0; i < size / CNWN_READ_WRITE_BUFFER_SIZE; i++) {
        ssize_t writeret = write(f->fd, buffer, CNWN_READ_WRITE_BUFFER_SIZE);
        if (writeret < 0) {
            cnwn_set_error("%s", strerror(errno));
            return -1;
        }
        ret += writeret;
    }
    if (size % CNWN_READ_WRITE_BUFFER_SIZE) {
        int writeret = write(f->fd, buffer, size % CNWN_READ_WRITE_BUFFER_SIZE);
        if (writeret < 0) {
            cnwn_set_error("%s", strerror(errno));
            return -1;
        }
        ret += writeret;
    }
    return ret;
#endif
}

int64_t cnwn_file_copy_bytes(cnwn_File * f, cnwn_File * output_f, int64_t size)
{
    if (f == NULL) {
        cnwn_set_error("file is NULL");
        return -1;
    }
    if (size <= 0)
        return 0;
#ifdef SOME_PLATFORM
#else
    int64_t ret = 0;
    uint8_t buffer[CNWN_READ_WRITE_BUFFER_SIZE] = {0};
    for (int i = 0; i < size / CNWN_READ_WRITE_BUFFER_SIZE; i++) {
        ssize_t readret = write(f->fd, buffer, CNWN_READ_WRITE_BUFFER_SIZE);
        if (readret < 0) {
            cnwn_set_error("%s", strerror(errno));
            return -1;
        }
        ssize_t writeret = write(output_f->fd, buffer, CNWN_READ_WRITE_BUFFER_SIZE);
        if (writeret < 0) {
            cnwn_set_error("%s", strerror(errno));
            return -1;
        }
        ret += writeret;
    }
    if (size % CNWN_READ_WRITE_BUFFER_SIZE) {
        int readret = read(f->fd, buffer, size % CNWN_READ_WRITE_BUFFER_SIZE);
        if (readret < 0) {
            cnwn_set_error("%s", strerror(errno));
            return -1;
        }
        int writeret = write(output_f->fd, buffer, size % CNWN_READ_WRITE_BUFFER_SIZE);
        if (writeret < 0) {
            cnwn_set_error("%s", strerror(errno));
            return -1;
        }
        ret += writeret;
    }
    return ret;
#endif
}

int64_t cnwn_file_read_uint64(cnwn_File * f, uint64_t * ret_i)
{
    uint8_t data[8];
    int64_t ret = cnwn_file_read(f, 8, data);
    if (ret < 0)
        return -1;
    if (ret < 8) {
        cnwn_set_error("only read %"PRId64" of 8 bytes", ret);
        return -1;
    }
    if (ret_i != NULL)
        *ret_i = *((uint64_t *)data);
    return ret;
}

int64_t cnwn_file_write_uint64(cnwn_File * f, uint64_t i)
{
    uint8_t data[8];
    *((uint64_t *)data) = i;
    int64_t ret = cnwn_file_write(f, 8, data);
    if (ret < 0)
        return -1;
    return ret;
}

int64_t cnwn_file_read_uint32(cnwn_File * f, uint32_t * ret_i)
{
    uint8_t data[4];
    int64_t ret = cnwn_file_read(f, 4, data);
    if (ret < 0)
        return -1;
    if (ret < 4) {
        cnwn_set_error("only read %"PRId64" of 4 bytes", ret);
        return -1;
    }
    if (ret_i != NULL)
        *ret_i = *((uint32_t *)data);
    return ret;
}

int64_t cnwn_file_write_uint32(cnwn_File * f, uint32_t i)
{
    uint8_t data[4];
    *((uint32_t *)data) = i;
    int64_t ret = cnwn_file_write(f, 4, data);
    if (ret < 0)
        return -1;
    return ret;
}

int64_t cnwn_file_read_uint16(cnwn_File * f, uint16_t * ret_i)
{
    uint8_t data[2];
    int64_t ret = cnwn_file_read(f, 2, data);
    if (ret < 0)
        return -1;
    if (ret < 2) {
        cnwn_set_error("only read %"PRId64" of 2 bytes", ret);
        return -1;
    }
    if (ret_i != NULL)
        *ret_i = *((uint16_t *)data);
    return ret;
}

int64_t cnwn_file_write_uint16(cnwn_File * f, uint16_t i)
{
    uint8_t data[2];
    *((uint16_t *)data) = i;
    int64_t ret = cnwn_file_write(f, 2, data);
    if (ret < 0)
        return -1;
    return ret;
}

int64_t cnwn_file_read_uint8(cnwn_File * f, uint8_t * ret_i)
{
    uint8_t data[1];
    int64_t ret = cnwn_file_read(f, 1, data);
    if (ret < 0)
        return -1;
    if (ret < 1) {
        cnwn_set_error("only read %"PRId64" bytes", ret);
        return -1;
    }
    if (ret_i != NULL)
        *ret_i = *((uint8_t *)data);
    return ret;
}

int64_t cnwn_file_write_uint8(cnwn_File * f, uint8_t i)
{
    uint8_t data[1];
    *((uint8_t *)data) = i;
    int64_t ret = cnwn_file_write(f, 1, data);
    if (ret < 0)
        return -1;
    return ret;
}

int cnwn_path_unescape(char * s, int max_size, const char * path)
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
    return cnwn_copy_string(s, max_size, tmps, -1);
}

int cnwn_path_concat(char * s, int max_size, ...)
{
    int offset = 0;
    char tmps[CNWN_PATH_MAX_SIZE];
    va_list args;
    va_start(args, max_size);
    char * as;
    while ((as = va_arg(args, char *)) != NULL && offset < sizeof(tmps) - 1) {
        if (offset > 0 && offset < sizeof(tmps) - 1)
            tmps[offset++] = CNWN_PATH_SEPARATOR;
        int aslen = strlen(as);
        aslen = CNWN_MINMAX(aslen, 0, sizeof(tmps) - offset - 1);
        if (aslen > 0) {
            if (as[aslen - 1] == CNWN_PATH_SEPARATOR) {
                bool escaped = false;
                for (int j = aslen - 1; j >= 0 && as[j] == CNWN_PATH_ESCAPE; j--)
                    escaped = !escaped;
                if (!escaped)
                    aslen--;
            }
            memcpy(tmps + offset, as, sizeof(char) * aslen);
        }
        offset += aslen;
    }    
    va_end(args);
    tmps[offset] = 0;
    return cnwn_copy_string(s, max_size, tmps, -1);
}

char ** cnwn_path_split(const char * path, int max_splits)
{
    if (path == NULL || path[0] == 0) {
        char ** ret = malloc(sizeof(char *) * 2);
        ret[0] = malloc(sizeof(char));
        ret[0][0] = 0;
        ret[1] = NULL;
        return ret;
    }
    int count = 0;
    for (int i = 0; path[i] != 0 && (count < max_splits || max_splits < 0); i++) {
        if (path[i] == CNWN_PATH_SEPARATOR) {
            bool escaped = false;
            for (int j = i - 1; j >= 0 && path[j] == CNWN_PATH_ESCAPE; j--)
                escaped = !escaped;
            if (!escaped)
                count++;
        }
    }
    int offset = 0;
    int last = 0;
    char ** ret = malloc(sizeof(char *) * (count + 2));
    int index = 0;
    for (int i = 0; path[i] != 0 && index < count; i++) {
        if (path[i] == CNWN_PATH_SEPARATOR) {
            bool escaped = false;
            for (int j = i - 1; j >= 0 && path[j] == CNWN_PATH_ESCAPE; j--)
                escaped = !escaped;
            if (!escaped) {
                int slen = offset - last - 1;
                ret[index] = malloc(sizeof(char) * (slen + 1));
                if (slen > 0)
                    memcpy(ret[index], path + i, sizeof(char) * slen);
                ret[index][slen] = 0;
                last = offset + 1;
                index++;
            }
        }
    }
    ret[index] = 0;
    return ret;
}

int cnwn_path_join(char * s, int max_size, char ** strings)
{
    int offset = 0;
    if (strings != NULL) {
        for (char ** p = strings; *p != NULL && offset < max_size - 1; p++) {
            int plen = strlen(*p);
            if (max_size > 0) {
                plen = CNWN_MINMAX(plen, 0, max_size - offset - 1);
                if (s != NULL) {
                    if (plen > 0)
                        memcpy(s + offset, *p, sizeof(char) * plen);
                }
            } 
            offset += plen;
            if (*(p + 1) != NULL) {
                if (max_size > 0) {
                    if (offset < max_size - 1) {
                        if (s != NULL)
                            s[offset] = CNWN_PATH_SEPARATOR;
                        offset++;
                    }
                } else
                    offset++;
            }
        }
    }
    if (s != NULL && max_size > 0)
        s[offset] = 0;
    return offset;
}

int cnwn_path_dirpart(char * s, int max_size, const char * path)
{
    if (path != NULL) {
        int plen = strlen(path);
        for (int i = plen - 1; i >= 0; i--) {
            if (path[i] == CNWN_PATH_SEPARATOR) {
                bool escaped = false;
                for (int j = i - 1; j >= 0 && path[j] == CNWN_PATH_ESCAPE; j--)
                    escaped = !escaped;
                if (!escaped) 
                    return cnwn_copy_string(s, max_size, path, i + 1);
            }
        }
    }
    if (s != NULL && max_size > 0)
        s[0] = 0;
    return 0;
}

int cnwn_path_basepart(char * s, int max_size, const char * path)
{
    if (path != NULL) {
        int plen = strlen(path);
        for (int i = plen - 1; i >= 0; i--) {
            if (path[i] == CNWN_PATH_SEPARATOR) {
                bool escaped = false;
                for (int j = i - 1; j >= 0 && path[j] == CNWN_PATH_ESCAPE; j--)
                    escaped = !escaped;
                if (!escaped) 
                    return cnwn_copy_string(s, max_size, path + i + 1, -1);
            }
        }
        return cnwn_copy_string(s, max_size, path, -1);
    }
    if (s != NULL && max_size > 0)
        s[0] = 0;
    return 0;
}

int cnwn_path_filepart(char * s, int max_size, const char * path)
{
    char tmps[CNWN_PATH_MAX_SIZE];
    cnwn_path_basepart(tmps, sizeof(tmps), path);
    for (int i = 0; tmps[i] != 0; i++) 
        if (tmps[i] == '.') 
            return cnwn_copy_string(s, max_size, tmps, i);
    return cnwn_copy_string(s, max_size, tmps, -1);
}

int cnwn_path_extpart(char * s, int max_size, const char * path)
{
    char tmps[CNWN_PATH_MAX_SIZE];
    cnwn_path_basepart(tmps, sizeof(tmps), path);
    for (int i = 0; tmps[i] != 0; i++) 
        if (tmps[i] == '.') 
            return cnwn_copy_string(s, max_size, tmps + i + 1, -1);
    if (s != NULL && max_size > 0)
        s[0] = 0;
    return 0;
}

int cnwn_path_exists(const char * path)
{
    if (path == NULL || path[0] == 0) {
        cnwn_set_error("invalid empty path");
        return -1;
    }
#ifdef SOME_PLATFORM
#else
    struct stat st = {0};
    if (stat(path, &st) < 0 && errno != ENOENT) {
        cnwn_set_error("%s", strerror(errno));
        return -1;
    }
    if (S_ISDIR(st.st_mode) || S_ISREG(st.st_mode))
        return 1;
    return 0;
#endif
}

int cnwn_path_isdir(const char * path)
{
    if (path == NULL || path[0] == 0) {
        cnwn_set_error("invalid empty path");
        return -1;
    }
#ifdef SOME_PLATFORM
#else
    struct stat st = {0};
    if (stat(path, &st) < 0 && errno != ENOENT) {
        cnwn_set_error("%s", strerror(errno));
        return -1;
    }
    if (S_ISDIR(st.st_mode))
        return 1;
    return 0;
#endif
}

int cnwn_path_isfile(const char * path)
{
    if (path == NULL || path[0] == 0) {
        cnwn_set_error("invalid empty path");
        return -1;
    }
#ifdef SOME_PLATFORM
#else
    struct stat st = {0};
    if (stat(path, &st) < 0 && errno != ENOENT) {
        cnwn_set_error("%s", strerror(errno));
        return -1;
    }
    if (S_ISREG(st.st_mode))
        return 1;
    return 0;
#endif
}

static int cnwn_mkdir(const char * path) {
    if (path == NULL || path[0] == 0) {
        cnwn_set_error("invalid empty path");
        return -1;
    }
#ifdef SOME_PLATFORM
#else
    int exists = cnwn_path_isdir(path);
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
#endif
}

int cnwn_mkdirs(const char * path)
{
    if (path == NULL || path[0] == 0) {
        cnwn_set_error("invalid empty path");
        return -1;
    }
    int ret = 0;
    int i;
    int last = 0;
    for (i = 0; path[i] != 0; i++) {
        if (path[i] == CNWN_PATH_SEPARATOR) {
            bool escaped = false;
            for (int j = i - 1; j >= 0 && path[j] == CNWN_PATH_ESCAPE; j--)
                escaped = !escaped;
            if (!escaped) {
                char tmppath[CNWN_PATH_MAX_SIZE];
                cnwn_copy_string(tmppath, sizeof(tmppath), path, i + 1);
                int mkdirret = cnwn_mkdir(tmppath);
                if (mkdirret < 0)
                    return -1;
                ret += mkdirret;
                last = i;
            }
        }
    }
    if (last < i - 1) {
        char tmppath[CNWN_PATH_MAX_SIZE];
        cnwn_copy_string(tmppath, sizeof(tmppath), path, i + 1);
        int mkdirret = cnwn_mkdir(tmppath);
        if (mkdirret < 0)
            return -1;
        ret += mkdirret;
    }
    return ret;
}

char ** cnwn_listdir(const char * path, bool full, const cnwn_RegexpArray * regexp_array)
{
    if (path == NULL)
        path = "."; // FIXME: different on windows maybe?
    DIR * dp;
    struct dirent * ep;
    dp = opendir(path);
    if (dp == NULL) {
        cnwn_set_error("%s", strerror(errno));
        return NULL;
    }
    int count = 0;
    while ((ep = readdir(dp))) {
        if (ep->d_name != NULL && ep->d_name[0] != 0) {
            char tmps[CNWN_PATH_MAX_SIZE];
            cnwn_path_concat(tmps, sizeof(tmps), path, ep->d_name, NULL);
            if (cnwn_path_isfile(tmps) == 1
                && cnwn_regexp_array_match_any(regexp_array, ep->d_name))
                count++;
        }
    }
    rewinddir(dp);
    char ** ret = malloc(sizeof(char *) * (count + 1));
    int index = 0;
    while ((ep = readdir(dp)) && index < count) {
        if (ep->d_name != NULL && ep->d_name[0] != 0) {
            char tmps[CNWN_PATH_MAX_SIZE];
            cnwn_path_concat(tmps, sizeof(tmps), path, ep->d_name, NULL);
            if (cnwn_path_isfile(tmps) == 1
                && cnwn_regexp_array_match_any(regexp_array, ep->d_name)) {
                int namelen = full ? strlen(tmps) : strlen(ep->d_name);
                ret[index] = malloc(sizeof(char) * (namelen + 1));
                cnwn_copy_string(ret[index], namelen + 1, (full ? tmps : ep->d_name), -1);
                index++;
            }
        }
    }
    ret[index] = NULL;
    closedir(dp);
    return ret;
}

char ** cnwn_listdir2(const char * path, bool full, bool extended, const char ** strings)
{
    if (path == NULL)
        path = "."; // FIXME: different on windows maybe?
    cnwn_RegexpArray regexp_array;
    if (cnwn_regexp_array_init2(&regexp_array, extended, strings) < 0)
        return NULL;
    char ** ret = cnwn_listdir(path, full, &regexp_array);
    cnwn_regexp_array_deinit(&regexp_array);
    return ret;
}

int64_t cnwn_path_filesize(const char * path)
{
     if (path == NULL || path[0] == 0) {
        cnwn_set_error("invalid empty path");
        return -1;
    }
#ifdef SOME_PLATFORM
#else
     struct stat st; 
     if (stat(path, &st) != 0) {
         cnwn_set_error("%s", strerror(errno));
         return -1;
     }
     return st.st_size;
#endif
}
