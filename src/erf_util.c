#include "cnwn/erf_util.h"

const cnwn_CliOption CNWN_ERF_UTIL_OPTIONS_GENERAL[] = {
    {"h", "help", NULL, "Show this help.", CNWN_ERF_UTIL_OPTION_HELP},
    {"V", "version", NULL, "Show the version.", CNWN_ERF_UTIL_OPTION_VERSION},
    {"v", "verbose", NULL, "Verbose prints.", CNWN_ERF_UTIL_OPTION_VERBOSE},
    {"i", NULL, NULL, "Don't quit on errors.", CNWN_ERF_UTIL_OPTION_IGNORE_ERRORS},
    {"c", NULL, NULL, "Disable color output.", CNWN_ERF_UTIL_OPTION_COLOR},
    {0}
};

const cnwn_CliOption CNWN_ERF_UTIL_OPTIONS_INFO[] = {
    {0}
};

const cnwn_CliOption CNWN_ERF_UTIL_OPTIONS_LIST[] = {
    {"e", NULL, NULL, "Use extended regular expressions.", CNWN_ERF_UTIL_OPTION_EXTENDED_REGEXP},
    {0}
};

const cnwn_CliOption CNWN_ERF_UTIL_OPTIONS_EXTRACT[] = {
    {"e", NULL, NULL, "Use extended regular expressions.", CNWN_ERF_UTIL_OPTION_EXTENDED_REGEXP},
    {"f", NULL, NULL, "Force overwrites.", CNWN_ERF_UTIL_OPTION_FORCE},
    {"d", NULL, "dir", "Specify an output directory.", CNWN_ERF_UTIL_OPTION_DIRECTORY},
    {"x", NULL, NULL, "Disable XML output when possible.", CNWN_ERF_UTIL_OPTION_XML},
    {0}
};

const cnwn_CliOption CNWN_ERF_UTIL_OPTIONS_CREATE[] = {
    {"f", NULL, NULL, "Force overwrites.", CNWN_ERF_UTIL_OPTION_FORCE},
    {"s", NULL, "version", "Specify an ERF version: \"1.0\" (default) or \"1.1\".", CNWN_ERF_UTIL_OPTION_VERSION},
    {0}
};

const cnwn_CliOption CNWN_ERF_UTIL_OPTIONS_ADD[] = {
    {"f", NULL, NULL, "Force overwrites.", CNWN_ERF_UTIL_OPTION_FORCE},
    {"T", NULL, "path", "Specify a temporary file.", CNWN_ERF_UTIL_OPTION_TEMPORARY_FILE},
    {0}
};

const cnwn_CliOption CNWN_ERF_UTIL_OPTIONS_REMOVE[] = {
    {"T", NULL, "path", "Specify a temporary file.", CNWN_ERF_UTIL_OPTION_TEMPORARY_FILE},
    {0}
};

const cnwn_CliOption CNWN_ERF_UTIL_OPTIONS_DIFF[] = {
    {0}
};

