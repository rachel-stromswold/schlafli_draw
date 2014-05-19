#include <SFML/Graphics.hpp>
#include ".\gui\text.h"
#include ".\gui\button.h"
#include ".\gui\diagram.h"
#include ".\gui\checkbox.h"
#include <iostream>

int main() {
    sf::RenderWindow window(sf::VideoMode(640, 480), "Schlafli");

    sf::Font inFont;
    if(!inFont.loadFromFile("VeraMono.ttf")){/*error handling*/}

    InputBox input = InputBox(&window, inFont, 1, 1, window.getSize().x * .25, window.getSize().y * .035);

    Button but = Button(&window, inFont, window.getSize().x * .25 + 6, 2, window.getSize().x * .25 ,
                        window.getSize().y * .03, "Click to Submit");

    Checkbox showFaces = Checkbox(&window, inFont, 495, 60, "Display Faces", false);

    Checkbox showColors = Checkbox(&window, inFont, 495, 80, "Display Colors", true);

    Checkbox autoRotate = Checkbox(&window, inFont, 495, 40, "Auto Rotate", true);

    Diagram poly = Diagram(&window, window.getSize().x / 2,
                           (window.getSize().y - but.GetHeight()) / 2 + but.GetHeight());

    double rotationAngle = 2000;

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
            } else if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Tab ||
                       event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left &&
                       autoRotate.IsPressed(event.mouseButton.x, event.mouseButton.y)) {
                autoRotate.Toggle();
            } else if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::L ||
                       event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left
                       && showFaces.IsPressed(event.mouseButton.x, event.mouseButton.y)) {
                poly.ToggleEdges();
                showFaces.Toggle();
            } else if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::X) {
                rotationAngle += 20;
            } else if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::C) {
                rotationAngle -= 20;
                if(rotationAngle < 20) rotationAngle = 20;
            } else if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Z ||
                       event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left
                       && showColors.IsPressed(event.mouseButton.x, event.mouseButton.y)) {
                poly.ToggleColors();
                showColors.Toggle();
            } else if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Return){
                poly.MakePoly(input.GetStoredString());
            } else if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left){
                if( but.IsPressed(event.mouseButton.x, event.mouseButton.y) ){
                    poly.MakePoly(input.GetStoredString());
                }
            }
        }
        window.clear();
        poly.RotateSolid(sf::Keyboard::isKeyPressed(sf::Keyboard::W) - sf::Keyboard::isKeyPressed(sf::Keyboard::S),
                         sf::Keyboard::isKeyPressed(sf::Keyboard::A) - sf::Keyboard::isKeyPressed(sf::Keyboard::D),
                         sf::Keyboard::isKeyPressed(sf::Keyboard::E) - sf::Keyboard::isKeyPressed(sf::Keyboard::Q),
                         autoRotate.IsToggled(), rotationAngle);
        // Draw our diagram, input box, and button
        poly.Draw();
        input.Draw();
        but.Draw();

        // Draw our checkboxes
        showFaces.Draw();
        showColors.Draw();
        autoRotate.Draw();

        // Display everything that we've drawn
        window.display();
    }
    return 0;
}
