#include "stdlib/genglsl/lib/mx_noise.glsl"

void mx_worleynoise2d_vector3(float jitter, vec2 texcoord, out vec3 result)
{
    result = mx_worley_noise_vec3(texcoord, jitter);
}
