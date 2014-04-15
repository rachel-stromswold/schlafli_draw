#ifndef BUTTON_H
#define BUTTON_H

#include <SFML/Graphics.hpp>
#include <string>
#include "text.h"
#include "diagram.h"

class Button{
private:
    int m_x;
    int m_y;
    int m_width;
    int m_height;
    //Diagram m_d;

    sf::Text m_text;
    sf::Font m_f;
    sf::RenderWindow* m_w;
    sf::RectangleShape m_rectangle;
    sf::VertexArray m_shape;

    //Returns a Vertex Array of the shape that can be drawn to the window
    sf::VertexArray GetDiagram(std::string str);

public:
    void Draw();
    void Press(int xP,int yP,InputBox b); //only activates if the button was actually at the x and y location
    //void activate

    Button(sf::RenderWindow* window, sf::Font font, int x, int y, int width, int height, std::string str);
};

#endif //BUTTON_H
