/**
 * @file graphviz.cpp
 * @author Lorenzo LUCAS -- ROBLOT (lorenzo.lucas--roblot@ensiie.eu), Alicia PLATH (alicia.plath@ensiie.eu)
 * @brief graphviz implementation
 * @version 0.1
 * @date 2025-10-20
 *
 * @copyright Copyright (c) 2025
 *
 */

#include "graphviz.h"


char* cfgviz_generate_filename(function * fun, const char* suffix)
{
	char* target_filename;

	target_filename = (char *)xmalloc(2048 * sizeof( char));

	snprintf(
		target_filename,
		1024,
		"%s_%s_%d_%s.dot",
		current_function_name(),
		LOCATION_FILE( fun->function_start_locus ),
		LOCATION_LINE( fun->function_start_locus ),
		suffix
	);

	return target_filename;
}


void cfgviz_internal_dump(function* fun, FILE* out, int* collectives, int* rangs)
{
	// Print the header line and open the main graph
	fprintf(out, "Digraph G{\n");

    basic_block bb;
    FOR_ALL_BB_FN(bb, fun)
	{
        int index = bb->index;
        edge e;
        edge_iterator ei;

        fprintf(out, "N%d [label=\"Node %d\n collective:%d\n rang:%d\" shape=ellipse]\n", index, index,collectives[index],rangs[index]);
        FOR_EACH_EDGE(e, ei, bb->succs)
		{
            basic_block src = e->src;
            basic_block dst = e->dest;

            fprintf(out, "N%d -> N%d [color=red label=\"\"]\n", src->index, dst->index);
        }
    }

	// Close the main graph
	fprintf(out, "}\n");
}


void cfgviz_dump(function * fun, const char* suffix, int* collectives, int* rangs)
{
	char* target_filename;
	FILE* out;

	target_filename = cfgviz_generate_filename(fun, suffix);

	printf("[GRAPHVIZ] Generating CFG of function %s in file <%s>\n", current_function_name(), target_filename);

	out = fopen(target_filename, "w");

	cfgviz_internal_dump(fun, out, collectives, rangs);

	fclose(out);
	free(target_filename);
}
