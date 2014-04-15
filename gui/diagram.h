#ifndef DIAGRAM_H
#define DIAGRAM_H

#include <SFML/Graphics.hpp>
#include <string>

class Diagram{
private:
    int m_x;
    int m_y;
    int m_width;
    int m_height;

    int m_p;
    int m_q;
    int m_r;

    sf::VertexArray m_shape;

    sf::RenderWindow* m_w;
    sf::RectangleShape m_rectangle;

public:
    Diagram();
    Diagram(sf::RenderWindow* window, int x, int y, int width, int height, std::string str);

    void draw();
};

// Converts a string with a number in it to an integer containing that number; move to better location in the future
int toInt(std::string str);

#endif //H_DIAGRAM
