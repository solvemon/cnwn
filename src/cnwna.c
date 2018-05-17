#include "cnwn/cnwna.h"

const cnwn_Option CNWN_CNWNA_OPTIONS_GENERAL[] = {
    {'h', "help", NULL, "Print help to stdout.", 1},
    {'V', "show-version", NULL, "Print version (and only version) to stdout.", 2},
    {0}
};

const cnwn_Option CNWN_CNWNA_OPTIONS_LIST[] = {
    {'d', "depth", "n", "Set recursion depth (-1 for no limit).", 1},
    {'v', "verbose", NULL, "Verbose output to stdout.", 2},
    {0}
};

const cnwn_Option CNWN_CNWNA_OPTIONS_EXTRACT[] = {
    {'d', "depth", "n", "Set recursion depth (-1 for no limit).", 1},
    {'q', "quiet", NULL, "Supress output to stdout.", 2},
    {'o', "output", "path", "Set the output directory.", 3},
    {0}
};

const cnwn_Option CNWN_CNWNA_OPTIONS_CREATE[] = {
    {'v', "version", "x.y", "Set the version for the file format.", 1},
    {'q', "quiet", NULL, "Supress output to stdout.", 2},
    {0}
};

bool cnwn_cnwna_has_help(int argc, char * argv[])
{
    int index = 1;
    cnwn_OptionResult result;
    int ret;
    while ((ret = cnwn_options_parse_argument(CNWN_CNWNA_OPTIONS_GENERAL, index, argc, argv, &result)) != 0) {
        if (result.optvalue == 1)
            return true;
        if (ret > 0)
            index += ret;
        else
            index++;
    }
    return false;
}

bool cnwn_cnwna_has_version(int argc, char * argv[])
{
    int index = 1;
    cnwn_OptionResult result;
    int ret;
    while ((ret = cnwn_options_parse_argument(CNWN_CNWNA_OPTIONS_GENERAL, index, argc, argv, &result)) != 0) {
        if (result.optvalue == 2)
            return true;
        if (ret > 0)
            index += ret;
        else
            index++;
    }
    return false;
}

