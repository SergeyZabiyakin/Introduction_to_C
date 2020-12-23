#include "Percolation.h"
#include <stack>

Percolation::Percolation(size_t dimension2) {
    dimension = dimension2;
    open_cells = 0;
    table.assign(dimension, std::vector<Cell>());
    for (size_t i = 0; i < dimension; i++) {
        table[i].assign(dimension, Cell::LOCK);
    }
}

void Percolation::open(size_t row, size_t column) {
    if (table[row][column] == Cell::LOCK) {
        open_cells++;
        table[row][column] = Cell::OPEN;
        if ((row == dimension - 1)
            || (row + 1 < dimension && is_full(row + 1, column))
            || (column + 1 < dimension && is_full(row, column + 1))
            || (row != 0 && is_full(row - 1, column))
            || (column != 0 && is_full(row, column - 1))) {
            table[row][column] = Cell::FULL;
        }
        if (is_full(row, column)) {
            std::stack<std::pair<size_t, size_t >> stack;
            stack.push(std::make_pair(row, column));
            while (!stack.empty()) {
                std::pair<size_t, size_t> &pair = stack.top();
                stack.pop();
                row = pair.first;
                column = pair.second;
                table[row][column] = Cell::FULL;
                if (row + 1 < dimension && table[row + 1][column] == Cell::OPEN) {
                    stack.push(std::make_pair(row + 1, column));
                }
                if (column + 1 < dimension && table[row][column + 1] == Cell::OPEN) {
                    stack.push(std::make_pair(row, column + 1));
                }
                if (row != 0 && table[row - 1][column] == Cell::OPEN) {
                    stack.push(std::make_pair(row - 1, column));
                }
                if (column != 0 && table[row][column - 1] == Cell::OPEN) {
                    stack.push(std::make_pair(row, column - 1));
                }
            }
        }
    }
}

bool Percolation::is_open(size_t row, size_t column) const {
    return table[row][column] != Cell::LOCK;
}

bool Percolation::is_full(size_t row, size_t column) const {
    return table[row][column] == Cell::FULL;
}

size_t Percolation::get_numbet_of_open_cells() const {
    return open_cells;
}

bool Percolation::has_percolation() const {
    for (size_t i = 0; i < dimension; i++) {
        if (table[0][i] == Cell::FULL) {
            return true;
        }
    }
    return false;
}

void Percolation::clean() {
    open_cells = 0;
    table.clear();
    table.assign(dimension, std::vector<Cell>());
    for (size_t i = 0; i < dimension; i++) {
        table[i].assign(dimension, Cell::LOCK);
    }
}


