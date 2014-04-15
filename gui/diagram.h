#ifndef DIAGRAM_H
#define DIAGRAM_H

#include <SFML/Graphics.hpp>
#include <string>

class Diagram {
private:
    int m_p;
    int m_q;
    int m_r;

    sf::RenderWindow* m_w;

    sf::VertexArray m_vertices;
    sf::VertexArray m_shape;
public:
    Diagram();
    Diagram(sf::RenderWindow* window, std::string str = "");

    void SetPQR(std::string str);
    void MakeDiagram(std::string str = "");

sf::VertexArray GetDiagram();


    void Draw();
};

// Converts a string with a number in it to an integer containing that number; move to better location in the future
int ToInt(std::string str);
// Returns the greatest common factor of the inputs; move to a better location in the future
int GreatestCommonFactor(int a, int b);

#endif //H_DIAGRAM
