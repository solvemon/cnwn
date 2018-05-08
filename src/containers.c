#include "cnwn/containers.h"

static void cnwn_string_array_init_elements(void * elements, int length, const void * source)
{
    char ** element_ptrs_src = (char **)source;
    char ** element_ptrs = (char **)elements;
    if (source != NULL)
        for (int i = 0; i < length; i++)
            element_ptrs[i] = cnwn_strdup(element_ptrs_src[i]);
    else
        memset(element_ptrs, 0, sizeof(char *) * length);
}

static void cnwn_string_array_deinit_elements(void * elements, int length)
{
    char ** element_ptrs = (char **)elements;
    for (int i = 0; i < length; i++)
        if (element_ptrs[i] != NULL)
            free(element_ptrs[i]);
}

static int cnwn_string_array_compare_elements(const void * element_a, const void * element_b)
{
    return cnwn_strcmp((const char *)element_a, (const char *)element_b);
}

const cnwn_ContainerCallbacks CNWN_STRING_ARRAY_FUNCTIONS = {
    &cnwn_string_array_init_elements,
    &cnwn_string_array_deinit_elements,
    &cnwn_string_array_compare_elements
};

////////////////////////////////////////////////////////////////
//
//
// Array
//
//
////////////////////////////////////////////////////////////////

void cnwn_array_init(cnwn_Array * array, int element_size, const cnwn_ContainerCallbacks * cb)
{
    memset(array, 0, sizeof(cnwn_Array));
    array->element_size = CNWN_MAX(0, element_size);
    if (cb != NULL)
        array->cb = *cb;
}

cnwn_Array * cnwn_array_new(int element_size, const cnwn_ContainerCallbacks * cb)
{
    cnwn_Array * ret = malloc(sizeof(cnwn_Array));
    cnwn_array_init(ret, element_size, cb);
    return ret;
}

void cnwn_array_init_clone(cnwn_Array * array, const cnwn_Array * source)
{
    cnwn_array_init(array, source->element_size, &source->cb);
    cnwn_array_append(array, source->length, source->data);
}

cnwn_Array * cnwn_array_new_clone(const cnwn_Array * source)
{
    cnwn_Array * ret = malloc(sizeof(cnwn_Array));
    cnwn_array_init_clone(ret, source);
    return ret;
}

void cnwn_array_deinit(cnwn_Array * array)
{
    if (array->data != NULL) {
        if (array->cb.deinit_elements != NULL)
            array->cb.deinit_elements(array->data, array->length);
        free(array->data);
    }
}

void cnwn_array_free(cnwn_Array * array)
{
    cnwn_array_deinit(array);
    free(array);
}

void cnwn_array_clear(cnwn_Array * array)
{
    if (array->data != NULL) {
        if (array->cb.deinit_elements != NULL)
            array->cb.deinit_elements(array->data, array->length);
        free(array->data);
        array->data = NULL;
    }
    array->length = 0;
}

int cnwn_array_get_length(const cnwn_Array * array)
{
    return array->length;
}

void cnwn_array_set_length(cnwn_Array * array, int length)
{
    if (array->element_size > 0) {
        if (length > 0) {
            if (length > array->length) {
                array->data = realloc(array->data, array->element_size * length);
                if (array->cb.init_elements != NULL)
                    array->cb.init_elements(((uint8_t *)array->data) + array->element_size * array->length, length - array->length, NULL);
                else
                    memset(((uint8_t *)array->data) + array->element_size * array->length, 0, array->element_size * (length - array->length));
            } else if (length < array->length) {
                if (array->cb.deinit_elements != NULL)
                    array->cb.deinit_elements(((uint8_t *)array->data) + array->element_size * length, array->length - length);
                array->data = realloc(array->data, array->element_size * length);
            }
            array->length = length;
        } else
            cnwn_array_clear(array);
    }
}

void * cnwn_array_element_ptr(const cnwn_Array * array, int index)
{
    if (array->element_size > 0) {
        index = CNWN_WRAP_INDEX(index, array->length);
        if (index >= 0 && index < array->length)
            return ((uint8_t *)array->data) + array->element_size * index;
    }
    return NULL;
}

int cnwn_array_get(const cnwn_Array * array, int index, int length, void * ret_elements)
{
    if (array->element_size > 0 && length > 0) {
        index = CNWN_WRAP_INDEX(index, array->length);
        if (index >= 0 && index < array->length) {
            if (index + length > array->length)
                length = array->length - index;
            if (ret_elements != NULL)
                memcpy(ret_elements, ((const uint8_t *)array->data) + array->element_size * index, array->element_size * length);
            return length;
        }
    }
    return 0;
}

