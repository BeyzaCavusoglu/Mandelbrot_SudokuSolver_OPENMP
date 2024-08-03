# Mandelbrot_SudokuSolver_OPENMP
Parallelizing two different applications, a Mandelbrot generator and Sudoku solver using OpenMP.

## PART I: MandelBrot
  Converting a serial version of a simple MandelBrot generator into a parallel one using OpenMP. The serial version is provided which produces a mandelbrot image given specific zoom and movement directions (you can specify which region in the mandelbrot to zoom into). 

### Tests:

Several experiments were done for the performance which are scheduling and scaling tests.

a) Scheduling Test
The speedup of the parallel code under different OpenMP schedule clauses is tested.

• The baseline is the static scheduler.
• Performance is tested with a dynamic scheduler, dynamic scheduler with chunksize 10,
and dynamic scheduler with a chunksize of choice.

b) Scalability Test
The speedup of the parallel code under different numbers of threads.

• Here, baseline is the serial code.
• In addition, the parallel execution time with single thread is reported and checked if there is any parallelization overhead by comparing the running times of the serial version with the parallel version with one thread.
• Performance is tested under serial, 1, 2, 4, 8, 16, and 32 threads.

## PART II: Parallel Sudoku Solver
Parallelizing a serial sudoku solver with OpenMP. Serial program takes a sudoku problem as an input and finds all possible solutions from it. Originally, a brute
force search is used for searching for all possible solutions to the problem. The idea is to generate every possible move by trying each number within the game and then test to see whether it satisfies the sudoku (every number appears only once in a column, once in a row and once in a square block). We provided 9 by 9 matrix for debugging purposes, which doesn’t take much time to solve. 16 by 16 matrices are the ones I used for performance study.

**Part-II-A Task Parallelism:**

Parallelizing the sudoku solver with task parallelism.

**Part-II-B Task Parallelism with Cutoff:**

The first implementation results in too many tasks in the system which can easily degrade the performance. As a result, any speedup or very disappointing speedup can be experienced. This part is implementing an optimization to improve the performance of the parallel code by using a cutoff parameter to limit the number of parallel tasks in code. To limit task generations, beyond certain depth in the call-path tree of the recursive function, switched to the serial execution and tasks are not generated. This depth is determined by the cutoff parameter choosen.

**Part-II-C Task Parallelism with Early Termination:**

Using the implementation in Part-A, modified the code so that it stops after finding one solution. To make a fair performance comparison, serial version is also modified so that it also stops after finding one solution to sudoku.

### Tests
For performance studies, 4x4 hard 3.csv is used.

a) Scalability Test
The same scalability test performed as described in Part-I.
• For the parallel code from Part-A and B, the speedup computed with respect to the given serial sudoku solver.
• For the parallel code from Part-C, the speedup computed with respect to a modified version of the given serial sudoku solver that also terminates after finding one
valid solution.

b) Tests on Sudoku Problems of Different Grids
For this test, the running times of the parallel code from Part-B on sudoku problems of different sizes and difficulties by using 32 threads is reported. Three sudoku problems are provided, those are 4x4 hard1, 2 and 3.


For more details, please refer to "ToDo.pdf" file.

*Note: This project is done for a course project, the serialized version of this code was already designed and given by the instructor. My implementation was to parallelize them using OPENMP and conducting the performance tests.*
