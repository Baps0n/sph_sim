#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <random>
#include "Particle.cpp"

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
                            if (sim_particles.size() < 1000)
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
                            if (sim_particles.size() < 1000)
                            {
                                sim_particles.emplace_back(sf::Vector2f(static_cast<float>(x + dist(gen)), static_cast<float>(y)));
                            }
                        }
                    }
                }
                if (event.key.code == sf::Keyboard::E)
                {
                    for (int y = PARTICLE_RADIUS; y < HEIGHT - PARTICLE_RADIUS*2; y += PARTICLE_RADIUS*2)
                    {
                        for (int x = WIDTH * 1 / 4 + 300 - y/2 % 300; x <= WIDTH * 3 / 4 - (300 - y/2 % 300); x += PARTICLE_RADIUS*2)
                        {
                            if (sim_particles.size() < 1000) {
                                sim_particles.emplace_back(sf::Vector2f(static_cast<float>(x + dist(gen)), static_cast<float>(y)));
                            }
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
        sim_particles = Leapfrog_movement(sim_particles);
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
