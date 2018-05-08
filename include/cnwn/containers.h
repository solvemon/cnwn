/**
 * @file containers.h
 * Part of cnwn: Small C99 library and tools for Neverwinter Nights.
 */
#ifndef CNWN_CONTAINERS_H
#define CNWN_CONTAINERS_H

#include "cnwn/common.h"
#include "cnwn/string.h"
#include "cnwn/hash.h"

/**
 * The default hash function for dicts.
 */
#define CNWN_DICT_DEFAULT_HASH cnwn_hash32_murmur3

/**
 * Wrap an index.
 * @param index The index.
 * @param length The length to wrap for.
 * @returns A wrapped index, may be out of range.
 */
#define CNWN_WRAP_INDEX(index, length) ((index) < 0 ? (index) + (length) : (index))

/**
 * @see struct cnwn_ContainerCallbacks_s
 */
typedef struct cnwn_ContainerCallbacks_s cnwn_ContainerCallbacks;

/**
 * @see struct cnwn_Array_s
 */
typedef struct cnwn_Array_s cnwn_Array;

/**
 * Used for code clarity.
 */
typedef cnwn_Array cnwn_StringArray;

/**
 * @see struct cnwn_DictBucket_s
 */
typedef struct cnwn_DictBucket_s cnwn_DictBucket;

/**
 * @see struct cnwn_Dict_s
 */
typedef struct cnwn_Dict_s cnwn_Dict;

/**
 * @see struct cnwn_Queue_s
 */
typedef struct cnwn_Queue_s cnwn_Queue;

/**
 * Used as a callback for containers to initialize a number of elements.
 * @param elements The elements memory to initialize.
 * @param length The number of elements to initialize.
 * @param source The source data to initialize elements from, may differ for different implementations.
 * @note All implementations must be ready to accept @p source as NULL.
 */
typedef void (*cnwn_ContainerInitElements)(void * elements, int length, const void * source);

/**
 * Used as a callback for containers to deinitialize a number of elements.
 * @param elements The elements memory to initialize.
 * @param length The number of elements to initialize.
 */
typedef void (*cnwn_ContainerDeinitElements)(void * elements, int length);

/**
 * Used as a callback for containers to compare two elements of the same type.
 * @param element The element to compare for.
 * @param compare The element to compare with.
 * @returns Should return -1 if @p element < @p compare, 1 if @p element > @p compare and 0 if they are equal.
 */
typedef int (*cnwn_ContainerCompareElements)(const void * element, const void * compare);


/**
 * Used to implement special behavior when initializing, deinitializing and comparing elements in a container.
 */
struct cnwn_ContainerCallbacks_s {

    /**
     * Callback for initializing elements.
     */
    cnwn_ContainerInitElements init_elements;

    /**
     * Callback for deinitializing elements.
     */
    cnwn_ContainerDeinitElements deinit_elements;

    /**
     * Callback for comparing elements.
     */
    cnwn_ContainerCompareElements compare_elements;
};

/**
 * An array.
 */
struct cnwn_Array_s {

    /**
     * The size of each element.
     */
    int element_size;
    
    /**
     * The length of the array.
     */
    int length;

    /**
     * The elements.
     */
    void * data;

    /**
     * Callbacks.
     */
    cnwn_ContainerCallbacks cb;
};

/**
 * A ditionary (hash map) bucket.
 */
struct cnwn_DictBucket_s {

    /**
     * The length of the dictionary.
     */
    int length;

    /**
     * The keys for each individual element in the bucket.
     */
    char ** keys;

    /**
     * The elements.
     */
    void * data;
};

/**
 * A ditionary (hash map).
 */
struct cnwn_Dict_s {
    
    /**
     * The size of each element.
     */
    int element_size;

    /**
     * A hash implementation.
     */
    cnwn_HashFunction32 hash_function;
    
    /**
     * The length (number of key/element pairs) of the dictionary.
     */
    int length;

    /**
     * The number of allocated buckets.
     */
    int num_buckets;

    /**
     * The buckets.
     */
    cnwn_DictBucket * buckets;

    /**
     * Callbacks.
     */
    cnwn_ContainerCallbacks cb;
};

/**
 * A queue.
 */
