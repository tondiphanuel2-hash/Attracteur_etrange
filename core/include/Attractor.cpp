#include "Attractor.h"
#include "Utils.h"
#include <glm/glm.hpp>

Attractor::Attractor() : parameterA(1.0f), parameterB(2.0f), speed(1.0f), x(0.0f), y(0.0f) {}

void Attractor::initialize() {
    points.reserve(10000); // Reserve space for efficiency
    x = 0.1f;
    y = 0.1f;
}

void Attractor::update() {
    for (int i = 0; i < speed; ++i) {
        float newX = sin(parameterA * y);
        float newY = sin(parameterB * x);
        x += newX * 0.01f;
        y += newY * 0.01f;
        points.emplace_back(x, y);
    }
}

const std::vector<glm::vec2>& Attractor::getPoints() const {
    return points;
}
/**
 * Attractor.cpp
 * 
 * Contient l'implémentation des méthodes de la classe Attractor.
 */