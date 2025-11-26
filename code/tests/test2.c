/**
 * @file test2.c
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


void mpi_call(int c)
{
	MPI_Barrier(MPI_COMM_WORLD);

	if(c<10)
	{
		printf("je suis dans le if (c=%d)\n", c);
		MPI_Barrier(MPI_COMM_WORLD);
		MPI_Barrier(MPI_COMM_WORLD);

	}
	else
	{
		printf("je suis dans le else (c=%d)\n", c);
		MPI_Barrier(MPI_COMM_WORLD);
	}
}

#pragma ProjetCA mpicoll_check (mpi_call, soutennance)

int main(int argc, char * argv[])
{
	MPI_Init(&argc, &argv);

	int i;
	int a = 2;
	int b = 3;
	int c=0;

	for(i=0; i<10; i++)
	{

		mpi_call(c);
		c+= (a+b);
	}

	printf("c=%d\n", c);

	MPI_Finalize();
	return 1;
}

#pragma ProjetCA mpicoll_check mpi_call