void * cnwn_array_get_ptr(const cnwn_Array * array, int index)
{
    if (array->element_size > 0) {
        index = CNWN_WRAP_INDEX(index, array->length);
        if (index >= 0 && index < array->length) 
            return *((void **)(((uint8_t *)array->data) + index * array->element_size));
    }
    return NULL;
}

int cnwn_array_set(cnwn_Array * array, int index, int length, const void * elements)
{
    if (array->element_size > 0 && length > 0) {
        index = CNWN_WRAP_INDEX(index, array->length);
        if (index >= 0 && index < array->length) {
            if (index + length > array->length)
                length = array->length - index;
            void * tmp = NULL;
            if (array->cb.deinit_elements != NULL) {
                tmp = malloc(array->element_size * length);
                memcpy(tmp, ((const uint8_t *)array->data) + array->element_size * index, array->element_size * length);
            }
            if (array->cb.init_elements != NULL)
                array->cb.init_elements(((uint8_t *)array->data) + array->element_size * index, length, elements);
            else if (elements != NULL)
                memcpy(((uint8_t *)array->data) + array->element_size * index, elements, array->element_size * length);
            else
                memset(((uint8_t *)array->data) + array->element_size * index, 0, array->element_size * length);
            if (array->cb.deinit_elements != NULL && tmp != NULL) {
                array->cb.deinit_elements(tmp, length);
                free(tmp);
            }
            return length;
        }
    }
    return 0;
}

int cnwn_array_insert(cnwn_Array * array, int index, int length, const void * elements)
{
    if (array->element_size > 0 && length > 0) {
        index = CNWN_WRAP_INDEX(index, array->length);
        index = CNWN_MINMAX(index, 0, array->length);
        array->data = realloc(array->data, array->element_size * (array->length + length));
        if (index < array->length)
            memmove(((uint8_t *)array->data) + array->element_size * (index + length),
                    ((uint8_t *)array->data) + array->element_size * index,
                    array->element_size * (array->length - index));
        if (array->cb.init_elements != NULL)
            array->cb.init_elements(((uint8_t *)array->data) + array->element_size * index, length, elements);
        else if (elements != NULL)
            memcpy(((uint8_t *)array->data) + array->element_size * index, elements, array->element_size * length);
        else
            memset(((uint8_t *)array->data) + array->element_size * index, 0, array->element_size * length);
        array->length += length;
        return length;
    }
    return 0;
}

int cnwn_array_append(cnwn_Array * array, int length, const void * elements)
{
    if (array->element_size > 0 && length > 0) {
        array->data = realloc(array->data, array->element_size * (array->length + length));
        if (array->cb.init_elements != NULL)
            array->cb.init_elements(((uint8_t *)array->data) + array->element_size * array->length, length, elements);
        else if (elements != NULL)
            memcpy(((uint8_t *)array->data) + array->element_size * array->length, elements, array->element_size * length);
        else
            memset(((uint8_t *)array->data) + array->element_size * array->length, 0, array->element_size * length);
        array->length += length;
        return length;
    }
    return 0;
}
        
int cnwn_array_remove(cnwn_Array * array, int index, int length)
{
    if (array->element_size > 0 && length > 0) {
        index = CNWN_WRAP_INDEX(index, array->length);
        if (index >= 0 && index < array->length) {
            if (index + length > array->length)
                length = array->length - index;
            if (array->cb.deinit_elements != NULL)
                array->cb.deinit_elements(((uint8_t *)array->data) + array->element_size * index, length);
            if (length < array->length) {
                if (index + length < array->length)
                    memmove(((uint8_t *)array->data) + array->element_size * index,
                            ((uint8_t *)array->data) + array->element_size * (index + length),
                            array->element_size * (array->length - index - length));
                array->data = realloc(array->data, array->element_size * length);
                array->length -= length;
            } else {
                free(array->data);
                array->data = NULL;
                array->length = 0;
            }
            return length;
        }
    }
    return 0;
}

bool cnwn_array_swap(cnwn_Array * array, int index_a, int index_b)
{
    if (array->element_size > 0) {
        index_a = CNWN_WRAP_INDEX(index_a, array->length);
        index_b = CNWN_WRAP_INDEX(index_b, array->length);
        if (index_a >= 0 && index_a < array->length && index_b >= 0 && index_b < array->length) {
            char * tmp = malloc(array->element_size);
            memcpy(tmp, ((const uint8_t *)array->data) + array->element_size * index_a, array->element_size);
            memcpy(((uint8_t *)array->data) + array->element_size * index_a, ((const uint8_t *)array->data) + array->element_size * index_b, array->element_size);
            memcpy(((uint8_t *)array->data) + array->element_size * index_b, tmp, array->element_size);
            free(tmp);
            return true;
        }
    }
    return false;
}

