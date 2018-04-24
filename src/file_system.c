#include "cnwn/file_system.h"

#ifdef BUILD_WINDOWS_FILE
#include <Windows.h>
struct cnwn_File_s { HFILE hfile; };
#else
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <dirent.h>
struct cnwn_File_s { int fd; };
#endif

int cnwn_file_system_count(const char * path, bool recurse)
{
    if (path == NULL || path[0] == 0)
        path = "."; // FIXME: Maybe different on winblows?
#ifdef BUILD_WINDOWS_FILE
#else
    DIR * dp;
    struct dirent * ep;
    dp = opendir(path);
    if (dp == NULL) {
        cnwn_set_error("%s", strerror(errno));
        return -1;
    }
    int count = 0;
    while ((ep = readdir(dp))) {
        int eplen = cnwn_strlen(ep->d_name);
        if (eplen > 0 && cnwn_path_isfiledirectory(ep->d_name)) {
            char tmps[CNWN_PATH_MAX_SIZE];
            snprintf(tmps, sizeof(tmps), "%s%s%s", path, CNWN_PATH_SEPARATOR, ep->d_name);
            if (cnwn_file_system_isfile(tmps) > 0)
                count++;
            else if (cnwn_file_system_isdirectory(tmps) > 0) {
                count++;
                if (recurse) {
                    int reccount = cnwn_file_system_count(tmps, true);
                    if (reccount < 0) {
                        closedir(dp);
                        return -1;
                    }
                    count += reccount;
                }
            }
        }
    }
    closedir(dp);
    return count;
#endif
}

static int cnwn_file_system_ls_recurse(const char * path, bool recurse, char ** ret)
{
#ifdef BUILD_WINDOWS_FILE
#else
    DIR * dp;
    struct dirent * ep;
    dp = opendir(path);
    if (dp == NULL) {
        cnwn_set_error("%s", strerror(errno));
        return -1;
    }
    int index = 0;
    while ((ep = readdir(dp))) {
        int eplen = cnwn_strlen(ep->d_name);
        if (eplen > 0 && cnwn_path_isfiledirectory(ep->d_name)) {
            char tmps[CNWN_PATH_MAX_SIZE];
            snprintf(tmps, sizeof(tmps), "%s%s%s", path, CNWN_PATH_SEPARATOR, ep->d_name);
            if (cnwn_file_system_isdirectory(tmps) > 0) {
                ret[index++] = cnwn_strdup(tmps);
                if (recurse) {
                    int recret = cnwn_file_system_ls_recurse(tmps, true, ret + index);
                    if (recret < 0)
                        return -1;
                    index += recret;
                }
            } else if (cnwn_file_system_isfile(tmps))
                ret[index++] = cnwn_strdup(tmps);
        }
    }
    closedir(dp);
    return index;
#endif
}

char ** cnwn_file_system_ls(const char * path, bool recurse)
{
    if (path == NULL || path[0] == 0)
        path = "."; // FIXME: Maybe different on winblows?
    int count = cnwn_file_system_count(path, recurse);
    if (count < 0)
        return NULL;
    char ** ret = malloc(sizeof(char *) * (count + 1));
    memset(ret, 0, sizeof(char *) * (count + 1));
    int recret = cnwn_file_system_ls_recurse(path, recurse, ret);
    if (recret < 0) {
        cnwn_strings_free(ret);
        return NULL;
    }
    return ret;
}