struct cnwn_Queue_s {

    /**
     * The size of each element.
     */
    int element_size;

    /**
     * The maximum length.
     */
    int max_length;

    /**
     * The length of pending elements.
     */
    int length;
    
    /**
     * The current tail index of the queue.
     */
    int tail;
    
    /**
     * The elements.
     */
    void * data;

    /**
     * Callbacks.
     */
    cnwn_ContainerCallbacks cb;
};

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Callback implementations for a string array.
 * @see cnwn_StringArray
 */
extern CNWN_PUBLIC const cnwn_ContainerCallbacks CNWN_STRING_ARRAY_FUNCTIONS;

////////////////////////////////////////////////////////////////
//
//
// Array
//
//
////////////////////////////////////////////////////////////////


/**
 * Initialize an array.
 * @param array The array struct to initialize.
 * @param element_size The size (in bytes) of each element.
 * @param cb Callbacks, NULL for no callbacks.
 */
extern CNWN_PUBLIC void cnwn_array_init(cnwn_Array * array, int element_size, const cnwn_ContainerCallbacks * cb);

/**
 * Create a new array.
 * @param element_size The size (in bytes) of each element.
 * @param cb Callbacks, NULL for no callbacks.
 * @returns The newly created array.
 */
extern CNWN_PUBLIC cnwn_Array * cnwn_array_new(int element_size, const cnwn_ContainerCallbacks * cb);

/**
 * Initialize an array as a clone of another array.
 * @param array The array struct to initialize as a clone.
 * @param source The array to clone.
 */
extern CNWN_PUBLIC void cnwn_array_init_clone(cnwn_Array * array, const cnwn_Array * source);

/**
 * Create a new array as a clone of another array.
 * @param source The array to clone.
 * @returns The newly created array clone.
 */
extern CNWN_PUBLIC cnwn_Array * cnwn_array_new_clone(const cnwn_Array * source);

/**
 * Deinitialize an array.
 * @param array The array to deinitialize.
 */
extern CNWN_PUBLIC void cnwn_array_deinit(cnwn_Array * array);

/**
 * Deinitialize and free an array.
 * @param array The array to deinitialize and free.
 */
extern CNWN_PUBLIC void cnwn_array_free(cnwn_Array * array);

/**
 * Remove all elements in the array.
 * @param array The array to remove all elements for.
 * @note Will trigger callback cnwn_ContainerDeinitElements() for all the elements.
 */
extern CNWN_PUBLIC void cnwn_array_clear(cnwn_Array * array);

/**
 * Get the length of an array.
 * @param array The array to get the length for.
 * @returns The number of elements in the array.
 */
extern CNWN_PUBLIC int cnwn_array_get_length(const cnwn_Array * array);

/**
 * Set the length of an array.
 * @param array The array to set the length for.
 * @param length The length to set.
 * @note Will trigger callback cnwn_ContainerInitElements() with a NULL source for new elements and cnwn_ContainerDeinitElements() for truncated elements.
 */
extern CNWN_PUBLIC void cnwn_array_set_length(cnwn_Array * array, int length);

/**
 * Get the pointer to an element.
 * @param array The array to get the element pointer from.
 * @param index The index of the element to get the pointer for, negative values will wrap from the end of the array.
 * @returns The pointer to an element or NULL if @p index is out of range or if the array has an invalid element size.
 */
extern CNWN_PUBLIC void * cnwn_array_element_ptr(const cnwn_Array * array, int index);

/**
 * Get a number of elements from the array.
 * @param array The array to get elements from.
 * @param index The index to start the get from, negative values will wrap from the end of the array.
 * @param length The number of elements to get.
 * @param[out] ret_elements Copy the elements to this memory, NULL to omit.
 * @returns The number of returned elements.
 */
extern CNWN_PUBLIC int cnwn_array_get(const cnwn_Array * array, int index, int length, void * ret_elements);

/**
 * Get an element of pointer type.
 * @param array The array to get element from.
 * @param index The index to get the element from, negative values will wrap from the end of the array.
 * @returns The pointer element or NULL if @p index is out of range.
 * @note Undefined consequences if you try to do this with non-pointer elements!
 */
