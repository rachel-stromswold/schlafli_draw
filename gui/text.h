#include <SFML/Graphics.hpp>
#include <string>
#ifndef H_INPUT_BOX
#define H_INPUT_BOX
/*
class TextSettings {
public:
    int charSize;
    sf::Font f_regular;
    sf::Font f_bold;
    sf::Font f_italic;
    sf::Font f_special;

    TextSettings(int cSize, sf::Font f_r, sf::Font f_b, sf::Font f_i, sf::Font f_s) : f_regular(f_r), f_bold(f_b), f_italic(f_i), f_special(f_s){
        charSize=cSize;
    };
};
*/
class InputBox {
public:
    sf::Font f;
    bool focused;
    int x;
    int y;
    int width;
    int height;

    sf::Text stored;

    void draw();
    void enterText(char n);

    InputBox(sf::RenderWindow* myWindow, sf::Font font, int myX, int myY, int charWidth, int charHeight) : stored(),rectangle(sf::Vector2f(charWidth*15, charHeight*15)){
        w=myWindow;
        f=font;
        x=myX;
        y=myY;
        width=charWidth;
        height=charHeight;
        focused=false;

        stored.setString("");
        stored.setFont(f);
        stored.setCharacterSize(15);
        stored.setColor(sf::Color::Black);
        rectangle.setOutlineThickness(2);
        rectangle.setOutlineColor(sf::Color(100,100,100));

        rectangle.setPosition(x,y);
        stored.setPosition(x,y-2);
    };
private:
    sf::RenderWindow* w;
    sf::RectangleShape rectangle;
};

#endif /*H_INPUT_BOX*/
