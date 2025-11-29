#include "../Noise.h"
#include <cmath>
#include <cstdlib>

using namespace Gum::Maths;

namespace Gum {
namespace Noise {

	int randomNumber = 0;

    int setSeed(long seed)
    {
        srand48(seed);
        randomNumber = rand() % 100000;
        
        return randomNumber;
    }

    float noiseInterpolate(const float x) 
    { 
        float x2 = x * x;
        return x2 * x * (x * (x * 6.0 - 15.0) + 10.0); 
    }
    vec2 noiseInterpolate(const vec2 x) 
    { 
        vec2 x2 = x * x;
        return x2 * x * (x * (x * 6.0 - 15.0) + 10.0); 
    }
    vec3 noiseInterpolate(const vec3 x) 
    { 
        vec3 x2 = x * x;
        return x2 * x * (x * (x * 6.0 - 15.0) + 10.0); 
    }
    vec4 noiseInterpolate(const vec4 x) 
    { 
        vec4 x2 = x * x;
        return x2 * x * (x * (x * 6.0 - 15.0) + 10.0); 
    }
    vec2 noiseInterpolateDu(const float x) 
    { 
        float x2 = x * x;
        float u = x2 * x * (x * (x * 6.0 - 15.0) + 10.0); 
        float du = 30.0 * x2 * (x * (x - 2.0) + 1.0);
        return vec2(u, du);
    }
    vec4 noiseInterpolateDu(const vec2 x) 
    { 
        vec2 x2 = x * x;
        vec2 u = x2 * x * (x * (x * 6.0 - 15.0) + 10.0); 
        vec2 du = x2 * 30.0 * (x * (x - 2.0) + 1.0);
        return vec4(u.x, u.y, du.x, du.y);
    }
    void noiseInterpolateDu(const vec3 x, vec3& u, vec3& du) 
    { 
        vec3 x2 = x * x;
        u = x2 * x * (x * (x * 6.0 - 15.0) + 10.0); 
        du = x2 * 30.0 * (x * (x - 2.0) + 1.0);
    }

    // 1D Value noise.
    // @param scale Number of tiles, must be an integer for tileable results, range: [2, inf]
    // @param seed Seed to randomize result, range: [0, inf]
    // @return Value of the noise, range: [-1, 1]
    float noise(float pos, float scale, float seed)
    {
        pos *= scale;
        vec2 i = vec2(0.0, 1.0) + floor(pos);
        float f = pos - i.x;
        i = vec2::mod(i, vec2(scale)) + seed;

        float u = noiseInterpolate(f);
        return mix(hash1D(i.x), hash1D(i.y), u) * 2.0 - 1.0;
    }

    // 2D Value noise.
    // @param scale Number of tiles, must be an integer for tileable results, range: [2, inf]
    // @param seed Seed to randomize result, range: [0, inf]
    // @return Value of the noise, range: [-1, 1]
    float noise(vec2 pos, vec2 scale, float seed) 
    {
        pos *= scale;
        vec2 i0 = vec2::floor(pos);
        vec4 i = vec4(i0, i0.x + 1.0f, i0.y + 1.0f);
        vec2 f = pos - i0;
        i = vec4::mod(i, vec4(scale.x, scale.y, scale.x, scale.y)) + seed;

        vec4 hash = multiHash2D(i);
        float a = hash.x;
        float b = hash.y;
        float c = hash.z;
        float d = hash.w;

        vec2 u = noiseInterpolate(f);

        //std::cout << i.toString() << " " << hash.toString() << " " << u.toString() << std::endl;

        float value = mix(a, b, u.x) + (c - a) * u.y * (1.0 - u.x) + (d - b) * u.x * u.y;
        return value * 2.0 - 1.0;
    }

    // 2D Value noise.
    // @param scale Number of tiles, must be an integer for tileable results, range: [2, inf]
    // @param phase The phase for rotating the hash, range: [0, inf], default: 0.0
    // @param seed Seed to randomize result, range: [0, inf]
    // @return Value of the noise, range: [-1, 1]
    float noisep(vec2 pos, vec2 scale, float phase, float seed) 
    {
        const float kPI2 = 6.2831853071;
        pos *= scale;
        vec2 i0 = vec2::floor(pos);
        vec4 i = vec4(i0, i0.x + 1.0f, i0.y + 1.0f);
        vec2 f = pos - i0;
        i = vec4::mod(i, vec4(scale.x, scale.y, scale.x, scale.y)) + seed;

        vec4 hash = multiHash2D(i);
        hash = vec4::sin(hash * kPI2 + phase) * 0.5 + 0.5;
        float a = hash.x;
        float b = hash.y;
        float c = hash.z;
        float d = hash.w;

        vec2 u = noiseInterpolate(f);
        float value = mix(a, b, u.x) + (c - a) * u.y * (1.0 - u.x) + (d - b) * u.x * u.y;
        return value * 2.0 - 1.0;
    }

