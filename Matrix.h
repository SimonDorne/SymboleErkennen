#ifndef MATRIX_H
#define MATRIX_H

#include <cstdint>
#include <vector>
#include <cmath>
#include <cassert>
#include <functional>

class Matrix {
public:
    uint32_t _cols;
    uint32_t _rows;
    std::vector<float> _vals;

public:

    Matrix();
    Matrix(uint32_t cols, uint32_t rows);
    float &at(uint32_t col, uint32_t row);
    Matrix multiply(Matrix target);
    Matrix multiplyScalar(float s);
    Matrix addScalar(float s);
    Matrix add(Matrix target);
    Matrix negative();
    Matrix transpose();
    Matrix applyFunction(std::function<float(const float)> func);
    Matrix multiplyElements(Matrix target);
};

#endif // MATRIX_H
