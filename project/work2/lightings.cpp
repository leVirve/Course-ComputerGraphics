#include "graphics.h"


void set(float (&p)[4], Vector4 v)
{
    for (int i = 0; i < 4; ++i) p[i] = v[i];
}

void setLightingSource()
{
    /** Directional Light **/
    set(world.lights[0].position, Vector4(2, 2, 2, 0));
    set(world.lights[0].ambient, Vector4(0.5f, 0.5f, 0.5f, 1));
    set(world.lights[0].diffuse, Vector4(1, 1, 1, 1));
    set(world.lights[0].specular, Vector4(0.2f, 0.2f, 0.2f, 1));
    world.lights[0].constantAttenuation = 0.8f;
    world.lights[0].spotExponent = 1;
    world.lights[0].spotCutoff = 0;
    world.lights[0].is_on = 1;

    /** Point Light **/
    set(world.lights[1].position, Vector4(-2, -2, 2, 1));
    set(world.lights[1].ambient, Vector4(0.5f, 0.5f, 0.5f, 1));
    set(world.lights[1].diffuse, Vector4(1, 1, 1, 1));
    set(world.lights[1].specular, Vector4(0.8f, 0.8f, 0.8f, 1));
    world.lights[1].constantAttenuation = 1;
    world.lights[1].linearAttenuation = 0.7f;
    world.lights[1].quadraticAttenuation = 0.3f;
    world.lights[1].spotExponent = 1;
    world.lights[1].spotCutoff = 0;
    world.lights[1].is_on = 0;

    /** Spotlight **/
    set(world.lights[2].position, Vector4(0, 0, 5, 1));
    set(world.lights[2].ambient, Vector4(0.5f, 0.5f, 0.5f, 1));
    set(world.lights[2].diffuse, Vector4(1, 1, 1, 1));
    set(world.lights[2].specular, Vector4(0.6f, 0.8f, 0.8f, 1));
    set(world.lights[2].spotDirection, Vector4(0, 0, 1, 0));
    world.lights[2].constantAttenuation = 0.4f;
    world.lights[2].linearAttenuation = 0.8f;
    world.lights[2].quadraticAttenuation = 0.3f;
    world.lights[2].spotCutoff = 2;
    world.lights[2].spotExponent = 1;
    world.lights[2].is_on = 0;
}
