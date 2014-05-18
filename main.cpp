#include <SFML/Graphics.hpp>
#include ".\gui\text.h"
#include ".\gui\button.h"
#include ".\gui\diagram.h"
#include <iostream>

int main() {
    sf::RenderWindow window(sf::VideoMode(640, 480), "Schlafli");

    sf::Font inFont;
    if(!inFont.loadFromFile("VeraMono.ttf")){/*error handling*/}

    InputBox input = InputBox(&window, inFont, 1, 1, window.getSize().x * .25, window.getSize().y * .035);

    Button but = Button(&window, inFont, window.getSize().x * .25 + 6, 2, window.getSize().x * .25 ,
                        window.getSize().y * .03, "Click to Submit");

    sf::Text edgeCap = sf::Text("faces (3d) ", inFont, 15);
    edgeCap.setPosition(window.getSize().x * .5 + 12, 2);
    edgeCap.setColor(sf::Color::White);

    Button edge = Button(&window, inFont, window.getSize().x * .66 + 2, 3, 14 ,
                        12, "");

    sf::Text colorCap = sf::Text("color ", inFont, 15);
    colorCap.setPosition(window.getSize().x * .7 - 3, 2);
    colorCap.setColor(sf::Color::White);

    Button color = Button(&window, inFont, window.getSize().x * .75 + 14, 3, 14 ,
                        12, "x");

    Diagram poly = Diagram(&window, window.getSize().x / 2,
                           (window.getSize().y - but.GetHeight()) / 2 + but.GetHeight());

    bool autoRotate = true;
    double rotationAngle = 2500;

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
            } else if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Tab) {
                autoRotate = !autoRotate;
            } else if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::L) {
                poly.ToggleEdges();
            } else if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::X) {
                rotationAngle += 20;
            } else if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::C) {
                rotationAngle -= 20;
                if(rotationAngle < 20) rotationAngle = 20;
            } else if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Z) {
                poly.ToggleColors();
            } else if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Return){
                poly.MakePoly(input.GetStoredString());
            } else if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left){
                if( but.IsPressed(event.mouseButton.x, event.mouseButton.y) ){
                    poly.MakePoly(input.GetStoredString());
                } else if(edge.IsPressed(event.mouseButton.x, event.mouseButton.y)){
                    poly.ToggleEdges();
                    if(edge.GetText()=="x")
                        edge.SetText("");
                    else
                        edge.SetText("x");
                } else if(color.IsPressed(event.mouseButton.x, event.mouseButton.y)){
                    poly.ToggleColors();
                    if(color.GetText()=="x")
                        color.SetText("");
                    else
                        color.SetText("x");
                }
            }
        }
        window.clear();
        poly.RotateSolid(sf::Keyboard::isKeyPressed(sf::Keyboard::W) - sf::Keyboard::isKeyPressed(sf::Keyboard::S),
                         sf::Keyboard::isKeyPressed(sf::Keyboard::A) - sf::Keyboard::isKeyPressed(sf::Keyboard::D),
                         sf::Keyboard::isKeyPressed(sf::Keyboard::E) - sf::Keyboard::isKeyPressed(sf::Keyboard::Q),
                         autoRotate, rotationAngle);
        poly.Draw();
        input.Draw();
        but.Draw();
        edge.Draw();
        color.Draw();
        window.draw(edgeCap);
        window.draw(colorCap);
        window.display();
    }
    return 0;
}
