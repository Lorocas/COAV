/**
 * @file rank.cpp
 * @author Lorenzo LUCAS -- ROBLOT (lorenzo.lucas--roblot@ensiie.eu), Alicia PLATH (alicia.plath@ensiie.eu)
 * @brief Rank functions implementation
 * @version 0.3
 * @date 2025-11-17
 *
 * @copyright Copyright (c) 2025
 *
 */

#include "rank.h"
#include "mpi_collectives.h"

/**
 * @brief Recursively removes cycles in the control flow graph (CFG).
 *
 * @param bb Current basic block being analyzed.
 * @param visited Bitmap representing the set of already visited basic blocks.
 * @param cfgPrim Array of bitmaps representing the modified control flow graph (CFG'). Each bitmap index corresponds to a basic block and stores the removed edges.
 */
void remove_circle_recursive(basic_block bb, bitmap_head visited, bitmap_head *cfgPrim)
{
	edge p;
  	edge_iterator ei;

  	FOR_EACH_EDGE (p, ei, bb->succs)
	{
        //si un des successeurs a dejà été traité c'est qu'on a fait un cycle dont on enlève l'arête
		if (bitmap_bit_p(&visited, p->dest->index))
		{
            //un bit set dans cfgPrim correspond à une arrête enlevé
			bitmap_set_bit(&cfgPrim[bb->index], ei.index);
		}
		else
		{
            bitmap_set_bit(&visited, p->dest->index);
			remove_circle_recursive(p->dest, visited, cfgPrim);
		}
	}
}

/**
 * @brief Removes cycles from the control flow graph (CFG) of the given function.
 *
 * @param fun Pointer to the function currently being analyzed.
 * @return bitmap_head* Pointer to the array of bitmaps representing the modified, acyclic CFG (CFG').
 */
bitmap_head* remove_cycle(function* fun)
{
    basic_block bb;
    bitmap_head visited;
    bitmap_initialize (&visited, &bitmap_default_obstack);

    //on initialise le cfg' qui contiendra le nouveau graphe
	bitmap_head* cfgPrim = XNEWVEC(bitmap_head, last_basic_block_for_fn (fun));
	FOR_ALL_BB_FN (bb, fun)
	{
		bitmap_initialize (&cfgPrim[bb->index], &bitmap_default_obstack);
	}

	bb = ENTRY_BLOCK_PTR_FOR_FN(fun);
	bitmap_set_bit(&visited, bb->index);
	remove_circle_recursive(bb, visited, cfgPrim);

    bitmap_release(&visited);
    return cfgPrim;
}


/**
 * @brief Initializes the rank array for all basic blocks in the function.
 *
 * @param fun Pointer to the function currently being analyzed.
 * @param rangs Array of integers representing the rank of each basic block.
 */
void initialisation_rang(function* fun, int* rangs)
{
	basic_block bb;
	FOR_ALL_BB_FN(bb, fun)
	{
		rangs[bb->index] = 0;
	}
}


/**
 * @brief Recursively computes the rank of each basic block in the acyclic control flow graph (CFG').
 *
 * @param bb Current basic block being analyzed.
 * @param rangs Array of integers representing the rank of each basic block.
 * @param cfgPrim Array of bitmaps representing the modified control flow graph (CFG'). Each bitmap index corresponds to a basic block and stores the removed edges.
 * @param collectives Array of ID for each MPI collective operations.
 */
void rang_recursive(basic_block bb, int* rangs, bitmap_head* cfgPrim, int* collectives)
{
	edge p;
  	edge_iterator ei;

    // on récupère le rang actuelle
	int rang = rangs[bb->index];

  	FOR_EACH_EDGE (p, ei, bb->succs)
	{
        // si cette arrête existe encore dans cfg'
		if(!bitmap_bit_p(&cfgPrim[bb->index], ei.index))
		{
			int newRang = rang;

            // s'il y a une collective dans le bloc suivant, on augmente le rang
			if (collectives[p->dest->index] != LAST_AND_UNUSED_MPI_COLLECTIVE_CODE) newRang += 1;

			// on met à jour le rang du bloc successeur si le nouveau rang calculé est plus élevé que son rang actuel
			if (newRang > rangs[p->dest->index]) rangs[p->dest->index] = newRang;

			// appel récursif de la fonction pour parcourir le successeur du noeud étudié
			rang_recursive(p->dest, rangs, cfgPrim, collectives);
		}
	}
}


void rang(function* fun, int* rangs, int* collectives)
{
	bitmap_head* cfgPrim = remove_cycle(fun);

	initialisation_rang(fun, rangs);

    //le premier bloc a le rang 0
	basic_block bb = ENTRY_BLOCK_PTR_FOR_FN(fun);
	rangs[bb->index] = 0;

	rang_recursive(bb, rangs, cfgPrim, collectives);

	free(cfgPrim);
}


void printRang(function* fun, int* rangs)
{
	basic_block bb;
	FOR_ALL_BB_FN(bb, fun)
	{
		printf("Le rang du bloc %d est %d\n", bb->index, rangs[bb->index]);
	}
}


int maxRang(function* fun, int* rangs)
{
	int max_rang = 0;
	basic_block bb;
	FOR_ALL_BB_FN(bb, fun)
	{
		if (rangs[bb->index] > max_rang)
		{
			max_rang = rangs[bb->index];
		}
	}
	return max_rang;
}
