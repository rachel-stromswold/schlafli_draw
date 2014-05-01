#include <SFML/Graphics.hpp>
#include ".\gui\text.h"
#include ".\gui\button.h"
#include ".\gui\diagram.h"
#include <iostream>

int main() {
    sf::RenderWindow window(sf::VideoMode(640, 480), "Schlaefli 2D");

    sf::Font inFont;
    if(!inFont.loadFromFile("VeraMono.ttf")){/*error handling*/}

    InputBox input = InputBox(&window, inFont, 1, 1, window.getSize().x * .25, window.getSize().y * .035);
    Button but = Button(&window, inFont, window.getSize().x * .25 + 6, 2, window.getSize().x * .25 ,
                        window.getSize().y * .03, "Click to Submit");
    Diagram poly = Diagram(&window, window.getSize().x / 2,
                           (window.getSize().y - but.GetHeight()) / 2 + but.GetHeight());

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if(event.type == sf::Event::Closed || (event.type == sf::Event::KeyPressed &&
                                                   event.key.code == sf::Keyboard::Escape)) {
                window.close();
            } else if (event.type == sf::Event::TextEntered) {
                input.EnterText(event.text.unicode);
            } else if ((event.type == sf::Event::MouseButtonPressed &&
                        event.mouseButton.button == sf::Mouse::Left &&
                        but.IsPressed(event.mouseButton.x, event.mouseButton.y)) ||
                       (event.type == sf::Event::KeyPressed &&
                        event.key.code == sf::Keyboard::Return))
            {
                poly.SetPQR(input.GetStoredString());
                poly.MakePoly();
            }
        }

        window.clear();
        poly.Draw();
        input.Draw();
        but.Draw();
        window.display();
    }
    return 0;
}
