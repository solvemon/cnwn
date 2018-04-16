#include "cnwn/erf.h"

#include <getopt.h>

const cnwn_CliOption GENERAL_OPTIONS[] = {
    {"h", false, "Show this help."},
    {"v", false, "Show version."},
    {"q", false, "Supress verbose output to stdout (will work for all commadns)."},
    {0}
};

const cnwn_CliOption LIST_OPTIONS[] = {
    {"l", false, "Long format."},
    {"H", false, "ERF header info only, do not list entries."},
    {0}
};

const cnwn_CliOption EXTRACT_OPTIONS[] = {
    {"d", true, "Output directory."},
    {"f", false, "Force overwrites of already existing files."},
    {"m", true, "Specify manifest file path."},
    {0}
};

const cnwn_CliOption ARCHIVE_OPTIONS[] = {
    {"d", true, "Input directory."},
    {"m", true, "Specify manifest file path."},
    {0}
};

void print_help(void)
{
    printf("cnwn-erf [general options] <list |extract|archive> <FILE> [command options] [REGEXP ...]\n");
    printf("Version: %d.%d.%d", VERSION_MAJOR, VERSION_MINOR, VERSION_PATCH);
    printf("\n");
    printf("  General options:\n");
    cnwn_print_cli_options(GENERAL_OPTIONS, "    ");
    printf("\n");
    printf("  'list', 'l' or 'ls':\n");
    printf("    FILE is the ERF file to list the contents of.\n");
    printf("    REGEXP will filter the entries to list.\n");
    cnwn_print_cli_options(LIST_OPTIONS, "    ");
    printf("\n");
    printf("  'extract', 'e' or 'x':\n");
    printf("    FILE is the ERF file to extract.\n");
    printf("    REGEXP will filter the entries to extract.\n");
    cnwn_print_cli_options(EXTRACT_OPTIONS, "    ");
    printf("\n");
    printf("  'archive' or 'a':\n");
    printf("    FILE is the ERF file to archive.\n");
    printf("    REGEXP will filter the files to archive.\n");
    cnwn_print_cli_options(ARCHIVE_OPTIONS, "    ");
    printf("\n");
    printf("  Multiple REGEXP's are allowed (OR operator).\n");
    printf("\n");
}


