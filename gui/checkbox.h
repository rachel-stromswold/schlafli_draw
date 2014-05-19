#ifndef CHECKBOX_H
#define CHECKBOX_H

#include <button.h>


class Checkbox : public Button
{
private:
    sf::Text m_cap;
    bool m_isToggled;
public:
    Checkbox(sf::RenderWindow* window, sf::Font font, int x, int y, std::string cap, bool isToggled);

    void Toggle();
    bool IsToggled();

    void Draw();
};

#endif // CHECKBOX_H