int cnwn_erf_util_parse_options(int argc, char * argv[], cnwn_ERFUtilOptions * ret_options, const char ** ret_erf_path, char * ret_command, char *** ret_cmdargs, int max_errors, cnwn_ERFUtilOptionError * ret_errors)
{
    int error_count = 0;
    cnwn_ERFUtilOptions options = {0};
    options.color = true;
    options.xml = true;
    char command = 0;
    const char * erf_path = NULL;
    int retparse = 0, argindex = 1, optindex;
    const char * optarg;
    const cnwn_CliOption * available_options = CNWN_ERF_UTIL_OPTIONS_GENERAL;
    int cmdargssize = 0;
    int numcmdargs = 0;
    char ** cmdargs = NULL;
    while ((retparse = cnwn_cli_options_parse(available_options, argindex, argc, argv, &optindex, &optarg)) != 0) {
        const cnwn_CliOption * used_options = available_options;
        if (retparse < 0) {
            used_options = CNWN_ERF_UTIL_OPTIONS_GENERAL;
            retparse = cnwn_cli_options_parse(CNWN_ERF_UTIL_OPTIONS_GENERAL, argindex, argc, argv, &optindex, &optarg);
            if (retparse == 0)
                break;
        }
        if (retparse < 0) {
            if (retparse == CNWN_CLI_OPTION_INVALID) {
                if (max_errors > 0 && ret_errors != NULL) {
                    ret_errors[error_count].argindex = argindex;
                    ret_errors[error_count].error = retparse;
                }
                error_count++;
            } else if (retparse == CNWN_CLI_OPTION_NOARG) {
                if (max_errors > 0 && ret_errors != NULL) {
                    ret_errors[error_count].argindex = argindex;
                    ret_errors[error_count].error = retparse;
                }
                error_count++;
            } else {
                if (max_errors > 0 && ret_errors != NULL) {
                    ret_errors[error_count].argindex = argindex;
                    ret_errors[error_count].error = retparse;
                }
                error_count++;
            }
        } else if (optindex >= 0) {
            cnwn_ERFUtilOption option = used_options[optindex].value;
            switch (option) {
            case CNWN_ERF_UTIL_OPTION_HELP:
                options.help = true;
                break;
            case CNWN_ERF_UTIL_OPTION_VERSION:
                options.version = true;
                break;
            case CNWN_ERF_UTIL_OPTION_VERBOSE:
                options.verbose = true;
                break;
            case CNWN_ERF_UTIL_OPTION_IGNORE_ERRORS:
                options.ignore_errors = true;
                break;
            case CNWN_ERF_UTIL_OPTION_COLOR:
                options.color = false;
                break;
            case CNWN_ERF_UTIL_OPTION_EXTENDED_REGEXP:
                options.extended_regexp = true;
                break;
            case CNWN_ERF_UTIL_OPTION_FORCE:
                options.force = true;
                break;
            case CNWN_ERF_UTIL_OPTION_DIRECTORY:
                options.directory = optarg;
                break;
            case CNWN_ERF_UTIL_OPTION_SET_VERSION:
                options.help = true;
                break;
            case CNWN_ERF_UTIL_OPTION_XML:
                options.xml = false;
                break;
            default:
                break;
            };
        } else if (erf_path == NULL) {
            erf_path = argv[argindex];
        } else if (command == 0) {
            if (cnwn_string_cmpicase(argv[argindex], "info") == 0
                || cnwn_string_cmpicase(argv[argindex], "i") == 0) {
                command = 'i';
                available_options = CNWN_ERF_UTIL_OPTIONS_INFO;
            } else if (cnwn_string_cmpicase(argv[argindex], "list") == 0
                || cnwn_string_cmpicase(argv[argindex], "ls") == 0
                || cnwn_string_cmpicase(argv[argindex], "l") == 0) {
                command = 'l';
                available_options = CNWN_ERF_UTIL_OPTIONS_LIST;
            } else if (cnwn_string_cmpicase(argv[argindex], "extract") == 0
                       || cnwn_string_cmpicase(argv[argindex], "x") == 0
                       || cnwn_string_cmpicase(argv[argindex], "e") == 0) {
                command = 'e';
                available_options = CNWN_ERF_UTIL_OPTIONS_EXTRACT;
            } else if (cnwn_string_cmpicase(argv[argindex], "create") == 0
                       || cnwn_string_cmpicase(argv[argindex], "c") == 0) {
                command = 'c';
                available_options = CNWN_ERF_UTIL_OPTIONS_CREATE;
            } else if (cnwn_string_cmpicase(argv[argindex], "add") == 0
                       || cnwn_string_cmpicase(argv[argindex], "a") == 0) {
                command = 'a';
                available_options = CNWN_ERF_UTIL_OPTIONS_ADD;
            } else if (cnwn_string_cmpicase(argv[argindex], "remove") == 0
                       || cnwn_string_cmpicase(argv[argindex], "r") == 0) {
                command = 'r';
                available_options = CNWN_ERF_UTIL_OPTIONS_REMOVE;
            } else if (cnwn_string_cmpicase(argv[argindex], "diff") == 0
                       || cnwn_string_cmpicase(argv[argindex], "d") == 0) {
                command = 'd';
                available_options = CNWN_ERF_UTIL_OPTIONS_DIFF;
            } else
                command = -1;
        } else if (ret_cmdargs != NULL && !cnwn_string_isempty(argv[argindex])) {
            if (numcmdargs >= cmdargssize) {
                cmdargs = realloc(cmdargs, sizeof(char *) * (numcmdargs + 1024));
                cmdargssize = numcmdargs + 1024;
            }
            cmdargs[numcmdargs++] = cnwn_string_dup(argv[argindex]);
        }
        argindex += CNWN_MAX(1, retparse);
    }
    if (numcmdargs > 0) {
        cmdargs = realloc(cmdargs, sizeof(char *) * (numcmdargs + 1));
        cmdargs[numcmdargs] = NULL;
    }
    if (ret_options != NULL)
        *ret_options = options;
    if (ret_erf_path != NULL)
        *ret_erf_path = erf_path;
    if (ret_command != NULL)
        *ret_command = command;
    if (ret_cmdargs != NULL)
        *ret_cmdargs = cmdargs;
    return error_count;
}

