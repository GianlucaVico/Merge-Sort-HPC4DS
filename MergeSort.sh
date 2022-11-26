#!/bin/bash

#PBS -l select=2:ncpus=1:mem=8gb
#PBS -l place=scatter:excl
#PBS -l walltime=0:10:0

#PBS -J 1-30

#PBS -q short_cpuQ
#PBS -N MergeSort
#PBS -o mergesort_.log
#PBS -e mergesort_error.log

#PBS -m abe
# #PBS -M email1 email2

module load mpich-3.2
cd $PBS_O_WORKDIR

SIZE=$((2 << $PBS_ARRAY_INDEX)) # up to 2gb

# main SIZE REPETITIONS ALGORITHM SEED
mpirun.actual -n $PBS_NNODES ./main $SIZE 10 2 0
mpirun.actual -n $PBS_NNODES ./main $SIZE 10 1 0
mpirun.actual -n 1 ./main $SIZE 10 0 0
