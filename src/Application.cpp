#include "Application.hpp"

Application::Application()
    : m_window(sf::VideoMode::getDesktopMode(), "particles", sf::Style::None)
{
    // this->m_window.setFramerateLimit(60);
    time_t seed = time(nullptr);
    std::cout << "Seed : " << seed << '\n';
    srand(seed);
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
        case sf::Event::Closed:
            return false;
        case sf::Event::LostFocus:
            return false;
        
        case sf::Event::KeyPressed: {
            switch (event.key.code)
            {
            case sf::Keyboard::Escape:
                return false;
            
            default:
                break;
            }
            break;
        }
        case sf::Event::MouseWheelMoved: {
            int id = rand();
            Particle newObj = {};
            newObj.radius = id%5 + 10;
            newObj.position = static_cast<sf::Vector2f>(sf::Mouse::getPosition(this->m_window));
            // newObj.position = {0, 0};
            newObj.prevPosition = newObj.position - sf::Vector2f(id%6 - 3, id%7 - 3) * UNIT_DISTANCE / 2.F;
            
            newObj.color.r = id%255;
            newObj.color.g = id%254;
            newObj.color.b = id%253;
            newObj.color.a = 255;

            newObj.lifetime = (rand() % 150) / 10;
            newObj.age = 0;
            
            this->m_particles.push_back(newObj);
            // std::printf("id: %5u     pos: %4.f,%4.f     vel: %2.f,%2.f      rad: %2.f\n",
            // id, newObj.position.x, newObj.position.y, newObj.velocity.x, newObj.velocity.y, newObj.radius);
            break;
        }

        default:
            break;
        }
    }
    

    return true;
}

void Application::update_particles(float deltaTime)
{
    sf::Vector2f winSize(this->m_window.getSize());
    for (std::vector<Particle>::iterator obj = this->m_particles.begin(); obj != this->m_particles.end(); obj++)
    {
        sf::Vector2f newPos = 2.F*obj->position - obj->prevPosition;
        obj->prevPosition = obj->position;
        obj->position = newPos;
        obj->position.y += 5 * UNIT_DISTANCE * deltaTime;

        // Simple despawn timer
        // obj->age += deltaTime;
        // if (obj->age > obj->lifetime)
        // {
        //     this->m_particles.erase(obj);
        //     obj--;
        //     continue;
        // }
        // obj->color.a = 255 * (1-(obj->age/obj->lifetime));
        
        // warparound accounting for radius of the particle
        if ((obj->position.x < obj->radius) || (obj->position.x > winSize.x - obj->radius))
        {
            clamp(obj->position.x, obj->radius, winSize.x - obj->radius);
            obj->prevPosition.x = 2*obj->position.x - obj->prevPosition.x;
        }
        if ((obj->position.y < obj->radius) || (obj->position.y > winSize.y - obj->radius))
        {
            clamp(obj->position.y, obj->radius, winSize.y - obj->radius);
            obj->prevPosition.y = 2*obj->position.y - obj->prevPosition.y;
        }

        for (std::vector<Particle>::iterator otherObj = (++obj)--; otherObj != this->m_particles.end(); otherObj++)
        {
            // sf::Vector2f direction = obj->position - otherObj->position;
            // float overlapDistance = length(direction);
            // if (overlapDistance <= obj->radius + otherObj->radius)
            // {
            //     direction /= overlapDistance;

            //     obj     ->velocity -= 2.F * dot(direction, obj     ->velocity) * direction;
            //     otherObj->velocity -= 2.F * dot(direction, otherObj->velocity) * direction;

            //     obj     ->position += direction * (overlapDistance/2);
            //     otherObj->position -= direction * (overlapDistance/2);
            // }

            const float maxInteractDistance = 20; // in terms of unit distances
            sf::Vector2f relative = obj->position - otherObj->position;
            float distance = length(relative) / UNIT_DISTANCE;
            if (distance > maxInteractDistance) continue;
            float t = std::min(std::max(distance, 0.F), maxInteractDistance) / maxInteractDistance;
            float interactStrength = t*t*(2*t - 3) + 1;
            // interactStrength *= std::sqrt(maxInteractDistance);
            obj->position += (relative / distance) * interactStrength * deltaTime;
            otherObj->position -= (relative / distance) * interactStrength * deltaTime;
        }
        


        
        if (sf::Mouse::isButtonPressed(sf::Mouse::Left) || sf::Mouse::isButtonPressed(sf::Mouse::Right))
        {
            const float maxInteractDistance = 20; // in terms of unit distances
            sf::Vector2f relative = obj->position - static_cast<sf::Vector2f>(sf::Mouse::getPosition(this->m_window));
            float distance = length(relative) / UNIT_DISTANCE;
            if (distance > maxInteractDistance) continue;
            float t = std::min(std::max(distance, 0.F), maxInteractDistance) / maxInteractDistance;
            float interactStrength = t*t*(2*t - 3) + 1;
            interactStrength *= sf::Mouse::isButtonPressed(sf::Mouse::Left) ? UNIT_DISTANCE : -UNIT_DISTANCE;
            // interactStrength *= std::sqrt(maxInteractDistance);
            obj->position += (relative / distance) * interactStrength * deltaTime;
        }
        
    }
    
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
    sf::CircleShape dot;
    // dot.setPointCount(10);
    for (const Particle& obj : this->m_particles)
    {
        dot.setRadius(obj.radius);
        dot.setFillColor(obj.color);
        dot.setPosition(obj.position - sf::Vector2f(obj.radius, obj.radius));
        m_window.draw(dot);
    }
}
