#pragma once

#include <assert.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>


// Logging

#ifdef DEBUG
#define debug(...) fprintf(stderr, "[debug] " __VA_ARGS__)
#else
#define debug(...)
#endif

#ifdef INFO
#define info(...) fprintf(stderr, "[info] " __VA_ARGS__)
#else
#define info(...)
#endif

#ifdef NOERROR
#define error(...)
#else
#define error(...) fprintf(stderr, "[error] " __VA_ARGS__)
#endif

#define critical(...) fprintf(stderr, "[critical] " __VA_ARGS__)


// Memory allocation function with line numbers

extern void * _mb_nalloc(
        size_t num,
        size_t size,
        int32_t line,
        const char * file,
        const char * func);

extern void * _mb_alloc(
        size_t size,
        int32_t line,
        const char * file,
        const char * func);

#define mem_alloc(size) (_mb_alloc(size, __LINE__, __FILE__, __func__))


// Memory reallocation function with line numbers

extern void * _mb_realloc(
        void * ptr,
        size_t size,
        int32_t line,
        const char * file,
        const char * func);

#define mem_realloc(ptr, size) (_mb_realloc(ptr, size, __LINE__, __FILE__, __func__))


// Free memory functions

#define mem_free(p) \
    do { debug("free memory %p at line %d in %s:%s\n", \
            (void *) p, __LINE__, __FILE__, __func__); \
        free(p);} while (0)
