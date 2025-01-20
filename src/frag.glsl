#ifdef GL_ES
    precision mediump float;
#endif

#define PI_TWO			1.570796326794897
#define PI				3.141592653589793
#define TWO_PI			6.283185307179586
#define ONE_TWO_PI      0.159154943091895

uniform vec2 positions[512];

// layout(std430, binding = 1) buffer Positions
// {
//     vec2 positions[512];
// }

uniform vec2 u_resolution;
uniform float spread;

vec2 coord(in vec2 p) {
    p = p / u_resolution.xy *2.0 -1.0;
    p.x *= u_resolution.x / u_resolution.y;
    return p;
}

float density(const vec2 origin, const vec2 pos) {
    vec2 rel = pos-origin;
    // return exp(-(rel.x*rel.x + rel.y*rel.y)/(spread*spread));
    return exp(-(rel.x*rel.x + rel.y*rel.y)/(2*spread*spread))*ONE_TWO_PI/(10*spread*spread);
}

void main() {
    const vec4 base_color = vec4(0.41, 0.7, 0.87, 1);

    // vec2 mCoord = coord(position);
    vec2 uv = coord(gl_FragCoord.xy);
    uv.y *= -1;

    float pointDensity = 0;
    for (int i = 0; i < 512; i++) {
        pointDensity += density(coord(positions[i]), uv);
    }

    gl_FragColor = vec4(base_color.xyz * pointDensity * 0.1, 1);
}