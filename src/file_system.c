#include "cnwn/file_system.h"

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>

int cnwn_file_system_count(const char * path, bool recurse)
{
    if (path == NULL || path[0] == 0)
        path = "."; // FIXME: Maybe different on winblows?
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
}

static int cnwn_file_system_ls_recurse(const char * path, bool recurse, char ** ret)
{
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
            int mkret = mkdir(tmps, S_IRUSR | S_IWUSR | S_IXUSR | S_IRGRP | S_IWGRP | S_IXGRP);
            if (mkret < 0) {
                cnwn_set_error("%s \"%s\"", strerror(errno), tmps);
                return -1;
            }
            ret++;
        }
        index += seplen;
        index = CNWN_PATH_CASE_INSENSETIVE ? cnwn_strfindi(path, index, CNWN_PATH_SEPARATOR, CNWN_PATH_ESCAPE) : cnwn_strfind(path, index, CNWN_PATH_SEPARATOR, CNWN_PATH_ESCAPE);
    }
    cnwn_strcpy(tmps, sizeof(tmps), clean_path, -1);
    int isdir = cnwn_file_system_isdirectory(tmps);
    if (isdir < 0)
        return -1;
    if (isdir == 0) {
        int mkret = mkdir(tmps, S_IRUSR | S_IWUSR | S_IXUSR | S_IRGRP | S_IWGRP | S_IXGRP);
        if (mkret < 0) {
            cnwn_set_error("%s \"%s\"", strerror(errno), tmps);
            return -1;
        }
        ret++;
    }
    return ret;
}

int cnwn_file_system_rm(const char * path)
{
    if (!cnwn_path_isfiledirectory(path)) {
        cnwn_set_error("invalid path \"%s\"", path);
        return -1;
    }
    int ret = 0;
    if (cnwn_file_system_isdirectory(path) > 0) {
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
    }
    int remret = remove(path);
    if (remret < 0) {
        cnwn_set_error("%s \"%s\"", strerror(errno), path);
        return -1;
    }
    ret++;
    return ret;
}

int cnwn_file_system_mv(const char * path, const char * to_path)
{
    if (cnwn_strcmp(path, to_path) == 0)
        return 0;
    int ret = rename(path, to_path);
    if (ret < 0) {
        cnwn_set_error("%s", strerror(errno));
        return -1;
    }
    return 1;
}

int64_t cnwn_file_system_size(const char * path, bool recurse)
{
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
}

int cnwn_file_system_exists(const char * path)
{
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
}


int cnwn_file_system_isfile(const char * path)
{
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
}

int cnwn_file_system_isdirectory(const char * path)
{
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
}

cnwn_File * cnwn_file_open(const char * path, const char * mode);
void cnwn_file_close(cnwn_File * f);
int64_t cnwn_file_seek(cnwn_File * f, int64_t offset);
int64_t cnwn_file_seek_delta(cnwn_File * f, int64_t delta_offset);
int64_t cnwn_file_seek_end(cnwn_File * f);
int64_t cnwn_file_get_seek(cnwn_File * f);
int64_t cnwn_file_read(cnwn_File * f, int64_t size, void * ret_buffer);
int64_t cnwn_file_write(cnwn_File * f, int64_t size, const void * buffer);
int64_t cnwn_file_read_fixed(cnwn_File * f, int64_t size, void * ret_buffer);
int64_t cnwn_file_read64(cnwn_File * f, int64_t * ret_i);
int64_t cnwn_file_write64(cnwn_File * f, int64_t i);
int64_t cnwn_file_read32(cnwn_File * f, int32_t * ret_i);
int64_t cnwn_file_write32(cnwn_File * f, int32_t i);
int64_t cnwn_file_read16(cnwn_File * f, int16_t * ret_i);
int64_t cnwn_file_write16(cnwn_File * f, int16_t i);
int64_t cnwn_file_get_size(cnwn_File * f);

