/**
 * @file rank.h
 * @author Lorenzo LUCAS -- ROBLOT (lorenzo.lucas--roblot@ensiie.eu), Alicia PLATH (alicia.plath@ensiie.eu)
 * @brief Rank functions interface
 * @version 0.3
 * @date 2025-11-17
 *
 * @copyright Copyright (c) 2025
 *
 */

#include <gcc-plugin.h>

/**
 * @brief Computes the rank of all basic blocks in the function based on the acyclic control flow graph (CFG').
 *
 * @param fun Pointer to the function currently being analyzed.
 * @param rangs Array of integers representing the rank of each basic block.
 * @param collectives Array of ID for each MPI collective operations.
 */
void rang(function* fun, int* rangs, int* collectives);

/**
 * @brief Prints the rank of each basic block in the function to the standard output.
 *
 * @param fun Pointer to the function currently being analyzed.
 * @param rangs Array of integers representing the rank of each basic block.
 */
void printRang(function* fun, int* rangs);

/**
 * @brief Returns the maximum rank value among all basic blocks in the function.
 *
 * @param fun Pointer to the function currently being analyzed.
 * @param rangs Array of integers representing the rank of each basic block.
 * @return int Value of the maximum rank.
 */
int maxRang(function* fun, int* rangs);
