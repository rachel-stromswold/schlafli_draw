#include <SFML/Graphics.hpp>
#include <string>
#include <iostream>
#include <math.h>
#include "diagram.h"

// Converts a string with a number in it to an integer containing that number
int ToInt(std::string str) {
    int ret = 0;
    for(int i = str.length() - 1; i >= 0; i--) {
        ret += pow(10, str.length() - 1 - i) * (str[i] - '0');
    }
    return ret;
}

Diagram::Diagram() {
        m_x = 0;
        m_y = 0;
        m_width = 0;
        m_height = 0;
        m_p = 1;
        m_q = 1;
        m_r = 0;
}

Diagram::    Diagram(sf::RenderWindow* window, int x, int y, int width, int height, std::string str) :
    m_shape()
{
    m_x = x;
    m_y = y;
    m_width = width;
    m_height = height;

    if(str.find('{') != std::string::npos && str.rfind('}') != std::string::npos) { //if both { and } are present
        str = str.substr(str.find('{') + 1, str.rfind('}') - str.find('{') - 1);
    }

    if(str.find('/') != std::string::npos) {
        m_p = ToInt(str.substr(0, str.find('/')));
        m_q = ToInt(str.substr(str.find('/') + 1, std::string::npos));
    } else {
        m_p = ToInt(str);
        m_q = 1;
    }

    m_shape.setPrimitiveType(sf::LinesStrip);
    m_shape.resize(m_p);

    for(int iii = 0; iii < m_shape.getVertexCount(); iii++) {
        std::cout << cos(iii * 6.283 / m_p) + x << std::endl;
        std::cout << sin(iii * 6.283 / m_p) + y << std::endl;
        m_shape[iii].position = sf::Vector2f(cos(iii * 6.283 / m_p) + m_x, sin(iii * 6.283 / m_p) + m_y);
        m_shape[(iii + 1) % m_p].position = sf::Vector2f(cos((iii + 1) % m_p * 6.283 / m_p) + m_x,
                                                     sin((iii + 1) % m_p * 6.283 / m_p) + m_y);
        m_shape[iii].color = sf::Color::White;
        m_shape[(iii + 1) % m_p].color = sf::Color::White;
    }
};

void Diagram::Draw(){
    //w->Draw(shape);
    std::cout << "Chuck testa";
}
