#include "cnwn/cnwna.h"

const cnwn_Option CNWN_CNWNA_OPTIONS_GENERAL[] = {
    {'h', "help", NULL, "Print help to stdout.", 1},
    {'V', "show-version", NULL, "Print version (and only version) to stdout.", 2},
    {'q', "quiet", NULL, "Suppress verbose output to stdout.", 3},
    {0}
};

const cnwn_Option CNWN_CNWNA_OPTIONS_LIST[] = {
    {'d', "depth", "n", "Set recursion depth (-1 for no limit).", 1},
    {0}
};

const cnwn_Option CNWN_CNWNA_OPTIONS_EXTRACT[] = {
    {'d', "depth", "n", "Set recursion depth (-1 for no limit).", 1},
    {'o', "output", "path", "Set recursion depth (-1 for no lmit).", 2},
    {0}
};

const cnwn_Option CNWN_CNWNA_OPTIONS_CREATE[] = {
    {'v', "version", "x.y", "Set the version for the file format.", 1},
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
            } else if (used_options == CNWN_CNWNA_OPTIONS_EXTRACT) {
            } else if (used_options == CNWN_CNWNA_OPTIONS_CREATE) {
            }
        } else if (settings->command == NULL) {
            settings->command = cnwn_strdup(result.arg != NULL ? result.arg : "");
            if (cnwn_strstartswith("list", settings->command) || cnwn_strcmp("ls", settings->command) == 0) 
                options = CNWN_CNWNA_OPTIONS_LIST;
            else if (cnwn_strstartswith("extract", settings->command) || cnwn_strcmp("x", settings->command) == 0) 
                options = CNWN_CNWNA_OPTIONS_EXTRACT;
            else if (cnwn_strstartswith("create", settings->command))
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
    fp = fprintf(stdout, "\n'list' command:\n");
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
    fp = fprintf(stdout, "\n'extract' command:\n");
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
    fp = fprintf(stdout, "\n'create' command:\n");
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
        
        return cnwn_cnwna_execute_list(settings->path, settings->quiet, settings->depth, NULL);
    }
    if (cnwn_strstartswith("extract", settings->command) || cnwn_strcmp("x", settings->command) == 0) {
        return cnwn_cnwna_execute_extract(settings->path, settings->quiet, settings->depth, NULL, settings->output_path);
    }
    if (cnwn_strstartswith("create", settings->command)) {
        return cnwn_cnwna_execute_create(settings->path, settings->quiet, settings->depth, NULL);
    }
    cnwn_set_error("no command specified");
    return -1;
}

int cnwn_cnwna_execute_list(const char * path, bool quiet, int depth, const cnwn_RegexpArray * regexps)
{
    return 0;
}

int cnwn_cnwna_execute_extract(const char * path, bool quiet, int depth, const cnwn_RegexpArray * regexps, const char * output_path)
{
    return 0;
}

int cnwn_cnwna_execute_create(const char * path, bool quiet, int depth, const cnwn_StringArray * paths)
{
    return 0;
}
