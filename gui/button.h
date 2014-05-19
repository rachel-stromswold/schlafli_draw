#ifndef BUTTON_H
#define BUTTON_H

#include <SFML/Graphics.hpp>
#include <string>
#include "text.h"

class Button
{
protected:
    int m_x;
    int m_y;
    int m_width;
    int m_height;

    sf::Text m_text;
    sf::Font m_f;
    sf::RenderWindow* m_w;
    sf::RectangleShape m_rectangle;

public:
    Button(sf::RenderWindow* window, sf::Font font, int x, int y, int width, int height, std::string str);

    bool IsPressed(int xP,int yP); //only activates if the button was actually at the x and y location

    double GetHeight();

    void Draw();

    void SetText(std::string str);
    std::string GetText();
};

#endif //BUTTON_H