int cnwn_cnwna_settings_init(cnwn_CNWNASettings * settings, int argc, char * argv[])
{
    memset(settings, 0, sizeof(cnwn_CNWNASettings));
    cnwn_string_array_init(&settings->arguments);
    int index = 1;
    cnwn_OptionResult result;
    int ret;
    const cnwn_Option * options = CNWN_CNWNA_OPTIONS_GENERAL;
    while ((ret = cnwn_options_parse_argument(options, index, argc, argv, &result)) != 0) {
        const cnwn_Option * used_options = options;
        if (ret == CNWN_OPTION_ERROR_INVALID) {
            used_options = CNWN_CNWNA_OPTIONS_GENERAL;
            ret = cnwn_options_parse_argument(CNWN_CNWNA_OPTIONS_GENERAL, index, argc, argv, &result);
        }
        if (ret <= 0)
            break;
        if (result.option != NULL) {
            if (used_options == CNWN_CNWNA_OPTIONS_GENERAL) {
            } else if (used_options == CNWN_CNWNA_OPTIONS_LIST) {
                if (result.optvalue == 1 && !cnwn_strint(result.optarg, 10, &settings->depth)) {
                    cnwn_set_error("invalid depth value (int): %s", result.optarg);
                    cnwn_cnwna_settings_deinit(settings);
                    return -1;
                } else if (result.optvalue == 2)
                    settings->verbose = true;
            } else if (used_options == CNWN_CNWNA_OPTIONS_EXTRACT) {
                if (result.optvalue == 1 && !cnwn_strint(result.optarg, 10, &settings->depth)) {
                    cnwn_set_error("invalid depth value (int): %s", result.optarg);
                    cnwn_cnwna_settings_deinit(settings);
                    return -1;
                } else if (result.optvalue == 2)
                    settings->quiet = true;
                else if (result.optvalue == 3)
                    settings->output_path = cnwn_strdup(result.optarg);
            } else if (used_options == CNWN_CNWNA_OPTIONS_CREATE) {
            }
        } else if (settings->command == NULL) {
            settings->command = cnwn_strdup(result.arg != NULL ? result.arg : "");
            if (cnwn_strstartswith("list", settings->command) || cnwn_strcmp("ls", settings->command) == 0) {
                options = CNWN_CNWNA_OPTIONS_LIST;
                settings->depth = -1;                
            } else if (cnwn_strstartswith("extract", settings->command) || cnwn_strcmp("x", settings->command) == 0) {
                options = CNWN_CNWNA_OPTIONS_EXTRACT;
                settings->depth = -1;
            } else if (cnwn_strstartswith("create", settings->command))
                options = CNWN_CNWNA_OPTIONS_CREATE;
            else {
                cnwn_set_error("invalid command: %s", result.arg);
                cnwn_cnwna_settings_deinit(settings);
                return -1;
            }
        } else if (settings->path == NULL) 
            settings->path = cnwn_strdup(result.arg != NULL ? result.arg : "");
        else
            cnwn_string_array_append(&settings->arguments, result.arg);
        index += ret;
    }    
    if (ret == CNWN_OPTION_ERROR_INVALID) {
        cnwn_set_error("invalid option at %d: %s", result.index - 1, result.arg);
        cnwn_cnwna_settings_deinit(settings);
        return -1;
    }
    if (ret == CNWN_OPTION_ERROR_NOARG) {
        cnwn_set_error("missing option argument at %d: %s", result.index - 1, result.arg);
        cnwn_cnwna_settings_deinit(settings);
        return -1;
    }
    if (ret < 0) {
        cnwn_set_error("options failed?");
        cnwn_cnwna_settings_deinit(settings);
        return -1;
    }
    return index;
}

void cnwn_cnwna_settings_deinit(cnwn_CNWNASettings * settings)
{
    if (settings->command != NULL)
        free(settings->command);
    if (settings->path != NULL)
        free(settings->path);
    if (settings->output_path != NULL)
        free(settings->output_path);
    cnwn_array_deinit(&settings->arguments);
    memset(settings, 0, sizeof(cnwn_CNWNASettings));
}