int cnwn_array_find(const cnwn_Array * array, int index, bool reverse, const void * element)
{
    if (array->element_size > 0 && array->length > 0) {
        index = CNWN_WRAP_INDEX(index, array->length);
        index = CNWN_MINMAX(index, 0, array->length - 1);
        if (reverse) {
            for (int i = index; i >= 0; i--) {
                if (array->cb.compare_elements != NULL) {
                    if (array->cb.compare_elements(element, ((uint8_t *)array->data) + array->element_size * i) == 0)
                        return i;
                } else if (element == (void *)(((const uint8_t *)array->data) + array->element_size * i))
                    return i;
            }
        } else {
            for (int i = index; i < array->length; i++) {
                if (array->cb.compare_elements != NULL) {
                    if (array->cb.compare_elements(element, ((uint8_t *)array->data) + array->element_size * i) == 0)
                        return i;
                } else if (element == (void *)(((const uint8_t *)array->data) + array->element_size * i))
                    return i;
            }
        }
    }
    return -1;
}

void cnwn_array_sort(cnwn_Array * array, bool reverse)
{
    if (array->element_size > 0 && array->length > 0 && array->cb.compare_elements != NULL) {
        for (int i = 0; i < array->length; i++) {
            for (int j = 0; j < array->length; j++) {
                if (j != i) {
                    int cmp = array->cb.compare_elements(((const uint8_t *)array->data) + array->element_size * i, ((const uint8_t *)array->data) + array->element_size * j);
                    if ((!reverse && cmp < 0) || (reverse && cmp > 0))
                        cnwn_array_swap(array, i, j);
                }
            }
        }
    }
 }

int cnwn_array_set_slice(cnwn_Array * array, int index, int length, const cnwn_Array * slice, int slice_index)
{
    if (array->element_size > 0 && array->element_size == slice->element_size) {
        slice_index = CNWN_WRAP_INDEX(slice_index, slice->length);
        if (slice_index >= 0 && slice_index < slice->length) {
            if (slice_index + length > slice->length)
                length = slice->length - slice_index;
            return cnwn_array_set(array, index, length, ((const uint8_t *)array->data) + slice->element_size * slice_index);
        }
    }
    return 0;
}

int cnwn_array_insert_slice(cnwn_Array * array, int index, int length, const cnwn_Array * slice, int slice_index)
{
    if (array->element_size > 0 && array->element_size == slice->element_size) {
        slice_index = CNWN_WRAP_INDEX(slice_index, slice->length);
        if (slice_index >= 0 && slice_index < slice->length) {
            if (slice_index + length > slice->length)
                length = slice->length - slice_index;
            return cnwn_array_insert(array, index, length, ((const uint8_t *)array->data) + slice->element_size * slice_index);
        }
    }
    return 0;
}

int cnwn_array_append_slice(cnwn_Array * array, int length, const cnwn_Array * slice, int slice_index)
{
    if (array->element_size > 0 && array->element_size == slice->element_size) {
        slice_index = CNWN_WRAP_INDEX(slice_index, slice->length);
        if (slice_index >= 0 && slice_index < slice->length) {
            if (slice_index + length > slice->length)
                length = slice->length - slice_index;
            return cnwn_array_append(array, length, ((const uint8_t *)array->data) + slice->element_size * slice_index);
        }
    }
    return 0;
}

int cnwn_array_reverse(cnwn_Array * array, int index, int length)
{
    if (index < 0)
        index += array->length;
    if (array->element_size > 0 && index >= 0 && index < array->length && length > 0) {
        if (index + length > array->length)
            length = array->length - index;
        void * tmp = malloc(array->element_size * length);
        memcpy(tmp, ((const uint8_t *)array->data) + array->element_size * index, array->element_size * length);
        for (int i = 0; i < length; i++)
            memcpy(((uint8_t *)array->data) + array->element_size * (index + i), ((uint8_t *)tmp) + array->element_size * (length - i - 1), array->element_size);
        free(tmp);
        return length;
    }
    return 0;
}

void cnwn_array_reverse2(cnwn_Array * array)
{
    cnwn_array_reverse(array, 0, array->length);
}

////////////////////////////////////////////////////////////////
//
//
// Dict
//
//
////////////////////////////////////////////////////////////////

