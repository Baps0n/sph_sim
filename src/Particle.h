#ifndef SPH_SIM_PARTICLE_H
#define SPH_SIM_PARTICLE_H

#include <SFML/System/Vector2.hpp>

class Particle
{
public:
    explicit Particle(sf::Vector2f position);

    sf::Vector2f pos;
    sf::Vector2f velocity;
    sf::Vector2f force;

    float density;
    float pressure;

    bool operator==(Particle const& c) const;
//    friend std::ostream& operator<<(std::ostream& os, const Particle& p);
};

#endif //SPH_SIM_PARTICLE_H
