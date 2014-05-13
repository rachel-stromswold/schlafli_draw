#include <SFML/Graphics.hpp>
#include <string>
#include <iostream>
#include <math.h>
#include "diagram.h"

#define PI (atan(1) * 4)
#define TAU (2 * PI)

#define TET_PHI 19.471220333
#define CUBE_PHI 35.264391
#define OCT_PHI 0.0
#define ICO_PHI 26.56505
#define DOD_PHIA 52.62263590
#define DOD_PHIB 10.81231754
//TODO: find how to actually calculate these values

Diagram::Diagram() {
    m_p = 2;
    m_q = 1;
    m_r = -1;
    m_s = -1;

    m_scale = 220;
    m_center = sf::Vector3f(300, 250, 0);
}

Diagram::Diagram(sf::RenderWindow* window, int centerX, int centerY, std::string str) :
    m_shape(sf::Lines, 0),
    m_center(centerX, centerY, 0) {
    m_w = window;
    if (!str.empty())
        SetPQR(str);
    m_scale = 200;
};

bool Diagram::IsGood(sf::Vertex vert1, sf::Vertex vert2) {
    if(m_shape.getVertexCount() <= 1) // An empty set, or only one point, doesn't have any lines for us to overlap with
        return true;
    for(int iii = 0; iii < m_shape.getVertexCount() - 1; iii += 2) { // Check all lines in m_shape
        // Returns true if the two vertices appear in sequence anywhere in m_shape (not necessarily in order)
        if(m_shape[iii].position.x == vert1.position.x && m_shape[iii].position.y == vert1.position.y &&
           m_shape[iii + 1].position.x == vert2.position.x && m_shape[iii + 1].position.y == vert2.position.y ||
           m_shape[iii].position.x == vert2.position.x && m_shape[iii].position.y == vert2.position.y &&
           m_shape[iii + 1].position.x == vert1.position.x && m_shape[iii + 1].position.y == vert1.position.y)
        {
            return false;
        }
    }
    return true;
}

bool Diagram::IsGood(sf::Vector3f vect1, sf::Vector3f vect2, sf::Vector3f vect3) {
    for(int iii = 0; iii < m_vertices.size(); iii  += 1) {
       if((IsApproxEqual(m_vertices[iii], vect1) &&
                (IsApproxEqual(m_vertices[iii + 1], vect2) && IsApproxEqual(m_vertices[iii + 2], vect3) ||
                 IsApproxEqual(m_vertices[iii + 1], vect3) && IsApproxEqual(m_vertices[iii + 2], vect2))) ||
          (IsApproxEqual(m_vertices[iii], vect2) &&
                (IsApproxEqual(m_vertices[iii + 1], vect1) && IsApproxEqual(m_vertices[iii + 2], vect3) ||
                 IsApproxEqual(m_vertices[iii + 1], vect3) && IsApproxEqual(m_vertices[iii + 2], vect1))) ||
          (IsApproxEqual(m_vertices[iii], vect3) &&
                (IsApproxEqual(m_vertices[iii + 1], vect2) && IsApproxEqual(m_vertices[iii + 2], vect1) ||
                 IsApproxEqual(m_vertices[iii + 1], vect1) && IsApproxEqual(m_vertices[iii + 2], vect2))))
            {
                return false;
                std::cout << "false\n";
            }
    }
    std::cout << "true\n";
    return true;
}

bool Diagram::CanAdd(int x, int y, int z, double * arr, int len){
    for(int i=0;i+2<len;i+=3){
        if(x>arr[i]-.001 && x<arr[i]
            && y>arr[i+1]-.001 && y<arr[i+1]+.001
            && z>arr[i+2]-.001 && z<arr[i+2]+.001 ){
                return false;
        }
    }
    return true;
}

void Diagram::SetPQR(std::string str) {
    m_p = 2; // Empty schlafli set = line segment
    m_q = 1;
    m_r = -1;
    m_s = 1;

    if(str.find('{') != std::string::npos) {  // If '{' is present (which it should be)
        str = str.substr(str.find('{') + 1, str.length() - 1); // Edit the string to remove it
    }
    if(str.find('}') != std::string::npos) {  // If '}' is present (which it also should be)
        str = str.substr(0, str.find('}')); // Edit the string to remove it
    }

    if(str == "") return;

    std::string pqStr = str.substr(0, str.find(',')); // Look only up until the first comma, if there is one
    if(pqStr.find('/') != std::string::npos) { // If there's a slash, we need to set both p and q
        m_p = ToInt(pqStr.substr(0, pqStr.find('/')));
        m_q = ToInt(pqStr.substr(pqStr.find('/') + 1, std::string::npos));
    } else { // Otherwise, we can just set p
            m_p = ToInt(pqStr); // No commas, slashes or braces, so the string is just p
    }
    if(str.find(',') != std::string::npos) { // There's a comma, so we'll have to at least set r as well
        str = str.substr(str.find(',') + 1, std::string::npos); // Look only past the first comma
        if(str.find('/') != std::string::npos) { // There's a slash in the second part
            m_r = ToInt(str.substr(0, str.find('/')));
            m_s = ToInt(str.substr(str.find('/') + 1, std::string::npos));
        } else { // No slash present
            m_r = ToInt(str);
        }
    }

    while(m_q > m_p / 2) m_q = abs(m_p - m_q); // Put q in (1, p/2) for convenience

    //(p-2)*180/p is the interior angle of 1 vertex on the regular shape
    if(m_r * ((m_p - 2) * 180 / m_p) == 360){
        m_tess = true; // This is a tessellation of a plane
    } else {
        m_tess = false;
    }
}

