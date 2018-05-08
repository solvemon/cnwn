#include "cnwn/path.h"


#if defined(_WIN32) || defined(_WIN64)
const char * CNWN_PATH_SEPARATOR = "\\";
const char * CNWN_PATH_ESCAPE = "^";
const bool CNWN_PATH_CASE_INSENSETIVE = true;
#else
const char * CNWN_PATH_SEPARATOR = "/";
const char * CNWN_PATH_ESCAPE = "\\";
const bool CNWN_PATH_CASE_INSENSETIVE = false;
#endif

int cnwn_path_clean(char * r, int max_size, const char * path)
{
    char tmps[CNWN_PATH_MAX_SIZE];
    int offset = 0;
    int last = 0;
    int pathlen = cnwn_strlen(path);
    int index = (CNWN_PATH_CASE_INSENSETIVE ? cnwn_strfindi(path, 0, CNWN_PATH_SEPARATOR, CNWN_PATH_ESCAPE) : cnwn_strfind(path, 0, CNWN_PATH_SEPARATOR, CNWN_PATH_ESCAPE));
    while (index >= 0 && offset < sizeof(tmps) - 1) {
        int len = index - last;
        if (len > 0)
            offset += cnwn_strcpy(tmps + offset, sizeof(tmps) - offset, path + last, len);
        last = index + 1;
        index = (CNWN_PATH_CASE_INSENSETIVE ? cnwn_strfindi(path, last, CNWN_PATH_SEPARATOR, CNWN_PATH_ESCAPE) : cnwn_strfind(path, last, CNWN_PATH_SEPARATOR, CNWN_PATH_ESCAPE));
        if (index >= 0 && index - last > 1)
            offset += cnwn_strcpy(tmps + offset, sizeof(tmps) - offset, CNWN_PATH_SEPARATOR, -1);
    }
    if (pathlen - last > 1) {
        if (last > 0)
            offset += cnwn_strcpy(tmps + offset, max_size - offset, CNWN_PATH_SEPARATOR, -1);
        offset += cnwn_strcpy(tmps + offset, sizeof(tmps) - offset, path + last, pathlen - last);
    }
    return cnwn_strcpy(r, max_size, tmps, -1);
}

bool cnwn_path_isfiledirectory(const char * path)
{
    if (!cnwn_strisblank(path)) {
        bool is_special_posix = ((path[0] == '.' && path[1] == 0) || (path[0] == '.' && path[1] == '.' && path[2] == 0));
        bool is_special_win = (((path[0] >= 'a' && path[0] <= 'z') || (path[0] >= 'A' && path[0] <= 'Z')) && path[1] == ':' && path[2] == 0);
        if (!is_special_posix && !is_special_win)
            return true;
    }
    return false;
}

int cnwn_path_directorypart(char * r, int max_size, const char * path)
{
    int index = cnwn_strrfind(path, -1, CNWN_PATH_SEPARATOR, CNWN_PATH_ESCAPE);
    if (index >= 0)
        return cnwn_strcpy(r, max_size, path, index);
    if (r != NULL && max_size > 0)
        r[0] = 0;
    return 0;
}

int cnwn_path_basepart(char * r, int max_size, const char * path)
{
    int index = cnwn_strrfind(path, -1, CNWN_PATH_SEPARATOR, CNWN_PATH_ESCAPE);
    return cnwn_strcpy(r, max_size, path + (index > 0 ? index + 1 : 0), -1);
}

int cnwn_path_filenamepart(char * r, int max_size, const char * path)
{
    int index = cnwn_strrfind(path, -1, CNWN_PATH_SEPARATOR, CNWN_PATH_ESCAPE);
    if (index < 0)
        index = 0;
    else
        index++;
    int index2 = cnwn_strfind(path, index, ".", NULL);
    return cnwn_strcpy(r, max_size, path + index, (index2 < 0 ? -1 : index2 - index));
}

int cnwn_path_extensionpart(char * r, int max_size, const char * path)
{
    int index = cnwn_strrfind(path, -1, CNWN_PATH_SEPARATOR, CNWN_PATH_ESCAPE);
    if (index < 0)
        index = 0;
    else
        index++;
    int index2 = cnwn_strfind(path, index, ".", NULL);
    if (index2 >= 0)
        return cnwn_strcpy(r, max_size, path + index2 + 1, -1);
    if (r != NULL && max_size > 0)
        r[0] = 0;
    return 0;
}

// int cnwn_path_extensionparts(const char * path, int max_extension_size, char * ret_extension, int max_subextension_size, char * ret_subextension)
// {
//     int ret = 0;
//     int index = cnwn_strrfind(path, -1, CNWN_PATH_SEPARATOR, CNWN_PATH_ESCAPE);
//     if (index < 0)
//         index = 0;
//     else
//         index++;
//     int index2 = cnwn_strrfind(path + index, -1, ".", NULL);
//     if (index2 >= 0) {
//         if (cnwn_strcpy(ret_extension, max_extension_size, path + index + index2 + 1, -1) > 0)
//             ret++;
//         if (index2 > 0) {
//             int index3 = cnwn_strrfind(path + index, index2 - 1, ".", NULL);
//             if (index3 >= 0 && cnwn_strcpy(ret_subextension, max_subextension_size, path + index + index3 + 1, index2 - index3 - 1) > 0)
//                 ret++;
//         } else if (ret_subextension != NULL && max_subextension_size > 0)
//             ret_subextension[0] = 0;
//     } else {
//         if (ret_extension != NULL && max_extension_size > 0)
//             ret_extension[0] = 0;
//         if (ret_subextension != NULL && max_subextension_size > 0)
//             ret_subextension[0] = 0;
//     }
//     return ret;
// }

char ** cnwn_path_split(const char * path, int max_splits)
{
    if (CNWN_PATH_CASE_INSENSETIVE)
        return cnwn_strspliti(path, max_splits, CNWN_PATH_SEPARATOR, CNWN_PATH_ESCAPE);
    return cnwn_strsplit(path, max_splits, CNWN_PATH_SEPARATOR, CNWN_PATH_ESCAPE);
}

int cnwn_path_join(char * r, int max_size, char ** strings)
{
    return cnwn_strjoin(r, max_size, strings, CNWN_PATH_SEPARATOR);
}
