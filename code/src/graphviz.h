/**
 * @file graphviz.h
 * @author Lorenzo LUCAS -- ROBLOT (lorenzo.lucas--roblot@ensiie.eu), Alicia PLATH (alicia.plath@ensiie.eu)
 * @brief graphviz interface
 * @version 0.4
 * @date 2025-11-17
 *
 * @copyright Copyright (c) 2025
 *
 */

#include <gcc-plugin.h>

/**
 * @brief Build a filename (as a string) based on function name.
 *
 * @param fun Pointer to the function currently being analyzed.
 * @param suffix Suffix of the filename as a string.
 * @return char* String of the filename.
 */
char* cfgviz_generate_filename(function* fun, const char* suffix);

/**
 * @brief Dump the graphviz representation of function 'fun' in file 'out'.
 *
 * @param fun Pointer to the function currently being analyzed.
 * @param out Output stream.
 * @param collectives ID of an MPI collective.
 */
void cfgviz_internal_dump(function* fun, FILE* out, int* collectives);


/**
 * @brief Dump the graphviz representation of function 'fun' in file 'out'.
 *
 * @param fun Pointer to the function currently being analyzed.
 * @param suffix Suffix of the filename as a string.
 * @param collectives ID of an MPI collective.
 * @param rangs Array of integers representing the rank of each basic block.
 */
void cfgviz_dump(function * fun, const char* suffix, int* collectives, int* rangs);