double Diagram::GetAngle() {
    double thetaFace = (m_p - 2 * m_q) * PI / m_p; // Interior angle of the polygon forming the face
    double thetaVertex = TAU / m_r; // Interior angle of the vertex figure
    if(m_s <= 1) { // The second number doesn't have a slash
        return asin(sqrt((1 - cos(thetaFace)) / (1 - cos(thetaVertex))));
    } else {
        return asin(sqrt((1 - cos(thetaFace)) / 2) / sin(thetaVertex));
    }
}

void Diagram::MakePoly(std::string str) {
    if(str != "")
        SetPQR(str);
    if(m_r == -1) { // 2D Polygon
        MakeDiagram();
    } else if(m_tess){ // 2D tessellation
        Tessellate();
    } else { // 3D Polytope
        /*double l=-2/((double)(m_r)/2-(double)(m_r)/(double)(m_p)-1); // Sam Stuff
        double verts[3*(int)l];//3 times the number of needed verts
        CreateNet(10,verts,3*(int)l);*/
        MakeSolid();
    }
}

void Diagram::MakeDiagram() {
    m_scale = (m_w->getSize().y * .99) / 2.2; // Scale to window size
    m_shape = sf::VertexArray(sf::Lines, 2 * m_p);
    // The initial angle (for the first vertex)
    double angle = PI / 2 + (PI / m_p * ((m_p + 1) % 2)); // Ensure there is a flat line on the bottom
    // Calculate the location of each vertex
    for(int iii = 0; iii < m_shape.getVertexCount() - 1; iii += 2) {
        // Adds the vertices to the diagram
        m_shape[iii].position = sf::Vector2f(m_center.x + cos(angle) * m_scale,
                                             m_center.y - sin(angle) * m_scale);
        angle += m_q * (TAU / m_p);
        m_shape[iii + 1].position = sf::Vector2f(m_center.x + cos(angle) * m_scale,
                                                 m_center.y - sin(angle) * m_scale);
        angle -= (m_q - 1) * (TAU / m_p);
        m_shape[iii].color = Colorgen(iii);
        m_shape[iii + 1].color = Colorgen(iii + 1);
    }
}

void Diagram::Tessellate() {
    m_scale = 50; // smaller tessellations
    m_shape = sf::VertexArray(sf::Lines, 1);
    double delta = (m_p - 2) * PI / m_p; // The amount by which we rotate for the tessellation
    m_shape[0] = sf::Vertex(sf::Vector2f(m_center.x, m_center.y)); // Create our starting vertex at the center
    m_shape[0].color = Colorgen(639); // Set starting color
    for(int iii = 0; iii < m_shape.getVertexCount(); iii += 2) { // Go until all vertices are off screen
        int x = m_shape[iii].position.x, y = m_shape[iii].position.y; // For ease of use
        if(0 < x && x < m_w->getSize().x && 0 < y && y < m_w->getSize().y) { // False if the vertex is out of the window
            m_shape[m_shape.getVertexCount() - 1] = m_shape[iii]; // Set the last vertex to the current center one
            double initAngle = 0;
            if(iii != 0) { // First run doesn't have any lines to work from, so just start with 0
                // Set our initial angle to be that of the line we came on, so that we draw lines relative to that
                initAngle += atan(-(m_shape[iii].position.y - m_shape[iii - 1].position.y) /
                                   (m_shape[iii].position.x - m_shape[iii - 1].position.x));
                if (x >= m_shape[iii - 1].position.x) initAngle += PI; // Compensate for the restricted range of atan
            }
            for(double angle = initAngle; angle < TAU + initAngle; angle += delta) {
                // Create the next vertex
                sf::Vertex vert = sf::Vertex(sf::Vector2f(m_shape[iii].position.x + m_scale * cos(angle),
                                                          m_shape[iii].position.y - m_scale * sin(angle)));
                vert.color = Colorgen(m_shape.getVertexCount() - 1); // Set the color
                if(IsGood(vert, m_shape[m_shape.getVertexCount() - 1])) { // Don't draw over existing lines
                    m_shape.append(vert); // Add the vertex (completes a line with the center vertex)
                    m_shape.append(m_shape[iii]); // Add the center vertex again to begin more lines
                }
            }
        }
        if(iii == 0) iii--; // Want to only deal with odd-numbered vertices after first run
    }
    m_shape.resize(m_shape.getVertexCount() - 1); // Remove the last vertex, since we have one too many
}

