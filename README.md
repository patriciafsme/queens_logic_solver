# Queens Game Solver: From Python Logic to HPC with C++/MPI

## Project Overview
This project addresses the **LinkedIn Queens** logic puzzle through two distinct computational lenses: a functional Python implementation and a high-performance, parallelized version in **C++ using MPI**.

The objective is to place a queen in each colored region of an $N \times N$ grid such that no two queens share the same row, column, region, or are adjacent (even diagonally). While Python serves as the logical prototype, the C++ implementation explores the limits of state-space search through parallel computing.

## Technical Stack & Evolution
* **Prototyping:** Python (Backtracking algorithm).
* **High Performance:** C++ (Manual memory management and pointer logic).
* **Parallel Computing:** **MPI (Message Passing Interface)** for distributing the search space across multiple processors.
* **Tools:** Standard C++ libraries, MPI clusters.

## Key Features
* **Dual-Language Implementation:**
    * **Python:** Focused on readability and rapid constraint validation.
    * **C++ & MPI:** Optimized for massive $12 \times 12$ grids or larger, where the search space grows exponentially.
* **Parallel Search Space:** Implementation of MPI to split the root-level branches of the backtracking tree, allowing simultaneous exploration of different potential solutions.
* **Custom Constraint Engine:** * Non-adjacency rules (Queens cannot touch).
    * Color-region validation.
    * Standard row/column exclusion.
* **Scalability:** Handles everything from the standard $8 \times 8$ daily challenge to high-complexity $12 \times 12$ grids.

<img width="927" height="901" alt="image" src="https://github.com/user-attachments/assets/da7febf0-f2fa-48ef-97eb-68112c98449b" />

---
*Project developed during the specialization in Concurrent and Parallel Programming (UC Riverside).*
