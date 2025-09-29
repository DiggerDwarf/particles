#version 430

#define ID gl_GlobalInvocationID.x

layout(local_size_x = 8, local_size_y = 1, local_size_z = 1) in;

uniform bool init;
uniform uvec2 windowSize;

uniform float deltaTime;
#define dt (deltaTime*20.0)

#define nbParticles 1000

uniform uvec2[nbParticles] spatial_ID_hash;
uniform int[500] spatial_hashToSegmentStart;

struct Particle
{
    vec2 position;
    vec2 velocity;
};

layout(std430, binding = 1) buffer Positions
{
    // vec2 positions[nbParticles];
    // vec2 velocities[nbParticles];
    Particle particles[nbParticles];
};

uniform vec2 mPos;
uniform int mState;

#define pos particles[ID].position
#define vel particles[ID].velocity

#define ONE_TWO_PI      0.159154943091895
#define spread 0.35

#define scale 6
#define grid_size scale

vec2 density_gradient(const vec2 rel)
{
    const float t = clamp(1 - (length(rel)/scale), 0, 1);
    return rel * (scale * t * t);
}

void main()
{
    if (init) {
        pos = vec2((ID%70)*40.0 + 20.0, (ID/35.0)*20.0 + 20.0);
        // pos = vec2(100, 100);
        vel = vec2(0);
        return;
    }

    // vel.y += 5.0 * dt;     // gravity
    vel *= 0.99;

    vec2 predictedPos = pos + vel*dt;

    #define uhh 0.9
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

    ivec2 gridPos = ivec2(int(pos.x/140.0), int(pos.y/140.0));
    vec2 gradient = vec2(0);


    int hash;
    for (int i = -1; i <= 1; i++) {
        for (int j = -1; j <= 1; j++) {
            hash = ((((gridPos.x+i) * 6101 + (gridPos.y+j) * 1999) % 500) + 500) % 500;
            if (spatial_hashToSegmentStart[hash] == -1) continue;
            for (uint pID = spatial_hashToSegmentStart[hash]; spatial_ID_hash[pID].y == hash && pID < nbParticles; pID++) {
                gradient += density_gradient((predictedPos - particles[spatial_ID_hash[pID].x].position)*0.05) * dt;
            }
        }
    }

    if (mState != 0) {
        vec2 dir = predictedPos - mPos;
        float dist = length(dir);
        // dir /= dist;
        dir = dist != 0 ? dir/dist : vec2(0);
        float t = max(1- dist/400, 0);
        gradient += (dir * (mState*200) - vel) * t * dt;
    }

    vel += gradient * dt;
    pos += vel * dt;
}
