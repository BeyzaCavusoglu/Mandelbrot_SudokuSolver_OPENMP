#!/usr/bin/env bash
#
# You should only work under the /scratch/users/<username> directory.
#
# Example job submission script
#
# -= Resources =-
#
#SBATCH --job-name=mandelbrot-jobs
#SBATCH --nodes=1
#SBATCH --ntasks-per-node=16
#SBATCH --partition=shorter
#SBATCH --time=00:05:00
#SBATCH --output=mandelbrot-jobs.out

################################################################################
##################### !!! DO NOT EDIT ABOVE THIS LINE !!! ######################
################################################################################
# Set stack size to unlimited
echo "Setting stack size to unlimited..."
ulimit -s unlimited
ulimit -l unlimited
ulimit -a

echo "Loading GCC 11..."
module load gcc/11.2.0

echo

echo "Running Job...!"
echo "==============================================================================="
echo "Running compiled binary..."

echo "Parallel version with 16 threads"
export OMP_NUM_THREADS=16
make -C ./mandelbrot run
