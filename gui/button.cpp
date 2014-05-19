#include "button.h"
#include <iostream>

Button::Button(sf::RenderWindow* window, sf::Font font, int x, int y, int width, int height, std::string str) :
        m_text(),
        m_f(font),
        m_rectangle(sf::Vector2f(width, height))

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
    m_text.setPosition(x, y - 4);
}

double Button::GetHeight() {
    return m_height;
}

void Button::Draw() {
    m_w->draw(m_rectangle);
    m_w->draw(m_text);
}

bool Button::IsPressed(int xP, int yP) {
    if(m_x - 3 < xP && xP < m_x + m_width + 3 && m_y - 3 < yP && yP < m_y + m_height + 2)
        return true;
    else
        return false;
}

void Button::SetText(std::string str){
    m_text.setString(str);
}

std::string Button::GetText(){
    return m_text.getString();
}
