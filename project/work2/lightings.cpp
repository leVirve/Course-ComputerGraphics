#include "graphics.h"


LightSource lights[3];

void set(float (&p)[4], Vector4 v)
{
    for (int i = 0; i < 4; ++i) p[i] = v[i];
}

void setLightingSource()
{
    set(lights[0].position, Vector4(2, 2, 2, 0));
    set(lights[0].ambient, Vector4(0.1, 0.1, 0.1, 1));
    set(lights[0].diffuse, Vector4(0.3, 0.3, 0.3, 1));

    set(lights[0].specular, Vector4(0.9, 0.9, 0.9, 1)); //  for Test

    set(lights[1].position, Vector4(0, 0, 5, 0));
    set(lights[1].ambient, Vector4(0.1, 0.1, 0.1, 1));
    set(lights[1].diffuse, Vector4(0.6, 0.6, 0.6, 1));
    set(lights[1].specular, Vector4(0.6, 0.6, 0.6, 1));
}
