#include "stdlib/genglsl/lib/mx_noise.glsl"

void mx_worleynoise3d_vector2(float jitter, vec3 position, out vec2 result)
{
    result = mx_worley_noise_vec2(position, jitter);
}
