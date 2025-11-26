/**
 * @file test_simple2.c
 * @author Lorenzo LUCAS -- ROBLOT (lorenzo.lucas--roblot@ensiie.eu), Alicia PLATH (alicia.plath@@ensiie.eu)
 * @brief Test file
 * @version 0.1
 * @date 2025-10-20
 *
 * @copyright Copyright (c) 2025
 *
 */

#include <stdio.h>
#include <stdlib.h>

#include <mpi.h>


int main(int argc, char * argv[])
{
	MPI_Init(&argc, &argv);

	int rank;
    int i=0;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    if (rank %2 ==0){
        i += 1;
        if (rank == 0){
            MPI_Barrier(MPI_COMM_WORLD);
        }
    }


	MPI_Finalize();
	return i;
}