extern CNWN_PUBLIC void * cnwn_array_get_ptr(const cnwn_Array * array, int index);

/**
 * Set a number of elements for the array.
 * @param array The array to set elements for.
 * @param index The index to start the set from, negative values will wrap from the end of the array.
 * @param length The number of elements to set.
 * @param elements Copy the elements from this memory, NULL to omit.
 * @returns The number of set elements.
 * @note Will trigger callback cnwn_ContainerInitElements() for new elements and cnwn_ContainerDeinitElements() for the overwritten elements.
 * @note The cnwn_ContainerDeinitElements() will be called AFTER cnwn_ContainerInitElements(), so it's safe if you're refcounting etc.
 */
extern CNWN_PUBLIC int cnwn_array_set(cnwn_Array * array, int index, int length, const void * elements);

/**
 * Insert a number of elements into the array.
 * @param array The array to insert elements for.
 * @param index The index to insert at, negative values will wrap from the end of the array.
 * @param length The number of elements to insert.
 * @param elements Copy the elements from this memory, NULL to omit.
 * @returns The number of inserted elements.
 * @note If the wrapped @p index is < 0 the element will be inserted at 0, if >= length it will be appended to the end.
 * @note Will trigger callback cnwn_ContainerInitElements() for the inserted elements.
 */
extern CNWN_PUBLIC int cnwn_array_insert(cnwn_Array * array, int index, int length, const void * elements);

/**
 * Append a number of elements to the array.
 * @param array The array to append elements for.
 * @param length The number of elements to append.
 * @param elements Copy the elements from this memory, NULL to omit.
 * @returns The number of appended elements.
 * @note Will trigger callback cnwn_ContainerInitElements() for the appended elements.
 */
extern CNWN_PUBLIC int cnwn_array_append(cnwn_Array * array, int length, const void * elements);

/**
 * Remove a number of elements from the array.
 * @param array The array to remove elements from.
 * @param index The index of where to start the removal, negative values will wrap from the end of the array.
 * @param length The number of elements to remove.
 * @returns The number of removed elements.
 * @note Will trigger callback cnwn_ContainerDeinitElements() for the removed elements.
 */
extern CNWN_PUBLIC int cnwn_array_remove(cnwn_Array * array, int index, int length);

/**
 * Swap the positions of two elements.
 * @param array The array to swap elements for.
 * @param index_a The index of an element to swap, negative values will wrap from the end of the array.
 * @param index_b The index of an element to swap, negative values will wrap from the end of the array.
 * @returns True if the elements were swapped, false if either @p index_a or @p index_b are out of range.
 */
extern CNWN_PUBLIC bool cnwn_array_swap(cnwn_Array * array, int index_a, int index_b);

/**
 * Iterate the array in search of an element.
 * @param array The array to find an element in.
 * @param index The index of where to start searching, negative values will wrap from the end of the array.
 * @param reverse True to reverse the search direction.
 * @param element The element to search for.
 * @returns The index of the found element or -1 if the element was not found.
 * @note Will trigger callback cnwn_ContainerCompareElements() when searching.
 * @note If there is no cnwn_ContainerCompareElements() implementation a simple pointer comparison is made.
 */
extern CNWN_PUBLIC int cnwn_array_find(const cnwn_Array * array, int index, bool reverse, const void * element);

/**
 * Sort the array.
 * @param array The array to sort.
 * @param reverse True to reverse the sort order (descending order), false if not (ascending order).
 * @note Will trigger callback cnwn_ContainerCompareElements() when sorting.
 * @note If there is no cnwn_ContainerCompareElements() implementation there will be no sorting.
 */
extern CNWN_PUBLIC void cnwn_array_sort(cnwn_Array * array, bool reverse);

/**
 * Set elements from a slice of another array.
 * @param array The array to set elements for.
 * @param index The index of where to start setting elements, negative values will wrap from the end of the array.
 * @param length The number of elements to set.
 * @param slice The array to get a slice from.
 * @param slice_index The index of where to start getting elements from the slice array, negative values will wrap from the end of the slice array.
 * @returns The number of set elements.
 * @note This only works if both arrays have the same element size, otherwise the function simply returns 0.
 */
