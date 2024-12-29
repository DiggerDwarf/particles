#ifndef Application_hpp_INCLUDED
#define Application_hpp_INCLUDED

#include <vector>
#include <random>
#define SFML_STATIC
#include <SFML/Graphics.hpp>

#include <iostream>
#include <cmath>
#include <utility.hpp>

#define UNIT_DISTANCE 20.F

typedef struct {
    sf::Vector2f    position;
    sf::Vector2f    prevPosition;
    float           radius;
    sf::Color       color;
    float lifetime;
    float age;
} Particle;


class Application
{
private:
    sf::RenderWindow        m_window;
    std::vector<Particle>   m_particles;
    sf::Clock               m_clock;
    sf::Time                m_runtime;
public:
    Application();

    // Application update wrapper function. Output is wether the app can keep running.
    bool Update();
    // Handles main inputs. Tells wrapper function what to return.
    bool handle_inputs();
    // Update the particles themselves.
    void update_particles(float deltaTime);

    // Application render process wrapper function.
    void Render();
    // Draw particles to the window
    void draw_particles();

};

#endif // Application_hpp_INCLUDED