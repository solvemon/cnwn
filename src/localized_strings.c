#include "cnwn/localized_strings.h"

static void cnwn_localized_string_array_init_elements(void * elements, int length, const void * source)
{
    const cnwn_LocalizedString * source_strings = source;
    cnwn_LocalizedString * strings = elements;
    for (int i = 0; i < length; i++)
        if (source != NULL)
            cnwn_localized_string_init(strings + i, source_strings[i].language_id, source_strings[i].text);
        else
            memset(strings + i, 0, sizeof(cnwn_LocalizedString));
}

static void cnwn_localized_string_array_deinit_elements(void * elements, int length)
{
    cnwn_LocalizedString * strings = elements;
    for (int i = 0; i < length; i++)
        cnwn_localized_string_deinit(strings + i);
}

static int cnwn_localized_string_array_compare_elements(const void * element_a, const void * element_b)
{
    const cnwn_LocalizedString * string_a = element_a;
    const cnwn_LocalizedString * string_b = element_b;
    if (string_a == string_b)
        return 0;
    if (string_a != NULL && string_b == NULL)
        return 1;
    if (string_a == NULL && string_b != NULL)
        return -1;
    if (string_a->language_id > string_b->language_id)
        return 1;
    if (string_a->language_id < string_b->language_id)
        return -1;
    return cnwn_strcmp(string_a->text, string_b->text);
}


void cnwn_localized_string_init(cnwn_LocalizedString * localized_string, int language_id, const char * text)
{
    memset(localized_string, 0, sizeof(cnwn_LocalizedString));
    localized_string->language_id = language_id;
    localized_string->text = cnwn_strdup(text != NULL ? text : "");
}

int64_t cnwn_localized_string_init_from_file(cnwn_LocalizedString * localized_string, cnwn_File * f)
{
    int64_t bytes = 0;
    int32_t language_id;
    int32_t size;
    int64_t ret;
    ret = cnwn_file_read32(f, &language_id);
    if (ret < 0) {
        cnwn_set_error("%s (%s)", cnwn_get_error(), "reading language id");
        return -1;
    }
    bytes += ret;
    ret = cnwn_file_read32(f, &size);
    if (ret < 0) {
        cnwn_set_error("%s (%s)", cnwn_get_error(), "reading size");
        return -1;
    }
    if (size > 0) {
        char * text = malloc(sizeof(char) * (size + 1));
        ret = cnwn_file_read_string(f, size, text);        
        if (ret < 0) {
            cnwn_set_error("%s (%s)", cnwn_get_error(), "reading text");
            free(text);
            return -1;
        }
        cnwn_localized_string_init(localized_string, language_id, text);
        free(text);
    } else
        cnwn_localized_string_init(localized_string, language_id, "");
    return ret;
}

void cnwn_localized_string_deinit(cnwn_LocalizedString * localized_string)
{
    if (localized_string != NULL && localized_string->text != NULL)
        free(localized_string->text);
}

int64_t cnwn_localized_string_write(const cnwn_LocalizedString * localized_string, cnwn_File * f)
{
    int64_t bytes = 0;
    int64_t ret;
    int textlen = cnwn_strlen(localized_string->text);
    ret = cnwn_file_write32(f, localized_string->language_id);
    if (ret < 0) {
        cnwn_set_error("%s (%s)", cnwn_get_error(), "writing language id");
        return -1;
    }
    bytes += ret;
    ret = cnwn_file_write32(f, localized_string->language_id);
    if (ret < 0) {
        cnwn_set_error("%s (%s)", cnwn_get_error(), "writing size");
        return -1;
    }
    bytes += ret;
    if (textlen > 0) {
        ret = cnwn_file_write_string(f, localized_string->text);
        if (ret < 0) {
            cnwn_set_error("%s (%s)", cnwn_get_error(), "writing text");
            return -1;
        }
        bytes += ret;
    }
    return ret;
}

int64_t cnwn_localized_string_array_init_from_file(cnwn_LocalizedStringArray * array, int num, cnwn_File * f)
{
    cnwn_ContainerCallbacks cb = {&cnwn_localized_string_array_init_elements,
                                  &cnwn_localized_string_array_deinit_elements,
                                  &cnwn_localized_string_array_compare_elements};
    cnwn_array_init(array, sizeof(cnwn_LocalizedString), &cb);
    int64_t ret = 0;
    if (num > 0) {
        cnwn_array_set_length(array, num);
        for (int i = 0; i < num; i++) {
            int64_t r = cnwn_localized_string_init_from_file(((cnwn_LocalizedString *)array->data) + i, f);
            if (r < 0) {
                cnwn_set_error("%s (%s %d)", cnwn_get_error(), "localized string", i);
                cnwn_array_deinit(array);
                return -1;
            }
            ret += r;
        }
    }
    return ret;    
}

int64_t cnwn_localized_string_array_write(const cnwn_LocalizedStringArray * array, cnwn_File * f)
{
    return 0;
}

void cnwn_localized_string_array_append(cnwn_LocalizedStringArray * array, int lang_id, const char * s)
{
    cnwn_LocalizedString ls = {lang_id, (char *)s};
    cnwn_array_append(array, 1, &ls);
}
