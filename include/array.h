#ifndef ARRAY_H
#define ARRAY_H

/* Macro to push a value onto an array */
#define array_push(array, value)                                              \
    do                                                                        \
    {                                                                         \
        (array) = array_hold((array), 1, sizeof(*(array)));                   \
        (array)[array_length(array) - 1] = (value);                           \
    } while (0);

/* Function to allocate or reallocate memory for an array */
void* array_hold(void* array, int count, int item_size);

/* Function to get the length of an array */
int array_length(void* array);

/* Function to free the memory allocated for an array */
void array_free(void* array);

#endif /* ARRAY_H */