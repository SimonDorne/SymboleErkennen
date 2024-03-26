#include "Matrix.h"

Matrix::Matrix() : _cols(0), _rows(0), _vals({}) {}

Matrix::Matrix(uint32_t cols, uint32_t rows) : _cols(cols), _rows(rows), _vals({}) {
    _vals.resize(cols * rows, 0.0f);
}

float &Matrix::at(uint32_t col, uint32_t row){
    return _vals[row * _cols + col];
}

Matrix Matrix::multiply(Matrix target) {
    assert(_cols == target._rows);
    Matrix output(target._cols, _rows);
    for (uint y = 0; y < output._rows; y++) {
        for (uint x = 0; x < output._cols; x++) {
            float result = 0.0f;
            for (uint32_t k = 0; k < _cols; k++) {
                result += at(k, y) * target.at(x, k);
            }
            output.at(x, y) = result;
        }
    }
    return output;
}

Matrix Matrix::multiplyScalar(float s) {
    Matrix output(_cols, _rows);
    for (uint y = 0; y < output._rows; y++) {
        for (uint x = 0; x < output._cols; x++) {
            output.at(x, y) = at(x, y) * s;
        }
    }
    return output;
}

Matrix Matrix::addScalar(float s) {
    Matrix output(_cols, _rows);
    for (uint y = 0; y < output._rows; y++) {
        for (uint x = 0; x < output._cols; x++) {
            output.at(x, y) = at(x, y) + s;
        }
    }
    return output;
}

Matrix Matrix::add(Matrix target) {
    assert(_rows == target._rows && _cols == target._cols);
    Matrix output(_cols, _rows);
    for (uint y = 0; y < output._rows; y++) {
        for (uint x = 0; x < output._cols; x++) {
            output.at(x, y) = at(x, y) + target.at(x, y);
        }
    }
    return output;
}

Matrix Matrix::negative() {
    Matrix output(_cols, _rows);
    for (uint y = 0; y < output._rows; y++) {
        for (uint x = 0; x < output._cols; x++) {
            output.at(x, y) = -at(x, y);
        }
    }
    return output;
}

Matrix Matrix::transpose() {
    Matrix output(_rows, _cols);
    for (uint y = 0; y < _rows; y++) {
        for (uint x = 0; x < _cols; x++) {
            output.at(y, x) = at(x, y); // Fix: Changed '-' to '+'
        }
    }
    return output;
}

Matrix Matrix::applyFunction(std::function<float(const float)> func) {
    Matrix output(_cols, _rows);
    for (uint y = 0; y < output._rows; y++) {
        for (uint x = 0; x < output._cols; x++) {
            output.at(x, y) = func(at(x, y));
        }
    }
    return output;
}

Matrix Matrix::multiplyElements(Matrix target) {
    assert(_rows == target._rows && _cols == target._cols);
    Matrix output(_cols, _rows);
    for (uint y = 0; y < output._rows; y++) {
        for (uint x = 0; x < output._cols; x++) {
            output.at(x, y) = at(x, y) * target.at(x, y);
        }
    }
    return output;
}

