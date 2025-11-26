/**
 * @file post_dominance.cpp
 * @author Lorenzo LUCAS -- ROBLOT (lorenzo.lucas--roblot@ensiie.eu), Alicia PLATH (alicia.plath@ensiie.eu)
 * @brief Post-dominance functions implementation
 * @version 0.5
 * @date 2025-11-17
 *
 * @copyright Copyright (c) 2025
 *
 */

#include <string>
#include "post_dominance.h"
#include "rank.h"
#include "mpi_collectives.h"
#include <tree.h>
#include <gimple.h>
#include <gimple-iterator.h>
#include <diagnostic.h>

/**
 * @brief Checks if a node (basic block) @p x post-dominates a set @p e .
 *
 * @param fun Pointer to the function currently being analyzed.
 * @param e Bitmap representing the initial set of basic blocks (E).
 * @param x Basic block
 * @param visited Bitmap representing the set of already visited basic blocks.
 * @return bool Return true if the node (basic block) @p x post-dominates the set @p e , false otherwise.
 */
bool doesEPostDomineX(function* fun, bitmap_head* e, basic_block x, bitmap_head visited)
{
	// si x appartient à e alors x est post dominé par e
	if (bitmap_bit_p(e, x->index)) return true;

	// si x n'a pas de successeur, x ne peut pas être postdominé
	if (EDGE_COUNT(x->succs) == 0) return false;

	if (bitmap_bit_p(&visited, x->index)) return true;

	edge p;
	edge_iterator ei;

	// si x!=e, x est post dominé par e si tous ses successeurs sont post-dominé par e
	// donc on parcourt les successeurs
	FOR_EACH_EDGE (p, ei, x->succs)
	{
		bitmap_set_bit(&visited, x->index);
		if(!doesEPostDomineX(fun, e, p->dest, visited)){
			return false;
		}
	}
	return true;
}


/**
 * @brief Checks if a node (basic block) @p x post-dominates a set @p e .
 *
 * @param fun Pointer to the function currently being analyzed.
 * @param e Bitmap representing the initial set of basic blocks (E).
 * @param x Basic block
 * @return bool Return true if the node (basic block) @p x post-dominates the set @p e , false otherwise.
 */
bool doesEPostDomineX(function* fun, bitmap_head* e, basic_block x)
{
	bitmap_head visited;
	bitmap_initialize(&visited, &bitmap_default_obstack);
	return doesEPostDomineX(fun, e, x, visited);
}


/**
 * @brief computes the post-dominance frontier of a set
 *
 * @param fun Pointer to the function currently being analyzed.
 * @param e Bitmap representing the initial set of basic blocks (E).
 * @param pdf Bitmap representing the resulting iterated post-dominance frontier.
 * @return bool Return true if pdf is empty, false otherwise.
 */
bool frontierePostDominance(function* fun, bitmap_head* e, bitmap_head* pdf)
{
	bool isPdfEmpty = true;

    // on parcourt tous les noeuds
    basic_block bb;
	FOR_ALL_BB_FN(bb, fun)
	{
		// on cherche un basic block bb tel que e ne postdomine pas bb
		// cf def du TP
		if (doesEPostDomineX(fun, e, bb)) continue;

		// on parcourt les successeurs de bb
		edge p;
		edge_iterator ei;
		FOR_EACH_EDGE (p, ei, bb->succs)
		{
			// si un successeur de bb est post-dominé par e alors bb fait partie de la frontière de post-dominance
			if (doesEPostDomineX(fun, e, p->dest))
			{
				bitmap_set_bit(pdf, bb->index);
				isPdfEmpty = false;
			}
		}
	}
	return isPdfEmpty;
}


/**
 * @brief Computes the iterated post-dominance frontier of a given set of basic blocks.
 *
 * @param fun Pointer to the function currently being analyzed.
 * @param pdf Bitmap representing the resulting post-dominance frontier.
 * @param pdf_itere Bitmap representing the memory space to store the resulting iterated post-dominance frontier
 */
