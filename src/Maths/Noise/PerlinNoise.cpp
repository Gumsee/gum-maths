#include "../Noise.h"

using namespace Gum::Maths;

namespace Gum {
namespace Noise {
    // 2D Perlin noise.
    // @param scale Number of tiles, must be  integer for tileable results, range: [2, inf]
    // @param seed Seed to randomize result, range: [0, inf], default: 0.0
    // @return Value of the noise, range: [-1, 1]
    float perlinNoise(vec2 pos, vec2 scale, float seed)
    {
        // based on Modifications to Classic Perlin Noise by Brian Sharpe: https://archive.is/cJtlS
        pos *= scale;
        vec2 i0 = vec2::floor(pos);
        vec4 i = vec4(i0, i0.x + 1.0f, i0.y + 1.0f);
        vec4 f = (vec4(pos.x, pos.y, pos.x, pos.y) - vec4(i.x, i.y, i.x, i.y)) - vec4(0.0f, 0.0f, 1.0f, 1.0f);
        i = vec4::mod(i, vec4(scale.x, scale.y, scale.x, scale.y)) + seed;

        // grid gradients
        vec4 gradientX, gradientY;
        multiHash2D(i, gradientX, gradientY);
        gradientX -= 0.49999f;
        gradientY -= 0.49999f;

        // perlin surflet
        vec4 gradients = vec4::inversesqrt(gradientX * gradientX + gradientY * gradientY) * (gradientX * vec4(f.x, f.z, f.x, f.z) + gradientY * vec4(f.y, f.y, f.w, f.w));
        // normalize: 1.0 / 0.75^3
        gradients *= 2.3703703703703703703703703703704;
        vec4 lengthSq = f * f;
        lengthSq = vec4(lengthSq.x, lengthSq.z, lengthSq.x, lengthSq.z) + vec4(lengthSq.y, lengthSq.y, lengthSq.w, lengthSq.w);
        vec4 xSq = vec4(1.0) - vec4::min(vec4(1.0), lengthSq); 
        xSq = xSq * xSq * xSq;
        return vec4::dot(xSq, gradients);
    }

    // 2D Perlin noise with derivatives.
    // @param scale Number of tiles, must be  integer for tileable results, range: [2, inf]
    // @param seed Seed to randomize result, range: [0, inf], default: 0.0
    // @return x = value of the noise, yz = derivative of the noise, range: [-1, 1]
    vec3 perlinNoised(vec2 pos, vec2 scale, float seed)
    {
        // based on Modifications to Classic Perlin Noise by Brian Sharpe: https://archive.is/cJtlS
        pos *= scale;
        vec2 i0 = vec2::floor(pos);
        vec4 i = vec4(i0, i0.x + 1.0f, i0.y + 1.0f);
        vec4 f = (vec4(pos.x, pos.y, pos.x, pos.y) - vec4(i.x, i.y, i.x, i.y)) - vec4(0.0f, 0.0f, 1.0f, 1.0f);
        i = vec4::mod(i, vec4(scale.x, scale.y, scale.x, scale.y)) + seed;

        // grid gradients
        vec4 gradientX, gradientY;
        multiHash2D(i, gradientX, gradientY);
        gradientX -= 0.49999;
        gradientY -= 0.49999;

        // perlin surflet
        vec4 gradients = vec4::inversesqrt(gradientX * gradientX + gradientY * gradientY) * (gradientX * vec4(f.x, f.z, f.x, f.z) + gradientY * vec4(f.y, f.y, f.w, f.w));
        vec4 m = f * f;
        m = vec4(m.x, m.z, m.x, m.z) + vec4(m.y, m.y, m.w, m.w);
        m = vec4::max(vec4(1.0f) - m, vec4(0.0));
        vec4 m2 = m * m;
        vec4 m3 = m * m2;
        // compute the derivatives
        vec4 m2Gradients = m2 * -6.0 * gradients;
        vec2 grad = vec2(vec4::dot(m2Gradients, vec4(f.x, f.z, f.x, f.z)), vec4::dot(m2Gradients, vec4(f.y, f.y, f.w, f.w))) + vec2(vec4::dot(m3, gradientX), vec4::dot(m3, gradientY));
        // sum the surflets and normalize: 1.0 / 0.75^3
        return vec3(vec4::dot(m3, gradients), grad.x, grad.y) * 2.3703703703703703703703703703704;
    }

    // 2D Variant of Perlin noise that produces and organic-like noise.
    // @param scale Number of tiles, must be  integer for tileable results, range: [2, inf]
    // @param density The density of the lower frequency details, range: [0, 1], default: 1.0
    // @param phase The phase of the noise, range: [-inf, inf], default: {0, 0}
    // @param contrast Controls the contrast of the result, range: [0, 1], default: 0.0
    // @param highlights Controls the highlights of the , range: [0, 1], default: 0.25
    // @param shift Shifts the angle of the highlights, range: [0, 1], default: 0.5
    // @param seed Seed to randomize result, range: [0, inf], default: 0.0
    // @return Value of the noise, range: [0, 1]
    float organicNoise(vec2 pos, vec2 scale, float density, vec2 phase, float contrast, float highlights, float shift, float seed)
    {
        vec2 s = vec2::mix(vec2(1.0), scale - 1.0, density);
        float nx = perlinNoise(pos + phase, scale, seed);
        float ny = perlinNoise(pos, s, seed);

        float n = (vec2(nx, ny) * vec2::mix(vec2(2.0, 0.0), vec2(0.0, 2.0), shift)).length();
        n = pow(n, 1.0 + 8.0 * contrast) + (0.15 * highlights) / n;
        return n * 0.5;
    }
}};
