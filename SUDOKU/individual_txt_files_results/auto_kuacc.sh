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


# Compilation configuration descriptions
CONFIG_DESC="gcc -o sudoku_solver sudoku_solver.c -fopenmp -lm" #original serial
CONFIG_DESC_PARTA="gcc -o sudoku_solver_parta sudoku_solver_parta.c -fopenmp -lm"  #part a parallel
CONFIG_DESC_PARTB="gcc -o sudoku_solver_partb sudoku_solver_partb.c -fopenmp -lm" #part b parallel
CONFIG_DESC_PARTC_SERIAL="gcc -o sudoku_solver_partc_serial sudoku_solver_partc_serial.c -fopenmp -lm"  #one solution serial
CONFIG_DESC_PARTC="gcc -o sudoku_solver_partc sudoku_solver_partc.c -fopenmp -lm"  #part c parallel

compile_and_run() {
  local config_desc="$1"
  local program_name="$2"
  local thread_count="$3"
  local input_file="$4"
  local output_file_suffix="$5"

  local output_file="output_${thread_count}th_${output_file_suffix}.txt"

  echo "Compiling: $config_desc"
  $config_desc
  echo "Running with $thread_count thread(s)"
  export OMP_NUM_THREADS=$thread_count
  { time ./$program_name 16 "$input_file"; } 2>&1 | tee "$output_file"

  # Extract elapsed time and append it to the final output file
  elapsed_time=$(grep "real" "$output_file" | awk '{print $2}')
  echo "$config_desc, Threads: $thread_count, Input File: $input_file, Elapsed Time: $elapsed_time" >> "$final_output_file"
}

# Array of thread counts
thread_counts=(1 2 4 8 16 32)

# Loop through all files starting with "4x4"
input_files=(grids/4x4*.csv)

final_output_file="final_output.txt"

# Create an empty final output file
echo "" > "$final_output_file"

for thread_count in "${thread_counts[@]}"; do
  for input_file in "${input_files[@]}"; do
    output_file_suffix=$(basename "$input_file" .csv)
    
    compile_and_run "$CONFIG_DESC" "sudoku_solver" "$thread_count" "$input_file" "$output_file_suffix"
    compile_and_run "$CONFIG_DESC_PARTA" "sudoku_solver_parta" "$thread_count" "$input_file" "$output_file_suffix"
    compile_and_run "$CONFIG_DESC_PARTB" "sudoku_solver_partb" "$thread_count" "$input_file" "$output_file_suffix"
    compile_and_run "$CONFIG_DESC_PARTC" "sudoku_solver_partc" "$thread_count" "$input_file" "$output_file_suffix"
    compile_and_run "$CONFIG_DESC_PARTC_SERIAL" "sudoku_solver_partc_serial" "$thread_count" "$input_file" "$output_file_suffix"

  done
done

echo "Automation completed!"