void cnwn_dict_init(cnwn_Dict * dict, int element_size, int num_buckets, cnwn_HashFunction32 hash_function, const cnwn_ContainerCallbacks * cb)
{
    memset(dict, 0, sizeof(cnwn_Dict));
    dict->element_size = CNWN_MAX(0, element_size);
    dict->num_buckets = CNWN_MAX(0, num_buckets);
    dict->hash_function = hash_function;
    if (cb != NULL)
        dict->cb = *cb;
    if (dict->num_buckets > 0) {
        dict->buckets = malloc(sizeof(cnwn_DictBucket) * dict->num_buckets);
        memset(dict->buckets, 0, sizeof(cnwn_DictBucket) * dict->num_buckets);
    }
}

cnwn_Dict * cnwn_dict_new(int element_size, int num_buckets, cnwn_HashFunction32 hash_function, const cnwn_ContainerCallbacks * cb)
{
    cnwn_Dict * ret = malloc(sizeof(cnwn_Dict));
    cnwn_dict_init(ret, element_size, num_buckets, hash_function, cb);
    return ret;
}

void cnwn_dict_deinit(cnwn_Dict * dict)
{
    if (dict->buckets != NULL) {
        for (int i = 0; i < dict->num_buckets; i++) {
            if (dict->buckets[i].data != NULL) {
                if (dict->cb.deinit_elements != NULL)
                    dict->cb.deinit_elements(dict->buckets[i].data, dict->buckets[i].length);
                free(dict->buckets[i].data);
            }
            if (dict->buckets[i].keys != NULL) {
                for (int j = 0; j < dict->buckets[i].length; j++)
                    free(dict->buckets[i].keys[j]);
                free(dict->buckets[i].keys);
            }
        }
        free(dict->buckets);
    }
}

void cnwn_dict_free(cnwn_Dict * dict)
{
    cnwn_dict_deinit(dict);
    free(dict);
}

void cnwn_dict_clear(cnwn_Dict * dict)
{
    if (dict->buckets != NULL) {
        for (int i = 0; i < dict->num_buckets; i++) {
            if (dict->buckets[i].data != NULL) {
                if (dict->cb.deinit_elements != NULL)
                    dict->cb.deinit_elements(dict->buckets[i].data, dict->buckets[i].length);
                free(dict->buckets[i].data);
            }
            if (dict->buckets[i].keys != NULL) {
                for (int j = 0; j < dict->buckets[i].length; j++)
                    free(dict->buckets[i].keys[j]);
                free(dict->buckets[i].keys);
            }
        }
        memset(dict->buckets, 0, sizeof(cnwn_DictBucket) * dict->num_buckets);
    }
    dict->length = 0;
}

int cnwn_dict_get_length(const cnwn_Dict * dict)
{
    return dict->length;
}

bool cnwn_dict_get_indices(const cnwn_Dict * dict, const char * key, int * ret_bucket_index, int * ret_element_index)
{
    if (dict->element_size > 0 && dict->buckets != NULL) {
        int size = cnwn_strlen(key);
        uint32_t hash = (dict->hash_function != NULL ? dict->hash_function(key, size) : CNWN_DICT_DEFAULT_HASH(key, size));
        int bucket_index = hash % dict->num_buckets;
        if (dict->buckets[bucket_index].length == 1) {
            if (ret_bucket_index != NULL)
                *ret_bucket_index = bucket_index;
            if (ret_element_index != NULL)
                *ret_element_index = 0;
            return true;
        }
        if (dict->buckets[bucket_index].length > 1) {
            for (int i = 0; i < dict->buckets[bucket_index].length; i++) {
                if (cnwn_strcmp(key, dict->buckets[bucket_index].keys[i]) == 0) {
                    if (ret_bucket_index != NULL)
                        *ret_bucket_index = bucket_index;
                    if (ret_element_index != NULL)
                        *ret_element_index = i;
                    return true;
                }
            }
        }
        if (ret_bucket_index != NULL)
            *ret_bucket_index = bucket_index;
        if (ret_element_index != NULL)
            *ret_element_index = -1;
        return true;
    }
    return false;
}

void * cnwn_dict_element_ptr(const cnwn_Dict * dict, const char * key)
{
    if (dict->element_size > 0 && dict->length > 0) {
        int bucket_index = -1;
        int element_index = -1;
        if (cnwn_dict_get_indices(dict, key, &bucket_index, &element_index) && bucket_index >= 0 && element_index >= 0)
            return ((uint8_t *)dict->buckets[bucket_index].data) + dict->element_size * element_index;
    }
    return NULL;
}

bool cnwn_dict_has(const cnwn_Dict * dict, const char * key)
{
    if (dict->element_size > 0 && dict->length > 0) {
        int bucket_index = -1;
        int element_index = -1;
        if (cnwn_dict_get_indices(dict, key, &bucket_index, &element_index) && bucket_index >= 0 && element_index >= 0)
            return true;
    }
    return false;
}

