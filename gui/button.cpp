#include <SFML/Graphics.hpp>
#include <string>
#include "button.h"
#include "diagram.h"
#include <iostream>

#define PI atan(1) * 4
#define TAU 2 * PI

Button::Button(sf::RenderWindow* window, sf::Font font, int x, int y, int width, int height, std::string str) :
        m_text(),
        m_rectangle(sf::Vector2f(width * 15, height * 15)) ,
        m_f(font)
{
        m_w = window;
        m_x = x;
        m_y = y;
        m_width = width;
        m_height = height;

        m_text.setString(str);
        m_text.setFont(m_f);
        m_text.setCharacterSize(15);
        m_text.setColor(sf::Color::Black);
        m_rectangle.setOutlineThickness(2);
        m_rectangle.setOutlineColor(sf::Color(255, 255, 255));

        m_rectangle.setPosition(x, y);
        m_text.setPosition(x, y - 2);
    }

sf::VertexArray Button::GetDiagram(std::string str) {
    int p = 0;
    int q = 1;
    if(str.find('{') != std::string::npos && str.rfind('}') != std::string::npos) {//if both { and } are present
        str = str.substr(str.find('{') + 1, str.rfind('}') - str.find('{') - 1);
    }

    if(str.find('/') != std::string::npos) {
        p = ToInt(str.substr(0, str.find('/')));
        q = ToInt(str.substr(str.find('/') + 1, std::string::npos));
    } else {
        p = ToInt(str);
        q = 1;
    }

    sf::VertexArray vertices = sf::VertexArray(sf::Points, p);

    // The initial angle (for the first vertex)
    double angle = PI / 2 + (TAU / (2 * p)) * (p + 1 % 2);
    int scale = 220;
    int centerX = 300;
    int centerY = 250;

    // Calculate the location of each vertex
    for(int iii = 0; iii < vertices.getVertexCount(); iii++) {
        // Adds the vertices to the diagram
        vertices[iii].position = sf::Vector2f(centerX + cos(angle) * scale, centerY - sin(angle) * scale);

        vertices[iii].color = sf::Color::White;

        // Calculates the angle that the next point will be at
        angle += TAU / p;
    }

    int gcf = GreatestCommonFactor(p, q);

    // Draws the lines on the diagram
    sf::VertexArray lines = sf::VertexArray(sf::Lines, 2 * p);
    for(int iii = 0; iii < gcf; iii++) {
        int lineIndex = 0;
        for(int vertexIndex = iii; lineIndex < lines.getVertexCount() / gcf; lineIndex++) {
            lines[lineIndex + iii * 2 * p / gcf] = vertices[vertexIndex];
            lines[lineIndex + iii * 2 * p / gcf].color = sf::Color((lineIndex * 23425) % 255,
                                                                   (lineIndex * 93245 + 200) % 255,
                                                                   (lineIndex * 65425 + 100) % 255,
                                                                   255);//sf::Color::White;

            if((lineIndex + iii * 2 * p / gcf) % 2 == 0)
                vertexIndex = (vertexIndex + q) % p;
        }
    }
    return lines;
}

void Button::Draw(){
    m_w->draw(m_rectangle);
    m_w->draw(m_text);
    m_w->draw(m_shape);
}

void Button::Press(int xP, int yP, InputBox b){
    if(m_x < xP && xP <m_x + m_width * 15 && m_y < yP && yP < m_y + m_height * 15) {
        //d = Diagram(w, 0, 0, 100, 100, b.stored.getString());
        m_shape = GetDiagram(b.GetStoredString());
    }
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
