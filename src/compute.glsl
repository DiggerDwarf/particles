#version 430

#define ID gl_WorkGroupID.x

layout(local_size_x = 1, local_size_y = 1, local_size_z = 1) in;

uniform bool init;
uniform uvec2 windowSize;

uniform float deltaTime;
#define dt (deltaTime*20.0)

#define nbParticles 512

// struct Particle {
//     vec2 position;
//     vec2 velocity;
// };

layout(std430, binding = 1) buffer Positions
{
    vec2 positions[nbParticles];
    vec2 velocities[nbParticles];
    // Particle particles[nbParticles];
};

uniform vec2 mPos;
uniform int mState;

#define pos positions[ID]
#define vel velocities[ID]

#define ONE_TWO_PI      0.159154943091895
#define spread 0.35

#define scale 7

vec2 density_gradient(const vec2 rel) {
    // return rel * (rel.x*rel.y*exp(-(rel.x*rel.x + rel.y*rel.y)/(5.0*2.0*spread*spread))*ONE_TWO_PI/pow(spread, 6.0));
const float t = clamp(1 - (length(rel)/scale), 0, 1);
    return rel * (scale * t * t);
    
}

void main()
{
    if (init) {
        pos = vec2((ID%32)*50.0, (ID/16.0)*50.0);
        vel = vec2(0);
        return;
    }

    pos += vel * dt;
    // vel.y += 5.0 * dt;     // gravity
    vel *= 0.99;

    #define uhh 0.8
    if (pos.y > windowSize.y) {
        pos.y = windowSize.y;
        vel.y *= -uhh;
    } else if (pos.y < 0) {
        pos.y = 0;
        vel.y *= -uhh;
    } if (pos.x > windowSize.x) {
        pos.x = windowSize.x;
        vel.x *= -uhh;
    } else if (pos.x < 0) {
        pos.x = 0;
        vel.x *= -uhh;
    }

    vec2 gradient = vec2(0);
    for (int i = 0; i < nbParticles; i++) {
        gradient += density_gradient((pos - positions[i])*0.05);
    }

    if (mState != 0) {
        gradient += 50*mState*density_gradient((pos - mPos)*0.05*0.5);
    }

    vel += gradient * dt;

}