    // 2D Value noise with derivatives.
    // @param scale Number of tiles, must be an integer for tileable results, range: [2, inf]
    // @param seed Seed to randomize result, range: [0, inf]
    // @return x = value of the noise, yz = derivative of the noise, range: [-1, 1]
    vec3 noised(vec2 pos, vec2 scale, float seed) 
    {
        // value noise with derivatives based on Inigo Quilez
        pos *= scale;
        vec2 i0 = vec2::floor(pos);
        vec4 i = vec4(i0, i0.x + 1.0f, i0.y + 1.0f);
        vec2 f = pos - i0;
        i = vec4::mod(i, vec4(scale.x, scale.y, scale.x, scale.y)) + seed;

        vec4 hash = multiHash2D(i);
        float a = hash.x;
        float b = hash.y;
        float c = hash.z;
        float d = hash.w;
        
        vec4 udu = noiseInterpolateDu(f);    
        float abcd = a - b - c + d;
        float value = a + (b - a) * udu.x + (c - a) * udu.y + abcd * udu.x * udu.y;
        vec2 derivative = vec2(udu.z, udu.w) * (vec2(udu.y, udu.x) * abcd + vec2(b, c) - a);
        return vec3(value * 2.0 - 1.0, derivative.x, derivative.y);
    }

    // 2D Value noise with derivatives.
    // @param scale Number of tiles, must be an integer for tileable results, range: [2, inf]
    // @param phase The phase for rotating the hash, range: [0, inf], default: 0.0
    // @param seed Seed to randomize result, range: [0, inf]
    // @return x = value of the noise, yz = derivative of the noise, range: [-1, 1]
    vec3 noisepd(vec2 pos, vec2 scale, float phase, float seed) 
    {
        const float kPI2 = 6.2831853071;
        // value noise with derivatives based on Inigo Quilez
        pos *= scale;
        vec2 i0 = vec2::floor(pos);
        vec4 i = vec4(i0, i0.x + 1.0f, i0.y + 1.0f);
        vec2 f = pos - i0;
        i = vec4::mod(i, vec4(scale.x, scale.y, scale.x, scale.y)) + seed;

        vec4 hash = multiHash2D(i);
        hash = vec4::sin(hash * kPI2 + phase) * 0.5 + 0.5;
        float a = hash.x;
        float b = hash.y;
        float c = hash.z;
        float d = hash.w;
        
        vec4 udu = noiseInterpolateDu(f);    
        float abcd = a - b - c + d;
        float value = a + (b - a) * udu.x + (c - a) * udu.y + abcd * udu.x * udu.y;
        vec2 derivative = vec2(udu.z, udu.w) * (vec2(udu.y, udu.x) * abcd + vec2(b, c) - a);
        return vec3(value * 2.0 - 1.0, derivative.x, derivative.y);
    }

    // 3D Value noise with height that is tileable on the XY axis.
    // @param scale Number of tiles, must be an integer for tileable results, range: [2, inf]
    // @param time The height phase for the noise value, range: [0, inf], default: 0.0
    // @param seed Seed to randomize result, range: [0, inf], default: 0.0
    // @return Value of the noise, range: [-1, 1]
    float noise3d(vec2 pos, vec2 scale, float height, float seed)
    {
        // classic value noise with 3D
        pos *= scale;
        vec3 i = vec3::floor(vec3(pos, height));
        vec3 ip1 = i + vec3(1.0);
        vec3 f = vec3(pos, height) - i;
        
        vec4 mi = vec4::mod(vec4(i.x, i.y, ip1.x, ip1.y), vec4(scale.x, scale.y, scale.x, scale.y));
        i.x = mi.x;
        i.y = mi.y;
        ip1.x = mi.z;
        ip1.y = mi.w;

        vec4 hashLow, hashHigh;
        multiHash3D(i + seed, ip1 + seed, hashLow, hashHigh);
        
        vec3 u = noiseInterpolate(f);
        vec4 r = vec4::mix(hashLow, hashHigh, u.z);
        r.x = mix(r.x, r.z, u.y);
        r.y = mix(r.y, r.w, u.y);
        r.z = mix(r.x, r.y, u.x);
        r.w = mix(r.z, r.w, u.x);
        return (r.x + (r.y - r.x) * u.x) * 2.0 - 1.0;
    }