int cnwn_cnwna_print_help(void)
{
    int ret = 0;
    int fp = fprintf(stdout, "cnwna [options] <command> [command options] <path> [command arguments]\nVersion: %d.%d.%d\n",
                     BUILD_VERSION_MAJOR, BUILD_VERSION_MINOR, BUILD_VERSION_PATCH);
    if (fp < 0) {
        cnwn_set_error("%s", strerror(errno));
        return -1;
    }
    ret += fp;
    fp = fprintf(stdout, "\nGeneral options:\n");
    if (fp < 0) {
        cnwn_set_error("%s", strerror(errno));
        return -1;
    }
    ret += fp;
    for (int i = 0; !CNWN_OPTION_SENTINEL(CNWN_CNWNA_OPTIONS_GENERAL + i); i++) {
        char tmps[1024];
        cnwn_option_to_string(CNWN_CNWNA_OPTIONS_GENERAL + i, sizeof(tmps), tmps);
        fp = fprintf(stdout, "  %s\n", tmps);
        if (fp < 0) {
            cnwn_set_error("%s", strerror(errno));
            return -1;
        }
        ret += fp;        
    }
    fp = fprintf(stdout, "\nlist [options] [regular expressions]:\n");
    if (fp < 0) {
        cnwn_set_error("%s", strerror(errno));
        return -1;
    }
    ret += fp;
    for (int i = 0; !CNWN_OPTION_SENTINEL(CNWN_CNWNA_OPTIONS_LIST + i); i++) {
        char tmps[1024];
        cnwn_option_to_string(CNWN_CNWNA_OPTIONS_LIST + i, sizeof(tmps), tmps);
        fp = fprintf(stdout, "  %s\n", tmps);
        if (fp < 0) {
            cnwn_set_error("%s", strerror(errno));
            return -1;
        }
        ret += fp;        
    }
    fp = fprintf(stdout, "\nextract [options] [regular expressions]:\n");
    if (fp < 0) {
        cnwn_set_error("%s", strerror(errno));
        return -1;
    }
    ret += fp;
    for (int i = 0; !CNWN_OPTION_SENTINEL(CNWN_CNWNA_OPTIONS_EXTRACT + i); i++) {
        char tmps[1024];
        cnwn_option_to_string(CNWN_CNWNA_OPTIONS_EXTRACT + i, sizeof(tmps), tmps);
        fp = fprintf(stdout, "  %s\n", tmps);
        if (fp < 0) {
            cnwn_set_error("%s", strerror(errno));
            return -1;
        }
        ret += fp;        
    }
    fp = fprintf(stdout, "\ncreate [options] [files and directories]:\n");
    if (fp < 0) {
        cnwn_set_error("%s", strerror(errno));
        return -1;
    }
    ret += fp;
    for (int i = 0; !CNWN_OPTION_SENTINEL(CNWN_CNWNA_OPTIONS_CREATE + i); i++) {
        char tmps[1024];
        cnwn_option_to_string(CNWN_CNWNA_OPTIONS_CREATE + i, sizeof(tmps), tmps);
        fp = fprintf(stdout, "  %s\n", tmps);
        if (fp < 0) {
            cnwn_set_error("%s", strerror(errno));
            return -1;
        }
        ret += fp;        
    }
    return ret;
}

int cnwn_cnwna_print_version(void)
{
    int ret = fprintf(stdout, "%d.%d.%d\n", BUILD_VERSION_MAJOR, BUILD_VERSION_MINOR, BUILD_VERSION_PATCH);
    if (ret < 0) {
        cnwn_set_error("%s", strerror(errno));
        return -1;
    }
    return ret;
}

int cnwn_cnwna_execute(const cnwn_CNWNASettings * settings)
{
    if (cnwn_strstartswith("list", settings->command) || cnwn_strcmp("ls", settings->command) == 0) {
        cnwn_RegexpArray * regexps = cnwn_regexp_array_new2(&settings->arguments);
        if (regexps == NULL) 
            return -1;
        int ret = cnwn_cnwna_execute_list(settings->path, settings->verbose, settings->depth, regexps);
        cnwn_regexp_array_free(regexps);
        return ret;
    }
    if (cnwn_strstartswith("extract", settings->command) || cnwn_strcmp("x", settings->command) == 0) {
        cnwn_RegexpArray * regexps = cnwn_regexp_array_new2(&settings->arguments);
        if (regexps == NULL) 
            return -1;
        int ret = cnwn_cnwna_execute_extract(settings->path, settings->quiet, settings->depth, regexps, settings->output_path);
        cnwn_regexp_array_free(regexps);
        return ret;
    }
    if (cnwn_strstartswith("create", settings->command)) {
        return cnwn_cnwna_execute_create(settings->path, settings->quiet, settings->depth, NULL);
    }
    cnwn_set_error("no command specified");
    return -1;
}

