/**
 * @file plugin.cpp
 * @author Lorenzo LUCAS -- ROBLOT (lorenzo.lucas--roblot@ensiie.eu), Alicia PLATH (alicia.plath@ensiie.eu)
 * @brief Plugin implementation
 * @version 0.3
 * @date 2025-11-17
 *
 * @copyright Copyright (c) 2025
 *
 */

#include "plugin.h"
#include "mpi_collectives.h"
#include "rank.h"
#include "post_dominance.h"
#include "pragma.h"
#include "graphviz.h"
#include <tree.h>
#include <gimple.h>
#include <gimple-iterator.h>

extern vec<const char*> list_func;
extern vec<const char*> used_func;
extern bool pragma_execute;

/**
 * @brief Splits blocks containing multiple collectives.
 *
 * @param fun Pointer to the function currently being analyzed.
 */
void splitBlock(function* fun)
{
    basic_block bb;

    // on parcourt chaque bloc
	FOR_ALL_BB_FN(bb, fun)
    {
		gimple_stmt_iterator gsi;

        // avons-nous déjà trouvé une collective ?
		bool collective_found = false;

        // on parcourt chaque statement
		for (gsi = gsi_start_bb(bb); !gsi_end_p(gsi); gsi_next(&gsi))
		{
			gimple *stmt = gsi_stmt(gsi);
			mpi_collective_code collective = get_mpi_call_code(stmt);

            // si le statement est une collective MPI
			if (collective != LAST_AND_UNUSED_MPI_COLLECTIVE_CODE)
            {

                // s'il y a déjà une collective avant dans le bloc, on sépare le bloc en deux
				if (collective_found)
                {
					gsi_prev(&gsi);
					gimple* stmt_prev = gsi_stmt(gsi);
					split_block(bb, stmt_prev);
				}
				else
                {
					collective_found = true;
				}
			}
		}
	}
}


/**
 * @brief Records MPI collective operations in each basic block.
 *
 * @param fun Pointer to the function currently being analyzed.
 * @param collective_tab Array of ID for each MPI collective operations
 */
void saveCollective(function* fun, int* collective_tab)
{
    basic_block bb;

    // on parcourt chaque bloc
	FOR_ALL_BB_FN (bb, fun)
    {
		gimple_stmt_iterator gsi;
        int collective_found = false;

        // on parcourt chaque statement
		for (gsi = gsi_start_bb (bb); !gsi_end_p (gsi); gsi_next (&gsi))
		{
			gimple *stmt = gsi_stmt (gsi);
			mpi_collective_code collective = get_mpi_call_code(stmt);

            // si le statement est une collective MPI
			if (collective != LAST_AND_UNUSED_MPI_COLLECTIVE_CODE)
            {
			    collective_tab[bb->index] = (int)collective;
                collective_found = true;
                break;
			}
		}

        // s'il n'y a aucune collective dans le bloc
		if (!collective_found) collective_tab[bb->index] = LAST_AND_UNUSED_MPI_COLLECTIVE_CODE;
	}
}


int plugin_init(struct plugin_name_args *plugin_info, struct plugin_gcc_version *version)
{
    my_pass pass(g);

    struct register_pass_info my_pass_info;
    my_pass_info.pass = &pass;
    my_pass_info.reference_pass_name = "cfg";
    my_pass_info.ref_pass_instance_number = 0;
    my_pass_info.pos_op = PASS_POS_INSERT_AFTER;

    register_callback(plugin_info->base_name, PLUGIN_PASS_MANAGER_SETUP, NULL, &my_pass_info);

    register_callback(plugin_info->base_name, PLUGIN_FINISH, check_unused_function, NULL);

    c_register_pragma("ProjetCA", "mpicoll_check", my_pragma_handler);

    return 0;
}

unsigned int my_pass::execute(function *fun)
{
    splitBlock(fun);

    // tableau qui contiendra les collectives associées à chaque basic block
    int* collectives = (int*)xmalloc(last_basic_block_for_fn (fun)* sizeof(int));

    // tableau qui contiendra le rang de chaque basic block
    int* rangs = (int*)xmalloc(last_basic_block_for_fn (fun)* sizeof(int));

    saveCollective(fun, collectives);

    rang(fun, rangs, collectives);

    //printRang(fun, rangs);

    foundAndPrintDeadlock(fun, rangs, collectives);

    cfgviz_dump(fun, "_cfg", collectives, rangs);

    // on libère la mémoire des tableaux précédemment alloués
    free(collectives);
    free(rangs);

    return 0;
}

bool my_pass::gate(function *fun)
{
    if (pragma_execute){
        const char* fname = fndecl_name(cfun->decl);

        if (list_func.contains(fname)){
            used_func.safe_push(fname);
            return true;
        }
        return false;
    }

    return true;

}
