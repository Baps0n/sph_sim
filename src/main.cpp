#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <cmath>
#include <random>

using namespace std;

const static int WIDTH = 800;
const static int HEIGHT = 600;
const static float FRAME_TIME = 0.004;

const static int PARTICLE_RADIUS = 8;
const static int COMPUTE_RADIUS = PARTICLE_RADIUS * PARTICLE_RADIUS;
const static float PARTICLE_MASS = 2;
const static float BORDER_PUSH = -0.2;
const static float PI = 3.14159265358;

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


class Particle
{
public:
    sf::Vector2f pos;
    sf::Vector2f velocity;
    sf::Vector2f force;

    float density;
    float pressure;

    explicit Particle(sf::Vector2f position)
    {
        pos = position;
        velocity = sf::Vector2f();
        force = sf::Vector2f();

        force.x = 0;
        force.y = 0;
        velocity.x = 0;
        velocity.y = 0;

        density = 1;
        pressure = 1;
    }
    bool operator==(Particle const& c) const
    {
        if (pos == c.pos && velocity == c.velocity && force == c.force && density == c.density && pressure == c.pressure)
        {
            return true;
        }
        return false;
    }
};


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


int main()
{
    sf::Shader shader;
    shader.loadFromFile("../shaders/shader.vert", "../shaders/shader.frag");

    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dist(0,1);

    sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "SPH");
    window.setKeyRepeatEnabled(false);
//    window.setVerticalSyncEnabled(true);
    window.setFramerateLimit(60);

    sf::RenderTexture renderTexture;
    renderTexture.create(WIDTH, HEIGHT);

    vector<Particle> sim_particles;
    char draw_mode = 'P';

    while (window.isOpen())
    {
        renderTexture.clear(sf::Color::Black);
        renderTexture.setSmooth(true);

        for (auto &i: sim_particles)
        {
            sf::CircleShape shape;
            shape.setRadius(KERNEL_R);
            shape.setFillColor(sf::Color(50, 100, 250));
            shape.setOutlineThickness(-2);
            shape.setOutlineColor(sf::Color(255, 255, 255));
            shape.setPosition(i.pos.x, i.pos.y);

            if (draw_mode == 'P')
            {
                shape.setRadius(KERNEL_R);
                shape.setOutlineThickness(-2);
                shape.setOutlineColor(sf::Color(255, 255, 255));
            }
            if (draw_mode == 'W')
            {
                shape.setRadius(KERNEL_R + KERNEL_R / 3);
            }

            renderTexture.draw(shape);
        }

        sf::Event event{};
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
            }

            if (event.type == sf::Event::KeyPressed)
            {
                if (event.key.code == sf::Keyboard::Escape)
                {
                    window.close();
                }
                if (event.key.code == sf::Keyboard::C)
                {
                    sim_particles.clear();
                }
                if (event.key.code == sf::Keyboard::O)
                {
                    draw_mode = 'W';
                }
                if (event.key.code == sf::Keyboard::P)
                {
                    draw_mode = 'P';
                }
                if (event.key.code == sf::Keyboard::Q)
                {
                    for (int y = PARTICLE_RADIUS; y < HEIGHT - PARTICLE_RADIUS*2; y += PARTICLE_RADIUS*2)
                    {
                        for (int x = WIDTH * 1 / 3; x <= WIDTH * 2 / 3; x += PARTICLE_RADIUS*2)
                        {
                            if (sim_particles.size() < 700)
                            {
                                sim_particles.emplace_back(sf::Vector2f(static_cast<float>(x + dist(gen)), static_cast<float>(y)));
                            }
                        }
                    }
                }
                if (event.key.code == sf::Keyboard::W)
                {
                    for (int y = PARTICLE_RADIUS; y < HEIGHT - PARTICLE_RADIUS*2; y += PARTICLE_RADIUS*2)
                    {
                        for (int x = WIDTH * 1 / 4 + y/2; x <= WIDTH * 3 / 4 - y/2; x += PARTICLE_RADIUS*2)
                        {
                            sim_particles.emplace_back(sf::Vector2f(static_cast<float>(x + dist(gen)), static_cast<float>(y)));
                        }
                    }
                }
                if (event.key.code == sf::Keyboard::E)
                {
                    for (int y = PARTICLE_RADIUS; y < HEIGHT - PARTICLE_RADIUS*2; y += PARTICLE_RADIUS*2)
                    {
                        for (int x = WIDTH * 1 / 4 + 300 - y/2 % 300; x <= WIDTH * 3 / 4 - (300 - y/2 % 300); x += PARTICLE_RADIUS*2)
                        {
                            sim_particles.emplace_back(sf::Vector2f(static_cast<float>(x + dist(gen)), static_cast<float>(y)));
                        }
                    }
                }
                if (event.key.code == sf::Keyboard::R)
                {
                    for (int y = PARTICLE_RADIUS; y < HEIGHT - PARTICLE_RADIUS*2; y += PARTICLE_RADIUS*2)
                    {
                        for (int x = WIDTH * 1 / 4 + (85 - y/2) * (-2 * (y/2 > 85) + 1); x <= WIDTH * 3 / 4 - (85 - y/2) * (-2 * (y/2 > 85) + 1); x += PARTICLE_RADIUS*2)
                        {
                            if (sim_particles.size() < 440) {
                                Particle a(sf::Vector2f(static_cast<float>(x + dist(gen)), static_cast<float>(y)));
                                a.velocity.x = 1000;
                                sim_particles.emplace_back(a);
                            }
                        }
                    }
                }
            }
            if (event.type == sf::Event::MouseButtonPressed)
            {
                if (event.mouseButton.button == sf::Mouse::Right)
                {
                    auto x = static_cast<float>(event.mouseButton.x - PARTICLE_RADIUS);
                    auto y = static_cast<float>(event.mouseButton.y - PARTICLE_RADIUS);
                    sim_particles.emplace_back(sf::Vector2f(x, y));
                }
            }
        }
        renderTexture.display();

        sim_particles = Pressure_calc(sim_particles);
        sim_particles = Forces_calc(sim_particles);
        sim_particles = Leap_frog_movement(sim_particles);
        sim_particles = Check_Borders(sim_particles);

        sf::Sprite sprite(renderTexture.getTexture());
        if(draw_mode == 'W')
        {
            window.draw(sprite, &shader);
        }
        if(draw_mode == 'P')
        {
            window.draw(sprite);
        }
        window.display();
    }
    return 0;
}