int cnwn_file_system_mkdir(const char * path)
{
    int ret = 0;
    int seplen = cnwn_strlen(CNWN_PATH_SEPARATOR);
    char tmps[CNWN_PATH_MAX_SIZE], clean_path[CNWN_PATH_MAX_SIZE];    
    cnwn_path_clean(clean_path, sizeof(clean_path), path);
    int index = CNWN_PATH_CASE_INSENSETIVE ? cnwn_strfindi(path, 0, CNWN_PATH_SEPARATOR, CNWN_PATH_ESCAPE) : cnwn_strfind(path, 0, CNWN_PATH_SEPARATOR, CNWN_PATH_ESCAPE);
    while (index >= 0) {
        cnwn_strcpy(tmps, sizeof(tmps), clean_path, index);
        int isdir = cnwn_file_system_isdirectory(tmps);
        if (isdir < 0)
            return -1;
        if (isdir == 0) {
#ifdef BUILD_WINDOWS_FILE
#else
            int mkret = mkdir(tmps, S_IRUSR | S_IWUSR | S_IXUSR | S_IRGRP | S_IWGRP | S_IXGRP);
            if (mkret < 0) {
                char tmps2[CNWN_PATH_MAX_SIZE];
                cnwn_strescape(tmps2, sizeof(tmps2), tmps, "\"", "\\");
                cnwn_set_error("%s \"%s\"", strerror(errno), tmps2);
                return -1;
            }
            ret++;
#endif
        }
        index += seplen;
        index = CNWN_PATH_CASE_INSENSETIVE ? cnwn_strfindi(path, index, CNWN_PATH_SEPARATOR, CNWN_PATH_ESCAPE) : cnwn_strfind(path, index, CNWN_PATH_SEPARATOR, CNWN_PATH_ESCAPE);
    }
    cnwn_strcpy(tmps, sizeof(tmps), clean_path, -1);
    int isdir = cnwn_file_system_isdirectory(tmps);
    if (isdir < 0)
        return -1;
    if (isdir == 0) {
#ifdef BUILD_WINDOWS_FILE
#else
        int mkret = mkdir(tmps, S_IRUSR | S_IWUSR | S_IXUSR | S_IRGRP | S_IWGRP | S_IXGRP);
        if (mkret < 0) {
            char tmps2[CNWN_PATH_MAX_SIZE];
            cnwn_strescape(tmps2, sizeof(tmps2), tmps, "\"", "\\");
            cnwn_set_error("%s \"%s\"", strerror(errno), tmps);
            return -1;
        }
        ret++;
#endif
    }
    return ret;
}

int cnwn_file_system_rm(const char * path)
{
    if (!cnwn_path_isfiledirectory(path)) {
        char tmps2[CNWN_PATH_MAX_SIZE];
        cnwn_strescape(tmps2, sizeof(tmps2), path, "\"", "\\");
        cnwn_set_error("invalid path \"%s\"", path);
        return -1;
    }
    int ret = 0;
    if (cnwn_file_system_isdirectory(path) > 0) {
#ifdef BUILD_WINDOWS_FILE
#else
        DIR * dp;
        struct dirent * ep;
        dp = opendir(path);
        if (dp == NULL) {
            cnwn_set_error("%s", strerror(errno));
            return -1;
        }
        while ((ep = readdir(dp))) {
            int eplen = cnwn_strlen(ep->d_name);
            if (eplen > 0 && cnwn_path_isfiledirectory(ep->d_name)) {
                char tmps[CNWN_PATH_MAX_SIZE];
                snprintf(tmps, sizeof(tmps), "%s%s%s", path, CNWN_PATH_SEPARATOR, ep->d_name);
                int recret = cnwn_file_system_rm(tmps);
                if (recret < 0) {
                    closedir(dp);
                    return -1;
                }
                ret += recret;
            }
        }
        closedir(dp);
#endif
    }
#ifdef BUILD_WINDOWS_FILE
#else
    int remret = remove(path);
    if (remret < 0) {
        cnwn_set_error("%s \"%s\"", strerror(errno), path);
        return -1;
    }
    ret++;
    return ret;
#endif
}

int cnwn_file_system_mv(const char * path, const char * to_path)
{
    if (cnwn_strcmp(path, to_path) == 0)
        return 0;
    if (!cnwn_path_isfiledirectory(path)) {
        char tmps2[CNWN_PATH_MAX_SIZE];
        cnwn_strescape(tmps2, sizeof(tmps2), path, "\"", "\\");
        cnwn_set_error("invalid path \"%s\"", path);
        return -1;
    }        
#ifdef BUILD_WINDOWS_FILE
#else
    int ret = rename(path, to_path);
    if (ret < 0) {
        cnwn_set_error("%s", strerror(errno));
        return -1;
    }
    return 1;
#endif
}

