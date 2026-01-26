#include <SDL/Graphics.hpp>
#include <vector>
#include <cmath>
#include "Attractor.h"
#include "Utils.h"

struct Point { float x, y, z; };

class Attractor {
public:
    int type = 1;
    Point p = {0.1, 0.0, 0.0};
    float dt = 0.05;

    void update() {
        float dx, dy, dz;
        if (type == 1) { // Lorenz
            dx = (10 * (p.y - p.x));
            dy = (p.x * (28 - p.z) - p.y);
            dz = (p.x * p.y - (8.0f/3.0f) * p.z);
        } else if (type == 2) { // Rössler
            dx = (-p.y - p.z);
            dy = (p.x + 0.2f * p.y);
            dz = (0.2f + p.z * (p.x - 5.7f));
        } else if (type == 3) { // Aizawa
            dx = (p.z - 0.7f) * p.x - 3.5f * p.y;
            dy = 3.5f * p.x + (p.z - 0.7f) * p.y;
            dz = 0.6f + 0.95f * p.z - (std::pow(p.z, 3)/3.0f) - (std::pow(p.x, 2) + std::pow(p.y, 2)) * (1.0f + 0.25f * p.z) + 0.1f * p.z * std::pow(p.x, 3);
        } else if (type == 4) { // Thomas
            dx = std::sin(p.y) - 0.2081f * p.x;
            dy = std::sin(p.z) - 0.2081f * p.y;
            dz = std::sin(p.x) - 0.2081f * p.z;
            dt = 0.1; // Plus lent pour Thomas
        } else if (type == 5) { // Hénon (Adapté en continu pour le tracé)
            dx = (1.0f - 1.4f * p.x * p.x + p.y) - p.x;
            dy = (0.3f * p.x) - p.y;
            dz = 0;
        } else if (type == 6) { // Ikeda
    float t = 0.4f - 6.0f / (1.0f + p.x * p.x + p.y * p.y);
    dx = (1.0f + 0.9f * (p.x * cos(t) - p.y * sin(t))) - p.x;
    dy = (0.9f * (p.x * sin(t) + p.y * cos(t))) - p.y;
    dz = 0;
}
else if (type == 7) { // Duffing
    dx = p.y;
    dy = (p.x - p.x * p.x * p.x - 0.35f * p.y + 0.3f * cos(p.z));
    dz = 1.4f; // Évolution de la phase temporelle
}
else if (type == 8) { // Van der Pol
    float mu = 1.5f;
    dx = mu * (p.x - (1.0f/3.0f) * p.x * p.x * p.x - p.y);
    dy = p.x / mu;
    dz = 0;
}
else if (type == 9) { // Clifford (Pickover)
    // Paramètres classiques : a=1.5, b=-1.8, c=1.6, d=2.0
    dx = (sin(1.5f * p.y) + 1.6f * cos(1.5f * p.x)) - p.x;
    dy = (sin(-1.8f * p.x) + 2.0f * cos(-1.8f * p.y)) - p.y;
    dz = 0;
}
else if (type == 10) { // Gumowski-Mira
    auto f_gm = [](float x) { return -0.75f * x + 2.0f * (1.0f + 0.75f) * x * x / (1.0f + x * x); };
    float nextX = 0.05f * p.y + f_gm(p.x);
    dx = nextX - p.x;
    dy = (-p.x + f_gm(nextX)) - p.y;
    dz = 0;
}
else if (type == 11) { // Chua (Circuit)
    
    float h = -0.714f * p.x + 0.5f * (-1.143f + 0.714f) * (abs(p.x + 1.0f) - abs(p.x - 1.0f));
    dx = 15.6f * (p.y - p.x - h);
    dy = p.x - p.y + p.z;
    dz = -28.0f * p.y;
}
else if (type == 12) { // Tamari
    dx = p.x - p.y * p.z;
    dy = p.x * p.z - p.y;
    dz = p.z + (p.x * p.y / 3.0f);
}
else if (type == 13) { // Kaplan-Yorke
    dx = (2.0f * p.x - floor(2.0f * p.x)) - p.x; // Modulo 1 simplifié
    dy = (0.2f * p.y + cos(4.0f * 3.14159f * p.x)) - p.y;
    dz = 0;
}
else (type == 14) { // Double Scroll
    dx = 0.7f * (p.y - p.x);
    dy = p.x - p.x * p.z + 7.0f * p.y;
    dz = p.x * p.y - 0.7f * p.z;
}

        p.x += dx * dt; p.y += dy * dt; p.z += dz * dt;
    }
};

int main() {
    sf::RenderWindow window(sf::VideoMode(1000, 800), "Attracteurs Etranges 2026");
    window.setFramerateLimit(144);
    
    Attractor att;
    std::vector<sf::Vertex> points;
    float zoom = 15.0f;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) window.close();
            if (event.type == sf::Event::KeyPressed) {
                points.clear();
                
                if (event.key.code == sf::Keyboard::Num1) { att.type = 1; points.clear(); att.p = {0.1, 0, 0}; zoom = 15; }
                if (event.key.code == sf::Keyboard::Num2) { att.type = 2; points.clear(); att.p = {0.1, 0, 0}; zoom = 15; }
                if (event.key.code == sf::Keyboard::Num3) { att.type = 3; points.clear(); att.p = {0.1, 1, 0}; zoom = 150; }
                if (event.key.code == sf::Keyboard::Num4) { att.type = 4; points.clear(); att.p = {0.1, 1, 0}; zoom = 150; }
                if (event.key.code == sf::Keyboard::Num5) { att.type = 5; points.clear(); att.p = {0.1, 1, 0}; zoom = 150; }
                if (event.key.code == sf::Keyboard::Num6) { att.type = 6; points.clear(); att.p = {0.1, 1, 0}; zoom = 150; }
                if (event.key.code == sf::Keyboard::Num7) { att.type = 7; points.clear(); att.p = {0.1, 1, 0}; zoom = 150; }
                if (event.key.code == sf::Keyboard::Num8) { att.type = 8; points.clear(); att.p = {0.1, 1, 0}; zoom = 150; }
                if (event.key.code == sf::Keyboard::Num9) { att.type = 9; points.clear(); att.p = {0.1, 1, 0}; zoom = 150; }
                if (event.key.code == sf::Keyboard::Num10) { att.type = 10; points.clear(); att.p = {0.1, 1, 0}; zoom = 150; }
                if (event.key.code == sf::Keyboard::Num11) { att.type = 11; points.clear(); att.p = {0.1, 1, 0}; zoom = 150; }
                if (event.key.code == sf::Keyboard::Num12) { att.type = 12; points.clear(); att.p = {0.1, 1, 0}; zoom = 200; }
                if (event.key.code == sf::Keyboard::Num13) { att.type = 13; points.clear(); att.p = {0.1, 0.1, 1}; zoom = 200; }

                /Modification de l attracteur 14 (Double scroll)
                 if (event.key.code == sf::Keyboard::Num1) { 
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift)) { // Utilise Shift+1 pour le 14
            att.type = 14; att.p = {0.1, 0.1, 0.1}; zoom = 10;




            }
        }

        for(int i=0; i<5; i++) { // Calculer 5 points par frame pour la vitesse
            att.update();
            sf::Color col = sf::Color::Cyan;
            points.push_back(sf::Vertex(sf::Vector2f(500 + att.p.x * zoom, 400 + att.p.y * zoom), col));
        }

        window.clear(sf::Color::Black);
        window.draw(&points[0], points.size(), sf::Points);
        window.display();
    }
    return 0;
}