int main(int argc, char * argv[])
{
    const cnwn_CliOption * options = GENERAL_OPTIONS;
    bool verbose = true;
    bool long_format = false;
    bool list_entries = true;
    const char * output_dir = NULL;
    bool force = false;
    const char * erf_file = NULL;
    const char * regexps[8192] = {0};
    int num_regexps = 0;
    for (int i = 1; i < argc; i++) {
        const char * arg;
        bool fallback = false;
        int optindex = cnwn_parse_cli_option(options, argv[i], i < argc - 1 ? argv[i + 1] : NULL, &arg);
        if (optindex < 0 && optindex != CNWN_CLI_OPTION_ARG) {
            optindex = cnwn_parse_cli_option(GENERAL_OPTIONS, argv[i], i < argc - 1 ? argv[i + 1] : NULL, &arg);
            fallback = true;
        }
        if (optindex >= 0) {
            if (fallback || options == GENERAL_OPTIONS) {
                if (optindex == 0) {
                    print_help();
                    return 0;
                } else if (optindex == 1) {
                    printf("%d.%d.%d\n", VERSION_MAJOR, VERSION_MINOR, VERSION_PATCH);
                    return 0;
                } else if (optindex == 2)
                    verbose = false;
            } else if (options == LIST_OPTIONS) {
                if (optindex == 0)
                    long_format = true;
                else if (optindex == 1)
                    list_entries = false;
            } else if (options == EXTRACT_OPTIONS) {
                if (optindex == 0)
                    output_dir = arg;
                else if (optindex == 1)
                    force = true;
            }
            if (CNWN_CLI_OPTION_IS_SHORT(argv[i]) && arg != NULL)
                i++;
        } else if (optindex == CNWN_CLI_OPTION_ARG) {
            if (options == GENERAL_OPTIONS) {
                if (strcmp(arg, "list") == 0 || strcmp(arg, "l") == 0 || strcmp(arg, "ls") == 0)
                    options = LIST_OPTIONS;
                else if (strcmp(arg, "extract") == 0 || strcmp(arg, "e") == 0 || strcmp(arg, "x") == 0)
                    options = EXTRACT_OPTIONS;
                else if (strcmp(arg, "archive") == 0 || strcmp(arg, "a") == 0)
                    options = ARCHIVE_OPTIONS;
            } else if (erf_file == NULL) {
                erf_file = arg;
            } else if (num_regexps < 8191) {
                regexps[num_regexps++] = arg;
            } else {
                fprintf(stderr, "ERROR: maximum number of REGEXP's (8192) reached\n");
                return 1;
            }
        } else if (optindex == CNWN_CLI_OPTION_INVALID) {
            fprintf(stderr, "ERROR: invalid option '%s' for argument %d\n", argv[i], i);
            return 1;
        } else if (optindex == CNWN_CLI_OPTION_INCOMPLETE) {
            fprintf(stderr, "ERROR: option '%s' for argument %d requires a value\n", argv[i], i);
            return 1;
        }
    }
    if (options == GENERAL_OPTIONS) {
        fprintf(stderr, "ERROR: a command must be specified (list, extract or archive).\n");
        return 1;
    }
    if (erf_file == NULL) {
        fprintf(stderr, "ERROR: an ERF file must be specified.\n");
        return 1;
    }
    /**
     *
     * Extract
     *
     */
    if (options == EXTRACT_OPTIONS) {
        cnwn_ERFHeader header = {0};
        int num_entries = cnwn_erf_read_contents_path(erf_file, regexps, &header, 0, NULL);
        uint32_t total_size = 0;
        if (num_entries < 0) {
            fprintf(stderr, "ERROR: %s.\n", cnwn_get_error());
            return 2;
        }
        if (verbose) 
            printf("ERF type '%s' version %d.%d:\n", CNWN_RESOURCE_TYPE_EXTENSION(header.type), header.version.major, header.version.minor);
        if (num_entries > 0) {
            cnwn_ERFEntry * entries = malloc(sizeof(cnwn_ERFEntry) * num_entries);
            memset(entries, 0, sizeof(cnwn_ERFEntry) * num_entries);
            int using_fd = -1;
            num_entries = cnwn_erf_read_contents_path_fd(erf_file, regexps, &header, num_entries, entries, &using_fd);
            if (num_entries < 0) {
                free(entries);
                close(using_fd);
                fprintf(stderr, "ERROR: %s.\n", cnwn_get_error());
                return 2;
            }
            if (output_dir != NULL) {
                int st = cnwn_exists(output_dir);
                if (st < 0) {
                    free(entries);
                    close(using_fd);
                    fprintf(stderr, "ERROR: %s\n", cnwn_get_error());
                    return 3;
                }
                if (st == 0) {
                    if (!force) {
                        printf("Output directory '%s' does not exist, create it? [y/N] ", output_dir);
                        int retchar = getchar();
                        if (retchar != (unsigned char)'Y' && retchar != (unsigned char)'y') {
                            printf("NO\n");
                            free(entries);
                            close(using_fd);
                            fprintf(stderr, "ERROR: could not write to output directory\n");
                            return 4;
                        }
                    }
                    int mkd = cnwn_mkdirs(output_dir);
                    if (mkd < 0) {
                        free(entries);
                        close(using_fd);
                        fprintf(stderr, "ERROR: %s\n", cnwn_get_error());
                        return 4;
                    } else if (mkd > 0 && verbose)
                        printf("Created path: %s\n", output_dir);
                }
            }
            char output_path[CNWN_PATH_MAX_SIZE];
            for (int i = 0; i < num_entries; i++) {
                if (CNWN_RESOUCE_TYPE_VALID(entries[i].type)) {
                    if (output_dir != NULL)
                        snprintf(output_path, sizeof(output_path), "%s%c%s.%s", output_dir, CNWN_PATH_SEPARATOR, entries[i].key, CNWN_RESOURCE_TYPE_EXTENSION(entries[i].type));
                    else
                        snprintf(output_path, sizeof(output_path), "%s.%s", entries[i].key, CNWN_RESOURCE_TYPE_EXTENSION(entries[i].type));
                } else if (output_dir != NULL)
                    snprintf(output_path, sizeof(output_path), "%s%c%s", output_dir, CNWN_PATH_SEPARATOR, entries[i].key);
                else
                    snprintf(output_path, sizeof(output_path), "%s", entries[i].key);
                if (!force) {
                    int existing = cnwn_exists(output_path);
                    if (existing < 0) {
                        free(entries);
                        close(using_fd);
                        fprintf(stderr, "ERROR: %s\n", cnwn_get_error());
                        return 5;
                    }
                    if (existing > 0) {
                        printf("The file '%s' already exists, overwrite it? [y/N/a] ", output_path);
                        int retchar = getchar();
                        if (retchar != (unsigned char)'Y' && retchar != (unsigned char)'y' && retchar != (unsigned char)'A' && retchar != (unsigned char)'a') {
                            continue;
                        }
                        if (retchar == (unsigned char)'A' || retchar == (unsigned char)'a') {
                            force = true;
                        } 
                    }
                }
                int extret =  cnwn_erf_entry_extract_path(using_fd, entries[i].resource_offset, entries[i].resource_size, output_path);
                if (extret < 0) {
                    free(entries);
                    close(using_fd);
                    fprintf(stderr, "ERROR: %s\n", cnwn_get_error());
                    return 5;
                }
                if (verbose) {
                    if (CNWN_RESOUCE_TYPE_VALID(entries[i].type)) 
                        printf("%s.%s => %s (%u)\n", entries[i].key, CNWN_RESOURCE_TYPE_EXTENSION(entries[i].type), output_path, entries[i].resource_size);
                    else 
                        printf("%s => %s (%u)\n", entries[i].key, output_path, entries[i].resource_size);
                }
                total_size += entries[i].resource_size;
            }
            free(entries);
            close(using_fd);
            if (verbose)
                printf("\nWrote %d entries, %u bytes.\n", num_entries, total_size);
        } else
            printf("Empty ERF, nothing was done.\n");
    }
    /**
     *
     * Archive
     *
     */
    else if (options == ARCHIVE_OPTIONS) {
    }
    /**
     *
     * List
     *
     */
    else {
        cnwn_ERFHeader header = {0};
        int num_entries = cnwn_erf_read_contents_path(erf_file, regexps, &header, 0, NULL);
        uint32_t total_size = 0;
        if (num_entries < 0) {
            fprintf(stderr, "ERROR: %s.\n", cnwn_get_error());
            return 2;
        }
        if (!list_entries) {
            printf("ERF type '%s' version %d.%d\n", CNWN_RESOURCE_TYPE_EXTENSION(header.type), header.version.major, header.version.minor);
            return 0;
        }
        if (verbose) 
            printf("ERF type '%s' version %d.%d:\n\n", CNWN_RESOURCE_TYPE_EXTENSION(header.type), header.version.major, header.version.minor);
        if (num_entries > 0) {
            cnwn_ERFEntry * entries = malloc(sizeof(cnwn_ERFEntry) * num_entries);
            memset(entries, 0, sizeof(cnwn_ERFEntry) * num_entries);
            num_entries = cnwn_erf_read_contents_path(erf_file, regexps, &header, num_entries, entries);
            if (num_entries < 0) {
                free(entries);
                fprintf(stderr, "ERROR: %s.\n", cnwn_get_error());
                return 2;
            }
            for (int i = 0; i < num_entries; i++) {
                if (long_format) {
                    if (CNWN_RESOUCE_TYPE_VALID(entries[i].type))
                        printf("%s.%s offset=%u (%u)\n", entries[i].key, CNWN_RESOURCE_TYPE_EXTENSION(entries[i].type), entries[i].resource_offset, entries[i].resource_size);
                    else
                        printf("%s offset=%u (%u) (unknown type)\n", entries[i].key, entries[i].resource_offset, entries[i].resource_size);
                } else if (CNWN_RESOUCE_TYPE_VALID(entries[i].type))
                    printf("%s.%s\n", entries[i].key, CNWN_RESOURCE_TYPE_EXTENSION(entries[i].type));
                else
                    printf("%s\n", entries[i].key);
                total_size = entries[i].resource_size;
            }
            free(entries);
        }
        if (verbose)
            printf("\n%d entries, %u bytes.\n", num_entries, total_size);
    }
    return 0;
}