extern CNWN_PUBLIC int cnwn_array_set_slice(cnwn_Array * array, int index, int length, const cnwn_Array * slice, int slice_index);

/**
 * Insert elements from a slice of another array.
 * @param array The array to insert elements for.
 * @param index The index of where to start inserting elements, negative values will wrap from the end of the array.
 * @param length The number of elements to insert.
 * @param slice The array to get a slice from.
 * @param slice_index The index of where to start getting elements from the slice array, negative values will wrap from the end of the slice array.
 * @returns The number of inserted elements.
 * @note This only works if both arrays have the same element size, otherwise the function simply returns 0.
 */
extern CNWN_PUBLIC int cnwn_array_insert_slice(cnwn_Array * array, int index, int length, const cnwn_Array * slice, int slice_index);

/**
 * Append elements from a slice of another array.
 * @param array The array to append elements for.
 * @param length The number of elements to append.
 * @param slice The array to get a slice from.
 * @param slice_index The index of where to start getting elements from the slice array, negative values will wrap from the end of the slice array.
 * @returns The number of appended elements.
 * @note This only works if both arrays have the same element size, otherwise the function simply returns 0.
 */
extern CNWN_PUBLIC int cnwn_array_append_slice(cnwn_Array * array, int length, const cnwn_Array * slice, int slice_index);

/**
 * Reverse the order of some elements.
 * @param array The array to reverse.
 * @param index Where to start reversing, negative values will wrap from the end of the array.
 * @param length The number of elements to reverse.
 * @returns The number of reversed elements.
 */
extern CNWN_PUBLIC int cnwn_array_reverse(cnwn_Array * array, int index, int length);

/**
 * Reverse the order of all the elements.
 * @param array The array to reverse.
 */
extern CNWN_PUBLIC void cnwn_array_reverse2(cnwn_Array * array);

////////////////////////////////////////////////////////////////
//
//
// Dict
//
//
////////////////////////////////////////////////////////////////

/**
 * Initialize a dict.
 * @param dict The dict struct to initialize.
 * @param element_size The size (in bytes) of each element.
 * @param num_buckets The number of buckets to allocate for the dict.
 * @param hash_function The hash function to use, NULL will use CNWN_DICT_DEFAULT_HASH.
 * @param cb Callbacks, NULL for no callbacks.
 */
extern CNWN_PUBLIC void cnwn_dict_init(cnwn_Dict * dict, int element_size, int num_buckets, cnwn_HashFunction32 hash_function, const cnwn_ContainerCallbacks * cb);

/**
 * Create a new dict.
 * @param element_size The size (in bytes) of each element.
 * @param num_buckets The number of buckets to allocate for the dict.
 * @param hash_function The hash function to use, NULL will use CNWN_DICT_DEFAULT_HASH.
 * @param cb Callbacks, NULL for no callbacks.
 * @returns The new dict.
 */
extern CNWN_PUBLIC cnwn_Dict * cnwn_dict_new(int element_size, int num_buckets, cnwn_HashFunction32 hash_function, const cnwn_ContainerCallbacks * cb);

/**
 * Deinitialize a dict.
 * @param dict The dict to deinitialize.
 */
extern CNWN_PUBLIC void cnwn_dict_deinit(cnwn_Dict * dict);

/**
 * Deinitialize a dict and free.
 * @param dict The dict to deinitialize and free.
 */
extern CNWN_PUBLIC void cnwn_dict_free(cnwn_Dict * dict);

/**
 * Remove all elements in the dict.
 * @param dict The dict to remove all elements for.
 * @note Will trigger callback cnwn_ContainerDeinitElements() for all the elements.
 */
extern CNWN_PUBLIC void cnwn_dict_clear(cnwn_Dict * dict);

/**
 * Get the length of a dict.
 * @param dict The dict to get the length for.
 * @returns The number of elements in the dict.
 */
extern CNWN_PUBLIC int cnwn_dict_get_length(const cnwn_Dict * dict);

/**
 * Get the bucket and element index for a key.
 * @param dict The dict to get the indices for.
 * @param key The key of the indices to get.
 * @param[out] ret_bucket_index The bucket index.
 * @param[out] ret_element_index The element index, can be -1 if the bucket is empty.
 * @returns True if indices were returned, false if @p key was not found.
 */
