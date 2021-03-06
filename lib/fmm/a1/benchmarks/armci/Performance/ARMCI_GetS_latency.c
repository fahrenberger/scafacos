/* 
 * The following is a notice of limited availability of the code, and disclaimer
 * which must be included in the prologue of the code and in all source listings
 * of the code.
 *
 * Copyright (c) 2010  Argonne Leadership Computing Facility, Argonne National
 * Laboratory
 *
 * Permission is hereby granted to use, reproduce, prepare derivative works, and
 * to redistribute to others.
 *
 *
 *                          LICENSE
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *
 * - Redistributions of source code must retain the above copyright
 *   notice, this list of conditions and the following disclaimer.
 *
 * - Redistributions in binary form must reproduce the above copyright
 *   notice, this list of conditions and the following disclaimer listed
 *   in this license in the documentation and/or other materials
 *   provided with the distribution.
 *
 * - Neither the name of the copyright holders nor the names of its
 *   contributors may be used to endorse or promote products derived from
 *   this software without specific prior written permission.
 *
 * The copyright holders provide no reassurances that the source code
 * provided does not infringe any patent, copyright, or any other
 * intellectual property rights of third parties.  The copyright holders
 * disclaim any liability to any recipient for claims brought against
 * recipient by any third party for infringement of that parties
 * intellectual property rights.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <armci.h>

#define MAX_DIM 1024 
#define ITERATIONS 100
#define SKIP 10

int main(int argc, char **argv)
{

    int i, j, rank, nranks, msgsize, dest;
    int dim;
    long bufsize;
    double **buffer;
    double t_start, t_stop, t_latency;
    int count[2], src_stride, trg_stride, stride_level;
    int provided;

    MPI_Init_thread(&argc, &argv, MPI_THREAD_MULTIPLE, &provided);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &nranks);

    ARMCI_Init_args(&argc, &argv);

    bufsize = MAX_DIM * MAX_DIM * sizeof(double);
    buffer = (double **) malloc(sizeof(double *) * nranks);
    ARMCI_Malloc((void **) buffer, bufsize);

    for (i = 0; i < bufsize / sizeof(double); i++)
    {
        *(buffer[rank] + i) = 1.0 + rank;
    }

    ARMCI_Barrier();

    if (rank == 0)
    {
        printf("ARMCI_GetS Latency in usec \n");
        printf("%30s %22s \n", "Dimensions(array of doubles)", "Latency");
        fflush(stdout);

        dest = 1;

        src_stride = MAX_DIM * sizeof(double);
        trg_stride = MAX_DIM * sizeof(double);
        stride_level = 1;

        for (dim = 1; dim <= MAX_DIM; dim *= 2)
        {

            count[0] = dim*sizeof(double);
            count[1] = 512;

                for (i = 0; i < ITERATIONS + SKIP; i++)
                {

                    if (i == SKIP) t_start = MPI_Wtime();

                    ARMCI_GetS((void *) buffer[dest],
                               &src_stride,
                               (void *) buffer[rank],
                               &trg_stride,
                               count,
                               stride_level,
                               1);

                }
                t_stop = MPI_Wtime();

                char temp[10];
                sprintf(temp, "%dX%d", count[1], count[0]);
                printf("%30s %20.2f \n", temp, ((t_stop - t_start) * 1000000)
                        / ITERATIONS);
                fflush(stdout);

                for (i = 0; i < count[1]; i++)
                {
                    for (j = 0; j < count[0]; j++)
                    {
                        if (*(buffer[rank] + i * MAX_DIM + j) != (1.0 + dest))
                        {
                            printf("Data validation failed at X: %d Y: %d Expected : %f Actual : %f \n",
                                   i,
                                   j,
                                   (1.0 + dest),
                                   *(buffer[rank] + i * MAX_DIM + j));
                            fflush(stdout);
                            return -1;
                        }
                    }
                }

        }

    }

    ARMCI_Barrier();

    ARMCI_Free((void *) buffer[rank]);

    ARMCI_Finalize();

    MPI_Finalize();

    return 0;
}
