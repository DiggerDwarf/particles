#version 430

#define ID gl_WorkGroupID.x

layout(local_size_x = 1, local_size_y = 1, local_size_z = 1) in;

uniform bool init;
uniform uvec2 windowSize;
uniform float deltaTime;

#define dt (deltaTime*20.0)

layout(std430, binding = 1) buffer Positions
{
    vec2 positions[512];
    vec2 velocities[512];
};

#define pos positions[ID]
#define vel velocities[ID]

#define ONE_TWO_PI      0.159154943091895
#define spread 0.35

vec2 density_gradient(vec2 rel) {
    return rel * (300.0*rel.x*rel.y*exp(-(rel.x*rel.x + rel.y*rel.y)/(2*spread*spread))*ONE_TWO_PI/pow(spread, 6.0));
}

void main()
{
    if (init) {
        pos = vec2((ID%32)*50.0, floor(ID/16.0)*50.0);
        vel = vec2(0);
        return;
    }

    pos += vel * dt;
    vel.y += 5.0 * dt;     // gravity
    
    if (pos.y > windowSize.y) {
        pos.y = windowSize.y;
        vel.y *= -0.9;
    }

    vec2 gradient = vec2(0);
    for (int i = 0; i < 512; i++) {
        gradient += density_gradient((pos - positions[i])*0.05);
    }

    vel += gradient * dt;

}