extern CNWN_PUBLIC bool cnwn_dict_get_indices(const cnwn_Dict * dict, const char * key, int * ret_bucket_index, int * ret_element_index);

/**
 * Get the pointer to an element.
 * @param dict The dict to get the element pointer from.
 * @param key The key of the element to get.
 * @returns The pointer to an element or NULL if @p key was not found.
 */
extern CNWN_PUBLIC void * cnwn_dict_element_ptr(const cnwn_Dict * dict, const char * key);

/**
 * Check if a key exists in the dict.
 * @param dict The dict to check for.
 * @param key The key to check.
 * @returns True if the key exists, false if not.
 */
extern CNWN_PUBLIC bool cnwn_dict_has(const cnwn_Dict * dict, const char * key);

/**
 * Get an element from the dict.
 * @param dict The dict to get the element from.
 * @param key The key of the element to get.
 * @param[out] ret_element Copy the element to this memory, NULL to omit.
 * @returns The number of returned key/element pairs (0 means @p key was not found).
 */
extern CNWN_PUBLIC int cnwn_dict_get(const cnwn_Dict * dict, const char * key, void * ret_element);

/**
 * Get an element of pointer type.
 * @param dict The dict to get element from.
 * @param key The key of the element to get as a pointer.
 * @returns The pointer element or NULL if @p key was not found (can also be NULL if the stored pointer is NULL).
 * @note Undefined consequences if you try to do this with non-pointer elements!
 */
extern CNWN_PUBLIC void * cnwn_dict_get_ptr(const cnwn_Dict * dict, const char * key);

/**
 * Set an element from the dict.
 * @param dict The dict to set the element for.
 * @param key The key of the element to set.
 * @param element Copy from this source.
 * @returns The number of set key/element pairs (always 1).
 * @note Will trigger callback cnwn_ContainerInitElements() for the element and cnwn_ContainerDeinitElements() for any overwritten element.
 * @note The cnwn_ContainerDeinitElements() will be called AFTER cnwn_ContainerInitElements(), so it's safe if you're refcounting etc.
 */
extern CNWN_PUBLIC int cnwn_dict_set(cnwn_Dict * dict, const char * key, const void * element);

/**
 * Remove an element from the dict.
 * @param dict The dict to remove the element from.
 * @param key The key of the element to remove.
 * @returns True if the element was removed, false if @p key was not found.
 * @note Will trigger callback cnwn_ContainerInitElements() for the element.
 */
extern CNWN_PUBLIC int cnwn_dict_remove(cnwn_Dict * dict, const char * key);

/**
 * Find an element and return it's key.
 * @param dict The dict to find the element in.
 * @param element The element to find.
 * @returns The key of the element or NULL if @p element was not found in the dict.
 * @note Will trigger callback cnwn_ContainerCompareElements() when searching.
 * @note If there is no cnwn_ContainerCompareElements() implementation a simple pointer comparison is made.
 */
extern CNWN_PUBLIC const char * cnwn_dict_find(const cnwn_Dict * dict, const void * element);

/**
 * Merge with another dict.
 * @param dict The dict to merge for.
 * @param other The dict to merge with.
 * @param override True to allow replace keys in @p dict, false if not.
 * @returns The number of merged key/element pairs.
 * @note Will trigger callback cnwn_ContainerInitElements() for the elements.
 * @note Will trigger callback cnwn_ContainerDeinitElements() for any overridden elements.
 * @note The element size of both dicts must be the same or this function simply returns 0.
 */
extern CNWN_PUBLIC int cnwn_dict_merge(cnwn_Dict * dict, const cnwn_Dict * other, bool override);

////////////////////////////////////////////////////////////////
//
//
// Queue
//
//
////////////////////////////////////////////////////////////////

/**
 * Initialize a queue.
 * @param queue The queue struct to initialize.
 * @param element_size The size (in bytes) of each element.
 * @param max_length The maximum length of the queue.
 * @param cb Callbacks, NULL for no callbacks.
 * @note Pointer elements must have an element size equal to sizeof(void *).
 */
