#!/bin/bash

#PBS -l select=1:ncpus=1:mem=8gb
#PBS -l walltime=00:10:00

#PBS -J 0-29

#PBS -q short_cpuQ
#PBS -N MergeSort
#PBS -o mergesort.log
#PBS -e mergesort_error.log

# #PBS -l place=scatter:excl
# #PBS -m abe
# #PBS -M email1 email2

module load mpich-3.2
cd $PBS_O_WORKDIR

SIZE=$((1 << $PBS_ARRAY_INDEX)) # up to 2gb
NPROCS=$(wc -l < $PBS_NODEFILE)

# main SIZE REPETITIONS ALGORITHM SEED
#mpirun.actual -n $NPROCS ./main $SIZE 10 2 0 > Results/mergesort_2_${NPROCS}_${PBS_ARRAY_INDEX}.log
#mpirun.actual -n $NPROCS ./main $SIZE 10 1 0 > Results/mergesort_1_${NPROCS}_${PBS_ARRAY_INDEX}.log
mpirun.actual -n 1 ./main $SIZE 10 0 0 > Results/mergesort_0_${NPROCS}_${PBS_ARRAY_INDEX}.log
