#include "utils.h"


// Memory allocation function with line numbers

inline void * _mb_nalloc(size_t num, size_t size, int32_t line, const char * file, const char * func) {
    // Allocate
    void * ptr = calloc(num, size);

    // Verify that memory was allocated
    if (ptr == NULL) {
        critical("failed to allocate memory at %d in %s:%s\n", line, file, func);
        critical("exiting\n");
        exit(1);
    }

    debug("allocate memory %p (%lu B) at line %d in %s:%s\n",
            ptr, num * size, line, file, func);

    // Return pointer
    return ptr;
}


inline void * _mb_alloc(size_t size, int32_t line, const char * file, const char * func) {
    return _mb_nalloc(1, size, line, file, func);
}


// Memory reallocation function with line numbers

inline void * _mb_realloc(void * ptr, size_t size, int32_t line, const char * file, const char * func) {
    // Allocate
    void * ptr_new = realloc(ptr, size);

    // Verify that memory was allocated
    if (ptr_new == NULL) {
        critical("failed to reallocate memory at %d in %s:%s\n", line, file, func);
        critical("exiting\n");
        exit(1);
    }

    debug("reallocate memory %p to %p (%lu B) at line %d in %s:%s\n",
            ptr, ptr_new, size, line, file, func);

    // Return pointer
    return ptr_new;
}
