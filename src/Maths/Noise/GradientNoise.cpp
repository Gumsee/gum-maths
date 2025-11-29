#include "../Noise.h"

using namespace Gum::Maths;

namespace Gum {
namespace Noise {
    // 2D Gradient noise.
    // @param scale Number of tiles, must be  integer for tileable results, range: [2, inf]
    // @param seed Seed to randomize result, range: [0, inf], default: 0.0
    // @return Value of the noise, range: [-1, 1]
    float gradientNoise(vec2 pos, vec2 scale, float seed) 
    {
        pos *= scale;
        vec2 i0 = vec2::floor(pos);
        vec4 i = vec4(i0.x, i0.y, i0.x + 1.0f, i0.y + 1.0f);
        vec4 f = (vec4(pos.x, pos.y, pos.x, pos.y) - vec4(i.x, i.y, i.x, i.y)) - vec4(0.0, 0.0, 1.0, 1.0);
        i = vec4::mod(i, vec4(scale.x, scale.y, scale.x, scale.y)) + seed;

        vec4 hashX, hashY;
        smultiHash2D(i, hashX, hashY);

        vec4 gradients = hashX * vec4(f.x, f.z, f.x, f.z) + hashY * vec4(f.y, f.y, f.w, f.w);
        vec2 u = noiseInterpolate(vec2(f.x, f.y));
        vec2 g = vec2::mix(vec2(gradients.x, gradients.z), vec2(gradients.y, gradients.w), u.x);
        return 1.4142135623730950 * mix(g.x, g.y, u.y);
    }

    // 2D Gradient noise with gradients transform (i.e. can be used to rotate the gradients).
    // @param scale Number of tiles, must be  integer for tileable results, range: [2, inf]
    // @param transform Transform matrix for the noise gradients.
    // @param seed Seed to randomize result, range: [0, inf], default: 0.0
    // @return Value of the noise, range: [-1, 1]
    float gradientNoiset(vec2 pos, vec2 scale, mat2 transform, float seed) 
    {
        pos *= scale;
        vec2 i0 = vec2::floor(pos);
        vec4 i = vec4(i0.x, i0.y, i0.x + 1.0f, i0.y + 1.0f);
        vec4 f = (vec4(pos.x, pos.y, pos.x, pos.y) - vec4(i.x, i.y, i.x, i.y)) - vec4(0.0, 0.0, 1.0, 1.0);
        i = vec4::mod(i, vec4(scale.x, scale.y, scale.x, scale.y)) + seed;
        
        vec4 hashX, hashY;
        smultiHash2D(i, hashX, hashY);

        // transform gradients
        vec4 m = vec4(
            transform[0][0], transform[0][1], 
            transform[1][0], transform[1][1]
        );
        vec4 rh = vec4(hashX.x, hashY.x, hashX.y, hashY.y);
        rh = vec4(rh.x, rh.x, rh.z, rh.z) * vec4(m.x, m.y, m.x, m.y) + vec4(rh.y, rh.y, rh.w, rh.w) * vec4(m.z, m.w, m.z, m.w);
        hashX.x = rh.x;
        hashX.y = rh.z;
        hashY.x = rh.y;
        hashY.y = rh.w;

        rh = vec4(hashX.z, hashY.z, hashX.w, hashY.w);
        rh = vec4(rh.x, rh.x, rh.z, rh.z) * vec4(m.x, m.y, m.x, m.y) + vec4(rh.y, rh.y, rh.w, rh.w) * vec4(m.z, m.w, m.z, m.w);
        hashX.z = rh.x;
        hashX.w = rh.z;
        hashY.z = rh.y;
        hashY.w = rh.w;
        
        vec4 gradients = hashX * vec4(f.x, f.z, f.x, f.z) + hashY * vec4(f.y, f.y, f.w, f.w);
        vec2 u = noiseInterpolate(vec2(f.x, f.y));
        vec2 g = vec2::mix(vec2(gradients.x, gradients.z), vec2(gradients.y, gradients.w), u.x);
        return 1.4142135623730950 * mix(g.x, g.y, u.y);
    }

