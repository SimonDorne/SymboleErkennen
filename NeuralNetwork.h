#ifndef NEURALNETWORK_H
#define NEURALNETWORK_H

#include <iostream>
#include <vector>
#include <cstdlib>
#include "Matrix.h"

inline float Sigmoid(float x) {
    return 1.0f / (1 + exp(-x));
}

inline float DSigmoid(float x) {
    return x * (1 - x);
}

class SimpleNeuralNetwork {
public:
    std::vector<uint32_t> _topology;
    std::vector<Matrix> _weightMatrices;
    std::vector<Matrix> _valueMatrices;
    std::vector<Matrix> _biasMatrices;
    float _learningRate;

public:
    SimpleNeuralNetwork(std::vector<uint32_t> topology, float learningRate = 0.2f);
    bool FeedForward(std::vector<float> input);
    bool backPropagate(std::vector<float> targetOutput);
    void saveToFile(const std::string& filename);
    SimpleNeuralNetwork loadFromFile(const std::string& filename);
    std::vector<float> getPrediction();
};

#endif // NEURALNETWORK_H
