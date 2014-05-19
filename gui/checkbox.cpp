#include "Checkbox.h"

Checkbox::Checkbox(sf::RenderWindow* window, sf::Font font, int x, int y, std::string cap, bool isToggled)
    : Button(window, font, x, y, 9, 9, "")
{
    m_isToggled = !isToggled;
    Toggle(); // Toggle so we update our string

    m_cap.setString(cap);
    m_cap.setFont(m_f);
    m_cap.setCharacterSize(15);
    m_cap.setColor(sf::Color::White);

    m_rectangle.setPosition(x, y);
    m_text.setPosition(x, y - 6);
    m_cap.setPosition(x + 15, y - 5);
}

void Checkbox::Toggle() {
    m_isToggled = !m_isToggled;
    if(m_isToggled)
        m_text.setString("x");
    else
        m_text.setString("");
}

bool Checkbox::IsToggled() {
    return m_isToggled;
}

void Checkbox::Draw() {
    m_w->draw(m_rectangle);
    m_w->draw(m_text);
    m_w->draw(m_cap);
}
