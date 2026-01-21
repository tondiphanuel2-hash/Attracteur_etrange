#include "Renderer.h"

Renderer::Renderer() : pointColor({255, 255, 255, 255}) {}

Renderer::~Renderer() {}

void Renderer::initialize() {
    // Set up any necessary OpenGL configurations
}

void Renderer::clear() {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
}

void Renderer::render(const Attractor& attractor) {
    const auto& points = attractor.getPoints();
    glBegin(GL_POINTS);
    for (const auto& point : points) {
        glColor3f(pointColor.r / 255.0f, pointColor.g / 255.0f, pointColor.b / 255.0f);
        glVertex2f(point.x, point.y);
    }
    glEnd();
}
/**
 * Renderer.cpp
 * 
 * Contient l'implémentation des méthodes de rendu.
 */