extern CNWN_PUBLIC void cnwn_queue_init(cnwn_Queue * queue, int element_size, int max_length, const cnwn_ContainerCallbacks * cb);

/**
 * Create a new queue.
 * @param element_size The size (in bytes) of each element.
 * @param max_length The maximum length of the queue.
 * @param cb Callbacks, NULL for no callbacks.
 * @returns The new queue.
 * @note Pointer elements must have an element size equal to sizeof(void *).
 */
extern CNWN_PUBLIC cnwn_Queue * cnwn_queue_new(int element_size, int max_length, const cnwn_ContainerCallbacks * cb);

/**
 * Deinitialize a queue.
 * @param queue The queue to deinitialize.
 */
extern CNWN_PUBLIC void cnwn_queue_deinit(cnwn_Queue * queue);

/**
 * Deinitialize and free a queue.
 * @param queue The queue to deinitialize and free.
 */
extern CNWN_PUBLIC void cnwn_queue_free(cnwn_Queue * queue);

/**
 * Remove all elements in the queue.
 * @param queue The queue to remove all elements for.
 * @note Will trigger callback cnwn_ContainerDeinitElements() for all the elements.
 */
extern CNWN_PUBLIC void cnwn_queue_clear(cnwn_Queue * queue);

/**
 * Get the length of a queue.
 * @param queue The queue to get the length for.
 * @returns The number of elements in the queue.
 */
extern CNWN_PUBLIC int cnwn_queue_get_length(const cnwn_Queue * queue);

/**
 * Get the pointer to an element.
 * @param queue The queue to get the element pointer from.
 * @param index The index of the element to get the pointer for, negative values will wrap from the end of the queue.
 * @returns The pointer to an element or NULL if @p index is out of range.
 */
extern CNWN_PUBLIC void * cnwn_queue_element_ptr(const cnwn_Queue * queue, int index);

/**
 * Get a number of elements from the queue (without removing them them).
 * @param queue The queue to get elements from.
 * @param index The index to start the get from, negative values will wrap from the end of the queue.
 * @param length The number of elements to get.
 * @param[out] ret_elements Copy the elements to this memory, NULL to omit.
 * @returns The number of returned elements.
 */
extern CNWN_PUBLIC int cnwn_queue_get(const cnwn_Queue * queue, int index, int length, void * ret_elements);

/**
 * Get an element of pointer type.
 * @param queue The queue to get element from.
 * @param index The index to get the element from, negative values will wrap from the end of the queue.
 * @returns The pointer element or NULL if @p index is out of range.
 * @note Undefined consequences if you try to do this with non-pointer elements!
 * @note The element size of a pointer queue must be exactly that of sizeof(void *).
 */
extern CNWN_PUBLIC void * cnwn_queue_get_ptr(const cnwn_Queue * queue, int index);

/**
 * Set a number of elements for the queue.
 * @param queue The queue to set elements for.
 * @param index The index to start the set from, negative values will wrap from the end of the queue.
 * @param length The number of elements to set.
 * @param elements Copy the elements from this memory, NULL to omit.
 * @returns The number of set elements.
 * @note Will trigger callback cnwn_ContainerInitElements() for new elements and cnwn_ContainerDeinitElements() for overwritten elements.
 * @note The cnwn_ContainerDeinitElements() will be called AFTER cnwn_ContainerInitElements(), so it's safe if you're refcounting etc.
 */
extern CNWN_PUBLIC int cnwn_queue_set(cnwn_Queue * queue, int index, int length, const void * elements);

/**
 * Insert a number of elements into the queue.
 * @param queue The queue to insert elements for.
 * @param index The index to insert at, negative values will wrap from the end of the queue.
 * @param length The number of elements to insert.
 * @param elements Copy the elements from this memory, NULL to omit.
 * @returns The number of inserted elements.
 * @note If the wrapped @p index is < 0 the element will be inserted at 0, if >= length it will be appended to the end.
 * @note Will trigger callback cnwn_ContainerInitElements() for the inserted elements.
 */
extern CNWN_PUBLIC int cnwn_queue_insert(cnwn_Queue * queue, int index, int length, const void * elements);

