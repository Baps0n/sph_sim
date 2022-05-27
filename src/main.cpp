#include <SFML/Window.hpp>
#include <iostream>
#include <SFML/Graphics/Shape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/CircleShape.hpp>

using namespace std;

struct PARTICLE{
    sf::Vector2f pos;
    sf::Vector2f velocity;
    sf::Vector2f force;

    float density, pressure;
};



int main()
{
    sf::RenderWindow window(sf::VideoMode(800, 600), "SPH");
    window.setKeyRepeatEnabled(false);

    while (window.isOpen())
    {
        //window.clear();
        sf::CircleShape shape(50.f);
        shape.setFillColor(sf::Color(100, 250, 50));

        sf::Event event;

        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::KeyPressed)
            {
                if (event.key.code == sf::Keyboard::Escape)
                {
                    window.close();
                }
            }
            if (event.type == sf::Event::MouseButtonPressed)
            {
                if (event.mouseButton.button == sf::Mouse::Right)
                {
                    cout << "x " << event.mouseButton.x << endl;
                    cout << "y " << event.mouseButton.y << endl;
                    float x = event.mouseButton.x;
                    float y = event.mouseButton.y;
                    sf::CircleShape shape(50.f);
                    shape.setFillColor(sf::Color(100, 250, 50));
                    shape.setPosition(x,y);
                    window.draw(shape);

                }
            }
        }
        window.draw(shape);

        window.display();
    }

    return 0;
}