void Diagram::CreateNet(int scale, double * arr, int len) {
    int loops=m_r-1;//usually this works except for the dodecohedron

    int ii=0;

    double phi=0.0;
    double ang=0.0;
    double theta=0.0;

    double x=0;
    double y=0;
    double z=0;

    //TODO: actually find a formula for these values
    switch(2/(m_p/m_q - m_p/2 + 1)){
    case 4:
        phi=TET_PHI;
        break;
    case 6:
        phi=CUBE_PHI;
        break;
    case 8:
        phi=OCT_PHI;
        break;
    case 20:
        phi=ICO_PHI;
        break;
    default:
        break;
    }
    if(m_p==5){//the dodecahedron is a special and very annoying case
        loops=4;//for the dodecahedron
    }else{
        for(double i=PI/2;i>=0;i-=PI/(2*loops)){
            ang=(double)((int)(0.99+cos(i)))*ICO_PHI;
            //std::cout<<ang<<std::endl;
            //std::cout<<std::endl;
            theta=0;
            for(int j=0;j<11;j++){
                x=scale*cos(theta)*cos(ang);
                y=scale*sin(theta)*cos(ang);
                z=scale*sin(ang);
                if(CanAdd(x,y,z,arr,len) && ii+2<len){
                    arr[ii]=x;
                    std::cout<<x<<std::endl;
                    arr[ii+1]=y;
                    std::cout<<y<<std::endl;
                    arr[ii+2]=z;
                    std::cout<<z<<std::endl;
                    std::cout<<std::endl;
                    ii+=3;
                }
                theta+=TAU/(m_p);//increments the angle by the proper amount
            }
        }
    }
}

void Diagram::MakeSolid() {
    /* Strategy:
        Starting with one vertex, draw the vertex figure.
            Extend the lines so that the ends are equidistant from the center
        Connect the appropriate lines to form triangular faces
        From each new vertex, repeat until done
            Check each time to ensure you're not drawing over an existing edge
            Ensure that the orientation is correct; the place of the vertex figure should be perpendicular to
                the line through its vertex and the center
    */
    m_scale = 100; //TODO: replace dummy value
    double theta = GetAngle();  // Calculate our deflection (polar) angle
    double phi = 0;             // The azimuth angle; start at 0 for simplicity's sake
    double length = 2 * m_scale * cos(theta); // How long each side is
    m_vertices = std::vector<sf::Vector3f>(2);
    m_vertices[0] = sf::Vector3f(m_center.x + m_scale, m_center.y, m_center.z); // Starting with a point on the sphere
    m_vertices[1] = sf::Vector3f(m_vertices[0].x + length * cos(PI - theta),    // First edge to be created
                               m_vertices[0].y,
                               m_vertices[0].z + length * sin(theta));

    for(int iii = 1; iii < 30/*m_vertices.size()*/; iii += 3) { // Main loop for creating the other edges
        for(phi = TAU / m_r * m_s; phi < TAU  * m_s; phi += TAU / m_r * m_s) {
            sf::Vector3f nextVert = RotatePointAboutLine(m_vertices[iii - 1], phi, m_center, m_vertices[iii]);
            if(fabs(nextVert.x) < .0001) nextVert.x = 0;
            if(fabs(nextVert.y) < .0001) nextVert.y = 0;
            if(fabs(nextVert.z) < .0001) nextVert.z = 0;

            if(IsGood(m_vertices[iii - 1], m_vertices[iii], nextVert)) {
                m_vertices.push_back(m_vertices[iii - 1]);
                m_vertices.push_back(m_vertices[iii]);
                m_vertices.push_back(nextVert);
                std::cout << iii << "\n";
            }
        }
    }

    m_shape = sf::VertexArray(sf::Triangles, 0);
    std::cout << std::endl;
    for(int iii = 2; iii < m_vertices.size(); iii += 3) {
        if(m_vertices[iii].y >= m_center.y || m_vertices[iii + 1].y >= m_center.y || m_vertices[iii + 2].y >= m_center.y)
        {
            m_shape.append(sf::Vertex(sf::Vector2f(m_vertices[iii].x,
                                                   m_vertices[iii].z + m_center.y)));
            m_shape[iii - 2].color = Colorgen(iii);
            m_shape.append(sf::Vertex(sf::Vector2f(m_vertices[iii + 1].x,
                                                   m_vertices[iii + 1].z + m_center.y)));
            m_shape[iii - 1].color = Colorgen(iii + 1);
            m_shape.append(sf::Vertex(sf::Vector2f(m_vertices[iii + 2].x,
                                                   m_vertices[iii + 2].z + m_center.y)));
            m_shape[iii].color = Colorgen(iii + 2);
        //std::cout << m_shape[iii].position.x << ", " << m_shape[iii].position.y << std::endl;
        }
    }
}