static int cnwn_cnwna_execute_list_recurse(const cnwn_Resource * resource, bool top, bool verbose, int depth, const cnwn_RegexpArray * regexps, int64_t * ret_resource_bytes, int * ret_num_meta_files, int64_t * ret_meta_file_bytes)
{
    char path[CNWN_PATH_MAX_SIZE];
    cnwn_resource_get_path(resource, sizeof(path), path);
    if (!top && regexps != NULL && !cnwn_regexp_array_match_any(regexps, path)) {
        if (ret_resource_bytes != NULL)
            *ret_resource_bytes = 0;
        if (ret_num_meta_files != NULL)
            *ret_num_meta_files = 0;
        if (ret_meta_file_bytes != NULL)
            *ret_meta_file_bytes = 0;
        return 0;
    }
    int has_num_resources = 0;
    int64_t has_resource_bytes = 0;
    int has_num_meta_files = 0;
    int64_t has_meta_file_bytes = 0;
    const cnwn_ResourceTypeInfo info = CNWN_RESOURCE_TYPE_INFO(resource->type);
    const char * stype = (cnwn_strisblank(info.name) ? info.extension : info.name);
    int num_meta_files = cnwn_resource_get_num_meta_files(resource);
    int num_resources = cnwn_resource_get_num_resources(resource);
    if (!top) {
        if (verbose) {
            char meta_files_str[1024];
            if (num_meta_files > 0)
                snprintf(meta_files_str, sizeof(meta_files_str), ", %d meta files", num_meta_files);
            else
                meta_files_str[0] = 0;
            char resources_str[1024];
            if (num_meta_files > 0)
                snprintf(resources_str, sizeof(resources_str), ", %d resources", num_resources);
            else
                meta_files_str[0] = 0;
            printf("%s %"PRId64" (%s%s%s)\n",
                   path,
                   resource->size,
                   stype,
                   meta_files_str,
                   resources_str);
        } else
            printf("%s %"PRId64"\n", path, resource->size);
    }
    if (depth != 0) {
        for (int i = 0; i < num_meta_files; i++) {
            cnwn_MetaFile meta_file;
            if (cnwn_resource_get_meta_file(resource, i, &meta_file) > 0) {
                if (verbose && !cnwn_strisblank(meta_file.description)) 
                    printf("META %s %"PRId64" (%s)\n", meta_file.name, meta_file.size, meta_file.description);
                else
                    printf("META %s %"PRId64"\n", meta_file.name, meta_file.size);
                has_num_meta_files++;
                has_meta_file_bytes += meta_file.size;
            }
        }
        for (int i = 0; i < num_resources; i++) {
            int64_t tmp_resource_bytes = 0;
            int tmp_num_meta_files = 0;
            int64_t tmp_meta_file_bytes = 0;
            cnwn_Resource * subresource = cnwn_resource_get_resource(resource, i);
            int tmp_num_resources = cnwn_cnwna_execute_list_recurse(subresource, false, verbose, (depth > 0 ? depth - 1 : -1), regexps, &tmp_resource_bytes, &tmp_num_meta_files, &tmp_meta_file_bytes);
            if (tmp_num_resources < 0) 
                return -1;
            has_num_resources += tmp_num_resources;
            has_resource_bytes += tmp_resource_bytes;
            has_num_meta_files += tmp_num_meta_files;
            has_meta_file_bytes += tmp_meta_file_bytes;
        }
    } else {
        has_resource_bytes += resource->size;
    }
    has_num_resources++;
    if (ret_resource_bytes)
        *ret_resource_bytes = has_resource_bytes;
    if (ret_num_meta_files != NULL)
        *ret_num_meta_files = has_num_meta_files;
    if (ret_meta_file_bytes != NULL)
        *ret_meta_file_bytes = has_meta_file_bytes;
    return has_num_resources;
}

