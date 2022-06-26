#include <SFML/System/Vector2.hpp>

/**
 * Класс частицы воды.
 */
class Particle
{
public:
    explicit Particle(sf::Vector2f position);

    /// Вектор координат частицы.
    sf::Vector2f pos;
    /// Вектор скорости частицы.
    sf::Vector2f velocity;
    /// Вектор результирующей силы, действующей на частицу.
    sf::Vector2f force;

    /// Величина плотности.
    float density;
    /// Величина давления.
    float pressure;

    bool operator==(Particle const& c) const;
//    friend std::ostream& operator<<(std::ostream& os, const Particle& p);
};