int cnwn_dict_get(const cnwn_Dict * dict, const char * key, void * ret_element)
{
    if (dict->element_size > 0 && dict->length > 0) {
        int bucket_index = -1;
        int element_index = -1;
        if (cnwn_dict_get_indices(dict, key, &bucket_index, &element_index) && bucket_index >= 0 && element_index >= 0) {
            if (ret_element != NULL)
                memcpy(ret_element, ((const uint8_t *)dict->buckets[bucket_index].data) + dict->element_size * element_index, dict->element_size);
            return 1;
        }
    }
    return 0;
}

void * cnwn_dict_get_ptr(const cnwn_Dict * dict, const char * key)
{
    if (dict->element_size > 0 && dict->length > 0) {
        int bucket_index = -1;
        int element_index = -1;
        if (cnwn_dict_get_indices(dict, key, &bucket_index, &element_index) && bucket_index >= 0 && element_index >= 0) 
            return ((uint8_t *)dict->buckets[bucket_index].data) + element_index * dict->element_size;
    }
    return NULL;
}

int cnwn_dict_set(cnwn_Dict * dict, const char * key, const void * element)
{
    if (dict->element_size > 0) {
        int bucket_index = -1;
        int element_index = -1;
        if (cnwn_dict_get_indices(dict, key, &bucket_index, &element_index) && bucket_index >= 0 && element_index >= 0) {
            char * tmp = NULL;
            if (dict->cb.deinit_elements != NULL) {
                malloc(dict->element_size);
                memcpy(tmp, ((const uint8_t *)dict->buckets[bucket_index].data) + dict->element_size * element_index, dict->element_size);
            }
            if (dict->cb.init_elements != NULL) 
                dict->cb.init_elements(((uint8_t *)dict->buckets[bucket_index].data) + dict->element_size * element_index, 1, element);
            else if (element != NULL)
                memcpy(((uint8_t *)dict->buckets[bucket_index].data) + dict->element_size * element_index, element, dict->element_size);
            else
                memset(((uint8_t *)dict->buckets[bucket_index].data) + dict->element_size * element_index, 0, dict->element_size);
            if (tmp != NULL) {
                dict->cb.deinit_elements(tmp, 1);
                free(tmp);
            }
            return 1;
        }
        if (bucket_index >= 0) {
            dict->buckets[bucket_index].data = realloc(dict->buckets[bucket_index].data, dict->element_size * (dict->buckets[bucket_index].length + 1));
            if (dict->cb.init_elements != NULL)
                dict->cb.init_elements(((uint8_t *)dict->buckets[bucket_index].data) + dict->element_size * dict->buckets[bucket_index].length, 1, element);
            else if (element != NULL)
                memcpy(((uint8_t *)dict->buckets[bucket_index].data) + dict->element_size * dict->buckets[bucket_index].length, element, dict->element_size);
            else 
                memset(((uint8_t *)dict->buckets[bucket_index].data) + dict->element_size * dict->buckets[bucket_index].length, 0, dict->element_size);
            dict->buckets[bucket_index].keys = realloc(dict->buckets[bucket_index].keys, sizeof(char *) * (dict->buckets[bucket_index].length + 1));
            dict->buckets[bucket_index].keys[dict->buckets[bucket_index].length] = cnwn_strdup(key != NULL ? key : "");
            dict->buckets[bucket_index].length++;
            dict->length++;
            return 1;
        }
    }
    return 0;
}

int cnwn_dict_remove(cnwn_Dict * dict, const char * key)
{
    if (dict->element_size > 0 && dict->length > 0) {
        int bucket_index = -1;
        int element_index = -1;
        if (cnwn_dict_get_indices(dict, key, &bucket_index, &element_index) && bucket_index >= 0 && element_index >= 0) {
            if (dict->cb.deinit_elements != NULL)
                dict->cb.deinit_elements(((uint8_t *)dict->buckets[bucket_index].data) + dict->element_size * element_index, 1);
            if (dict->buckets[bucket_index].length > 1) {
                if (dict->buckets[bucket_index].keys[element_index] != NULL)
                    free(dict->buckets[bucket_index].keys[element_index]);
                if (element_index < dict->buckets[bucket_index].length - 1) {
                    memmove(((uint8_t *)dict->buckets[bucket_index].data) + dict->element_size * element_index,
                            ((uint8_t *)dict->buckets[bucket_index].data) + dict->element_size * (element_index + 1),
                            dict->element_size);
                    memmove(dict->buckets[bucket_index].keys + element_index, dict->buckets[bucket_index].keys + element_index + 1, sizeof(char *));
                }
                dict->buckets[bucket_index].data = realloc(dict->buckets[bucket_index].data, dict->element_size * (dict->buckets[bucket_index].length - 1));
                dict->buckets[bucket_index].keys = realloc(dict->buckets[bucket_index].keys, sizeof(char *) * (dict->buckets[bucket_index].length - 1));
                dict->buckets[bucket_index].length--;
            } else {
                free(dict->buckets[bucket_index].data);
                free(dict->buckets[bucket_index].keys[0]);
                free(dict->buckets[bucket_index].keys);
                dict->buckets[bucket_index].data = NULL;
                dict->buckets[bucket_index].keys = NULL;
                dict->buckets[bucket_index].length = 0;
            }
            dict->length--;
            return 1;
        }
    }
    return 0;
}

