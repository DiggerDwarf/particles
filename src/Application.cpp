#include "Application.hpp"
#include <random>

#define DOWNSCALE_FACTOR    1

Application::Application()
    : m_spread(0.02F)
{
    sf::ContextSettings contextSettings;
    contextSettings.majorVersion = 4;
    contextSettings.minorVersion = 3;
    this->m_window.create(sf::VideoMode::getDesktopMode(), "particles", sf::Style::None, contextSettings);

    this->m_fragShader.loadFromFile("src/frag.glsl", sf::Shader::Type::Fragment);

    sf::Vector2u winSize = this->m_window.getSize();

    this->m_renderBuffer.create(winSize.x / DOWNSCALE_FACTOR, winSize.y / DOWNSCALE_FACTOR);
    this->m_renderBuffer.setSmooth(true);

    this->m_window.setVerticalSyncEnabled(true);

    srand(time(nullptr));
    // for (size_t i = 0; i < 32; i++)
    // {
    //     for (size_t j = 0; j < 16; j++)
    //     {
    //         Particle newParticle;
    //         newParticle.pos = sf::Vector2f(i*50, j*50);
    //         // newParticle.radius = 10.F + 5.F*rand()/RAND_MAX;
    //         // newParticle.color = sf::Color(rand()%255, rand()%255, rand()%255);
    //         // newParticle.velocity = sf::Vector2f(0, 0);
    //         this->m_particles.push_back(newParticle);
    //     }
    // }

    // for (size_t i = 0; i < 32; i++)
    // {
    //     for (size_t j = 0; j < 16; j++)
    //     {
    //         this->m_particles[i*16 + j] = Particle{.pos=sf::Vector2f(i, j)};
    //     }
    // }

    glewInit();
    this->m_computeShader = compile_compute_shader_file("src/compute.glsl");
    set_uniform(this->m_computeShader, "windowSize", this->m_window.getSize());
    this->pBuffer = set_buffer(this->m_computeShader, NULL, sizeof(float[512][2][2]), 1);

    set_uniform(this->m_computeShader, "init", true);
    execute_compute_shader(this->m_computeShader, 512, 1, 1);
    set_uniform(this->m_computeShader, "init", false);
}

Application::~Application()
{
    this->m_window.close();
}

bool Application::Update()
{
    float deltaTime = this->m_clock.restart().asSeconds();
    bool canContinue = this->handle_inputs();
    if (!canContinue) return false;
    this->update_particles(deltaTime);
    return true;
}

bool Application::handle_inputs()
{
    sf::Event event;

    while (this->m_window.pollEvent(event))
    {
        switch (event.type)
        {
        case sf::Event::EventType::Closed:
            return false;
        // case sf::Event::LostFocus:
        //     return false;
        
        case sf::Event::EventType::KeyPressed: {
            switch (event.key.code)
            {
            case sf::Keyboard::Escape:
                return false;
            
            case sf::Keyboard::Up:
                this->m_spread *= 1.5;
                break;
            case sf::Keyboard::Down:
                this->m_spread /= 1.5;
                break;

            default:
                break;
            }
            break;
        }

        case sf::Event::EventType::Resized:
            this->m_window.setSize(sf::Vector2u(event.size.width, event.size.height));
            set_uniform(this->m_computeShader, "windowSize", this->m_window.getSize());
            break;

        default:
            break;
        }
    }
    

    return true;
}