int cnwn_file_system_cp(const char * path, const char * to_path)
{
    if (cnwn_strcmp(path, to_path) == 0)
        return 0;
    if (!cnwn_path_isfiledirectory(path)) {
        char tmps2[CNWN_PATH_MAX_SIZE];
        cnwn_strescape(tmps2, sizeof(tmps2), path, "\"", "\\");
        cnwn_set_error("invalid path \"%s\"", path);
        return -1;
    }
    cnwn_File * input_f = cnwn_file_open(path, "r");
    if (input_f == NULL) {
        cnwn_set_error("%s (source)", cnwn_get_error());
        return -1;
    }
    cnwn_File * output_f = cnwn_file_open(path, "r");
    if (output_f == NULL) {
        cnwn_set_error("%s (destination)", cnwn_get_error());
        cnwn_file_close(input_f);
        return -1;
    }
    int64_t size = cnwn_file_size(input_f);
    if (size < 0) {
        cnwn_set_error("%s (size)", cnwn_get_error());
        cnwn_file_close(input_f);
        cnwn_file_close(output_f);
        return -1;
    }
    int64_t ret = cnwn_file_copy(input_f, size, output_f);
    cnwn_file_close(input_f);
    cnwn_file_close(output_f);
    if (ret < 0) {
        cnwn_set_error("%s (copy)", cnwn_get_error());
        return -1;
    }
    return ret;
}

int64_t cnwn_file_system_size(const char * path, bool recurse)
{
 #ifdef BUILD_WINDOWS_FILE
#else
   struct stat st = {0};
    if (stat(path, &st) < 0) {
        cnwn_set_error("%s", strerror(errno));
        return -1;
    }
    if (S_ISREG(st.st_mode))
        return st.st_size;
    int64_t ret = 0;
    if (S_ISDIR(st.st_mode)) {
        DIR * dp;
        struct dirent * ep;
        dp = opendir(path);
        if (dp == NULL) {
            cnwn_set_error("%s", strerror(errno));
            return -1;
        }
        while ((ep = readdir(dp))) {
            int eplen = cnwn_strlen(ep->d_name);
            if (eplen > 0 && cnwn_path_isfiledirectory(ep->d_name)) {
                char tmps[CNWN_PATH_MAX_SIZE];
                snprintf(tmps, sizeof(tmps), "%s%s%s", path, CNWN_PATH_SEPARATOR, ep->d_name);
                if (recurse) {
                    int r = cnwn_file_system_size(tmps, true);
                    if (r < 0) {
                        closedir(dp);
                        return -1;
                    }
                    ret += r;
                }
            }
        }
        closedir(dp);
    }
    return ret;
#endif
}

int cnwn_file_system_exists(const char * path)
{
#ifdef BUILD_WINDOWS_FILE
#else
    struct stat st = {0};
    if (stat(path, &st) < 0) {
        if (errno == ENOENT)
            return 0;
        cnwn_set_error("%s", strerror(errno));
        return -1;
    }
    if (S_ISREG(st.st_mode) || S_ISDIR(st.st_mode))
        return 1;
    return 0;
#endif
}


int cnwn_file_system_isfile(const char * path)
{
#ifdef BUILD_WINDOWS_FILE
#else
    struct stat st = {0};
    if (stat(path, &st) < 0) {
        if (errno == ENOENT)
            return 0;
        cnwn_set_error("%s", strerror(errno));
        return -1;
    }
    if (S_ISREG(st.st_mode))
        return 1;
    return 0;
#endif
}

int cnwn_file_system_isdirectory(const char * path)
{
#ifdef BUILD_WINDOWS_FILE
#else
    struct stat st = {0};
    if (stat(path, &st) < 0) {
        if (errno == ENOENT)
            return 0;
        cnwn_set_error("%s", strerror(errno));
        return -1;
    }
    if (S_ISDIR(st.st_mode))
        return 1;
    return 0;
#endif
}

cnwn_File * cnwn_file_open(const char * path, const char * mode)
{
    bool flag_read = false;
    bool flag_write = false;
    bool flag_truncate = false;
    if (mode != NULL) {
        for (int i = 0; mode[i] != 0; i++)
            switch (mode[i]) {
            case 'w':
            case 'W':
                flag_write = true;
                break;
            case 'r':
            case 'R':
                flag_read = true;
                break;
            case 't':
            case 'T':
                flag_truncate = true;
                break;
            default:
                cnwn_set_error("invalid mode flag '%c'\n", mode[i]);
                return NULL;
            }
    }
    if (!flag_read && !flag_write && !flag_truncate)
        flag_read = true;
    if (flag_truncate)
        flag_write = true;
#ifdef BUILD_WINDOWS_FILE
#else
    int flags = 0;
    if (flag_read && !flag_write)
        flags |= O_RDONLY;
    else if (!flag_read && flag_write) {
        flags |= O_WRONLY;
        flags |= O_CREAT;
    }
    else if (flag_read && flag_write)
        flags |= O_RDWR;
    if (flag_truncate)
        flags |= O_TRUNC;
    int fd = open(path, flags, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP);
    if (fd < 0) {
        cnwn_set_error("%s\n", strerror(errno));
        return NULL;
    }
    cnwn_File * ret = malloc(sizeof(cnwn_File));
    ret->fd = fd;
    return ret;
#endif
}

