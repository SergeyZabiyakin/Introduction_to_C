#include <Percolation.h>
#include "PercolationStats.h"
#include <random>
#include <rand_generator.h>

PercolationStats::PercolationStats(size_t dimension2, size_t trials2) {
    dimension = dimension2;
    sells = dimension * dimension;
    trials = trials2;
    answers.assign(trials2, 0);
    execute();
}

double PercolationStats::get_mean() const {
    double out = 0;
    for (size_t i = 0; i < trials; i++) {
        out += answers[i];
    }
    return out / static_cast<double>(trials);
}

double PercolationStats::get_standard_deviation() const {
    double x = get_mean();
    double out = 0;
    for (size_t i = 0; i < trials; i++) {
        out += (answers[i] - x) * (answers[i] - x);
    }
    return sqrt(out / static_cast<double>(trials - 1));
}

double PercolationStats::get_confidence_low() const {
    double x = get_mean();
    double s = get_standard_deviation();
    return x - (1.96 * s) / sqrt(trials);
}

double PercolationStats::get_confidence_high() const {
    double x = get_mean();
    double s = get_standard_deviation();
    return x + (1.96 * s) / sqrt(trials);
}

void PercolationStats::execute() {
    Percolation p(dimension);
    for (size_t i = 0; i < trials; i++) {
        while (!p.has_percolation()) {
            p.open((dimension) * get_rand(), (dimension) * get_rand());
        }
        answers[i] = static_cast<double>(p.get_numbet_of_open_cells()) /
                     static_cast<double>(sells);
        p.clean();
    }
}