int cnwn_cnwna_execute_list(const char * path, bool verbose, int depth, const cnwn_RegexpArray * regexps)
{
    cnwn_ResourceType rtype = cnwn_resource_type_from_path(path);
    if (!CNWN_RESOURCE_TYPE_VALID(rtype)) {
        cnwn_set_error("invalid resource type (%s)", path);
        return -1;
    }
    char name[CNWN_PATH_MAX_SIZE];
    cnwn_path_filenamepart(name, sizeof(name), path);
    cnwn_File * f = cnwn_file_open(path, "r");
    if (f == NULL) {
        cnwn_set_error("%s (open %s)", cnwn_get_error(), path);
        return -1;
    }
    int64_t size = cnwn_file_size(f);
    if (size < 0) {
        cnwn_set_error("%s (size %s)", cnwn_get_error(), path);
        cnwn_file_close(f);
        return -1;
    }
    cnwn_Resource resource;
    int ret = cnwn_resource_init_from_file(&resource, rtype, name, 0, size, NULL, f);
    if (ret < 0) {
        cnwn_set_error("%s (%s)", cnwn_get_error(), path);
        cnwn_file_close(f);
        return -1;
    }
    int64_t resource_bytes = 0;
    int num_meta_files = 0;
    int64_t meta_file_bytes = 0;
    ret = cnwn_cnwna_execute_list_recurse(&resource, true, verbose, depth, regexps, &resource_bytes, &num_meta_files, &meta_file_bytes);
    if (ret < 0) {
        cnwn_resource_deinit(&resource);
        cnwn_file_close(f);
        return -1;
    }
    cnwn_resource_deinit(&resource);
    cnwn_file_close(f);
    if (verbose)
        printf("Total %d meta files (%"PRId64" bytes) and %d resources (%"PRId64" bytes)\n", num_meta_files, meta_file_bytes, ret, resource_bytes);
    return ret;
}

static int cnwn_cnwna_execute_extract_recurse(const cnwn_Resource * resource, cnwn_File * input_f, bool top, bool quiet, int depth, const cnwn_RegexpArray * regexps, const char * output_path, int64_t * ret_resource_bytes, int * ret_num_meta_files, int64_t * ret_meta_file_bytes)
{
    char path[CNWN_PATH_MAX_SIZE];
    cnwn_resource_get_path(resource, sizeof(path), path);
    if (!top && regexps != NULL && !cnwn_regexp_array_match_any(regexps, path)) {
        if (ret_resource_bytes != NULL)
            *ret_resource_bytes = 0;
        if (ret_num_meta_files != NULL)
            *ret_num_meta_files = 0;
        if (ret_meta_file_bytes != NULL)
            *ret_meta_file_bytes = 0;
        return 0;
    }
    char use_path[CNWN_PATH_MAX_SIZE];
    if (cnwn_strisblank(output_path))
        snprintf(use_path, sizeof(use_path), path);
    else 
        snprintf(use_path, sizeof(use_path), "%s%s%s", output_path, CNWN_PATH_SEPARATOR, path);
    int has_num_resources = 0;
    int64_t has_resource_bytes = 0;
    int has_num_meta_files = 0;
    int64_t has_meta_file_bytes = 0;
    int num_meta_files = cnwn_resource_get_num_meta_files(resource);
    int num_resources = cnwn_resource_get_num_resources(resource);
    if (depth != 0) {
        for (int i = 0; i < num_meta_files; i++) {
            cnwn_MetaFile meta_file;
            if (cnwn_resource_get_meta_file(resource, i, &meta_file) > 0) {
                char meta_file_path[CNWN_PATH_MAX_SIZE];
                if (cnwn_strisblank(output_path)) {
                    if (top)
                        snprintf(meta_file_path, sizeof(meta_file_path), "%s", meta_file.name);
                    else
                        snprintf(meta_file_path, sizeof(meta_file_path), "%s%s%s", path, CNWN_PATH_SEPARATOR, meta_file.name);
                } else {
                    if (top)
                        snprintf(meta_file_path, sizeof(meta_file_path), "%s%s%s", output_path, CNWN_PATH_SEPARATOR, meta_file.name);
                    else
                        snprintf(meta_file_path, sizeof(meta_file_path), "%s%s%s%s%s", output_path, CNWN_PATH_SEPARATOR, path, CNWN_PATH_SEPARATOR, meta_file.name);
                }
                int64_t er = cnwn_resource_meta_file_extract_to_path(resource, i, input_f, meta_file_path);
                if (er < 0) {
                    cnwn_set_error("%s (extracting meta file \"%s\")", cnwn_get_error(), meta_file_path);
                    return -1;
                }
                if (!quiet)
                    printf("%s => %s %"PRId64"\n", path, meta_file_path, er);
                has_num_meta_files++;
                has_meta_file_bytes += er;
            }
        }
        for (int i = 0; i < num_resources; i++) {
            int64_t tmp_resource_bytes = 0;
            int tmp_num_meta_files = 0;
            int64_t tmp_meta_file_bytes = 0;
            cnwn_Resource * subresource = cnwn_resource_get_resource(resource, i);
            int tmp_num_resources = cnwn_cnwna_execute_extract_recurse(subresource, input_f, false, quiet, (depth > 0 ? depth - 1 : -1), regexps, output_path, &tmp_resource_bytes, &tmp_num_meta_files, &tmp_meta_file_bytes);
            if (tmp_num_resources < 0) 
                return -1;
            has_num_resources += tmp_num_resources;
            has_resource_bytes += tmp_resource_bytes;
            has_num_meta_files += tmp_num_meta_files;
            has_meta_file_bytes += tmp_meta_file_bytes;
        }
    }
    if (!top) {
        int64_t er = cnwn_resource_extract_to_path(resource, input_f, use_path);
        if (er < 0) {
            cnwn_set_error("%s (extracting \"%s\")", cnwn_get_error(), use_path);
            return -1;
        }
        if (!quiet)
            printf("%s => %s %"PRId64"\n", path, use_path, er);
        has_resource_bytes += resource->size;
    }
    has_num_resources++;
    if (ret_resource_bytes)
        *ret_resource_bytes = has_resource_bytes;
    if (ret_num_meta_files != NULL)
        *ret_num_meta_files = has_num_meta_files;
    if (ret_meta_file_bytes != NULL)
        *ret_meta_file_bytes = has_meta_file_bytes;
    return has_num_resources;
}