/**
 * Append a number of elements to the queue.
 * @param queue The queue to append elements for.
 * @param length The number of elements to append.
 * @param elements Copy the elements from this memory, NULL to omit.
 * @returns The number of appended elements.
 * @note Will trigger callback cnwn_ContainerInitElements() for the appended elements.
 * @note Synonymous with cnwn_queue_push().
 */
extern CNWN_PUBLIC int cnwn_queue_append(cnwn_Queue * queue, int length, const void * elements);

/**
 * Remove a number of elements from the queue.
 * @param queue The queue to remove elements from.
 * @param index The index of where to start the removal, negative values will wrap from the end of the queue.
 * @param length The number of elements to remove.
 * @returns The number of removed elements.
 * @note Will trigger callback cnwn_ContainerDeinitElements() for the removed elements.
 */
extern CNWN_PUBLIC int cnwn_queue_remove(cnwn_Queue * queue, int index, int length);

/**
 * Push a number of elements to the queue.
 * @param queue The queue to push elements for.
 * @param length The number of elements to append.
 * @param elements Copy the elements from this memory, NULL to omit.
 * @returns The number of appended elements.
 * @note Will trigger callback cnwn_ContainerInitElements() for the appended elements.
 * @note Synonymous with cnwn_queue_append().
 */
extern CNWN_PUBLIC int cnwn_queue_push(cnwn_Queue * queue, int length, const void * elements);

/**
 * Pop a number of elements from the queue.
 * @param queue The queue to pop elements from.
 * @param index The index to start the pop from (zero is the first element in the queue), negative values will wrap from the end of the queue.
 * @param length The number of elements to pop.
 * @param[out] ret_elements Copy the elements to this memory, NULL to omit.
 * @returns The number of popped elements.
 * @note Will trigger callback cnwn_ContainerDeinitElements() for the removed elements.
 */
extern CNWN_PUBLIC int cnwn_queue_pop(cnwn_Queue * queue, int index, int length, void * ret_elements);

////////////////////////////////////////////////////////////////
//
//
// Special array implementations.
//
//
////////////////////////////////////////////////////////////////

/**
 * Initialize a string array.
 * @param array The array struct to initialize.
 */
extern CNWN_PUBLIC void cnwn_string_array_init(cnwn_StringArray * array);

/**
 * Create a new string array.
 * @returns The newly created array.
 */
extern CNWN_PUBLIC cnwn_StringArray * cnwn_string_array_new(void);

/**
 * Get a string from the array.
 * @param array The array to get the string from.
 * @param index Where to set the string, negative values will wrap from the end of the array.
 * @returns The string or NULL if the string is NULL or @p index is out of range.
 */
extern CNWN_PUBLIC const char * cnwn_string_array_get(const cnwn_StringArray * array, int index);

/**
 * Set a formatted string in the array.
 * @param array The array to set the string for.
 * @param index Where to set the string, negative values will wrap from the end of the array.
 * @param format The format of the string.
 * @param args The arguments for the format.
 * @returns The number of set strings.
 */
extern CNWN_PUBLIC int cnwn_string_array_set_va(cnwn_StringArray * array, int index, const char * format, va_list args);

/**
 * Set a formatted string to the array.
 * @param array The array to set the string for.
 * @param index Where to set the string, negative values will wrap from the end of the array.
 * @param format The format of the string.
 * @returns The number of set strings.
 */
extern CNWN_PUBLIC int cnwn_string_array_set(cnwn_StringArray * array, int index, const char * format, ...) CNWN_PRINTF(3, 4);

/**
 * Insert a formatted string to the array.
 * @param array The array to insert the string for.
 * @param index Where to insert the string, negative values will wrap from the end of the array.
 * @param format The format of the string.
 * @param args The arguments for the format.
 * @returns The number of inserted strings.
 */
extern CNWN_PUBLIC int cnwn_string_array_insert_va(cnwn_StringArray * array, int index, const char * format, va_list args);

/**
 * Insert a formatted string to the array.
 * @param array The array to insert the string for.
 * @param index Where to insert the string, negative values will wrap from the end of the array.
 * @param format The format of the string.
 * @returns The number of inserted strings.
 */
extern CNWN_PUBLIC int cnwn_string_array_insert(cnwn_StringArray * array, int index, const char * format, ...) CNWN_PRINTF(3, 4);