    // 3D Value noise with height and derivatives that is tileable on the XY axis.
    // @param scale Number of tiles, must be an integer for tileable results, range: [2, inf]
    // @param time The height phase for the noise value, range: [0, inf], default: 0.0
    // @param seed Seed to randomize result, range: [0, inf]
    // @return x = value of the noise, yz = derivative of the noise, w = derivative of the time, range: [-1, 1]
    vec4 noised3d(vec2 pos, vec2 scale, float time, float seed) 
    {
        // based on Analytical Noise Derivatives by Brian Sharpe
        // classic value noise with 3D
        pos *= scale;
        vec3 i = vec3::floor(vec3(pos, time));
        vec3 ip1 = i + vec3(1.0);
        vec3 f = vec3(pos, time) - i;
        
        vec4 mi = vec4::mod(vec4(i.x, i.y, ip1.x, ip1.y), vec4(scale.x, scale.y, scale.x, scale.y));
        i.x = mi.x;
        i.y = mi.y;
        ip1.x = mi.z;
        ip1.y = mi.w;

        vec4 hashLow, hashHigh;
        multiHash3D(i + seed, ip1 + seed, hashLow, hashHigh);

        vec3 u, du;
        noiseInterpolateDu(f, u, du);
        vec4 res0 = vec4::mix(hashLow, hashHigh, u.z);
        vec4 res1;
        res1.x = mix(res0.x, res0.z, u.y);
        res1.y = mix(res0.y, res0.w, u.y);
        res1.z = mix(res0.x, res0.y, u.x);
        res1.w = mix(res0.z, res0.w, u.x);
        vec4 res2 = vec4::mix(vec4(hashLow.x, hashLow.y, hashHigh.x, hashHigh.y), vec4(hashLow.z, hashLow.w, hashHigh.z, hashHigh.w), u.y);
        vec2 res3 = vec2::mix(vec2(res2.x, res2.z), vec2(res2.y, res2.w), u.x);
        vec4 results = vec4(res1.x, 0.0, 0.0, 0.0) + (vec4(res1.y, res1.y, res1.w, res3.y) - vec4(res1.x, res1.x, res1.z, res3.x)) * vec4(u.x, du.x, du.y, du.z);

        return vec4(results.x * 2.0 - 1.0, results.y, results.z, results.w);
    }

    float distanceMetric(vec2 pos, uint metric)
    {
        switch (metric)
        {
            case 0u:
                // squared euclidean
                return vec2::dot(pos, pos);
            case 1u:
                // manhattam   
                return vec2::dot(vec2::abs(pos), vec2(1.0));
            case 2u:
                // chebyshev
                return max<float>({std::abs(pos.x), std::abs(pos.y)});
            default:
                // triangular
                return max<float>({std::abs(pos.x) * 0.866025f + pos.y * 0.5f, -pos.y});
        }
    }

    vec4 distanceMetric(vec4 px, vec4 py, uint metric)
    {
        switch (metric)
        {
            case 0u:
                // squared euclidean
                return px * px + py * py;
            case 1u:
                // manhattam   
                return vec4::abs(px) + vec4::abs(py);
            case 2u:
                // chebyshev
                return vec4::max(vec4::abs(px), vec4::abs(py));
            default:
                // triangular
                return vec4::max(vec4::abs(px) * 0.866025f + py * 0.5f, py * -1.0f);
        }
    }