    float gradientNoiser(vec2 pos, vec2 scale, float rotation, float seed) 
    {
        vec2 sinCos = vec2(sin(rotation), cos(rotation));
        return gradientNoiset(pos, scale, mat2(sinCos.y, sinCos.x, sinCos.x, sinCos.y), seed);
    }

    // 2D Gradient noise with derivatives.
    // @param scale Number of tiles, must be  integer for tileable results, range: [2, inf]
    // @param seed Seed to randomize result, range: [0, inf], default: 0.0
    // @return x = value of the noise, yz = derivative of the noise, range: [-1, 1]
    vec3 gradientNoised(vec2 pos, vec2 scale, float seed) 
    {
        // gradient noise with derivatives based on Inigo Quilez
        pos *= scale;
        vec2 i0 = vec2::floor(pos);
        vec4 i = vec4(i0.x, i0.y, i0.x + 1.0f, i0.y + 1.0f);
        vec4 f = (vec4(pos.x, pos.y, pos.x, pos.y) - vec4(i.x, i.y, i.x, i.y)) - vec4(0.0, 0.0, 1.0, 1.0);
        i = vec4::mod(i, vec4(scale.x, scale.y, scale.x, scale.y)) + seed;
        
        vec4 hashX, hashY;
        smultiHash2D(i, hashX, hashY);
        vec2 a = vec2(hashX.x, hashY.x);
        vec2 b = vec2(hashX.y, hashY.y);
        vec2 c = vec2(hashX.z, hashY.z);
        vec2 d = vec2(hashX.w, hashY.w);
        
        vec4 gradients = hashX * vec4(f.x, f.z, f.x, f.z) + hashY * vec4(f.y, f.y, f.w, f.w);

        vec4 udu = noiseInterpolateDu(vec2(f.x, f.y));
        vec2 u = vec2(udu.x, udu.y);
        vec2 g = vec2::mix(vec2(gradients.x, gradients.z), vec2(gradients.y, gradients.w), u.x);
        
        vec2 dxdy = a + (b - a) * u.x + (c - a) * u.y + (a - b - c + d) * u.x * u.y;
        dxdy += vec2(udu.z, udu.w) * (vec2(u.y, u.x) * (gradients.x - gradients.y - gradients.z + gradients.w) + vec2(gradients.y, gradients.z) - gradients.x);
        return vec3(mix(g.x, g.y, u.y) * 1.4142135623730950, dxdy.x, dxdy.y);
    }

