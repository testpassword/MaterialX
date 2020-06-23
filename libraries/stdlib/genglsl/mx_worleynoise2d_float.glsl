#include "stdlib/genglsl/lib/mx_noise.glsl"

void mx_worleynoise2d_float(float jitter, vec2 texcoord, out float result)
{
    result = mx_worley_noise_float(texcoord, jitter);
}
