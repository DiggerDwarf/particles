#ifndef Application_hpp_INCLUDED
#define Application_hpp_INCLUDED

#include <vector>
#include <random>

#include <SFML/Graphics.hpp>

#define COMPUTE_INTEGRATE_SFML
#include "computeHandler.hpp"

#include <iostream>
#include <cmath>
#include <utility.hpp>

#define UNIT_DISTANCE 20.F

typedef struct {
    sf::Vector2f    pos;
    // sf::Vector2f    velocity;
    // float           radius;
    // sf::Color       color;
    // float lifetime;
    // float age;
} Particle;



class Application
{
private:
    sf::RenderWindow        m_window;
    // std::vector<Particle>   m_particles;
    Particle                m_particles[512];
    sf::Clock               m_clock;
    // sf::Time                m_runtime;
    sf::Shader              m_fragShader;
    sf::RenderTexture       m_renderBuffer;
    float                   m_spread;   // maybe remove later
    ComputeShader           m_computeShader;
    unsigned int            pBuffer;
    // sf::Font                m_font;
    // bool                    m_canUseFont;
public:
    Application();
    ~Application();

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