int cnwn_erf_util_info(const char * path, const cnwn_ERFUtilOptions * options)
{
    if (path == NULL || path[0] == 0) {
        fprintf(stderr, "%sERROR:%s invalid empty path for ERF\n",
                CNWN_COLORIZE_ERROR(options->color),
                CNWN_COLORIZE_NORMAL(options->color));
        return 1;
    }
    cnwn_File * f = cnwn_file_open_r(path);
    if (f == NULL) {
        fprintf(stderr, "%sERROR:%s could not open ERF (%s): %s%s\n",
                CNWN_COLORIZE_ERROR(options->color),
                CNWN_COLORIZE_NORMAL(options->color),
                cnwn_get_error(),
                CNWN_COLORIZE_PATH(options->color),
                path != NULL ? path : "");
        return 1;
    }
    cnwn_ERFHeader header = {0};
    cnwn_ERFEntry * entries = NULL;
    int num_entries = cnwn_erf_read_header(f, &header, &entries, NULL);
    if (num_entries < 0) {
        fprintf(stderr, "%sERROR:%s could not read entries (%s): %s%s\n",
                CNWN_COLORIZE_ERROR(options->color),
                CNWN_COLORIZE_NORMAL(options->color),
                cnwn_get_error(),
                CNWN_COLORIZE_PATH(options->color),
                path != NULL ? path : "");
        cnwn_file_close(f);
        return 1;
    }
    int resource_count = 0;
    int64_t resource_size = 0;
    for (int i = 0; i < header.num_entries; i++) {
        resource_count++;
        resource_size += entries[i].resource_size;
    }
    if (options->verbose) {
        printf("%sERF %s%s %s(%s) %s%d.%d %s(%s), %s%d %sentr%s, %s%"PRId64" %sbyte%s\n",
               CNWN_COLORIZE_EMPHASIS(options->color),
               CNWN_COLORIZE_NORMAL(options->color),
               CNWN_RESOURCE_TYPE_EXTENSION(header.type),
               CNWN_COLORIZE_DETAILS(options->color),
               header.type_str,
               CNWN_COLORIZE_NORMAL(options->color),
               header.version.major,
               header.version.minor,
               CNWN_COLORIZE_DETAILS(options->color),
               header.version_str,
               CNWN_COLORIZE_NUMBER(options->color),
               resource_count,
               CNWN_COLORIZE_NORMAL(options->color),
               resource_count != 1 ? "ies" : "y",
               CNWN_COLORIZE_NUMBER(options->color),
               resource_size,
               CNWN_COLORIZE_NORMAL(options->color),
               resource_size != 1 ? "s" : "");
        printf("%sOffsets: keys %s0x%x%s, resources %s0x%x%s, localized strings %s0x%x\n",
               CNWN_COLORIZE_NORMAL(options->color),
               CNWN_COLORIZE_DETAILS(options->color),
               header.keys_offset,
               CNWN_COLORIZE_NORMAL(options->color),
               CNWN_COLORIZE_DETAILS(options->color),
               header.resources_offset,
               CNWN_COLORIZE_NORMAL(options->color),
               CNWN_COLORIZE_DETAILS(options->color),
               header.localized_strings_offset);
        printf("%s%d%s localized strings, %s%u %sbyte%s\n",
               CNWN_COLORIZE_NUMBER(options->color),
               header.num_localized_strings,
               CNWN_COLORIZE_NORMAL(options->color),
               CNWN_COLORIZE_NUMBER(options->color),
               header.localized_strings_size,
               CNWN_COLORIZE_NORMAL(options->color),
               header.localized_strings_size != 1 ? "s" : "");
        char ** localized_strings = NULL;
        int64_t locbytes = 0;
        int locret = cnwn_erf_read_localized_strings(f, &header, &localized_strings, &locbytes);
        if (locret < 0) {
            cnwn_file_close(f);
            free(entries);
            return -1;
        }
        cnwn_free_strings(localized_strings);
    } else {
        printf("%sERF %s%s %d.%d, %s%d %sentr%s, %s%"PRId64" %sbyte%s\n",
               CNWN_COLORIZE_EMPHASIS(options->color),
               CNWN_COLORIZE_NORMAL(options->color),
               CNWN_RESOURCE_TYPE_EXTENSION(header.type),
               header.version.major,
               header.version.minor,
               CNWN_COLORIZE_NUMBER(options->color),
               resource_count,
               CNWN_COLORIZE_NORMAL(options->color),
               resource_count != 1 ? "ies" : "y",
               CNWN_COLORIZE_NUMBER(options->color),
               resource_size,
               CNWN_COLORIZE_NORMAL(options->color),
               resource_size != 1 ? "s" : ""
               );
    }
    if (entries != NULL)
        free(entries);
    cnwn_file_close(f);
    return 0;
}