/**
 * Append a formatted string to the array.
 * @param array The array to append the string for.
 * @param format The format of the string.
 * @param args The arguments for the format.
 * @returns The number of appended strings.
 */
extern CNWN_PUBLIC int cnwn_string_array_append_va(cnwn_StringArray * array, const char * format, va_list args);

/**
 * Append a formatted string to the array.
 * @param array The array to append the string for.
 * @param format The format of the string.
 * @returns The number of appended strings.
 */
extern CNWN_PUBLIC int cnwn_string_array_append(cnwn_StringArray * array, const char * format, ...) CNWN_PRINTF(2, 3);

/**
 * Split a string and append it to the array.
 * @param array The array to append the strings for.
 * @param s The string to split.
 * @param max_splits The maximum number of splits, a negative value disables the limit.
 * @param splitstr The string to find and split on.
 * @param escstr An option escape string for @p splitstr, NULL or empty is fine.
 * @returns The number of appended strings.
 */
extern CNWN_PUBLIC int cnwn_string_array_split(cnwn_StringArray * array, const char * s, int max_splits, const char * splitstr, const char * escstr);

/**
 * Split a string and append it to the array (case insensetive).
 * @param array The array to append the tring for.
 * @param s The string to split.
 * @param max_splits The maximum number of splits, a negative value disables the limit.
 * @param splitstr The string to find and split on.
 * @param escstr An option escape string for @p splitstr, NULL or empty is fine.
 * @returns The number of appended strings.
 */
extern CNWN_PUBLIC int cnwn_string_array_spliti(cnwn_StringArray * array, const char * s, int max_splits, const char * splitstr, const char * escstr);

/**
 * Join an array into one long string.
 * @param array The array to join.
 ' @param index At what string to start the join, negative values will wrap from the end.
 * @param length The number of strings to join.
 * @param sepstr An optional separation string between every element, NULL or empty is fine.
 * @param max_size The maximum size (chars, not UTF-8 characters and including zero terminator) of the return string.
 * @param[out] ret_s Return the string here, NULL will return the required length.
 * @returns The size (chars, not UTF-8 characters and excluding zero terminator) of the return string.
 */
extern CNWN_PUBLIC int cnwn_string_array_join(const cnwn_StringArray * array, int index, int length, const char * sepstr, int max_size, char * ret_s);

/**
 * Join an array into one long string.
 * @param array The array to join.
 * @param sepstr An optional separation string between every element, NULL or empty is fine.
 * @returns A newly allocated string containing the joined elements.
 */
extern CNWN_PUBLIC char * cnwn_string_array_join2(const cnwn_StringArray * array, const char * sepstr);

/**
 * Insert this string in each string in the array.
 * @param array The array to operate on.
 * @param index Where to insert the string in each string, negative values will wrap from the end of the string.
 * @param format The printf format.
 * @param args The variable args for the format.
 * @returns The number of modified strings.
 */
extern CNWN_PUBLIC int cnwn_string_array_each_insert_va(cnwn_StringArray * array, int index, const char * format, va_list args);

/**
 * Insert this string in each string in the array.
 * @param array The array to operate on.
 * @param index Where to insert the string in each string, negative values will wrap from the end of the string.
 * @param format The printf format.
 * @returns The number of modified strings.
 */
extern CNWN_PUBLIC int cnwn_string_array_each_insert(cnwn_StringArray * array, int index, const char * format, ...) CNWN_PRINTF(3, 4);

/**
 * Append this string for each string in the array.
 * @param array The array to operate on.
 * @param format The printf format.
 * @param args The variable args for the format.
 * @returns The number of modified strings.
 */
extern CNWN_PUBLIC int cnwn_string_array_each_append_va(cnwn_StringArray * array, const char * format, va_list args);

/**
 * Append this string for each string in the array.
 * @param array The array to operate on.
 * @param format The printf format.
 * @returns The number of modified strings.
 */
extern CNWN_PUBLIC int cnwn_string_array_each_append(cnwn_StringArray * array, const char * format, ...) CNWN_PRINTF(2, 3);

#ifdef __cplusplus
}
#endif

#endif
