#include "cnwn/tool_erf.h"

int cnwn_tool_erf_command_info(const cnwn_ToolErfSettings * settings, const char * path)
{
    if (cnwn_strisblank(path)) {
        fprintf(stderr, "ERROR: invalid empty path\n");
        return 1;
    }
    cnwn_ErfHeader * header = NULL;
    cnwn_File * f = cnwn_file_open(path, "r");
    if (f == NULL) {
        fprintf(stderr, "ERROR: %s \"%s\"\n", cnwn_get_error(), path);
        return 1;
    }
    int64_t ret = cnwn_erf_header_read(f, &header);
    if (ret < 0) {
        fprintf(stderr, "ERROR: %s \"%s\"\n", cnwn_get_error(), path);
        cnwn_file_close(f);
        return 1;
    }
    printf("ERF %s %d.%d, %d string%s (size=%"PRId64"), %d entr%s (size=%"PRId64")\n",
           CNWN_ERF_TYPE_STRING(header->type),
           header->version.major, header->version.minor,
           header->num_string_entries,
           header->num_string_entries != 1 ? "s" : "",
           header->internal.string_entries_size,
           header->num_resource_entries,
           header->num_resource_entries != 1 ? "ies" : "y",
           cnwn_erf_header_sum_resources(header));
    if (settings != NULL && settings->verbose) {
        printf("Strings size: %"PRId64"\n", header->internal.string_entries_size);
        printf("Strings offset: %"PRId64"\n", header->internal.string_entries_offset);
        printf("Keys offset: %"PRId64"\n", header->internal.keys_offset);
        printf("Values offset: %"PRId64"\n", header->internal.values_offset);
        printf("String entries:\n");
        for (int i = 0; i < header->num_string_entries; i++) {
            int64_t ret = cnwn_file_seek(f, header->string_entries[i].offset);
            if (ret < 0) {
                fprintf(stderr, "ERROR: %s \"%s\"\n", cnwn_get_error(), path);
                cnwn_file_close(f);
                return 1;
            }
            char tmps[64];
            int64_t rsize = CNWN_MIN(header->string_entries[i].size, sizeof(tmps));
            ret = cnwn_file_read_string(f, rsize, tmps);
            if (ret < 0) {
                fprintf(stderr, "ERROR: %s \"%s\"\n", cnwn_get_error(), path);
                cnwn_file_close(f);
                return 1;
            }
            cnwn_strnoctl(tmps, sizeof(tmps), tmps);
            printf("  %d = \"%s\"%s (%"PRId64")\n", i, tmps, header->string_entries[i].size > 63 ? "+" : "", header->string_entries[i].size);
        }
        printf("Resource entries:\n");
        for (int i = 0; i < header->num_resource_entries; i++) {
            printf("  %s (size=%"PRId64" offset=%"PRId64" key=%"PRId64" value=%"PRId64")\n",
                   header->resource_entries[i].filename,
                   header->resource_entries[i].size,
                   header->resource_entries[i].offset,
                   header->resource_entries[i].internal.key_offset,
                   header->resource_entries[i].internal.value_offset
                   );
        }
    }
    cnwn_file_close(f);
    cnwn_erf_header_free(header);
    return 0;
}

int cnwn_tool_erf_command_list(const cnwn_ToolErfSettings * settings, const char * path, int argc, char * argv[])
{
    if (cnwn_strisblank(path)) {
        fprintf(stderr, "ERROR: invalid empty path\n");
        return 1;
    }
    cnwn_RegexpArray * regexps = cnwn_regexp_array_new_args(argc, argv);
    if (regexps == NULL) {
        fprintf(stderr, "ERROR: invalid regular expression: %s\n", cnwn_get_error());
        return 1;
    }
    cnwn_ErfHeader * header = NULL;
    cnwn_File * f = cnwn_file_open(path, "r");
    if (f == NULL) {
        fprintf(stderr, "ERROR: %s \"%s\"\n", cnwn_get_error(), path);
        cnwn_regexp_array_free(regexps);
        return 1;
    }
    int64_t ret = cnwn_erf_header_read(f, &header);
    if (ret < 0) {
        fprintf(stderr, "ERROR: %s \"%s\"\n", cnwn_get_error(), path);
        cnwn_file_close(f);
        cnwn_regexp_array_free(regexps);
        return 1;
    }
    for (int i = 0; i < header->num_resource_entries; i++) {
        if (cnwn_regexp_array_match_any(regexps, header->resource_entries[i].filename)) {
            if (settings != NULL && settings->verbose) {
                printf("%s %"PRId64"\n",
                       header->resource_entries[i].filename,
                       header->resource_entries[i].size);
            } else
                printf("%s\n", header->resource_entries[i].filename);
        }
    }
    cnwn_regexp_array_free(regexps);
    cnwn_file_close(f);
    cnwn_erf_header_free(header);
    return 0;
}

