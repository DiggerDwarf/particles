#include "Application.hpp"
#include <random>

#define DOWNSCALE_FACTOR    10

Application::Application()
    : m_spread(.05F)
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

    glewInit();
    this->m_computeShader = compile_compute_shader_file("src/compute.glsl");
    set_uniform(this->m_computeShader, "windowSize", this->m_window.getSize());
    this->pBuffer = set_buffer(this->m_computeShader, NULL, sizeof(float[nbParticles][2][2]), 1);

    set_uniform(this->m_computeShader, "init", true);
    execute_compute_shader(this->m_computeShader, nbParticles/8, 1, 1);
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
        
        case sf::Event::EventType::KeyPressed: {
            switch (event.key.code)
            {
            case sf::Keyboard::Escape:
                return false;
            
            case sf::Keyboard::Up:
                this->m_spread *= 1.2;
                break;
            case sf::Keyboard::Down:
                this->m_spread /= 1.2;
                break;
            case sf::Keyboard::R: {
                float temp[nbParticles*2*2];
                read_buffer(this->m_computeShader, temp, this->pBuffer, 0, sizeof(float[nbParticles][2][2]));
                delete_compute_shader(this->m_computeShader);
                this->m_computeShader = compile_compute_shader_file("src/compute.glsl");
                this->pBuffer = set_buffer(this->m_computeShader, temp, sizeof(float[2][2][nbParticles]), 1);
                set_uniform(this->m_computeShader, "windowSize", this->m_window.getSize());
                set_uniform(this->m_computeShader, "init", false);
            }
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
    // Calculate grid hash thingy
    
    for (unsigned int i = 0; i < nbParticles; i++)
    {
        // get grid pos
        sf::Vector2i gridPos(this->m_particles[i].pos / 140.0F);
        unsigned int hash = (((gridPos.x * 6101 + gridPos.y * 1999) % 500) + 500) % 500;
        this->firstThingy[i] = sf::Vector2u(i, hash);
    }

    struct {
        bool operator()(const sf::Vector2u a, const sf::Vector2u b) const { return a.y > b.y; }
    } customLess;

    std::sort(this->firstThingy.begin(), this->firstThingy.end(), customLess);

    
    for (int i = 0; i < 500; i++)
    {
        this->secondThingy[i] = -1;
    }
    
    for (int i = nbParticles - 1; i >= 0; i--)
    {
        this->secondThingy[this->firstThingy[i].y] = i;
    }
    
    set_uniform_array(this->m_computeShader, "spatial_ID_hash", &*this->firstThingy.begin(), nbParticles);
    set_uniform_array_scalar(this->m_computeShader, "spatial_hashToSegmentStart", this->secondThingy, 500);

    set_uniform(this->m_computeShader, "mPos", static_cast<sf::Vector2f>(sf::Mouse::getPosition(this->m_window)));
    set_uniform(this->m_computeShader, "mState", sf::Mouse::isButtonPressed(sf::Mouse::Button::Left) ? 1 :
                                                sf::Mouse::isButtonPressed(sf::Mouse::Button::Right) ? -1 : 0);
    set_uniform(this->m_computeShader, "deltaTime", deltaTime);
    execute_compute_shader(this->m_computeShader, nbParticles/8, 1, 1);
    read_buffer(this->m_computeShader, this->m_particles, this->pBuffer, 0, sizeof(float[nbParticles][2][2]));


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
}

void Application::Render()
{
    this->m_window.clear();

    this->draw_particles();

    this->m_window.display();
}

void Application::draw_particles()
{
    // sf::CircleShape dot;
    // dot.setPointCount(3);
    // for (const Particle& obj : this->m_particles)
    // {
    //     dot.setRadius(20);
    //     dot.setFillColor(sf::Color::Blue);
    //     dot.setPosition(obj.pos - sf::Vector2f(20, 20));
    //     m_window.draw(dot);
    // }



    sf::Vector2u windowSize = this->m_window.getSize();

    sf::ConvexShape quad(4);
    quad.setPoint(0, sf::Vector2f(0,            0           ));
    quad.setPoint(1, sf::Vector2f(windowSize.x, 0           ));
    quad.setPoint(2, sf::Vector2f(windowSize.x, windowSize.y));
    quad.setPoint(3, sf::Vector2f(0,            windowSize.y));

    sf::Glsl::Vec2* positions = new sf::Glsl::Vec2[nbParticles];
    for (size_t i = 0; i < nbParticles; i++)
    {
        positions[i] = this->m_particles[i].pos / (float)DOWNSCALE_FACTOR;
    }

    this->m_fragShader.setUniform("spread", this->m_spread);
    this->m_fragShader.setUniformArray("positions", positions, nbParticles);
    delete[] positions;
    this->m_fragShader.setUniform("u_resolution", sf::Glsl::Vec2(windowSize/(unsigned int)DOWNSCALE_FACTOR));

    this->m_window.clear();
    this->m_renderBuffer.clear();
    this->m_renderBuffer.draw(quad, sf::RenderStates(&this->m_fragShader));
    this->m_renderBuffer.display();

    quad.setTexture(&this->m_renderBuffer.getTexture(), true);
    
    this->m_window.draw(quad);
}
