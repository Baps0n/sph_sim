#include "Particle.h"
#include <vector>
#include <cmath>

using namespace std;

const static int WIDTH = 800;
const static int HEIGHT = 600;
const static float FRAME_TIME = 0.004;

const static int PARTICLE_RADIUS = 8;
const static int COMPUTE_RADIUS = PARTICLE_RADIUS * PARTICLE_RADIUS;
const static float PARTICLE_MASS = 2;
const static float BORDER_PUSH = -0.2;
const static float PI = 3.1415;

const static float KERNEL_R = static_cast<float>(PARTICLE_RADIUS);
const static float K = 200;
const static float REST_DENSITY = 1000;
const static float FLUID_VISCOSITY = 350;

//const static float W_POLY6 = 315.f / (64.f * PI * pow(KERNEL_R, 9.f));
//const static float W_SPIKY = -15.f / (2.f * PI * pow(KERNEL_R, 6.f));
//const static float W_VISCOSITY = 45.f / (PI * pow(KERNEL_R, 6.f));

float getW_POLY6()
{
    static float W_POLY6 = 315.f / (64.f * PI * pow(KERNEL_R, 9.f));
    return W_POLY6;
}

float getW_SPIKY()
{
    static float W_SPIKY = -15.f / (2.f * PI * pow(KERNEL_R, 6.f));
    return W_SPIKY;
}

float getW_VISCOSITY()
{
    static float W_VISCOSITY = 45.f / (PI * pow(KERNEL_R, 6.f));
    return W_VISCOSITY;
}

Particle::Particle(sf::Vector2f position)
{
    pos = position;
    velocity = sf::Vector2f(0,1);
    force = sf::Vector2f(0,10);

    density = 1;
    pressure = 1;
}

bool Particle::operator==(Particle const& p) const
{
    if (velocity == p.velocity && force == p.force && density == p.density && pressure == p.pressure)
    {
        return true;
    }
    return false;
}

//std::ostream &operator<<(std::ostream &os, const Particle& p) {
//    os << p.pos;
//    return os;
//}


vector<Particle> Check_Borders(vector<Particle> particles)
{
    for (auto &i : particles)
    {
        if (i.pos.y + PARTICLE_RADIUS * 2 > HEIGHT)
        {
            i.velocity.y *= BORDER_PUSH;
            i.pos.y = HEIGHT - PARTICLE_RADIUS * 2;
        }
        if (i.pos.x < 0)
        {
            i.velocity.x *= BORDER_PUSH;
            i.pos.x = 0;
        }
        if (i.pos.x + PARTICLE_RADIUS * 2 > WIDTH)
        {
            i.velocity.x *= BORDER_PUSH;
            i.pos.x = WIDTH - PARTICLE_RADIUS * 2;
        }
    }
    return particles;
}

vector<Particle> Leap_frog_movement(vector<Particle> particles)
{
    for (auto &i : particles)
    {
        i.velocity += FRAME_TIME * i.force / i.density;
        i.pos += FRAME_TIME * i.velocity;
    }
    return particles;
}

vector<Particle> Pressure_calc(vector<Particle> particles)
{
    for (auto &i : particles)
    {
        i.density = 0.1;
        for (auto &j : particles)
        {
            if(&i != &j)
            {
                float dist = sqrt(pow(abs(i.pos.x - j.pos.x), 2.f) + pow(abs(i.pos.y - j.pos.y), 2.f));
                if (dist < COMPUTE_RADIUS)
                {
                    i.density += PARTICLE_MASS * getW_POLY6() * pow(COMPUTE_RADIUS - dist, 3.f);
                }
            }
        }
        i.pressure = K * (i.density - REST_DENSITY);
    }
    return particles;
}

vector<Particle> Forces_calc(vector<Particle> particles)
{
    for (auto &i : particles)
    {
        sf::Vector2f f_gravity(0.f, 10.f * PARTICLE_MASS / i.density);
        sf::Vector2f f_pressure(0.f, 0.f);
        sf::Vector2f f_viscosity(0.f, 0.f);

        for (auto &j : particles)
        {
            sf::Vector2f ij = i.pos - j.pos;
            float ij_len = sqrt((pow(ij.x,2.f) + pow(ij.y,2.f)));

            if (&i != &j)
            {
                sf::Vector2f normal_ij(ij.x / ij_len, ij.y / ij_len);

                if (ij_len < KERNEL_R * 2)
                {
                    f_pressure += normal_ij * PARTICLE_MASS * (i.pressure + j.pressure) / (2.f * j.density) * getW_SPIKY() *
                                  pow(KERNEL_R * 2 - ij_len, 3.f);
                    f_viscosity +=
                            FLUID_VISCOSITY * PARTICLE_MASS * (j.velocity - i.velocity) / j.density * getW_VISCOSITY() *
                            (KERNEL_R * 2 - ij_len);
                }
            }
        }
        i.force = f_gravity + f_pressure + f_viscosity;
    }
    return particles;
}