    // 2D Gradient noise with gradients transform (i.e. can be used to rotate the gradients) and derivatives.
    // @param scale Number of tiles, must be  integer for tileable results, range: [2, inf]
    // @param transform Transform matrix for the noise gradients.
    // @param seed Seed to randomize result, range: [0, inf], default: 0.0
    // @return x = value of the noise, yz = derivative of the noise, range: [-1, 1]
    vec3 gradientNoisedt(vec2 pos, vec2 scale, mat2 transform, float seed) 
    {
        // gradient noise with derivatives based on Inigo Quilez
        pos *= scale;
        vec2 i0 = vec2::floor(pos);
        vec4 i = vec4(i0.x, i0.y, i0.x + 1.0f, i0.y + 1.0f);
        vec4 f = (vec4(pos.x, pos.y, pos.x, pos.y) - vec4(i.x, i.y, i.x, i.y)) - vec4(0.0, 0.0, 1.0, 1.0);
        i = vec4::mod(i, vec4(scale.x, scale.y, scale.x, scale.y)) + seed;
        
        vec4 hashX, hashY;
        smultiHash2D(i, hashX, hashY);

        // transform gradients
        vec4 m = vec4(
            transform[0][0], transform[0][1], 
            transform[1][0], transform[1][1]
        );
        vec4 rh = vec4(hashX.x, hashY.x, hashX.y, hashY.y);
        rh = vec4(rh.x, rh.x, rh.z, rh.z) * vec4(m.x, m.y, m.x, m.y) + vec4(rh.y, rh.y, rh.w, rh.w) * vec4(m.z, m.w, m.z, m.w);
        hashX.x = rh.x;
        hashX.y = rh.z;
        hashY.x = rh.y;
        hashY.y = rh.w;

        rh = vec4(hashX.z, hashY.z, hashX.w, hashY.w);
        rh = vec4(rh.x, rh.x, rh.z, rh.z) * vec4(m.x, m.y, m.x, m.y) + vec4(rh.y, rh.y, rh.w, rh.w) * vec4(m.z, m.w, m.z, m.w);
        hashX.z = rh.x;
        hashX.w = rh.z;
        hashY.z = rh.y;
        hashY.w = rh.w;
        
        vec2 a = vec2(hashX.x, hashY.x);
        vec2 b = vec2(hashX.y, hashY.y);
        vec2 c = vec2(hashX.z, hashY.z);
        vec2 d = vec2(hashX.w, hashY.w);
        
        vec4 gradients = hashX * vec4(f.x, f.z, f.x, f.z) + hashY * vec4(f.y, f.y, f.w, f.w);

        vec4 udu = noiseInterpolateDu(vec2(f.x, f.y));
        vec2 u = vec2(udu.x, udu.y);
        vec2 g = vec2::mix(vec2(gradients.x, gradients.z), vec2(gradients.y, gradients.w), u.x);
        
        vec2 dxdy = a + (b - a) * u.x + (c - a) * u.y + (a - b - c + d) * u.x * u.y;
        dxdy += vec2(udu.z, udu.w) * (vec2(u.y, u.x) * (gradients.x - gradients.y - gradients.z + gradients.w) + vec2(gradients.y, gradients.z) - gradients.x);
        return vec3(mix(g.x, g.y, u.y) * 1.4142135623730950, dxdy.x, dxdy.y);
    }

    // 2D Gradient noise with gradients rotation and derivatives.
    // @param scale Number of tiles, must be  integer for tileable results, range: [2, inf]
    // @param rotation Rotation for the noise gradients, useful to animate flow, range: [0, PI]
    // @param seed Seed to randomize result, range: [0, inf], default: 0.0
    // @return x = value of the noise, yz = derivative of the noise, range: [-1, 1]
    vec3 gradientNoisedr(vec2 pos, vec2 scale, float rotation, float seed) 
    {
        vec2 sinCos = vec2(sin(rotation), cos(rotation));
        return gradientNoisedt(pos, scale, mat2(sinCos.y, sinCos.x, sinCos.x, sinCos.y), seed);
    }

    // Variant of 2D Gradient noise with disorder/jitter for the gradients.
    // @param scale Number of tiles, must be  integer for tileable results, range: [2, inf]
    // @param disoder Jitter factor for the noise gradients,, range: [0, 1.0]
    // @param seed Seed to randomize result, range: [0, inf], default: 0.0
    // @return x = value of the noise, yz = derivative of the noise, range: [-1, 1]
    float gradientNoiseDisorder(vec2 pos, vec2 scale, float disoder, float seed) 
    {
        pos *= scale;
        vec2 i0 = vec2::floor(pos);
        vec4 i = vec4(i0.x, i0.y, i0.x + 1.0f, i0.y + 1.0f);
        vec4 f = (vec4(pos.x, pos.y, pos.x, pos.y) - vec4(i.x, i.y, i.x, i.y)) - vec4(0.0, 0.0, 1.0, 1.0);
        i = vec4::mod(i, vec4(scale.x, scale.y, scale.x, scale.y)) + seed;

        vec4 hashX, hashY;
        multiHash2D(i, hashX, hashY);
        hashX = (hashX * disoder) * 2.0 - 1.0;
        hashY = (hashY * disoder) * 2.0 - 1.0;

        vec4 gradients = hashX * vec4(f.x, f.z, f.x, f.z) + hashY * vec4(f.y, f.y, f.w, f.w);
        vec2 u = noiseInterpolate(vec2(f.x, f.y));
        vec2 g = vec2::mix(vec2(gradients.x, gradients.z), vec2(gradients.y, gradients.w), u.x);
        return 1.4142135623730950 * mix(g.x, g.y, u.y);
    }
}};
