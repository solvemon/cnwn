#include "cnwn/localized_strings.h"

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
