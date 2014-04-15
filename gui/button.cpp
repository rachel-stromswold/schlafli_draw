#include "button.h"
#include <iostream>

Button::Button(sf::RenderWindow* window, sf::Font font, int x, int y, int width, int height, std::string str) :
        m_text(),
        m_f(font),
        m_rectangle(sf::Vector2f(width * 15, height * 15))

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


void Button::Draw() {
    m_w->draw(m_rectangle);
    m_w->draw(m_text);
}

bool Button::IsPressed(int xP, int yP) {
    if(m_x < xP && xP <m_x + m_width * 15 && m_y < yP && yP < m_y + m_height * 15)
        return true;
    else
        return false;
}