const char * cnwn_dict_find(const cnwn_Dict * dict, const void * element)
{
    if (dict->element_size > 0 && dict->length > 0) {
        for (int i = 0; i < dict->num_buckets; i++) {
            for (int j = 0; j < dict->buckets[i].length; j++) {
                if (dict->cb.compare_elements != NULL) {
                    if (dict->cb.compare_elements(element, ((const uint8_t *)dict->buckets[i].data) + dict->element_size * j) == 0)
                        return dict->buckets[i].keys[j];
                } else if (element == ((const uint8_t *)dict->buckets[i].data) + dict->element_size * j)
                    return dict->buckets[i].keys[j];
            }
        }
    }
    return NULL;
}

int cnwn_dict_merge(cnwn_Dict * dict, const cnwn_Dict * other, bool override)
{
    int ret = 0;
    if (dict->element_size > 0 && dict->element_size == other->element_size) {
        for (int i = 0; i < other->num_buckets; i++) {
            for (int j = 0; j < other->buckets[i].length; j++) {
                void * eptr = cnwn_dict_element_ptr(dict, other->buckets[i].keys[j]);
                if (eptr == NULL || override)
                    ret += cnwn_dict_set(dict, other->buckets[i].keys[j], ((const uint8_t *)other->buckets[i].data) + other->element_size * j);
            }
        }
    }
    return ret;
}

////////////////////////////////////////////////////////////////
//
//
// Queue
//
//
////////////////////////////////////////////////////////////////

void cnwn_queue_init(cnwn_Queue * queue, int element_size, int max_length, const cnwn_ContainerCallbacks * cb)
{
    memset(queue, 0, sizeof(cnwn_Queue));
    queue->element_size = CNWN_MAX(element_size, 0);
    queue->max_length = CNWN_MAX(max_length, 0);
    if (queue->max_length > 0 && queue->element_size > 0) {
        queue->data = malloc(queue->element_size * queue->max_length);
        memset(queue->data, 0, queue->element_size * queue->max_length);
    }
}

cnwn_Queue * cnwn_queue_new(int element_size, int max_length, const cnwn_ContainerCallbacks * cb)
{
    cnwn_Queue * ret = malloc(sizeof(cnwn_Queue));
    cnwn_queue_init(ret, element_size, max_length, cb);
    return ret;
}

void cnwn_queue_deinit(cnwn_Queue * queue)
{
    if (queue->data != NULL) {
        if (queue->cb.deinit_elements != NULL)
            queue->cb.deinit_elements(queue->data, queue->length);
        free(queue->data);
    }
}

void cnwn_queue_free(cnwn_Queue * queue)
{
    cnwn_queue_deinit(queue);
    free(queue);
}

void cnwn_queue_clear(cnwn_Queue * queue)
{
    if (queue->data != NULL) {
        if (queue->cb.deinit_elements != NULL) {
            for (int i = 0; i < queue->length; i++) {
                int index = (i + queue->tail) % queue->max_length;
                queue->cb.deinit_elements(((uint8_t *)queue->data) + queue->element_size * index, 1);
            }
        }
    }
    queue->length = 0;
    queue->tail = 0;
}

int cnwn_queue_get_length(const cnwn_Queue * queue)
{
    return queue->length;
}
        
void * cnwn_queue_element_ptr(const cnwn_Queue * queue, int index)
{
    if (queue->element_size > 0 && queue->length > 0) {
        if (index < 0)
            index += queue->length;
        if (index >= 0 && index < queue->length) {
            int tindex = (index + queue->tail) % queue->max_length;
            return ((uint8_t *)queue->data) + queue->element_size * tindex;
        }
    }
    return NULL;
}

int cnwn_queue_get(const cnwn_Queue * queue, int index, int length, void * ret_elements)
{
    if (queue->element_size > 0 && queue->length > 0) {
        if (index < 0)
            index += queue->length;
        if (index >= 0 && index < queue->length) {
            if (index + length > queue->length)
                length = queue->length - index;
            int tindex = (index + queue->tail) % queue->max_length;
            if (ret_elements != NULL)
                memcpy(ret_elements, ((const uint8_t *)queue->data) + queue->element_size * tindex, queue->element_size * length);
            return length;
        }
    }
    return 0;
}

