#include <SFML/Graphics.hpp>
#include <string>
#include <iostream>
#include <math.h>
#include "diagram.h"

#define PI atan(1) * 4
#define TAU 2 * PI

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

bool Diagram::IsGood(sf::VertexArray* a, int x, int y) {
    sf::VertexArray arr =* a;
    int xx;
    int yy;
    if(arr.getVertexCount() == 0)
        return true;
    for(int i = 0; i < arr.getVertexCount() - 1; i += 2){
        xx = arr[i].position.x;
        yy = arr[i].position.y;
        if(x - xx < 5 || xx - x < 5 || yy - y < 5 || y - yy < 5) {
            return false;
        }
    }
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

    // The initial angle (for the first vertex)
    double angle = PI / 2 + (PI / m_p * ((m_p + 1) % 2));
    //double angle = 0;

    m_vertices = sf::VertexArray(sf::Points, 2 * m_p);
    m_shape = sf::VertexArray(sf::Lines, 2 * m_p);
    if(m_r == -1) {
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
        m_scale = 150; // smaller tesselations
        m_vertices = sf::VertexArray();
        m_vertices.setPrimitiveType(sf::Points);
        double angle = (m_p - 2) * PI / m_p;

        Grow(&m_vertices, m_centerX, m_centerY, 0.0, angle, m_scale, 4);
    }

    // Draws the lines on the diagram
    sf::VertexArray lines = sf::VertexArray(sf::Lines, m_vertices.getVertexCount());
    for(int iii = 0; iii < m_vertices.getVertexCount(); iii += 2) {
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

void Diagram::Grow(sf::VertexArray* arr, int x, int y, double angle, double delta, int scale, int i){
    if(i > 0/* && IsGood(arr,x,y)*/ ) {//if we are reasonably close
        for(double ii = angle + delta; ii < angle + 2 * PI; ii += delta) {
            arr->append(sf::Vertex(sf::Vector2f(x, y)));
            Grow(arr, x + scale * cos(angle + ii), y - scale * sin(angle + ii), angle + ii, delta, scale, i - 1);
        }
    } else {
        //arr->append(sf::Vertex(sf::Vector2f(x, y)));
    }
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

int GreatestCommonFactor(int a, int b) {
  int c = 1;
  while (a != 0) {
     c = a;
     a = b % a;
     b = c;
  }
  return b;
}