void cnwn_file_close(cnwn_File * f)
{
#ifdef BUILD_WINDOWS_FILE
#else
    if (f->fd >= 0)
        close(f->fd);
    f->fd = -1;
    free(f);
#endif
}

int64_t cnwn_file_seek(cnwn_File * f, int64_t offset)
{
#ifdef BUILD_WINDOWS_FILE
#else
    off_t ret = lseek(f->fd, offset, SEEK_SET);
    if (ret < 0) 
        cnwn_set_error("%s", strerror(errno));
    return ret;
#endif
}

int64_t cnwn_file_seek_delta(cnwn_File * f, int64_t delta_offset)
{
#ifdef BUILD_WINDOWS_FILE
#else
    off_t ret = lseek(f->fd, delta_offset, SEEK_CUR);
    if (ret < 0) 
        cnwn_set_error("%s", strerror(errno));
    return ret;
#endif
}

int64_t cnwn_file_seek_end(cnwn_File * f)
{
#ifdef BUILD_WINDOWS_FILE
#else
    off_t ret = lseek(f->fd, 0, SEEK_END);
    if (ret < 0) 
        cnwn_set_error("%s", strerror(errno));
    return ret;
#endif
}

int64_t cnwn_file_get_seek(cnwn_File * f)
{
#ifdef BUILD_WINDOWS_FILE
#else
    off_t ret = lseek(f->fd, 0, SEEK_CUR);
    if (ret < 0) 
        cnwn_set_error("%s", strerror(errno));
    return ret;
#endif
}

int64_t cnwn_file_read(cnwn_File * f, int64_t size, void * ret_buffer)
{
    if (size <= 0)
        return 0;
#ifdef BUILD_WINDOWS_FILE
#else
    if (ret_buffer != NULL) {
        ssize_t ret = read(f->fd, ret_buffer, size);
        if (ret < 0) {
            cnwn_set_error("%s", strerror(errno));
            return -1;
        }
        return ret;
    }
    uint8_t tmpbuffer[CNWN_FILE_BUFFER_SIZE];
    int64_t ret = 0;
    for (int i = 0; i < size / CNWN_FILE_BUFFER_SIZE; i++) {
        ssize_t rret = read(f->fd, tmpbuffer, CNWN_FILE_BUFFER_SIZE);
        if (rret < 0) {
            cnwn_set_error("%s", strerror(errno));
            return -1;
        }
        ret += rret;
    }
    if (size % CNWN_FILE_BUFFER_SIZE) {
        ssize_t rret = read(f->fd, tmpbuffer, size % CNWN_FILE_BUFFER_SIZE);
        if (rret < 0) {
            cnwn_set_error("%s", strerror(errno));
            return -1;
        }
        ret += rret;
    }
    return ret;
#endif
}

int64_t cnwn_file_write(cnwn_File * f, int64_t size, const void * buffer)
{
    if (size <= 0)
        return 0;
#ifdef BUILD_WINDOWS_FILE
#else
    if (buffer != NULL) {
        ssize_t ret = write(f->fd, buffer, size);
        if (ret < 0) {
            cnwn_set_error("%s", strerror(errno));
            return -1;
        }
        return ret;
    }
    uint8_t tmpbuffer[CNWN_FILE_BUFFER_SIZE];
    memset(tmpbuffer, 0, sizeof(tmpbuffer));
    int64_t ret = 0;
    for (int i = 0; i < size / CNWN_FILE_BUFFER_SIZE; i++) {
        ssize_t rret = write(f->fd, tmpbuffer, CNWN_FILE_BUFFER_SIZE);
        if (rret < 0) {
            cnwn_set_error("%s", strerror(errno));
            return -1;
        }
        ret += rret;
    }
    if (size % CNWN_FILE_BUFFER_SIZE) {
        ssize_t rret = write(f->fd, tmpbuffer, size % CNWN_FILE_BUFFER_SIZE);
        if (rret < 0) {
            cnwn_set_error("%s", strerror(errno));
            return -1;
        }
        ret += rret;
    }
    return ret;
#endif
}

