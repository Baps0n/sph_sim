#include <doctest.h>
#include <vector>
#include "Particle.cpp"

using namespace std;


TEST_CASE("W_POLY6") {
    REQUIRE(getW_POLY6() == 0.00000001167303054216972668655216693878173828125);
}

TEST_CASE("W_SPIKY") {
    REQUIRE(getW_SPIKY() == -0.0000091071879069204442203044891357421875);
}

TEST_CASE("W_SPIKY") {
    REQUIRE(getW_VISCOSITY() == 0.000054643125622533261775970458984375);
}


TEST_CASE("Leapfrog_movement") {
    vector<Particle> test_particles;
    test_particles.emplace_back(sf::Vector2f(100,200));
    test_particles.emplace_back(sf::Vector2f(100,210));
    auto test_res = Leapfrog_movement(test_particles);
    auto est_res = test_particles;
    est_res[0].velocity = sf::Vector2f(0,1 + 10 * FRAME_TIME);
    est_res[0].pos = sf::Vector2f(100,200 + FRAME_TIME);
    est_res[1].velocity = sf::Vector2f(0,1 + 10 * FRAME_TIME);
    est_res[1].pos = sf::Vector2f(100,210 + FRAME_TIME);

    REQUIRE(test_res == est_res);
}

TEST_CASE("Check_Borders_ok") {
    vector<Particle> test_particles;
    test_particles.emplace_back(sf::Vector2f(100,200));
    test_particles.emplace_back(sf::Vector2f(100,210));
    auto test_res = Check_Borders(test_particles);
    auto est_res = test_particles;

            REQUIRE(test_res == est_res);
}

TEST_CASE("Check_Borders_crossed") {
    vector<Particle> test_p;
    test_p.emplace_back(sf::Vector2f(-1,HEIGHT - 2 * PARTICLE_RADIUS + 1));
    test_p.emplace_back(sf::Vector2f(WIDTH - 2 * PARTICLE_RADIUS + 1, 100000));
    auto test_res = Check_Borders(test_p);

    auto est_res = test_p;
    est_res[0].pos = sf::Vector2f(0,HEIGHT - 2 * PARTICLE_RADIUS);
    est_res[0].velocity *= BORDER_PUSH;
    est_res[1].pos = sf::Vector2f(HEIGHT - 2 * PARTICLE_RADIUS,100000);
    est_res[1].velocity *= BORDER_PUSH;

    REQUIRE(test_res == est_res);
}

TEST_CASE("Pressure_calc") {
    vector<Particle> test_particles;
    test_particles.emplace_back(sf::Vector2f(100,200));
    test_particles.emplace_back(sf::Vector2f(100,210));
    auto test_res = Pressure_calc(test_particles);
    auto est_res = test_particles;
    est_res[0].density = 0.103676162660121917724609375;
    est_res[0].pressure = -199979.265625;
    est_res[1].density = 0.103676162660121917724609375;
    est_res[1].pressure = -199979.265625;

    REQUIRE(test_res == est_res);
}

TEST_CASE("Forces_calc_close") {
    vector<Particle> test_particles;
    test_particles.emplace_back(sf::Vector2f(100,200));
    test_particles.emplace_back(sf::Vector2f(100,205));
    auto test_res = Forces_calc(test_particles);
    auto est_res = test_particles;
    est_res[0].force = sf::Vector2f(0, 20.024242401123046875);
    est_res[1].force = sf::Vector2f(0, 19.975757598876953125);

    REQUIRE(test_res == est_res);
}

TEST_CASE("Forces_calc_far") {
    vector<Particle> test_particles;
    test_particles.emplace_back(sf::Vector2f(100,200));
    test_particles.emplace_back(sf::Vector2f(400,400));
    auto test_res = Forces_calc(test_particles);
    auto est_res = test_particles;
    est_res[0].force = sf::Vector2f(0, 20);
    est_res[1].force = sf::Vector2f(0, 20);

    REQUIRE(test_res == est_res);
}