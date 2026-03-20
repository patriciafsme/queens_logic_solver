#include <iostream>
#include <vector>
#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <random>

using namespace std;

// Función para generar un tablero con regiones contiguas
void generate_contiguous_grid(int n, vector<vector<int>>& board, vector<vector<pair<int, int>>>& regions) {
    board.assign(n, vector<int>(n, 0));
    regions.clear();
    regions.resize(n + 1);  // Índices de 1 a n

    int current_region = 1;
    int cells_per_region = (n * n + n - 1) / n;  // Tamaño máximo aproximado de cada región
    vector<pair<int, int>> directions = {{0, 1}, {1, 0}, {0, -1}, {-1, 0}};
    random_device rd;
    mt19937 gen(rd());

    for (int row = 0; row < n; ++row) {
        for (int col = 0; col < n; ++col) {
            if (board[row][col] == 0) {
                // Iniciar nueva región
                vector<pair<int, int>> stack = {{row, col}};
                while (!stack.empty() && regions[current_region].size() < cells_per_region) {
                    auto [r, c] = stack.back();
                    stack.pop_back();

                    if (r >= 0 && r < n && c >= 0 && c < n && board[r][c] == 0) {
                        board[r][c] = current_region;
                        regions[current_region].emplace_back(r, c);

                        // Barajar direcciones para aleatoriedad
                        shuffle(directions.begin(), directions.end(), gen);
                        for (auto [dr, dc] : directions) {
                            stack.emplace_back(r + dr, c + dc);
                        }
                    }
                }
                current_region = (current_region % n) + 1;
            }
        }
    }
}

// Verificar si es seguro colocar una reina
bool is_safe(int row, int col, vector<bool>& rows, vector<bool>& cols, vector<bool>& main_diag, vector<bool>& anti_diag, int n) {
    return !(rows[row] || cols[col] || main_diag[row - col + n - 1] || anti_diag[row + col]);
}

// Resolver el problema de las n-reinas con regiones
bool solve_n_queens(int n, vector<vector<int>>& board, vector<vector<pair<int, int>>>& regions, vector<pair<int, int>>& queens) {
    vector<bool> rows(n, false), cols(n, false), main_diag(2 * n - 1, false), anti_diag(2 * n - 1, false);
    queens.clear();

    for (int region = 1; region <= n; ++region) {
        bool placed = false;

        // Mezclar posiciones dentro de la región
        auto& region_cells = regions[region];
        random_device rd;
        mt19937 gen(rd());
        shuffle(region_cells.begin(), region_cells.end(), gen);

        for (auto [row, col] : region_cells) {
            if (is_safe(row, col, rows, cols, main_diag, anti_diag, n)) {
                queens.emplace_back(row, col);
                rows[row] = true;
                cols[col] = true;
                main_diag[row - col + n - 1] = true;
                anti_diag[row + col] = true;
                placed = true;
                break;
            }
        }

        if (!placed) return false;  // No se pudo colocar una reina en esta región
    }

    return true;
}

// Imprimir el tablero generado
void print_board(int n, const vector<vector<int>>& board) {
    cout << "Generated Board:" << endl;
    for (const auto& row : board) {
        for (auto cell : row) {
            cout << cell << " ";
        }
        cout << endl;
    }
}

// Imprimir el tablero con reinas
void print_board_with_queens(int n, const vector<vector<int>>& board, const vector<pair<int, int>>& queens) {
    vector<vector<char>> display(n, vector<char>(n, '.'));
    for (auto [row, col] : queens) {
        display[row][col] = 'Q';
    }

    cout << "Board with Queens:" << endl;
    for (const auto& row : display) {
        for (auto cell : row) {
            cout << cell << " ";
        }
        cout << endl;
    }
}

// Función principal
int main() {
    int N;
    cout << "IGrid size (N): ";
    cin >> N;

    vector<vector<int>> board;
    vector<vector<pair<int, int>>> regions;
    vector<pair<int, int>> queens;

    srand(time(0));

    while (true) {
        generate_contiguous_grid(N, board, regions);
        if (solve_n_queens(N, board, regions, queens)) {
            print_board(N, board);
            print_board_with_queens(N, board, queens);
            break;
        }
    }

    return 0;
}