int cnwn_erf_util_list(const char * path, const cnwn_ERFUtilOptions * options, const cnwn_RegexpArray * regexp_array)
{
    if (path == NULL || path[0] == 0) {
        fprintf(stderr, "%sERROR:%s invalid empty path for ERF\n",
                CNWN_COLORIZE_ERROR(options->color),
                CNWN_COLORIZE_NORMAL(options->color));
        return 1;
    }
    cnwn_File * f = cnwn_file_open_r(path);
    if (f == NULL) {
        fprintf(stderr, "%sERROR:%s could not open ERF (%s): %s%s\n",
                CNWN_COLORIZE_ERROR(options->color),
                CNWN_COLORIZE_NORMAL(options->color),
                cnwn_get_error(),
                CNWN_COLORIZE_PATH(options->color),
                path != NULL ? path : "");
        return 1;
    }
    cnwn_ERFHeader header = {0};
    cnwn_ERFEntry * entries = NULL;
    int num_entries = cnwn_erf_read_header(f, &header, &entries, NULL);
    if (num_entries < 0) {
        fprintf(stderr, "%sERROR:%s could not read entries (%s): %s%s\n",
                CNWN_COLORIZE_ERROR(options->color),
                CNWN_COLORIZE_NORMAL(options->color),
                cnwn_get_error(),
                CNWN_COLORIZE_PATH(options->color),
                path != NULL ? path : "");
        cnwn_file_close(f);
        return 1;
    }
    int resource_count = 0;
    int64_t resource_size = 0;
    for (int i = 0; i < header.num_entries; i++) {
        char number[1024];
        char filename[CNWN_PATH_MAX_SIZE];
        snprintf(number, sizeof(number), "%u", entries[i].resource_size);
        cnwn_resource_type_and_key_to_filename(entries[i].type, entries[i].key, sizeof(filename), filename);
        if (regexp_array == NULL || cnwn_regexp_array_match_any(regexp_array, filename)) {
            if (options->verbose)
                printf("%s%-32s %s%-10s %s(index=%d, ID=%u, itype=%u, koffset=0x%x, roffset=0x%x)\n",
                       CNWN_COLORIZE_ENTRY(options->color),
                       filename,
                       CNWN_COLORIZE_NUMBER(options->color),
                       number,
                       CNWN_COLORIZE_DETAILS(options->color),
                       entries[i].index,
                       entries[i].resource_id,
                       entries[i].itype,
                       entries[i].key_offset,
                       entries[i].resource_offset);
            else
                printf("%s%s %s%s\n",
                       CNWN_COLORIZE_ENTRY(options->color),
                       filename,
                       CNWN_COLORIZE_NUMBER(options->color),
                       number);
            resource_count++;
            resource_size += entries[i].resource_size;
        }
    }
    free(entries);
    cnwn_file_close(f);
    if (options->verbose)
        printf("%s%d %sentr%s, %s%"PRId64" %sbyte%s\n",
               CNWN_COLORIZE_NUMBER(options->color),
               resource_count,
               CNWN_COLORIZE_NORMAL(options->color),
               resource_count != 1 ? "ies" : "y",
               CNWN_COLORIZE_NUMBER(options->color),
               resource_size,
               CNWN_COLORIZE_NORMAL(options->color),
               resource_count != 1 ? "s" : "");
    return 0;
}

