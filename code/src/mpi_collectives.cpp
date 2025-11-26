/**
 * @file mpi_collectives.cpp
 * @author Lorenzo LUCAS -- ROBLOT (lorenzo.lucas--roblot@ensiie.eu), Alicia PLATH (alicia.plath@ensiie.eu)
 * @brief MPI collective operations implementation
 * @version 0.2
 * @date 2025-11-17
 *
 * @copyright Copyright (c) 2025
 *
 */

#include <string>
#include "mpi_collectives.h"
#include <tree.h>
#include <gimple.h>
#include <gimple-iterator.h>


enum mpi_collective_code get_mpi_call_code(gimple* stmt)
{
    if (!is_gimple_call(stmt))
    {
        return LAST_AND_UNUSED_MPI_COLLECTIVE_CODE;
    }

    tree called_fun = gimple_call_fndecl(stmt);
    if (NULL == called_fun)
    {
        return LAST_AND_UNUSED_MPI_COLLECTIVE_CODE;
    }

    std::string fun_name = std::string(IDENTIFIER_POINTER(DECL_NAME(called_fun)));

    for (int i = MPI_INIT; i < LAST_AND_UNUSED_MPI_COLLECTIVE_CODE; ++i)
    {
        std::string mpi = std::string(mpi_collective_name[i]);
        if (fun_name.compare(mpi) == 0)
        {
            return mpi_collective_code(i);
        }
    }

    return LAST_AND_UNUSED_MPI_COLLECTIVE_CODE;
}


bool isThereMPIStatement(basic_block bb)
{
    gimple_stmt_iterator gsi;

    for (gsi = gsi_start_bb (bb); !gsi_end_p (gsi); gsi_next (&gsi))
    {
        gimple *stmt = gsi_stmt (gsi);
        mpi_collective_code collective = get_mpi_call_code(stmt);
        if (collective != LAST_AND_UNUSED_MPI_COLLECTIVE_CODE) return true;
    }

    return false;
}
