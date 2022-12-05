#!/bin/bash

#PBS -l select=2:ncpus=1:mem=2gb
#PBS -l place=scatter:excl
#PBS -l walltime=0:10:0

#PBS -J 0-3

#PBS -q short_cpuQ
#PBS -N MergeSort
#PBS -o mergesort.log
#PBS -e mergesort_error.log

# #PBS -m abe
# #PBS -M email1 email2

module load mpich-3.2
cd $PBS_O_WORKDIR

SIZE=$((1 << $PBS_ARRAY_INDEX)) # up to 2gb
NPROCS=$(wc -l < $PBS_NODEFILE)

# main SIZE REPETITIONS ALGORITHM SEED
mpirun.actual -n $NPROCS ./main $SIZE 10 2 0 > mergesort_2_${NPROCS}_${PBS_ARRAY_INDEX}.log
mpirun.actual -n $NPROCS ./main $SIZE 10 1 0 > mergesort_1_${NPROCS}_${PBS_ARRAY_INDEX}.log
mpirun.actual -n 1 ./main $SIZE 10 0 0 > mergesort_0_${NPROCS}_${PBS_ARRAY_INDEX}.log