int cnwn_erf_util_extract(const char * path, const cnwn_ERFUtilOptions * options, const cnwn_RegexpArray * regexp_array)
{
    if (path == NULL || path[0] == 0) {
        fprintf(stderr, "%sERROR:%s invalid empty path for ERF\n",
                CNWN_COLORIZE_ERROR(options->color),
                CNWN_COLORIZE_NORMAL(options->color));
        return 1;
    }
    if (!cnwn_string_isempty(options->directory)) {
        int mkret = cnwn_mkdirs(options->directory);
        if (mkret < 0) {
            fprintf(stderr, "%sERROR:%s could create output directory (%s): %s%s\n",
                    CNWN_COLORIZE_ERROR(options->color),
                    CNWN_COLORIZE_NORMAL(options->color),
                    cnwn_get_error(),
                    CNWN_COLORIZE_PATH(options->color),
                    options->directory);
            return 1;
        } else if (mkret > 0 && options->verbose)
            printf("%sCreated path: %s%s\n", CNWN_COLORIZE_NORMAL(options->color), CNWN_COLORIZE_PATH(options->color), options->directory);
    }
    cnwn_File * f = cnwn_file_open_r(path);
    if (f == NULL) {
        fprintf(stderr, "%sERROR:%s could not open ERF (%s): %s%s\n",
                CNWN_COLORIZE_ERROR(options->color),
                CNWN_COLORIZE_NORMAL(options->color),
                cnwn_get_error(),
                CNWN_COLORIZE_PATH(options->color),
                path != NULL ? path : "");
        return 1;
    }
    cnwn_ERFHeader header = {0};
    cnwn_ERFEntry * entries = NULL;
    int num_entries = cnwn_erf_read_header(f, &header, &entries, NULL);
    if (num_entries < 0) {
        fprintf(stderr, "%sERROR:%s could not read entries (%s): %s%s\n",
                CNWN_COLORIZE_ERROR(options->color),
                CNWN_COLORIZE_NORMAL(options->color),
                cnwn_get_error(),
                CNWN_COLORIZE_PATH(options->color),
                path != NULL ? path : "");
        cnwn_file_close(f);
        return 1;
    }
    bool always = false;
    int errors = 0;
    int resource_count = 0;
    int64_t resource_size = 0;
    for (int i = 0; i < header.num_entries; i++) {
        char number[1024];
        char filename[CNWN_PATH_MAX_SIZE];
        cnwn_resource_type_and_key_to_filename(entries[i].type, entries[i].key, sizeof(filename), filename);
        char output_path[CNWN_PATH_MAX_SIZE];
        if (!cnwn_string_isempty(options->directory))
            cnwn_path_concat(output_path, sizeof(output_path), options->directory, filename, NULL);
        else
            cnwn_copy_string(output_path, sizeof(output_path), filename, -1);
        if (regexp_array == NULL || cnwn_regexp_array_match_any(regexp_array, filename)) {
            if (!always && !options->force && cnwn_path_exists(output_path)) {
                printf("%sQUESTION: %sfile \"%s%s%s\" already exists, overwrite? [yes/%sNO%s/always]: ",
                       CNWN_COLORIZE_EMPHASIS(options->color),
                       CNWN_COLORIZE_NORMAL(options->color),
                       CNWN_COLORIZE_PATH(options->color),
                       output_path,
                       CNWN_COLORIZE_NORMAL(options->color),
                       CNWN_COLORIZE_EMPHASIS(options->color),
                       CNWN_COLORIZE_NORMAL(options->color));
                int cret = fgetc(stdin);
                if (cret < 0) {
                    fprintf(stderr, "%sERROR:%s could not get a user response (%s)\n",
                            CNWN_COLORIZE_ERROR(options->color),
                            CNWN_COLORIZE_NORMAL(options->color),
                            strerror(errno));
                    cnwn_file_close(f);
                    free(entries);
                    return errors + 1;
                }
                int flret;
                while ((flret = fgetc(stdin)) != '\n')
                    if (flret < 0) {
                        fprintf(stderr, "%sERROR:%s could not purge stdin (%s)\n",
                                CNWN_COLORIZE_ERROR(options->color),
                                CNWN_COLORIZE_NORMAL(options->color),
                                strerror(errno));
                        cnwn_file_close(f);
                        free(entries);
                        return errors + 1;
                    } else if (flret == 0)
                        break;
                if (cret != 'y' && cret != 'Y' && cret != 'a' && cret != 'A')
                    continue;
                if (cret == 'a' || cret == 'A')
                    always = true;
            }
            cnwn_File * output_f = cnwn_file_open_w(output_path);
            if (output_f == NULL) {
                fprintf(stderr, "%sERROR:%s could not open output file (%s): %s%s\n",
                        CNWN_COLORIZE_ERROR(options->color),
                        CNWN_COLORIZE_NORMAL(options->color),
                        cnwn_get_error(),
                        CNWN_COLORIZE_PATH(options->color),
                        output_path);
                errors++;
                if (!options->ignore_errors) {
                    cnwn_file_close(f);
                    free(entries);
                    return errors;
                }
            }
            int64_t copyret = 0;
            if (entries[i].resource_size > 0) {
                int64_t seekret = cnwn_file_seek(f, entries[i].resource_offset);
                if (seekret < 0) {
                    fprintf(stderr, "%sERROR:%s could not seek to 0x%x for entry %s%d %s(%s): %s%s\n",
                            CNWN_COLORIZE_ERROR(options->color),
                            CNWN_COLORIZE_NORMAL(options->color),
                            entries[i].resource_offset,
                            CNWN_COLORIZE_NUMBER(options->color),
                            i,
                            CNWN_COLORIZE_NORMAL(options->color),
                            cnwn_get_error(),
                            CNWN_COLORIZE_ENTRY(options->color),
                            filename);
                    errors++;
                    if (!options->ignore_errors) {
                        cnwn_file_close(output_f);
                        cnwn_file_close(f);
                        free(entries);
                        return errors;
                    }
                }
                copyret = cnwn_file_copy_bytes(f, output_f, entries[i].resource_size);
                if (copyret < 0) {
                    fprintf(stderr, "%sERROR:%s could not copy from 0x%x for entry %s%d %s(%s): %s%s\n",
                            CNWN_COLORIZE_ERROR(options->color),
                            CNWN_COLORIZE_NORMAL(options->color),
                            entries[i].resource_offset,
                            CNWN_COLORIZE_NUMBER(options->color),
                            i,
                            CNWN_COLORIZE_NORMAL(options->color),
                            cnwn_get_error(),
                            CNWN_COLORIZE_ENTRY(options->color),
                            filename);
                    errors++;
                    if (!options->ignore_errors) {
                        cnwn_file_close(output_f);
                        cnwn_file_close(f);
                        free(entries);
                        return errors;
                    }
                }
            }
            cnwn_file_close(output_f);
            if (copyret >= 0) {
                snprintf(number, sizeof(number), "%"PRId64, copyret);
                if (options->verbose)
                    printf("%s%-32s %s%-10s %s=> %s%s\n",
                           CNWN_COLORIZE_ENTRY(options->color),
                           filename,
                           CNWN_COLORIZE_NUMBER(options->color),
                           number,
                           CNWN_COLORIZE_NORMAL(options->color),
                           CNWN_COLORIZE_PATH(options->color),
                           output_path);
                else
                    printf("%s%s %s%s %s=> %s%s\n",
                           CNWN_COLORIZE_ENTRY(options->color),
                           filename,
                           CNWN_COLORIZE_NUMBER(options->color),
                           number,
                           CNWN_COLORIZE_NORMAL(options->color),
                           CNWN_COLORIZE_PATH(options->color),
                           output_path);
                resource_count++;
                resource_size += copyret;
            }
        }
    }
    free(entries);
    cnwn_file_close(f);
    if (options->verbose)
        printf("%s%d %sentr%s, %s%"PRId64" %sbyte%s\n",
               CNWN_COLORIZE_NUMBER(options->color),
               resource_count,
               CNWN_COLORIZE_NORMAL(options->color),
               resource_count != 1 ? "ies" : "y",
               CNWN_COLORIZE_NUMBER(options->color),
               resource_size,
               CNWN_COLORIZE_NORMAL(options->color),
               resource_count != 1 ? "s" : "");
    return 0;
}

