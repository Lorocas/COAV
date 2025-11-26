/**
 * @file test_simple4.c
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


#pragma ProjetCA mpicoll_check main

int main(int argc, char *argv[])
{
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int value = rank + 1;
    int sum = 0;

    if (rank == 0) {
        MPI_Barrier(MPI_COMM_WORLD);
        MPI_Reduce(&value, &sum, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
    }
    else{
        MPI_Reduce(&value, &sum, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
        MPI_Barrier(MPI_COMM_WORLD);
    }



    MPI_Finalize();
    return 0;
}
