/**
 * @file mpi_collectives.h
 * @author Lorenzo LUCAS -- ROBLOT (lorenzo.lucas--roblot@ensiie.eu), Alicia PLATH (alicia.plath@ensiie.eu)
 * @brief MPI collective operations interface
 * @version 0.1
 * @date 2025-10-20
 *
 * @copyright Copyright (c) 2025
 *
 */

#ifndef MPI_COLLECTIVES_H
#define MPI_COLLECTIVES_H

#include <gcc-plugin.h>

/**
 * @brief An enum type. It reads the file \file MPI_collectives.def and creates an enumeration of the collective operations.
 */
enum mpi_collective_code
{
	#define DEFMPICOLLECTIVES(CODE, NAME) CODE,
	#include "MPI_collectives.def"
		LAST_AND_UNUSED_MPI_COLLECTIVE_CODE
	#undef DEFMPICOLLECTIVES
};


/* Name of each MPI collective operations */
#define DEFMPICOLLECTIVES(CODE, NAME) NAME,
const char *const mpi_collective_name[] = {
	#include "MPI_collectives.def"
};
#undef DEFMPICOLLECTIVES


/**
 * @brief Get the mpi call code object
 *
 * @param stmt GIMPLE statement
 * @return enum mpi_collective_code
 */
enum mpi_collective_code get_mpi_call_code(gimple* stmt);


/**
 * @brief checks if there is at least one MPI collective in a basic block
 *
 * @param bb basic block
 * @return bool true if @p bb contains at least one MPI collective, false otherwise
 */
bool isThereMPIStatement(basic_block bb);

#endif
