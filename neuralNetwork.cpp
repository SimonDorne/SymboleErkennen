#include "NeuralNetwork.h"
#include<fstream>
#include<sstream>
SimpleNeuralNetwork::SimpleNeuralNetwork(std::vector<uint32_t> topology, float learningRate)
    : _topology(topology), _weightMatrices({}), _valueMatrices({}), _biasMatrices({}), _learningRate(learningRate) {
    for (uint32_t i = 0; i < topology.size() - 1; i++) {
        Matrix weightMatrix(topology[i + 1], topology[i]);
        weightMatrix = weightMatrix.applyFunction([](const float &f) {
            return ((float)rand() / (float)RAND_MAX )/5;
        });
        _weightMatrices.push_back(weightMatrix);

        Matrix biasMatrix(topology[i + 1], 1);
        biasMatrix = biasMatrix.applyFunction([](const float &f) {
            return ((float)rand() / (float)RAND_MAX)/5;
        });
        _biasMatrices.push_back(biasMatrix);
    }
    _valueMatrices.resize(topology.size());
    std::cout<<"Weight and Bias Matrizes initalized"<<std::endl;
}

bool SimpleNeuralNetwork::FeedForward(std::vector<float> input) {
    if (input.size() != _topology[0]) {
        std::cout<<"error, FF not equal"<<std::endl;
        return false;
    }
    Matrix values(input.size(),1);
    for(uint32_t i=0;i<input.size();i++){
        values._vals[i]=input[i];
    }
    //feed forward to next layer
    for(uint32_t i=0;i<_weightMatrices.size();i++){
        _valueMatrices[i]=values;
        values=values.multiply(_weightMatrices[i]);
        values=values.add(_biasMatrices[i]);
        values=values.applyFunction(Sigmoid);
    }
    _valueMatrices.back()=values;
    return true;
}

bool SimpleNeuralNetwork::backPropagate(std::vector<float> targetOutput) {
    if (targetOutput.size() != _topology.back()) {
        std::cout<<"error, bP not equal: "<<targetOutput.size()<<" !="<<_topology.back()<<std::endl;
        return false;
    }
    Matrix errors(targetOutput.size(),1);
    errors._vals=targetOutput;
    errors=errors.add(_valueMatrices.back().negative());
    for(int i=_weightMatrices.size()-1;i>=0;i--){
        Matrix prevErrors=errors.multiply(_weightMatrices[i].transpose());
        Matrix dOutputs=_valueMatrices[i+1].applyFunction(
            DSigmoid
            );
        Matrix gradients= errors.multiplyElements(dOutputs);
        gradients=gradients.multiplyScalar(_learningRate);
        Matrix weightGradients=_valueMatrices[i].transpose().multiply(gradients);
        _weightMatrices[i]=_weightMatrices[i].add(weightGradients);
        _biasMatrices[i]=_biasMatrices[i].add(gradients);
        errors=prevErrors;
    }
    return true;
}

std::vector<float> SimpleNeuralNetwork::getPrediction() {
    return _valueMatrices.back()._vals;
}

void SimpleNeuralNetwork::saveToFile(const std::string& filename) {
    std::ofstream outputFile(filename);

    if (!outputFile.is_open()) {
        std::cerr << "Error: Unable to open file " << filename << " for writing." << std::endl;
        return;
    }

    // Schreibe Topologie in die Datei
    outputFile << "Topology:";
    for (const auto& layerSize : _topology) {
        outputFile << " " << layerSize;
    }
    outputFile << std::endl;

    // Schreibe Lernrate in die Datei
    outputFile << "LearningRate: " << _learningRate << std::endl;

    // Schreibe Gewichtsmatrizen in die Datei
    outputFile << "WeightMatrices:" << std::endl;
    for (auto& weightMatrix : _weightMatrices) {
        outputFile << weightMatrix._cols << " " << weightMatrix._rows << std::endl;
        for (uint32_t i = 0; i < weightMatrix._cols; ++i) {
            for (uint32_t j = 0; j < weightMatrix._rows; ++j) {
                outputFile << weightMatrix.at(i, j) << " ";
            }
            outputFile << std::endl;
        }
    }

    // Schreibe Biasmatrizen in die Datei
    outputFile << "BiasMatrices:" << std::endl;
    for (auto& biasMatrix : _biasMatrices) {
        outputFile << biasMatrix._cols << " " << biasMatrix._rows << std::endl;
        for (uint32_t i = 0; i < biasMatrix._cols; ++i) {
            for (uint32_t j = 0; j < biasMatrix._rows; ++j) {
                outputFile << biasMatrix.at(i, j) << " ";
            }
            outputFile << std::endl;
        }
    }

    // Schließe die Datei
    outputFile.close();
}