int64_t cnwn_file_read_fixed(cnwn_File * f, int64_t size, void * ret_buffer)
{
    int64_t rret = cnwn_file_read(f, size, ret_buffer);
    if (rret != size) {
        cnwn_set_error("wrong size");
        return -1;
    }
    return rret;
}

int64_t cnwn_file_read64(cnwn_File * f, int64_t * ret_i)
{
    uint8_t buffer[8];
    int64_t rret = cnwn_file_read(f, 8, buffer);
    if (rret < 0)
        return -1;
    if (rret != 8) {
        cnwn_set_error("wrong size");
        return -1;
    }
    *ret_i = *((int64_t *)buffer);
    return rret;
}

int64_t cnwn_file_write64(cnwn_File * f, int64_t i)
{
    uint8_t buffer[8];
    *((int64_t *)buffer) = i;
    int64_t rret = cnwn_file_write(f, 8, buffer);
    if (rret < 0)
        return -1;
    if (rret != 8) {
        cnwn_set_error("wrong size");
        return -1;
    }
    return rret;
}

int64_t cnwn_file_read32(cnwn_File * f, int32_t * ret_i)
{
    uint8_t buffer[4];
    int64_t rret = cnwn_file_read(f, 4, buffer);
    if (rret < 0)
        return -1;
    if (rret != 4) {
        cnwn_set_error("wrong size");
        return -1;
    }
    *ret_i = *((int32_t *)buffer);
    return rret;
}
        
int64_t cnwn_file_write32(cnwn_File * f, int32_t i)
{
    uint8_t buffer[4];
    *((int32_t *)buffer) = i;
    int64_t rret = cnwn_file_write(f, 4, buffer);
    if (rret < 0)
        return -1;
    if (rret != 4) {
        cnwn_set_error("wrong size");
        return -1;
    }
    return rret;
}

int64_t cnwn_file_read16(cnwn_File * f, int16_t * ret_i)
{
    uint8_t buffer[2];
    int64_t rret = cnwn_file_read(f, 2, buffer);
    if (rret < 0)
        return -1;
    if (rret != 2) {
        cnwn_set_error("wrong size");
        return -1;
    }
    *ret_i = *((int16_t *)buffer);
    return rret;
}

int64_t cnwn_file_write16(cnwn_File * f, int16_t i)
{
    uint8_t buffer[2];
    *((int32_t *)buffer) = i;
    int64_t rret = cnwn_file_write(f, 2, buffer);
    if (rret < 0)
        return -1;
    if (rret != 2) {
        cnwn_set_error("wrong size");
        return -1;
    }
    return rret;
}

int64_t cnwn_file_size(cnwn_File * f)
{
 #ifdef BUILD_WINDOWS_FILE
#else
    struct stat st = {0};
    if (fstat(f->fd, &st) < 0) {
        cnwn_set_error("%s", strerror(errno));
        return -1;
    }
    return st.st_size;
#endif
}

int64_t cnwn_file_copy(cnwn_File * f, int64_t size, cnwn_File * output_f)
{
    if (size <= 0)
        return 0;
#ifdef BUILD_WINDOWS_FILE
#else
    uint8_t tmpbuffer[CNWN_FILE_BUFFER_SIZE];
    int64_t ret = 0;
    for (int i = 0; i < size / CNWN_FILE_BUFFER_SIZE; i++) {
        ssize_t rret = read(f->fd, tmpbuffer, CNWN_FILE_BUFFER_SIZE);
        if (rret < 0) {
            cnwn_set_error("%s (read)", strerror(errno));
            return -1;
        }
        rret = write(output_f->fd, tmpbuffer, rret);
        if (rret < 0) {
            cnwn_set_error("%s (write)", strerror(errno));
            return -1;
        }
        ret += rret;
    }
    if (size % CNWN_FILE_BUFFER_SIZE) {
        ssize_t rret = read(f->fd, tmpbuffer, size % CNWN_FILE_BUFFER_SIZE);
        if (rret < 0) {
            cnwn_set_error("%s (read)", strerror(errno));
            return -1;
        }
        rret = write(output_f->fd, tmpbuffer, rret);
        if (rret < 0) {
            cnwn_set_error("%s (write)", strerror(errno));
            return -1;
        }
        ret += rret;
    }
    return ret;
#endif
}
