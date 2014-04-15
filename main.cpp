#include <SFML/Graphics.hpp>
#include ".\gui\text.h"
#include ".\gui\button.h"
#include <iostream>

int main() {
    sf::RenderWindow window(sf::VideoMode(640, 480), "Schlaefli 2D");

    sf::Font inFont;
    if(!inFont.loadFromFile("VeraMono.ttf")){/*error handling*/}

    InputBox b(&window, inFont, 5, 5, 10, 1);
    Button but(&window, inFont, 160, 5, 10, 1, "Click to Submit");

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if(event.type == sf::Event::Closed) {
                window.close();
            } else if (event.type == sf::Event::TextEntered) {
                b.EnterText(event.text.unicode);
            } else if (event.type == sf::Event::MouseButtonPressed) {
                if (event.mouseButton.button == sf::Mouse::Left) {
                    but.Press(event.mouseButton.x, event.mouseButton.y, b);
                }
            }
        }

        window.clear();
        b.Draw();
        but.Draw();
        window.display();
    }
    return 0;
}