SimpleNeuralNetwork SimpleNeuralNetwork::loadFromFile(const std::string& filename) {
    std::ifstream inputFile(filename);
    if (!inputFile.is_open()) {
        std::cerr << "Error: Unable to open file " << filename << " for reading." << std::endl;
        // Rückgabe eines leeren SimpleNeuralNetwork-Objekts, um Fehler zu signalisieren
        throw std::runtime_error("Error: Unable to open file " + filename + " for reading.");
        return SimpleNeuralNetwork({}, 0.1f); // Annahme: Standard-Lernrate von 0.1
    }
    else{

    std::string line;
    std::getline(inputFile, line); // Lese Topologie
    std::istringstream topologyStream(line.substr(9)); // Ignoriere "Topology:" und lese den Rest
    std::vector<uint32_t> topology;

    uint32_t layerSize;
    while (topologyStream >> layerSize) {
        topology.push_back(layerSize);
    }

    std::getline(inputFile, line); // Lese Lernrate
    float learningRate;
    std::istringstream(line.substr(13)) >> learningRate; // Ignoriere "LearningRate:" und lese den Rest
    SimpleNeuralNetwork network(topology, learningRate);


    std::cout << "Letzte Schicht der Gewichtsmatrix:" << std::endl;
    for (uint32_t i = 0; i < _weightMatrices.back()._cols; ++i) {
        for (uint32_t j = 0; j < _weightMatrices.back()._rows; ++j) {
                std::cout << _weightMatrices.back().at(i, j) << " ";
        }
        std::cout << std::endl;
    }

    // Lese Gewichtsmatrizen
    std::getline(inputFile, line); // Ignoriere "WeightMatrices:"
    for (int i = 0; i < network._weightMatrices.size(); ++i) {
        uint32_t cols, rows;
        std::getline(inputFile, line);
        std::istringstream sizeStream(line);
        sizeStream >> cols >> rows;

        // Neue Matrix erstellen
        Matrix currentWeightMatrix(cols, rows);
        // Werte direkt in die Matrix eintragen
        for (uint32_t i = 0; i < cols; ++i) {
                std::getline(inputFile, line);
                std::istringstream valuesStream(line);
                for (uint32_t j = 0; j < rows; ++j) {
                    float value;
                    valuesStream >> value;
                    currentWeightMatrix.at(i, j) = value;// Wert direkt in die Matrix eintragen
                }
        }
        std::cout<<"Reihen: "<<currentWeightMatrix._rows<<" Spalten: "<<currentWeightMatrix._cols<<std::endl;
        // Aktuelle Matrix der Liste hinzufügen
        _weightMatrices[i]=currentWeightMatrix;
    }

    // Lese Biasmatrizen
    std::getline(inputFile, line); // Ignoriere "BiasMatrices:"
    for (int i = 0; i < network._biasMatrices.size(); ++i) {
        uint32_t cols, rows;
        std::getline(inputFile, line);
        std::istringstream sizeStream(line);
        sizeStream >> cols >> rows;

        // Neue Matrix erstellen
        Matrix currentBiasMatrix(cols, rows);

        // Werte direkt in die Matrix eintragen
        for (uint32_t i = 0; i < cols; ++i) {
            std::getline(inputFile, line);
            std::istringstream valuesStream(line);
            for (uint32_t j = 0; j < rows; ++j) {
                float value;
                valuesStream >> value;
                currentBiasMatrix.at(i, j) = value; // Wert direkt in die Matrix eintragen
            }
        }

        // Aktuelle Matrix der Liste hinzufügen
        _biasMatrices[i]=currentBiasMatrix;
    }

    //std::cout<<"File eingelesen"<<std::endl;
    // Schließe die Datei
    inputFile.close();
    std::cout<<"loading was successfull"<<std::endl;
    return network;
    }
}


/*
    std::cout << "Letzte Schicht der Gewichtsmatrix:" << std::endl;
    for (uint32_t i = 0; i < _weightMatrices.back()._rows; ++i) {
        for (uint32_t j = 0; j < _weightMatrices.back()._cols; ++j) {
            std::cout << _weightMatrices.back().at(i, j) << " ";
        }
        std::cout << std::endl;
    }

    std::cout << "Letzte Schicht der Biasmatrix:" << std::endl;
    for (uint32_t i = 0; i < _biasMatrices.back()._rows; ++i) {
        std::cout << _biasMatrices.back().at(i, 0) << std::endl;
    }
*/
