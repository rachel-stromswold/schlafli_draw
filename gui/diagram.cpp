#include <SFML/Graphics.hpp>
#include <string>
#include <iostream>
#include <math.h>
#include "diagram.h"

#define PI (atan(1) * 4)
#define TAU (2 * PI)

Diagram::Diagram() {
    m_p = 2;
    m_q = 1;
    m_r = -1;

    m_scale = 220;
    m_centerX = 300;
    m_centerY = 250;
}

Diagram::Diagram(sf::RenderWindow* window, int scale, int centerX, int centerY, std::string str) :
    m_shape(sf::Lines, 0)
{
    m_w = window;
    if (!str.empty())
        SetPQR(str);
    m_scale = scale;
    m_centerX = centerX;
    m_centerY = centerY;
};

bool Diagram::IsGood(sf::Vertex vert1, sf::Vertex vert2) {
    if(m_shape.getVertexCount() == 0)
        return true;
    for(int iii = 0; iii < m_shape.getVertexCount() - 1; iii += 2) {
        if(m_shape[iii].position.x == vert1.position.x && m_shape[iii].position.y == vert1.position.y &&
           m_shape[iii + 1].position.x == vert2.position.x && m_shape[iii + 1].position.y == vert2.position.y ||
           m_shape[iii].position.x == vert2.position.x && m_shape[iii].position.y == vert2.position.y &&
           m_shape[iii + 1].position.x == vert1.position.x && m_shape[iii + 1].position.y == vert1.position.y)
        {
            return false;
        }
    }
    return true;
}

void Diagram::SetPQR(std::string str) {
    m_p = 2;
    m_q = 1;
    m_r = -1;

    if(str == "") return;

    if(str.find('{') != std::string::npos && str.rfind('}') != std::string::npos) { //if both { and } are present
        str = str.substr(str.find('{') + 1, str.rfind('}') - str.find('{') - 1);
    }

    if(str.find('/') != std::string::npos) {
        m_p = ToInt(str.substr(0, str.find('/')));
        m_q = ToInt(str.substr(str.find('/') + 1, std::string::npos));
    } else if(str.find(',') != std::string::npos) {
        m_p = ToInt(str.substr(0, str.find(',')));
        m_r = ToInt(str.substr(str.find(',') + 1, std::string::npos));

        //(p-2)*180/p is the interior angle of 1 vertex on the regular shape
        if(m_r * ((m_p - 2) * 180 / m_p) != 360){
            m_r = -1;
        }
    } else {
        m_p = ToInt(str);
        m_q = 1;
    }
}

void Diagram::MakeDiagram(std::string str) {
    if(str != "")
        SetPQR(str);

    if(m_r == -1) {
        CreatePoly();
    } else { // r > 0
        Tesselate();
    }
}

void Diagram::CreatePoly() {
    m_scale = 220;
    m_shape = sf::VertexArray(sf::Lines, 2 * m_p);
    // The initial angle (for the first vertex)
    double angle = PI / 2 + (PI / m_p * ((m_p + 1) % 2));
    // Calculate the location of each vertex
    for(int iii = 0; iii < m_shape.getVertexCount() - 1; iii += 2) {
        // Adds the vertices to the diagram
        m_shape[iii].position = sf::Vector2f(m_centerX + cos(angle) * m_scale,
                                             m_centerY - sin(angle) * m_scale);
        angle += m_q * (TAU / m_p);
        m_shape[iii + 1].position = sf::Vector2f(m_centerX + cos(angle) * m_scale,
                                                 m_centerY - sin(angle) * m_scale);
        angle -= (m_q - 1) * (TAU / m_p);
        m_shape[iii].color = Colorgen(iii);
        m_shape[iii + 1].color = Colorgen(iii + 1);
    }
}

void Diagram::Tesselate() {
    m_scale = 50; // smaller tessellations
    m_shape = sf::VertexArray(sf::Lines, 0);
    // The amount by which we rotate for the tessellation
    double delta = (m_p - 2) * PI / m_p;
    m_shape = sf::VertexArray(sf::Lines, 1);
    m_shape[0] = sf::Vertex(sf::Vector2f(m_centerX, m_centerY));
    m_shape[0].color = Colorgen(639);
    bool
    isFinished = false;
    for(int iii = 0; iii < m_shape.getVertexCount(); iii += 2) {
        int x = m_shape[iii].position.x, y = m_shape[iii].position.y;
        if(0 < x && x < m_w->getSize().x && 0 < y && y < m_w->getSize().y) {
            m_shape[m_shape.getVertexCount() - 1] = m_shape[iii];
            double initAngle = 0;
            if(iii == 0)
                initAngle = -delta;
            else {
                initAngle += atan(-(m_shape[iii].position.y - m_shape[iii - 1].position.y) /
                                   (m_shape[iii].position.x - m_shape[iii - 1].position.x));
                if (x >= m_shape[iii - 1].position.x) initAngle += PI;
            }
            for(double angle = initAngle; angle < TAU + initAngle; angle += delta) {
                sf::Vertex vert = sf::Vertex(sf::Vector2f(m_shape[iii].position.x + m_scale * cos(angle),
                                                          m_shape[iii].position.y - m_scale * sin(angle)));
                vert.color = Colorgen(m_shape.getVertexCount() - 1);
                if(iii == 0) {
                    m_shape.append(vert);
                    m_shape.append(m_shape[iii]);
                }
                else if(IsGood(vert, m_shape[m_shape.getVertexCount() - 1])) {
                    m_shape.append(vert);
                    m_shape.append(m_shape[iii]);
                }
            }
        }
        if(iii == 0) iii--;
    }
    m_shape.resize(m_shape.getVertexCount() - 1);
}

// Generate the colors for the lines so it's not all white and boring
sf::Color Diagram::Colorgen(int seed) {
    int hue = ((seed + 23657) * 15274) % 360;
    return HSVtoRGB(hue, 1, 1);
}

void Diagram::Draw() {
    m_w->draw(m_shape);
}

// Converts a string with a number in it to an integer containing that number
int ToInt(std::string str) {
    int ret = 0;
    for(int i = str.length() - 1; i >= 0; i--) {
        ret += pow(10, str.length() - 1 - i) * (str[i] - '0');
    }
    return ret;
}

// Convert HSV color values to RGB color values
sf::Color HSVtoRGB(int hue, double sat, double val) {
    hue %= 360;
    while(hue < 0) hue += 360;

    if(sat < 0) sat = 0.0;
    if(sat > 1) sat = 1.0;

    if(val < 0) val = 0.0;
    if(val > 1) val = 1.0;

    int h = hue / 60;
    double f = double(hue) / 60 - h;
    double p = val * (1 - sat);
    double q = val * (1 - sat * f);
    double t = val * (1 - sat * (1 - f));

    switch(h)
    {
        default:
        case 0:
        case 6: return sf::Color(val * 255, t * 255, p * 255);
        case 1: return sf::Color(q * 255, val * 255, p * 255);
        case 2: return sf::Color(p * 255, val * 255, t * 255);
        case 3: return sf::Color(p * 255, q * 255, val * 255);
        case 4: return sf::Color(t * 255, p * 255, val * 255);
        case 5: return sf::Color(val * 255, p * 255, q * 255);
    }
}
