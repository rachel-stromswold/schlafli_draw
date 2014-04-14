#include <SFML/Graphics.hpp>
#include <string>
#include "text.h"
#include "diagram.h"
#ifndef H_BUTTON
#define H_BUTTON

class Button{
public:
    int x;
    int y;
    int width;
    int height;
    //Diagram d;

    sf::Text text;
    sf::Font f;

    void draw();
    void press(int xP,int yP,InputBox b);//only activates if the button was actually at the x and y location
    //void activate

    Button(sf::RenderWindow* myWindow, sf::Font font,int myX,int myY,int myWidth,int myHeight,std::string str) : text(),rectangle(sf::Vector2f(myWidth*15, myHeight*15)),f(font){
        w=myWindow;
        x=myX;
        y=myY;
        width=myWidth;
        height=myHeight;

        text.setString(str);
        text.setFont(f);
        text.setCharacterSize(15);
        text.setColor(sf::Color::Black);
        rectangle.setOutlineThickness(2);
        rectangle.setOutlineColor(sf::Color(255,255,255));

        rectangle.setPosition(x,y);
        text.setPosition(x,y-2);
    };
private:
    sf::RenderWindow* w;
    sf::RectangleShape rectangle;

    sf::VertexArray shape;

    //Returns a Vertex Array of the shape that can be drawn to the window
    sf::VertexArray getDiagram(std::string str);
};

#endif //H_BUTTON