int cnwn_erf_util_create(const char * path, const cnwn_ERFUtilOptions * options, const char ** cmdargs)
{
    if (path == NULL || path[0] == 0) {
        fprintf(stderr, "%sERROR:%s invalid empty path for ERF\n",
                CNWN_COLORIZE_ERROR(options->color),
                CNWN_COLORIZE_NORMAL(options->color));
        return 1;
    }
    char output_directory[CNWN_PATH_MAX_SIZE];
    cnwn_path_dirpart(output_directory, sizeof(output_directory), path);
    if (!cnwn_string_isempty(output_directory)) {
        int mkret = cnwn_mkdirs(output_directory);
        if (mkret < 0) {
            fprintf(stderr, "%sERROR:%s could create output directory (%s): %s%s\n",
                    CNWN_COLORIZE_ERROR(options->color),
                    CNWN_COLORIZE_NORMAL(options->color),
                    cnwn_get_error(),
                    CNWN_COLORIZE_PATH(options->color),
                    output_directory);
            return 1;
        } else if (mkret > 0 && options->verbose)
            printf("%sCreated path: %s%s\n", CNWN_COLORIZE_NORMAL(options->color), CNWN_COLORIZE_PATH(options->color), output_directory);
    }
    if (cnwn_path_exists(path)) {
        printf("%sQUESTION: %sfile \"%s%s%s\" already exists, overwrite? [yes/%sNO%s]: ",
               CNWN_COLORIZE_EMPHASIS(options->color),
               CNWN_COLORIZE_NORMAL(options->color),
               CNWN_COLORIZE_PATH(options->color),
               path,
               CNWN_COLORIZE_NORMAL(options->color),
               CNWN_COLORIZE_EMPHASIS(options->color),
               CNWN_COLORIZE_NORMAL(options->color));
        int cret = fgetc(stdin);
        if (cret < 0) {
            fprintf(stderr, "%sERROR:%s could not get a user response (%s)\n",
                    CNWN_COLORIZE_ERROR(options->color),
                    CNWN_COLORIZE_NORMAL(options->color),
                    strerror(errno));
            return 1;
        }
        int flret;
        while ((flret = fgetc(stdin)) != '\n')
            if (flret < 0) {
                fprintf(stderr, "%sERROR:%s could not purge stdin (%s)\n",
                        CNWN_COLORIZE_ERROR(options->color),
                        CNWN_COLORIZE_NORMAL(options->color),
                        strerror(errno));
                return 1;
            } else if (flret == 0)
                break;
        if (cret != 'y' && cret != 'Y') {
            fprintf(stderr, "%sERROR:%s could not create ERF file (user says no to overwrite)\n",
                    CNWN_COLORIZE_ERROR(options->color),
                    CNWN_COLORIZE_NORMAL(options->color));
            return 1;
        }
    }
    cnwn_File * f = cnwn_file_open_w(path);
    if (f == NULL) {
        fprintf(stderr, "%sERROR:%s could not open ERF (%s): %s%s\n",
                CNWN_COLORIZE_ERROR(options->color),
                CNWN_COLORIZE_NORMAL(options->color),
                cnwn_get_error(),
                CNWN_COLORIZE_PATH(options->color),
                path != NULL ? path : "");
        return 1;
    }
    int argcount = cnwn_count_strings((char **)cmdargs);
    if (argcount <= 0) {
        if (options->verbose)
            printf("%sCreated %s*empty* %sERF file: %s%s\n",
                   CNWN_COLORIZE_NORMAL(options->color),
                   CNWN_COLORIZE_EMPHASIS(options->color),
                   CNWN_COLORIZE_NORMAL(options->color),
                   CNWN_COLORIZE_PATH(options->color),
                   path);
        cnwn_file_close(f);
        return 0;
    }
    // bool always = false;
    // int errors = 0;
    // int resource_count = 0;
    // int64_t resource_size = 0;
    // for (int i = 0; i < header.num_entries; i++) {
    //     char number[1024];
    //     char filename[CNWN_PATH_MAX_SIZE];
    //     cnwn_resource_type_and_key_to_filename(entries[i].type, entries[i].key, sizeof(filename), filename);
    //     char output_path[CNWN_PATH_MAX_SIZE];
    //     if (!cnwn_string_isempty(options->directory))
    //         cnwn_path_concat(output_path, sizeof(output_path), options->directory, filename, NULL);
    //     else
    //         cnwn_copy_string(output_path, sizeof(output_path), filename, -1);
    //     if (regexp_array == NULL || cnwn_regexp_array_match_any(regexp_array, filename)) {
    //         if (!always && !options->force && cnwn_path_exists(output_path)) {
    //             printf("%sQUESTION: %sfile \"%s%s%s\" already exists, overwrite? [yes/%sNO%s/always]: ",
    //                    CNWN_COLORIZE_EMPHASIS(options->color),
    //                    CNWN_COLORIZE_NORMAL(options->color),
    //                    CNWN_COLORIZE_PATH(options->color),
    //                    output_path,
    //                    CNWN_COLORIZE_NORMAL(options->color),
    //                    CNWN_COLORIZE_EMPHASIS(options->color),
    //                    CNWN_COLORIZE_NORMAL(options->color));
    //             int cret = fgetc(stdin);
    //             if (cret < 0) {
    //                 fprintf(stderr, "%sERROR:%s could not get a user response (%s)\n",
    //                         CNWN_COLORIZE_ERROR(options->color),
    //                         CNWN_COLORIZE_NORMAL(options->color),
    //                         strerror(errno));
    //                 cnwn_file_close(f);
    //                 free(entries);
    //                 return errors + 1;
    //             }
    //             int flret;
    //             while ((flret = fgetc(stdin)) != '\n')
    //                 if (flret < 0) {
    //                     fprintf(stderr, "%sERROR:%s could not purge stdin (%s)\n",
    //                             CNWN_COLORIZE_ERROR(options->color),
    //                             CNWN_COLORIZE_NORMAL(options->color),
    //                             strerror(errno));
    //                     cnwn_file_close(f);
    //                     free(entries);
    //                     return errors + 1;
    //                 } else if (flret == 0)
    //                     break;
    //             if (cret != 'y' && cret != 'Y' && cret != 'a' && cret != 'A')
    //                 continue;
    //             if (cret == 'a' || cret == 'A')
    //                 always = true;
    //         }
    //         cnwn_File * output_f = cnwn_file_open_w(output_path);
    //         if (output_f == NULL) {
    //             fprintf(stderr, "%sERROR:%s could not open output file (%s): %s%s\n",
    //                     CNWN_COLORIZE_ERROR(options->color),
    //                     CNWN_COLORIZE_NORMAL(options->color),
    //                     cnwn_get_error(),
    //                     CNWN_COLORIZE_PATH(options->color),
    //                     output_path);
    //             errors++;
    //             if (!options->ignore_errors) {
    //                 cnwn_file_close(f);
    //                 free(entries);
    //                 return errors;
    //             }
    //         }
    //         int64_t copyret = 0;
    //         if (entries[i].resource_size > 0) {
    //             int64_t seekret = cnwn_file_seek(f, entries[i].resource_offset);
    //             if (seekret < 0) {
    //                 fprintf(stderr, "%sERROR:%s could not seek to 0x%x for entry %s%d %s(%s): %s%s\n",
    //                         CNWN_COLORIZE_ERROR(options->color),
    //                         CNWN_COLORIZE_NORMAL(options->color),
    //                         entries[i].resource_offset,
    //                         CNWN_COLORIZE_NUMBER(options->color),
    //                         i,
    //                         CNWN_COLORIZE_NORMAL(options->color),
    //                         cnwn_get_error(),
    //                         CNWN_COLORIZE_ENTRY(options->color),
    //                         filename);
    //                 errors++;
    //                 if (!options->ignore_errors) {
    //                     cnwn_file_close(output_f);
    //                     cnwn_file_close(f);
    //                     free(entries);
    //                     return errors;
    //                 }
    //             }
    //             copyret = cnwn_file_copy_bytes(f, output_f, entries[i].resource_size);
    //             if (copyret < 0) {
    //                 fprintf(stderr, "%sERROR:%s could not copy from 0x%x for entry %s%d %s(%s): %s%s\n",
    //                         CNWN_COLORIZE_ERROR(options->color),
    //                         CNWN_COLORIZE_NORMAL(options->color),
    //                         entries[i].resource_offset,
    //                         CNWN_COLORIZE_NUMBER(options->color),
    //                         i,
    //                         CNWN_COLORIZE_NORMAL(options->color),
    //                         cnwn_get_error(),
    //                         CNWN_COLORIZE_ENTRY(options->color),
    //                         filename);
    //                 errors++;
    //                 if (!options->ignore_errors) {
    //                     cnwn_file_close(output_f);
    //                     cnwn_file_close(f);
    //                     free(entries);
    //                     return errors;
    //                 }
    //             }
    //         }
    //         cnwn_file_close(output_f);
    //         if (copyret >= 0) {
    //             snprintf(number, sizeof(number), "%"PRId64, copyret);
    //             if (options->verbose)
    //                 printf("%s%-32s %s%-10s %s=> %s%s\n",
    //                        CNWN_COLORIZE_ENTRY(options->color),
    //                        filename,
    //                        CNWN_COLORIZE_NUMBER(options->color),
    //                        number,
    //                        CNWN_COLORIZE_NORMAL(options->color),
    //                        CNWN_COLORIZE_PATH(options->color),
    //                        output_path);
    //             else
    //                 printf("%s%s %s%s %s=> %s%s\n",
    //                        CNWN_COLORIZE_ENTRY(options->color),
    //                        filename,
    //                        CNWN_COLORIZE_NUMBER(options->color),
    //                        number,
    //                        CNWN_COLORIZE_NORMAL(options->color),
    //                        CNWN_COLORIZE_PATH(options->color),
    //                        output_path);
    //             resource_count++;
    //             resource_size += copyret;
    //         }
    //     }
    // }
    // free(entries);
    // cnwn_file_close(f);
    // if (options->verbose)
    //     printf("%s%d %sentr%s, %s%"PRId64" %sbyte%s\n",
    //            CNWN_COLORIZE_NUMBER(options->color),
    //            resource_count,
    //            CNWN_COLORIZE_NORMAL(options->color),
    //            resource_count != 1 ? "ies" : "y",
    //            CNWN_COLORIZE_NUMBER(options->color),
    //            resource_size,
    //            CNWN_COLORIZE_NORMAL(options->color),
    //            resource_count != 1 ? "s" : "");
    return 0;
}
