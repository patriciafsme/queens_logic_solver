#include <iostream>        
#include <vector>         
#include <algorithm>       
#include <cstdlib>       
#include <ctime>          
#include <random>         
#include <omp.h>           
#include <chrono>         

using namespace std;     
using namespace chrono;   

// Function to generate a grid with contiguous regions
void generate_contiguous_grid(int n, vector<vector<int>>& board, vector<vector<pair<int, int>>>& regions) {
    // Assign a n x n board with initial values of 0
    board.assign(n, vector<int>(n, 0));
    
    // Clear the regions and resize it for n + 1 (to use indices from 1 to n)
    regions.clear();
    regions.resize(n + 1); 

    int current_region = 1;  // Start with region 1
    // Calculate the maximum number of cells each region can have
    int cells_per_region = (n * n + n - 1) / n;
    
    // Directions to move around the board (right, down, left, up)
    vector<pair<int, int>> directions = {{0, 1}, {1, 0}, {0, -1}, {-1, 0}};
    
    // Initialize random number generator
    random_device rd;
    mt19937 gen(rd()); 

    #pragma omp parallel for collapse(2) // Parallelize the loops
    for (int row = 0; row < n; ++row) {
        for (int col = 0; col < n; ++col) {
            if (board[row][col] == 0) {  // If the current cell is unvisited
                vector<pair<int, int>> stack = {{row, col}};
                // Perform region expansion until the region has enough cells
                while (!stack.empty() && regions[current_region].size() < cells_per_region) {
                    auto [r, c] = stack.back();
                    stack.pop_back();

                    if (r >= 0 && r < n && c >= 0 && c < n && board[r][c] == 0) {
                        board[r][c] = current_region;
                        regions[current_region].emplace_back(r, c);

                        shuffle(directions.begin(), directions.end(), gen); // Shuffle directions to randomize
                        for (auto [dr, dc] : directions) {
                            stack.emplace_back(r + dr, c + dc);  // Explore the neighboring cells
                        }
                    }
                }
                current_region = (current_region % n) + 1;  // Move to the next region
            }
        }
    }
}

// Function to check if it's safe to place a queen at (row, col)
bool is_safe(int row, int col, vector<bool>& rows, vector<bool>& cols, vector<bool>& main_diag, vector<bool>& anti_diag, int n) {
    return !(rows[row] || cols[col] || main_diag[row - col + n - 1] || anti_diag[row + col]);
}

// Solve the Queens problem with regions
bool solve_n_queens(int n, vector<vector<int>>& board, vector<vector<pair<int, int>>>& regions, vector<pair<int, int>>& queens) {
    vector<bool> rows(n, false), cols(n, false), main_diag(2 * n - 1, false), anti_diag(2 * n - 1, false);
    queens.clear();

    for (int region = 1; region <= n; ++region) {
        bool placed = false;

        // Shuffle positions within the region to try placing queens in random order
        auto& region_cells = regions[region];
        random_device rd;
        mt19937 gen(rd());
        shuffle(region_cells.begin(), region_cells.end(), gen);

        #pragma omp parallel for // Parallelize the queen placement check for each cell in the region
        for (int i = 0; i < region_cells.size(); ++i) {
            int row = region_cells[i].first;
            int col = region_cells[i].second;

            if (is_safe(row, col, rows, cols, main_diag, anti_diag, n)) {
                #pragma omp critical  // Ensure that only one queen is placed per region
                {
                    if (!placed) {
                        queens.emplace_back(row, col);
                        rows[row] = true;
                        cols[col] = true;
                        main_diag[row - col + n - 1] = true;
                        anti_diag[row + col] = true;
                        placed = true;
                    }
                }
            }
        }

        if (!placed) return false;  // No queen could be placed in this region
    }

    return true;
}

// Print the generated board
void print_board(int n, const vector<vector<int>>& board) {
    cout << "Generated Board:" << endl;
    for (const auto& row : board) {
        for (auto cell : row) {
            cout << cell << " ";
        }
        cout << endl;
    }
}

// Print the board with queens placed
void print_board_with_queens(int n, const vector<vector<int>>& board, const vector<pair<int, int>>& queens) {
    vector<vector<char>> display(n, vector<char>(n, '.'));
    for (auto [row, col] : queens) {
        display[row][col] = 'Q';  // Place queens on the board
    }

    cout << "Board with Queens:" << endl;
    for (const auto& row : display) {
        for (auto cell : row) {
            cout << cell << " ";
        }
        cout << endl;
    }
}

// Main function
int main() {
    int N = 12;  // Set the size of the board {8, 10, 12}

    vector<vector<int>> board;
    vector<vector<pair<int, int>>> regions;
    vector<pair<int, int>> queens;

    srand(time(0));

    // Record the start time for measuring execution duration
    auto start = high_resolution_clock::now();

    // Loop until a solution is found
    while (true) {
        generate_contiguous_grid(N, board, regions);  // Generate a board with contiguous regions
        if (solve_n_queens(N, board, regions, queens)) {  // Try to solve the N-Queens problem
            print_board(N, board);                      // Print the generated board
            print_board_with_queens(N, board, queens);  // Print the board with queens placed
            break;  // Exit the loop if a solution is found
        }
    }

    // Record the end time and calculate the duration
    auto end = high_resolution_clock::now();
    auto duration = duration_cast<milliseconds>(end - start);
    cout << "Time taken: " << duration.count() << " milliseconds" << endl;

    return 0;
}
