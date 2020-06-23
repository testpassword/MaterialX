#include "stdlib/genglsl/lib/mx_noise.glsl"

void mx_worleynoise3d_vector3(float jitter, vec3 position, out vec3 result)
{
    result = mx_worley_noise_vec3(position, jitter);
}