void * cnwn_queue_get_ptr(const cnwn_Queue * queue, int index)
{
    if (queue->element_size > 0) {
        if (index < 0)
            index += queue->length;
        if (index >= 0 && index < queue->length) {
            int tindex = (index + queue->tail) % queue->max_length;
            return ((uint8_t *)queue->data) + tindex * queue->element_size;
        }
    }
    return NULL;
}

int cnwn_queue_set(cnwn_Queue * queue, int index, int length, const void * elements)
{
    if (queue->element_size > 0 && queue->length > 0) {
        if (index < 0)
            index += queue->length;
        if (index >= 0 && index < queue->length) {
            if (index + length > queue->length)
                length = queue->length - index;
            int tindex = (index + queue->tail) % queue->max_length;
            void * tmp = NULL;
            if (queue->cb.deinit_elements != NULL) {
                tmp = malloc(queue->element_size * length);
                for (int i = 0; i < length; i++)
                    memcpy(((uint8_t *)tmp) + queue->element_size * i,
                           ((const uint8_t *)queue->data) + queue->element_size * ((tindex + i) % queue->max_length),
                           queue->element_size);
            }
            if (queue->cb.init_elements != NULL)
                for (int i = 0; i < length; i++)
                    queue->cb.init_elements(((uint8_t *)queue->data) + queue->element_size * ((tindex + i) % queue->max_length),
                                            1,
                                            ((const uint8_t *)elements) + queue->element_size * i);
            else if (elements != NULL)
                memcpy(((uint8_t *)queue->data) + queue->element_size * tindex, elements, queue->element_size * length);
            else
                memset(((uint8_t *)queue->data) + queue->element_size * tindex, 0, queue->element_size * length);
            if (tmp != NULL) {
                for (int i = 0; i < length; i++)
                    queue->cb.deinit_elements(((uint8_t *)tmp) + queue->element_size * i, 1);
                free(tmp);
            }
            return length;
        }
    }
    return 0;
}

int cnwn_queue_insert(cnwn_Queue * queue, int index, int length, const void * elements);
int cnwn_queue_append(cnwn_Queue * queue, int length, const void * elements);
int cnwn_queue_remove(cnwn_Queue * queue, int index, int length);
int cnwn_queue_push(cnwn_Queue * queue, int length, const void * elements);
int cnwn_queue_pop(cnwn_Queue * queue, int index, int length, void * ret_elements);

////////////////////////////////////////////////////////////////
//
//
// Special array implementations.
//
//
////////////////////////////////////////////////////////////////

void cnwn_string_array_init(cnwn_StringArray * array)
{
    cnwn_array_init(array, sizeof(char *), &CNWN_STRING_ARRAY_FUNCTIONS);
}

cnwn_StringArray * cnwn_string_array_new(void)
{
    return cnwn_array_new(sizeof(char *), &CNWN_STRING_ARRAY_FUNCTIONS);
}

const char * cnwn_string_array_get(const cnwn_StringArray * array, int index)
{
    return cnwn_array_get_ptr(array, index);
}

int cnwn_string_array_set_va(cnwn_StringArray * array, int index, const char * format, va_list args)
{
    if (array->element_size == sizeof(char *)) {
        index = CNWN_WRAP_INDEX(index, array->length);
        if (index >= 0 && index < array->length && cnwn_array_set(array, index, 1, NULL) > 0) {
            ((char **)array->data)[index] = cnwn_strformat_va(format, args);
            return 1;
        }
    }
    return 0;
}

int cnwn_string_array_set(cnwn_StringArray * array, int index, const char * format, ...)
{
    va_list args;
    va_start(args, format);
    int ret = cnwn_string_array_set_va(array, index, format, args);
    va_end(args);
    return ret;
}

int cnwn_string_array_insert_va(cnwn_StringArray * array, int index, const char * format, va_list args)
{
    if (array->element_size == sizeof(char *)) {
        index = CNWN_WRAP_INDEX(index, array->length);
        if (index >= 0 && index < array->length && cnwn_array_insert(array, index, 1, NULL) > 0) {
            ((char **)array->data)[index] = cnwn_strformat_va(format, args);
            return 1;
        }
    }
    return 0;
}

int cnwn_string_array_insert(cnwn_StringArray * array, int index, const char * format, ...)
{
    va_list args;
    va_start(args, format);
    int ret = cnwn_string_array_insert_va(array, index, format, args);
    va_end(args);
    return ret;
}