void frontierePostDominanceIteree(function* fun, bitmap_head* pdf, bitmap_head* pdf_itere)
{
	//tant que pdf n'est pas empty et qu'on a pas fait de cycle
	bool isPdfEmpty = false;
	while(!isPdfEmpty && !bitmap_intersect_p((const bitmap)pdf_itere, (const bitmap)pdf))
	{
		// on rajoute les blocs de la pdf à la pdf itérée
		bitmap_head copy;
		bitmap_initialize(&copy, &bitmap_default_obstack);
		bitmap_copy(&copy, pdf_itere);
		bitmap_ior(pdf_itere, (const_bitmap)&copy, (const_bitmap)pdf);

		// on calcule la pdf de la pdf
		bitmap_head pdf_of_pdf ;
		bitmap_initialize (&pdf_of_pdf, &bitmap_default_obstack);
		isPdfEmpty = frontierePostDominance(fun, pdf, &pdf_of_pdf);

		// pour calculer pdf de pdf_of_pdf si nécessaire
		if(!isPdfEmpty){
			bitmap_copy(pdf, &pdf_of_pdf);
		}
	}
}


/**
 * @brief Emits a warning message for problematic MPI collective intersections.
 *
 * @param fun Pointer to the function currently being analyzed.
 * @param pdf Bitmap representing the resulting iterated post-dominance frontier.
 * @param e Bitmap representing the initial set of basic blocks (E).
 */
void messageErreur(function* fun, bitmap_head* pdf, bitmap_head* e)
{
	std::string eString = "this intersection causes some problems on the MPI collective(s) line(s): ";

	// pour tous les noeuds
	basic_block bb;
	FOR_ALL_BB_FN(bb, fun)
	{
		// s'il fait partie de l'ensemble e
		if(bitmap_bit_p(e, bb->index))
		{
			// on récupère la ligne de la collective MPI
			gimple_stmt_iterator gsi;
			gsi = gsi_start_bb(bb);
			gimple *stmt = gsi_stmt(gsi);

			// on sait qu'il n'y a qu'une collective MPI par block donc on la cherche
			while(get_mpi_call_code(stmt) == LAST_AND_UNUSED_MPI_COLLECTIVE_CODE)
			{
				gsi_next(&gsi);
				stmt = gsi_stmt(gsi);
			}
			eString += std::to_string(gimple_lineno(stmt)) + " ";
		}
	}

	FOR_ALL_BB_FN(bb, fun)
	{
		// s'il fait partie de la pdf
		if (bitmap_bit_p(pdf, bb->index))
		{
			// on récupère la ligne de la séparation avec pb (dernière ligne du bloc)
			gimple_stmt_iterator gsi;
			gsi = gsi_start_bb(bb);
			while(!gsi_one_before_end_p(gsi)) gsi_next(&gsi);

			// on génère le warning
			gimple *stmt = gsi_stmt (gsi);
			location_t loc = gimple_location(stmt);

			warning_at(loc, 0, "%s", eString.c_str());
		}
	}
}


void foundAndPrintDeadlock(function* fun, int* rangs, int* collectives)
{
    int max_rang = maxRang(fun, rangs);

	// pour chaque ensemble
	for (int i=0; i<LAST_AND_UNUSED_MPI_COLLECTIVE_CODE; i++)
	{
		for (int j=0; j<max_rang +1; j++)
		{

            basic_block bb;

			// on enregistre qui appartient à l'ensemble dans un bitmap
			bitmap_head ensemble_E;
			bitmap_initialize (&ensemble_E, &bitmap_default_obstack);
			int nb_bb_in_E = 0;
			FOR_ALL_BB_FN (bb, fun)
			{
				if (collectives[bb->index]==i && rangs[bb->index]==j)
				{
					bitmap_set_bit(&ensemble_E, bb->index);
					nb_bb_in_E +=1;
				}
			}

            // si l'ensemble est vide, on cherche un autre ensemble non vide
            if (nb_bb_in_E == 0) continue;

            // PDF(ensemble_E)
            bitmap_head pdf;
			bitmap_initialize (&pdf, &bitmap_default_obstack);
			bool isPdfEmpty = frontierePostDominance(fun, &ensemble_E, &pdf);

			// si pdf(E) est vide, pas de problème, on passe au prochain ensemble
			if(isPdfEmpty) continue;

			// sinon il peut y avoir un deadlock, on cherche l'origine du pb
			bitmap_head pdf_itere;
			bitmap_initialize (&pdf_itere, &bitmap_default_obstack);
			frontierePostDominanceIteree(fun, &pdf, &pdf_itere);

			// print le message d'erreur
			messageErreur(fun, &pdf_itere, &ensemble_E);
		}
	}
}