int cnwn_tool_erf_command_extract(const cnwn_ToolErfSettings * settings, const char * path, int argc, char * argv[])
{
    cnwn_ToolErfSettings use_settings = {0};
    if (settings != NULL)
        use_settings = *settings;
    if (cnwn_strisblank(path)) {
        fprintf(stderr, "ERROR: invalid empty path\n");
        return 1;
    }
    cnwn_RegexpArray * regexps = cnwn_regexp_array_new_args(argc, argv);
    if (regexps == NULL) {
        fprintf(stderr, "ERROR: %s\n", cnwn_get_error());
        return 1;
    }
    cnwn_ErfHeader * header = NULL;
    cnwn_File * f = cnwn_file_open(path, "r");
    if (f == NULL) {
        fprintf(stderr, "ERROR: %s \"%s\"\n", cnwn_get_error(), path);
        cnwn_regexp_array_free(regexps);
        return 1;
    }
    int64_t ret = cnwn_erf_header_read(f, &header);
    if (ret < 0) {
        fprintf(stderr, "ERROR: %s \"%s\"\n", cnwn_get_error(), path);
        cnwn_file_close(f);
        cnwn_regexp_array_free(regexps);
        return 1;
    }
    if (!cnwn_strisblank(use_settings.directory)) {
        int mkdirret = cnwn_file_system_mkdir(use_settings.directory);
        if (mkdirret < 0) {
            fprintf(stderr, "ERROR: could not create dirs \"%s\", %s\n", use_settings.directory, cnwn_get_error());
            cnwn_file_close(f);
            cnwn_regexp_array_free(regexps);
            cnwn_erf_header_free(header);
            return 1;
        }
    }
    int counter = 0;
    int64_t sizer = 0;
    for (int i = 0; i < header->num_resource_entries; i++) {
        if (cnwn_regexp_array_match_any(regexps, header->resource_entries[i].filename)) {
            char output_path[CNWN_PATH_MAX_SIZE];
            if (cnwn_strisblank(use_settings.directory))
                snprintf(output_path, sizeof(output_path), "%s", header->resource_entries[i].filename);
            else
                snprintf(output_path, sizeof(output_path), "%s%s%s", use_settings.directory, CNWN_PATH_SEPARATOR, header->resource_entries[i].filename);
            int retisfile = cnwn_file_system_isfile(output_path);
            if (retisfile < 0) {
                fprintf(stderr, "ERROR: could not stat \"%s\", %s\n", output_path, cnwn_get_error());
                cnwn_file_close(f);
                cnwn_regexp_array_free(regexps);
                cnwn_erf_header_free(header);
                return 1;
            }
            if (retisfile > 0) {
                if (CNWN_TOOL_ERF_SETTINGS_FORCE_NO(&use_settings)) {
                    continue;
                } else if (!CNWN_TOOL_ERF_SETTINGS_FORCE_YES(&use_settings)) {
                    printf("QUESTION: overwrite \"%s\"? yes/NO/always: ", output_path);
                    int c = getchar();
                    int tmpc;
                    while ((tmpc = getchar()) != 0)
                        if (tmpc == '\n')
                            break;
                    if (c != 'y' && c != 'Y' && c != 'a' && c != 'A') 
                        continue;
                    if (c == 'a' || c == 'A')
                        use_settings.yes = true;
                }
            }
            cnwn_File * output_f = cnwn_file_open(output_path, "wt");
            if (output_f == NULL) {
                fprintf(stderr, "ERROR: could not write \"%s\", %s\n", output_path, cnwn_get_error());
                cnwn_file_close(f);
                cnwn_regexp_array_free(regexps);
                cnwn_erf_header_free(header);
                return 1;
            }
            int64_t copied = cnwn_file_copy(f, header->resource_entries[i].size, output_f);
            if (copied < 0) {
                fprintf(stderr, "ERROR: could not write \"%s\", %s\n", output_path, cnwn_get_error());
                cnwn_file_close(f);
                cnwn_regexp_array_free(regexps);
                cnwn_erf_header_free(header);
                return 1;
            }
            cnwn_file_close(output_f);
            counter++;
            sizer += copied;
            if (use_settings.verbose) 
                printf("%s => %s %"PRId64"\n", header->resource_entries[i].filename, output_path, copied);
        }
    }
    cnwn_regexp_array_free(regexps);
    cnwn_file_close(f);
    cnwn_erf_header_free(header);
    printf("Extracted %"PRId64" byte%s in %d entr%s.\n",
           sizer, sizer != 1 ? "s" : "",
           counter, counter != 1 ? "ies" : "y");
    return 0;
}
