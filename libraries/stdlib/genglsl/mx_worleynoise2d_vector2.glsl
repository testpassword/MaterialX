#include "stdlib/genglsl/lib/mx_noise.glsl"

void mx_worleynoise2d_vector2(float jitter, vec2 texcoord, out vec2 result)
{
    result = mx_worley_noise_vec2(texcoord, jitter);
}
