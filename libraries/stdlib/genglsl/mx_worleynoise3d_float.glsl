#include "stdlib/genglsl/lib/mx_noise.glsl"

void mx_worleynoise3d_float(float jitter, vec3 position, out float result)
{
    result = mx_worley_noise_float(position, jitter);
}