int cnwn_cnwna_execute_extract(const char * path, bool quiet, int depth, const cnwn_RegexpArray * regexps, const char * output_path)
{
    cnwn_ResourceType rtype = cnwn_resource_type_from_path(path);
    if (!CNWN_RESOURCE_TYPE_VALID(rtype)) {
        cnwn_set_error("invalid resource type (%s)", path);
        return -1;
    }
    char name[CNWN_PATH_MAX_SIZE];
    cnwn_path_filenamepart(name, sizeof(name), path);
    cnwn_File * f = cnwn_file_open(path, "r");
    if (f == NULL) {
        cnwn_set_error("%s (open %s)", cnwn_get_error(), path);
        return -1;
    }
    int64_t size = cnwn_file_size(f);
    if (size < 0) {
        cnwn_set_error("%s (size %s)", cnwn_get_error(), path);
        cnwn_file_close(f);
        return -1;
    }
    cnwn_Resource resource;
    int ret = cnwn_resource_init_from_file(&resource, rtype, name, 0, size, NULL, f);
    if (ret < 0) {
        cnwn_set_error("%s (%s)", cnwn_get_error(), path);
        cnwn_file_close(f);
        return -1;
    }
    int64_t resource_bytes = 0;
    int num_meta_files = 0;
    int64_t meta_file_bytes = 0;
    ret = cnwn_cnwna_execute_extract_recurse(&resource, f, true, quiet, depth, regexps, output_path, &resource_bytes, &num_meta_files, &meta_file_bytes);
    if (ret < 0) {
        cnwn_resource_deinit(&resource);
        cnwn_file_close(f);
        return -1;
    }
    cnwn_resource_deinit(&resource);
    cnwn_file_close(f);
    if (!quiet)
        printf("Total %d meta files (%"PRId64" bytes) and %d resources (%"PRId64" bytes)\n", num_meta_files, meta_file_bytes, ret, resource_bytes);
    return ret;
}

int cnwn_cnwna_execute_create(const char * path, bool quiet, int depth, const cnwn_StringArray * paths)
{
    return 0;
}
