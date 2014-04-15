#include <SFML/Graphics.hpp>
#include <string>
#include <iostream>
#include <math.h>
#include "diagram.h"

#define PI atan(1) * 4
#define TAU 2 * PI

Diagram::Diagram() {
        m_p = 1;
        m_q = 1;
        m_r = 0;
}

Diagram::Diagram(sf::RenderWindow* window, std::string str) :
    m_shape(sf::Lines, 5)
{
    if (!str.empty())
        SetPQR(str);
};

void Diagram::SetPQR(std::string str) {
    m_p = 0;
    m_q = 1;
    m_r = 0;

    if(str.find('{') != std::string::npos && str.rfind('}') != std::string::npos) {//if both { and } are present
        str = str.substr(str.find('{') + 1, str.rfind('}') - str.find('{') - 1);
    }

    if(str.find('/') != std::string::npos) {
        m_p = ToInt(str.substr(0, str.find('/')));
        m_q = ToInt(str.substr(str.find('/') + 1, std::string::npos));
    } else {
        m_p = ToInt(str);
        m_q = 1;
    }
}

void Diagram::MakeDiagram(std::string str) {
    if(!str.empty())
        SetPQR(str);

    m_vertices = sf::VertexArray(sf::Points, m_p);

    // The initial angle (for the first vertex)
    double angle = PI / 2 + (TAU / (2 * m_p)) * (m_p + 1 % 2);
    int scale = 220;
    int centerX = 300;
    int centerY = 250;

    // Calculate the location of each vertex
    for(int iii = 0; iii < m_vertices.getVertexCount(); iii++) {
        // Adds the vertices to the diagram
        m_vertices[iii].position = sf::Vector2f(centerX + cos(angle) * scale, centerY - sin(angle) * scale);
        m_vertices[iii].color = sf::Color::White;

        // Calculates the angle that the next point will be at
        angle += TAU / m_p;
    }

    int gcf = GreatestCommonFactor(m_p, m_q);

    // Draws the lines on the diagram
    sf::VertexArray lines = sf::VertexArray(sf::Lines, 2 * m_p);
    for(int iii = 0; iii < gcf; iii++) {
        int lineIndex = 0;
        for(int vertexIndex = iii; lineIndex < lines.getVertexCount() / gcf; lineIndex++) {
            lines[lineIndex + iii * 2 * m_p / gcf] = m_vertices[vertexIndex];
            lines[lineIndex + iii * 2 * m_p / gcf].color = sf::Color((lineIndex * 23425) % 255,
                                                                     (lineIndex * 93245) % 255,
                                                                     (lineIndex * 65425) % 255,
                                                                     255);//sf::Color::White;

            if((lineIndex + iii * 2 * m_p / gcf) % 2 == 0)
                vertexIndex = (vertexIndex + m_q) % m_p;
        }
    }
    m_shape = lines;
}

void Diagram::Draw() {
//    m_w->draw(m_shape);
}

sf::VertexArray Diagram::GetDiagram() {
    return m_shape;
}

// Converts a string with a number in it to an integer containing that number
int ToInt(std::string str) {
    int ret = 0;
    for(int i = str.length() - 1; i >= 0; i--) {
        ret += pow(10, str.length() - 1 - i) * (str[i] - '0');
    }
    return ret;
}

int GreatestCommonFactor(int a, int b) {
  int c = 1;
  while (a != 0) {
     c = a;
     a = b % a;
     b = c;
  }
  return b;
}