int cnwn_string_array_append_va(cnwn_StringArray * array, const char * format, va_list args)
{
    if (array->element_size == sizeof(char *)) {
        if (cnwn_array_append(array, 1, NULL) > 0) {
            ((char **)array->data)[array->length - 1] = cnwn_strformat_va(format, args);
            return 1;
        }
    }
    return 0;
}

int cnwn_string_array_append(cnwn_StringArray * array, const char * format, ...)
{
    va_list args;
    va_start(args, format);
    int ret = cnwn_string_array_append_va(array, format, args);
    va_end(args);
    return ret;
}

int cnwn_string_array_split(cnwn_StringArray * array, const char * s, int max_splits, const char * splitstr, const char * escstr)
{
    int count = 0;
    if (array->element_size == sizeof(char *)) {
        if (max_splits == 0) 
            return cnwn_array_append(array, 1, s);
        int splitstrsize = cnwn_strlen(s);
        if (splitstrsize == 0)
            return cnwn_array_append(array, 1, s);
        int lastfind = 0;
        int find = cnwn_strfind(s, 0, splitstr, escstr);
        while (find >= 0 && (count < max_splits || max_splits < 0)) {
            char * tmp = cnwn_strndup2(s, find, find - lastfind);
            count += cnwn_string_array_append(array, tmp);
            free(tmp);
            lastfind = find + splitstrsize;
            find = cnwn_strfind(s, lastfind, splitstr, escstr);
        }
    }
    return count;
}

int cnwn_string_array_spliti(cnwn_StringArray * array, const char * s, int max_splits, const char * splitstr, const char * escstr);

int cnwn_string_array_join(const cnwn_StringArray * array, int index, int length, const char * sepstr, int max_size, char * ret_s)
{
    int offset = 0;
    if (array->element_size == sizeof(char *)) {
        if (index < 0)
            index += array->length;
        if (index >= 0 && index < array->length) {
            if (index + length > array->length)
                length = array->length - index;
            int sepsize = cnwn_strlen(sepstr);
            for (int i = index; i < index + length; i++) {
                const char * s = cnwn_string_array_get(array, i);
                int ssize = cnwn_strlen(s);
                if (max_size > 0) {
                    int copylen = CNWN_MIN(ssize, max_size - offset - 1);
                    if (copylen <= 0)
                        break;
                    if (ret_s != NULL && copylen > 0)
                        memcpy(ret_s + offset, s, sizeof(char) * copylen);
                    offset += copylen;
                    if (sepsize > 0 && i > 0 && i < array->length - 1) {
                        copylen = CNWN_MIN(sepsize, max_size - offset - 1);
                        if (copylen <= 0)
                            break;
                        if (ret_s != NULL && copylen > 0)
                            memcpy(ret_s + offset, sepstr, sizeof(char) * copylen);
                        offset += copylen;
                    }
                } else 
                    offset += ssize;
            }
        }
    }
    if (ret_s != NULL && max_size > 0)
        ret_s[offset] = 0;
    return offset;
}

char * cnwn_string_array_join2(const cnwn_StringArray * array, const char * sepstr)
{
    int size = cnwn_string_array_join(array, 0, array->length, sepstr, 0, NULL);
    char * ret = malloc(sizeof(char) * (size + 1));
    cnwn_string_array_join(array, 0, array->length, sepstr, size + 1, ret);
    return ret;
}

int cnwn_string_array_each_insert_va(cnwn_StringArray * array, int index, const char * format, va_list args)
{
    if (array->element_size == sizeof(char *)) {
        for (int i = 0; i < array->length; i++) 
            ((char **)array->data)[i] = cnwn_strinsert_realloc_va(((char **)array->data)[i], index, format, args);
        return array->length;
    }
    return 0;
}

int cnwn_string_array_each_insert(cnwn_StringArray * array, int index, const char * format, ...)
{
    va_list args;
    va_start(args, format);
    int ret = cnwn_string_array_each_insert_va(array, index, format, args);
    va_end(args);
    return ret;
}

int cnwn_string_array_each_append_va(cnwn_StringArray * array, const char * format, va_list args)
{
    if (array->element_size == sizeof(char *)) {
        for (int i = 0; i < array->length; i++) 
            ((char **)array->data)[i] = cnwn_strappend_realloc_va(((char **)array->data)[i], format, args);
        return array->length;
    }
    return 0;
}

int cnwn_string_array_each_append(cnwn_StringArray * array, const char * format, ...)
{
    va_list args;
    va_start(args, format);
    int ret = cnwn_string_array_each_append_va(array, format, args);
    va_end(args);
    return ret;
}
