#include "graphics.h"


void set(float (&p)[4], Vector4 v)
{
    for (int i = 0; i < 4; ++i) p[i] = v[i];
}

void setLightingSource()
{
    set(world.lights[0].position, Vector4(2, 2, 2, 0));
    set(world.lights[0].ambient, Vector4(0.1, 0.1, 0.1, 1));
    set(world.lights[0].diffuse, Vector4(0.6, 0.6, 0.6, 1));
    world.lights[0].constantAttenuation = 3;
    world.lights[0].spotExponent = 1;
    world.lights[0].is_on = 1;
    
    set(world.lights[1].position, Vector4(2, 2, 2, 1));
    set(world.lights[1].ambient, Vector4(0.3, 0.3, 0.3, 1));
    set(world.lights[1].diffuse, Vector4(0.6, 0.6, 0.6, 1));
    set(world.lights[1].specular, Vector4(0.6, 0.6, 0.6, 1));
    world.lights[1].constantAttenuation = 3;
    world.lights[1].spotExponent = 1;
    world.lights[1].is_on = 0;

    set(world.lights[1].position, Vector4(2, 2, 2, 1));
    set(world.lights[1].ambient, Vector4(0.7, 0.3, 0.3, 1));
    set(world.lights[1].diffuse, Vector4(0.6, 0.1, 0.6, 1));
    set(world.lights[1].specular, Vector4(0.6, 0.1, 0.6, 1));
    world.lights[1].constantAttenuation = 5;
    world.lights[1].spotExponent = 1;
    world.lights[1].is_on = 0;
}