    // 2D Value noise that returns two values.
    // @param scale Number of tiles, must be an integer for tileable results, range: [2, inf]
    // @param phase The phase for rotating the hash, range: [0, inf], default: 0.0
    // @param seed Seed to randomize result, range: [0, inf]
    // @return Value of the noise, range: [-1, 1]
    vec2 multiNoise(vec4 pos, vec4 scale, float phase, vec2 seed) 
    {
        const float kPI2 = 6.2831853071;
        pos *= scale;
        vec4 i = vec4::floor(pos);
        vec4 f = pos - i;
        vec4 i0 = vec4::mod(vec4(i.x, i.y, i.x + 1.0f, i.y + 1.0f), vec4(scale.x, scale.y, scale.x, scale.y)) + seed.x;
        vec4 i1 = vec4::mod(vec4(i.z, i.w, i.z + 1.0f, i.w + 1.0f), vec4(scale.x, scale.y, scale.x, scale.y)) + seed.y;

        vec4 hash0 = multiHash2D(i0);
        vec4 hash1 = multiHash2D(i1);
        hash0 = vec4::sin(hash0 * kPI2 + phase) * 0.5f + 0.5;
        hash1 = vec4::sin(hash1 * kPI2 + phase) * 0.5f + 0.5;
        vec2 a = vec2(hash0.x, hash1.x);
        vec2 b = vec2(hash0.y, hash1.y);
        vec2 c = vec2(hash0.z, hash1.z);
        vec2 d = vec2(hash0.w, hash1.w);

        vec4 u = noiseInterpolate(f);
        vec2 value = vec2::mix(a, b, vec2(u.x, u.z)) + (c - a) * vec2(u.y, u.w) * (vec2(1.0) - vec2(u.x, u.z)) + (d - b) * vec2(u.x, u.z) * vec2(u.y, u.w);
        return value * 2.0 - 1.0;
    }

    // 2D Variant of Value noise that produces ridge-like noise by using multiple noise values.
    // @param scale Number of tiles, must be integer for tileable results, range: [2, inf]
    // @param translate Translate factors for the value noise , range: [-inf, inf], default: {0.5, -0.25, 0.15}
    // @param intensity The contrast for the noise, range: [0, 1], default: 0.75
    // @param time The height phase for the noise value, range: [0, inf], default: 0.0
    // @param seed Seed to randomize result, range: [0, inf]
    // @return Value of the noise, range: [0, 1]
    float gridNoiset(vec2 pos, vec2 scale, vec3 translate, float intensity, float time, float seed)
    {
        vec4 n;
        vec2 n0 = multiNoise(vec4(pos.x, pos.y, pos.x + translate.x, pos.y + translate.x), vec4(scale.x, scale.y, scale.x, scale.y), time, seed);
        vec2 n1 = multiNoise(vec4(pos.x + translate.y, pos.y + translate.y, pos.x + translate.z, pos.y + translate.z), vec4(scale.x, scale.y, scale.x, scale.y), time, seed);

        n.x = n0.x;
        n.y = n0.y;
        n.z = n1.x;
        n.w = n1.y;

        n.x = n.x * n.z;
        n.y = n.y * n.w;
        
        float t = std::abs(n.x * n.y);
        return pow(t, mix(0.5, 0.1, intensity));
    }

    // 2D Variant of Value noise that produces ridge-like noise by using multiple noise values.
    // @param scale Number of tiles, must be integer for tileable results, range: [2, inf]
    // @param intensity The contrast for the noise, range: [0, 1], default: 0.75
    // @param time The height phase for the noise value, range: [0, inf], default: 0.0
    // @param seed Seed to randomize result, range: [0, inf]
    // @return Value of the noise, range: [0, 1]
    float gridNoise(vec2 pos, vec2 scale, float intensity, float time, float seed)
    {
        vec3 trans0 = (hash3D(vec2(seed)) * 2.0 - 1.0) * vec3(scale.x, scale.y, scale.x);
        vec4 translate = vec4(0.0, trans0.x, trans0.y, trans0.z);
        
        vec4 n;
        vec2 n0 = multiNoise(vec4(pos.x + translate.x, pos.y + translate.x, pos.x + translate.y, pos.y + translate.y), vec4(scale.x, scale.y, scale.x, scale.y), time, seed);
        vec2 n1 = multiNoise(vec4(pos.x + translate.z, pos.y + translate.z, pos.x + translate.w, pos.y + translate.w), vec4(scale.x, scale.y, scale.x, scale.y), time, seed);

        n.x = n0.x;
        n.y = n0.y;
        n.z = n1.x;
        n.w = n1.y;

        n.x = n.x * n.z;
        n.y = n.y * n.w;
        
        float t = std::abs(n.x * n.y);
        return pow(t, mix(0.5f, 0.1f, intensity));
    }

