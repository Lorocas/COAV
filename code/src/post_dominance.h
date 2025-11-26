/**
 * @file post_dominance.h
 * @author Lorenzo LUCAS -- ROBLOT (lorenzo.lucas--roblot@ensiie.eu), Alicia PLATH (alicia.plath@ensiie.eu)
 * @brief Post-dominance functions interface
 * @version 0.2
 * @date 2025-11-17
 *
 * @copyright Copyright (c) 2025
 *
 */

#include <gcc-plugin.h>

/**
 * @brief computes the post-dominance frontier of a set and prints the warning messages.
 *
 * @param fun Pointer to the function currently being analyzed.
 * @param rangs Array of integers representing the rank of each basic block.
 * @param collectives Array of ID for each MPI collective operations.
 */
void foundAndPrintDeadlock(function* fun, int* rangs, int* collectives);