// Generate the colors for the lines so it's not all white and boring
sf::Color Diagram::Colorgen(int seed) {
    int hue = ((seed + 23657) * 15274) % 360;
    return HSVtoRGB(hue, 1, 1);
}

void Diagram::Draw() {
    m_w->draw(m_shape);
}

sf::Vector3f Diagram::RotatePointAboutLine(sf::Vector3f p, double theta, sf::Vector3f p1, sf::Vector3f p2)
{
    // Note: This code modified from http://paulbourke.net/geometry/rotate/example.c
    sf::Vector3f u,q1,q2;
    double d = 0;

    /* Step 1 */
    q1.x = p.x - p1.x;
    q1.y = p.y - p1.y;
    q1.z = p.z - p1.z;

    u.x = p2.x - p1.x;
    u.y = p2.y - p1.y;
    u.z = p2.z - p1.z;

    double magnitude = sqrt(u.x * u.x + u.y * u.y + u.z * u.z);
    u.x /= magnitude;
    u.y /= magnitude;
    u.z /= magnitude;
    d = sqrt(u.y * u.y + u.z * u.z);

    /* Step 2 */
    if (d != 0) {
        q2.x = q1.x;
        q2.y = q1.y * u.z / d - q1.z * u.y / d;
        q2.z = q1.y * u.y / d + q1.z * u.z / d;
    } else {
        q2 = q1;
    }

    /* Step 3 */
    q1.x = q2.x * d - q2.z * u.x;
    q1.y = q2.y;
    q1.z = q2.x * u.x + q2.z * d;

    /* Step 4 */
    q2.x = q1.x * cos(theta) - q1.y * sin(theta);
    q2.y = q1.x * sin(theta) + q1.y * cos(theta);
    q2.z = q1.z;

    /* Inverse of step 3 */
    q1.x =   q2.x * d + q2.z * u.x;
    q1.y =   q2.y;
    q1.z = - q2.x * u.x + q2.z * d;

    /* Inverse of step 2 */
    if (d != 0) {
        q2.x =   q1.x;
        q2.y =   q1.y * u.z / d + q1.z * u.y / d;
        q2.z = - q1.y * u.y / d + q1.z * u.z / d;
    } else {
        q2 = q1;
    }

   /* Inverse of step 1 */
    q1.x = q2.x + p1.x;
    q1.y = q2.y + p1.y;
    q1.z = q2.z + p1.z;
    return q1;
}

// Converts a string with a number in it to an integer containing that number
int ToInt(std::string str) {
    int ret = 0;
    for(int i = str.length() - 1; i >= 0; i--) {
        ret += pow(10, str.length() - 1 - i) * (str[i] - '0');
    }
    return ret;
}

// Convert HSV color values to RGB color values
sf::Color HSVtoRGB(int hue, double sat, double val) {
    hue %= 360;
    while(hue < 0) hue += 360;

    if(sat < 0) sat = 0.0;
    if(sat > 1) sat = 1.0;

    if(val < 0) val = 0.0;
    if(val > 1) val = 1.0;

    int h = hue / 60;
    double f = double(hue) / 60 - h;
    double p = val * (1 - sat);
    double q = val * (1 - sat * f);
    double t = val * (1 - sat * (1 - f));

    switch(h)
    {
        default:
        case 0:
        case 6: return sf::Color(val * 255, t * 255, p * 255);
        case 1: return sf::Color(q * 255, val * 255, p * 255);
        case 2: return sf::Color(p * 255, val * 255, t * 255);
        case 3: return sf::Color(p * 255, q * 255, val * 255);
        case 4: return sf::Color(t * 255, p * 255, val * 255);
        case 5: return sf::Color(val * 255, p * 255, q * 255);
    }
}

bool IsApproxEqual(sf::Vector3f v1, sf::Vector3f v2) {
    return (((int)(v1.x + .5) == (int)(v2.x + .5)) &&
           ((int)(v1.y + .5) == (int)(v2.y+ .5)) &&
           ((int)(v1.z + .5) == (int)(v2.z + .5)));
}