    // 2D Variant of Value noise that produces dots with random size or luminance.
    // @param scale Number of tiles, must be integer for tileable results, range: [2, inf]
    // @param density The density of the dots distribution, range: [0, 1], default: 1.0
    // @param size The radius of the dots, range: [0, 1], default: 0.5
    // @param sizeVariation The variation for the size of the dots, range: [0, 1], default: 0.75
    // @param roundness The roundness of the dots, if zero will result in square, range: [0, 1], default: 1.0
    // @param seed Seed to randomize result, range: [0, inf]
    // @return x = value of the noise, y = random luminance value, z = size of the dot, range: [0, 1]
    vec3 dotsNoise(vec2 pos, vec2 scale, float density, float size, float sizeVariation, float roundness, float seed) 
    {
        pos *= scale;
        vec2 posf = vec2::floor(pos);
        vec4 i = vec4(posf.x, posf.y, posf.x + 1.0f, posf.y + 1.0f);
        vec2 f = pos - vec2(i.x, i.y);
        i = vec4::mod(i, vec4(scale.x, scale.y, scale.x, scale.y));
        
        vec4 hash = hash4D(vec2(i.x, i.y) + seed);
        if (hash.w > density)
            return vec3(0.0);

        float radius = clamp(size + (hash.z * 2.0 - 1.0) * sizeVariation * 0.5, 0.0, 1.0);
        float value = radius / size;  
        radius = 2.0 / radius;
        f = f * radius - (radius - 1.0);
        f += vec2(hash.x, hash.y) * (radius - 2.0);
        f = vec2::pow(vec2::abs(f), mix(20.0, 1.0, sqrt(roundness)));

        float u = 1.0 - min(vec2::dot(f, f), 1.0f);
        return vec3(clamp(u * u * u * value, 0.0f, 1.0f), hash.w, hash.z);
    }

    // 2D Variant of Value noise that produces lines of random color and configurable width.
    // @param scale Number of tiles, must be integer for tileable results, range: [2, inf]
    // @param count The density of the lines, range: [1, inf], default: 4.0
    // @param jitter Jitter factor for the lines, if zero then it will result straight lines, range: [0, 1], default: 1.0
    // @param smoothness The radius of the dots, range: [0, 1], default: 0.5
    // @param seed Seed to randomize result, range: [0, inf]
    // @return x = value of the noise, range: [0, 1], y = id of lines, range: [0, count] 
    vec2 randomLines(vec2 pos, vec2 scale, float count, float width, float jitter, vec2 smoothness, float phase, float seed)
    {
        float strength = jitter * 1.25;

        // compute gradient
        // TODO: compute the gradient analytically
        vec2 grad;
        vec3 offsets = vec3(1.0, 0.0, -1.0) / 1024.0;
        vec4 p = vec4(pos.x, pos.y, pos.x, pos.y) + vec4(offsets.x, offsets.y, offsets.z, offsets.y);
        vec2 nv = (multiNoise(p, vec4(scale.x, scale.y, scale.x, scale.y), phase, vec2(seed)) * strength + vec2(p.y, p.w)) * count;
        grad.x = nv.x - nv.y;
        p = vec4(pos.x, pos.y, pos.x, pos.y) + vec4(offsets.y, offsets.x, offsets.y, offsets.z);
        nv = (multiNoise(p, vec4(scale.x, scale.y, scale.x, scale.y), phase, vec2(seed)) * strength + vec2(p.y, p.w)) * count;
        grad.y = nv.x - nv.y;
        
        float v =  count * (noisep(pos, scale, phase, seed) * strength + pos.y);
        float w = fract(v) / (grad / (2.0 * offsets.x)).length();
        width *= 0.1;
        smoothness *= width;
        smoothness += max<float>({std::abs(grad.x), std::abs(grad.y)}) * 0.02f;
        
        float d = smoothstep(0.0, smoothness.x, w) - smoothstep(max<float>({width - smoothness.y, 0.0f}), width, w);
        return vec2(d, std::fmod(floor(v), (double)count));
    }

    // 2D Variant of Value noise that produces lines of random color and configurable width.
    // @param scale Number of tiles, must be integer for tileable results, range: [2, inf]
    // @param count The density of the lines, range: [1, inf], default: 4.0
    // @param jitter Jitter factor for the lines, if zero then it will result straight lines, range: [0, 1], default: 1.0
    // @param smoothness The radius of the dots, range: [0, 1], default: 0.5
    // @param colorVariation The variation for the color of the lines, range: [0, 1], default: 1.0
    // @param seed Seed to randomize result, range: [0, inf]
    // @return Color of the lines, black if background, range: [0, 1]
    vec4 randomLinesv(vec2 pos, vec2 scale, float count, float width, float jitter, vec2 smoothness, float phase, float colorVariation, float seed)
    {
        vec2 l = randomLines(pos, scale, count, width, jitter, smoothness, phase, seed);
        vec3 r = hash3D(vec2(l.y, l.y) + seed);
        return vec4((r.x <= colorVariation ? r : vec3(r.x, r.x, r.x)) * l.x, l.x);
    }
}}