void Application::update_particles(float deltaTime)
{
    set_uniform(this->m_computeShader, "deltaTime", deltaTime);
    execute_compute_shader(this->m_computeShader, 512, 1, 1);
    read_buffer(this->m_computeShader, this->m_particles, this->pBuffer, 0, sizeof(float[512][2]));


    // sf::Vector2f winSize(this->m_window.getSize());
    // for (std::vector<Particle>::iterator obj = this->m_particles.begin(); obj != this->m_particles.end(); obj++)
    // {
    //     obj->velocity.y += 5*UNIT_DISTANCE*deltaTime;

    //     if (sf::Mouse::isButtonPressed(sf::Mouse::Left) || sf::Mouse::isButtonPressed(sf::Mouse::Right))
    //     {
    //         const float maxInteractDistance = 20; // in terms of unit distances
    //         sf::Vector2f relative = obj->pos - static_cast<sf::Vector2f>(sf::Mouse::getPosition(this->m_window));
    //         float distance = length(relative) / UNIT_DISTANCE;
    //         if (distance < maxInteractDistance)
    //         {
    //             float t = distance / maxInteractDistance;
    //             float interactStrength = t*t*(2*t - 3) + 1;
    //             interactStrength *= sf::Mouse::isButtonPressed(sf::Mouse::Left) ? UNIT_DISTANCE*10 : -UNIT_DISTANCE*2;
    //             // interactStrength *= std::sqrt(maxInteractDistance);
    //             obj->velocity += (relative / distance) * interactStrength * deltaTime;
    //         }
    //     }

    //     for (std::vector<Particle>::iterator otherObj = (++obj)--; otherObj != this->m_particles.end(); otherObj++)
    //     {
    //         const float maxInteractDistance = 5; // in terms of unit distances
    //         sf::Vector2f relative = obj->pos - otherObj->pos;
    //         float distance = length(relative) / UNIT_DISTANCE;
    //         if (distance < maxInteractDistance)
    //         {
    //             float t = distance / maxInteractDistance;
    //             float interactStrength = (t*t*(2*t - 3) + 1) * 5.F;
    //             // interactStrength *= std::sqrt(maxInteractDistance);
    //             obj->     velocity += (relative / distance) * interactStrength * deltaTime;
    //             otherObj->velocity -= (relative / distance) * interactStrength * deltaTime;
    //         }
            
    //     }
        


    //     obj->pos += obj->velocity;
        
        
    //     // bbox accounting for radius of the particle
    //     if ((obj->pos.x < obj->radius) || (obj->pos.x > winSize.x - obj->radius))
    //     {
    //         clamp(obj->pos.x, obj->radius, winSize.x - obj->radius);
    //         // obj->velocity.x = 2*obj->pos.x - obj->velocity.x;
    //         obj->velocity.x *= -0.9;
    //     }
    //     if ((obj->pos.y < obj->radius) || (obj->pos.y > winSize.y - obj->radius))
    //     {
    //         clamp(obj->pos.y, obj->radius, winSize.y - obj->radius);
    //         // obj->velocity.y = 2*obj->pos.y - obj->velocity.y;
    //         obj->velocity.y *= -0.9;
    //     }
    // }
    
}

void Application::Render()
{
    /**
     * This Wrapper's reason of existence is the slight possibility
     * that one day i want to render something other than the simple
     * particles and i don't want to clog the whole thing and reduce
     * readability. I apologize sincerely if you wholeheartedly read
     * this particularly useless comment that i know even I won't
     * read in the future.
     * I am here once again to continue this rant. However this time
     * it will take a bit of time to get to the actual matter since I
     * don't know what i want to talk about. Bbbb
     */

    this->m_window.clear();

    this->draw_particles();

    this->m_window.display();
}

void Application::draw_particles()
{
    // sf::CircleShape dot;
    // // dot.setPointCount(10);
    // for (const Particle& obj : this->m_particles)
    // {
    //     dot.setRadius(10);
    //     dot.setFillColor(sf::Color::Blue);
    //     dot.setPosition(obj.pos - sf::Vector2f(10, 10));
    //     m_window.draw(dot);
    // }

    sf::Vector2u windowSize = this->m_window.getSize();

    sf::ConvexShape quad(4);
    quad.setPoint(0, sf::Vector2f(0,            0           ));
    quad.setPoint(1, sf::Vector2f(windowSize.x, 0           ));
    quad.setPoint(2, sf::Vector2f(windowSize.x, windowSize.y));
    quad.setPoint(3, sf::Vector2f(0,            windowSize.y));

    sf::Glsl::Vec2* positions = new sf::Glsl::Vec2[1024];
    for (size_t i = 0; i < 512; i++)
    {
        positions[i] = this->m_particles[i].pos / (float)DOWNSCALE_FACTOR;
    }

    this->m_fragShader.setUniform("spread", this->m_spread);
    this->m_fragShader.setUniformArray("positions", positions, 512);
    delete[] positions;
    this->m_fragShader.setUniform("u_resolution", sf::Glsl::Vec2(windowSize/(unsigned int)DOWNSCALE_FACTOR));

    this->m_window.clear();
    this->m_renderBuffer.clear();
    this->m_renderBuffer.draw(quad, sf::RenderStates(&this->m_fragShader));
    this->m_renderBuffer.display();

    quad.setTexture(&this->m_renderBuffer.getTexture(), true);


    this->m_window.draw(quad);
    // this->m_window.display();
    
}
