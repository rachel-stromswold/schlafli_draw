#include <SFML/Graphics.hpp>
#include <string>
#include <iostream>
#include <math.h>
#include "diagram.h"

#define PI (atan(1) * 4)
#define TAU (2 * PI)

Diagram::Diagram() {
    m_p = 3;
    m_q = 1;
    m_r = -1;

    m_scale = 220;
    m_centerX = 300;
    m_centerY = 250;
}

Diagram::Diagram(sf::RenderWindow* window, int scale, int centerX, int centerY, std::string str) :
    m_shape(sf::Lines, 5)
{
    m_w = window;
    if (!str.empty())
        SetPQR(str);
    m_scale = scale;
    m_centerX = centerX;
    m_centerY = centerY;
};

bool Diagram::IsGood(sf::Vertex vert1, sf::Vertex vert2) {
    if(m_vertices.getVertexCount() == 0)
        return true;
    for(int iii = 0; iii < m_vertices.getVertexCount() - 1; iii += 2) {
        if(m_vertices[iii].position.x == vert1.position.x && m_vertices[iii].position.y == vert1.position.y &&
           m_vertices[iii + 1].position.x == vert2.position.x && m_vertices[iii + 1].position.y == vert2.position.y ||
           m_vertices[iii].position.x == vert2.position.x && m_vertices[iii].position.y == vert2.position.y &&
           m_vertices[iii + 1].position.x == vert1.position.x && m_vertices[iii + 1].position.y == vert1.position.y)
        {
            std::cout << "false" << std::endl;
            return false;
        }
    }
    std::cout << "true" << std::endl;
    return true;
}

void Diagram::SetPQR(std::string str) {
    m_p = 1;
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
            std::cout << m_p << std::endl;
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

    m_vertices = sf::VertexArray(sf::Points, 2 * m_p);
    m_shape = sf::VertexArray(sf::Lines, 2 * m_p);
    if(m_r == -1) {
        // The initial angle (for the first vertex)
        double angle = PI / 2 + (PI / m_p * ((m_p + 1) % 2));
        // Calculate the location of each vertex
        for(int iii = 0; iii < m_vertices.getVertexCount() - 1; iii += 2) {
            // Adds the vertices to the diagram
            m_vertices[iii].position = sf::Vector2f(m_centerX + cos(angle) * m_scale,
                                                    m_centerY - sin(angle) * m_scale);
            angle += m_q * (TAU / m_p);
            m_vertices[iii + 1].position = sf::Vector2f(m_centerX + cos(angle) * m_scale,
                                                        m_centerY - sin(angle) * m_scale);
            angle -= (m_q - 1) * (TAU / m_p);
            m_vertices[iii].color = sf::Color::White;
            std::cout << m_vertices[iii].position.x << std::endl;
        }
    } else { // r > 0
        m_scale = 50; // smaller tesselations
        // The initial angle (for the first vertex)
        double angle = (m_p - 2) * PI / m_p;
        m_vertices = sf::VertexArray();
        m_vertices.setPrimitiveType(sf::Points);

        //Grow(&m_vertices, m_centerX, m_centerY, 0.0, angle, 3);
        Tesselate(300, angle);
        std::cout << m_vertices.getVertexCount() << std::endl;
    }

    // Draws the lines on the diagram
    sf::VertexArray lines = sf::VertexArray(sf::Lines, m_vertices.getVertexCount());
    for(int iii = 0; iii < m_vertices.getVertexCount() - 1; iii += 2) {
        lines[iii] = m_vertices[iii];
        lines[iii + 1] = m_vertices[iii + 1];
        lines[iii].color = sf::Color(((iii + 209) * 23432) % 255,
                                     ((iii + 742) * 53320) % 255,
                                     ((iii + 564) * 87914) % 255, 255);//sf::Color::White;
        lines[iii + 1].color = sf::Color(((iii + 87) * 12345) % 255,
                                       ((iii + 343) * 53240) % 255,
                                       ((iii + 123) * 98765) % 255, 255);//sf::Color::White;
    }
    m_shape = lines;
}

void Diagram::Grow(sf::VertexArray* arr, int x, int y, double angle, double delta, int i){
    if(i>0 && x>0 && y>0 && x<m_w->getSize().x && y<m_w->getSize().y/* && IsGood(arr,x,y)*/ ) { //if we are reasonably close
        for(double ii = angle + delta; ii < angle + TAU; ii += delta) {
            arr->append(sf::Vertex(sf::Vector2f(x, y)));
            Grow(arr, x + m_scale * cos(angle + ii), y - m_scale * sin(angle + ii), angle + ii, delta, i - 1);
            std::cout << "looping" << std::endl;
        }
    } else {
        std::cout << "terminating" << std::endl;
        //arr->append(sf::Vertex(sf::Vector2f(x, y)));
    }
}

void Diagram::Tesselate(int i, double delta) {
    m_vertices = sf::VertexArray(sf::Lines, 1);
    m_vertices[0] = sf::Vertex(sf::Vector2f(m_centerX, m_centerY));
    for(int iii = 0; iii < i; iii += 2) {
        int x = m_vertices[iii].position.x, y = m_vertices[iii].position.y;
        if(0 < x && x < m_w->getSize().x && 0 < y && y < m_w->getSize().y) {
            m_vertices[m_vertices.getVertexCount() - 1] = m_vertices[iii];
            double initAngle = 0;
            if(iii == 0)
                initAngle = -delta;
            else {
                if (x >= m_vertices[iii - 1].position.x) initAngle += PI;
                initAngle += atan(-(m_vertices[iii].position.y - m_vertices[iii - 1].position.y) /
                                  (m_vertices[iii].position.x - m_vertices[iii - 1].position.x));
            }
            while(initAngle >= TAU) initAngle -= TAU; // put in [0, TAU) if too large
            std::cout << initAngle * 180/PI << ", " << iii << ", " << x << std::endl;
            for(double angle = initAngle; angle < TAU + initAngle; angle += delta) {
                sf::Vertex vert = sf::Vertex(sf::Vector2f(m_vertices[iii].position.x + m_scale * cos(angle),
                                                          m_vertices[iii].position.y - m_scale * sin(angle)));
                if(iii == 0) {
                    m_vertices.append(vert);
                    m_vertices.append(m_vertices[iii]);
                }
                else if(IsGood(vert, m_vertices[m_vertices.getVertexCount() - 1])) {
                    m_vertices.append(vert);
                    m_vertices.append(m_vertices[iii]);
                }
            }
        }
        if(iii == 0) iii--;
    }
    m_vertices.resize(m_vertices.getVertexCount() - 1);
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
