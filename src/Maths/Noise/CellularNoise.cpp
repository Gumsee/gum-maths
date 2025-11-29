#include "../Noise.h"


using namespace Gum::Maths;

namespace Gum {
namespace Noise {
    // Classic 3x3 Cellular noise with F1 and F2 distances.
    // @param scale Number of tiles, must be  integer for tileable results, range: [2, inf]
    // @param jitter Jitter factor for the cells, if zero then it will result in a square grid, range: [0, 1], default: 1.0
    // @param seed Seed to randomize result, range: [0, inf], default: 0.0
    // @return Returns the cellular distances, x = F1, y = F2, range: [0, 1].
    vec2 cellularNoise(vec2 pos, vec2 scale, float jitter, float seed) 
    {       
        pos *= scale;
        vec2 i = vec2::floor(pos);
        vec2 f = pos - i;
        
        const vec3 offset = vec3(-1.0, 0.0, 1.0);
        vec4 cells = vec4::mod(vec4(i.x, i.y, i.x, i.y) + vec4(offset.x, offset.x, offset.z, offset.z), vec4(scale.x, scale.y, scale.x, scale.y)) + seed;
        i = vec2::mod(i, scale) + seed;
        vec4 dx0, dy0, dx1, dy1;
        multiHash2D(vec4(cells.x, cells.y, i.x, cells.y), vec4(cells.z, cells.y, cells.x, i.y), dx0, dy0);
        multiHash2D(vec4(cells.z, cells.w, cells.z, i.y), vec4(cells.x, cells.w, i.x, cells.w), dx1, dy1);
        
        dx0 = vec4(offset.x, offset.y, offset.z, offset.x) + dx0 * jitter - vec4(f.x, f.x, f.x, f.x); // -1 0 1 -1
        dy0 = vec4(offset.x, offset.x, offset.x, offset.y) + dy0 * jitter - vec4(f.y, f.y, f.y, f.y); // -1 -1 -1 0
        dx1 = vec4(offset.z, offset.z, offset.x, offset.y) + dx1 * jitter - vec4(f.x, f.x, f.x, f.x); // 1 1 -1 0
        dy1 = vec4(offset.z, offset.y, offset.z, offset.z) + dy1 * jitter - vec4(f.y, f.y, f.y, f.y); // 1 0 1 1
        vec4 d0 = dx0 * dx0 + dy0 * dy0; 
        vec4 d1 = dx1 * dx1 + dy1 * dy1; 
        
        vec2 centerPos = multiHash2D(i) * jitter - f; // 0 0
        
        vec4 F0 = vec4::max(d0, d1);
        vec4 F = vec4::min(d0, d1);
        // shuffle into F the 4 lowest values
        F = vec4::min(F, vec4(F0.w, F0.z, F0.y, F0.x));
        // shuffle into F the 2 lowest values 
        F.x = min(min(F.x, F.z), max<float>({F.y, F.w}));
        F.y = min(min(F.y, F.w), max<float>({F.x, F.z}));

        // add the last value
        vec2 F1 = vec2(vec2::dot(centerPos, centerPos), 1e+5);
        F.z = F1.x;
        F.w = F1.y;

        // shuffle into F the final 2 lowest values 
        vec2 F02 = vec2::max(vec2(F.x, F.y), vec2(F.z, F.w));
        vec2 F2 = vec2::min(vec2::min(vec2(F.x, F.y), vec2(F.z, F.w)), vec2(F02.y, F02.x));
        F.x = F2.x;
        F.y = F2.y;

        vec2 f12 = vec2(min(F.x, F.y), max<float>({F.x, F.y}));
        // normalize: 0.75^2 * 2.0  == 1.125
        return vec2::sqrt(f12) * (1.0 / 1.125);
    }

    // Classic 3x3 Cellular noise with F1 and F2 distances.
    // @param scale Number of tiles, must be  integer for tileable results, range: [2, inf]
    // @param jitter Jitter factor for the cells, if zero then it will result in a square grid, range: [0, 1], default: 1.0
    // @param phase The phase for rotating the cells, range: [0, inf], default: 0.0
    // @param seed Seed to randomize result, range: [0, inf], default: 0.0
    // @return Returns the cellular distances, x = F1, y = F2, range: [0, 1].
    vec2 cellularNoisep(vec2 pos, vec2 scale, float jitter, float phase, float seed) 
    {        
        const float kPI2 = 6.2831853071;
        pos *= scale;
        vec2 i = vec2::floor(pos);
        vec2 f = pos - i;
        
        const vec3 offset = vec3(-1.0, 0.0, 1.0);
        vec4 cells = vec4::mod(vec4(i.x, i.y, i.x, i.y) + vec4(offset.x, offset.x, offset.z, offset.z), vec4(scale.x, scale.y, scale.x, scale.y)) + seed;
        i = vec2::mod(i, scale) + seed;
        vec4 dx0, dy0, dx1, dy1;
        multiHash2D(vec4(cells.x, cells.y, i.x, cells.y), vec4(cells.z, cells.y, cells.x, i.y), dx0, dy0);
        multiHash2D(vec4(cells.z, cells.w, cells.z, i.y), vec4(cells.x, cells.w, i.x, cells.w), dx1, dy1);
        dx0 = vec4::sin(dx0 * kPI2 + phase) * 0.5f + 0.5;
        dy0 = vec4::sin(dy0 * kPI2 + phase) * 0.5f + 0.5;
        dx1 = vec4::sin(dx1 * kPI2 + phase) * 0.5f + 0.5;
        dy1 = vec4::sin(dy1 * kPI2 + phase) * 0.5f + 0.5;
        
        dx0 = vec4(offset.x, offset.y, offset.z, offset.x) + dx0 * jitter - vec4(f.x, f.x, f.x, f.x); // -1 0 1 -1
        dy0 = vec4(offset.x, offset.x, offset.x, offset.y) + dy0 * jitter - vec4(f.y, f.y, f.y, f.y); // -1 -1 -1 0
        dx1 = vec4(offset.z, offset.z, offset.x, offset.y) + dx1 * jitter - vec4(f.x, f.x, f.x, f.x); // 1 1 -1 0
        dy1 = vec4(offset.z, offset.y, offset.z, offset.z) + dy1 * jitter - vec4(f.y, f.y, f.y, f.y); // 1 0 1 1
        vec4 d0 = dx0 * dx0 + dy0 * dy0; 
        vec4 d1 = dx1 * dx1 + dy1 * dy1; 
        
        vec2 centerPos = (vec2::sin(multiHash2D(i) * kPI2 + phase) * 0.5f + 0.5f) * jitter - f; // 0 0
        vec4 F = vec4::min(d0, d1);

        // shuffle into F the 4 lowest values
        vec4 F0 = vec4::max(d0, d1);
        F = vec4::min(F, vec4(F0.w, F0.z, F0.y, F0.x));

        // shuffle into F the 2 lowest values 
        vec2 F01 = vec2::max(vec2(F.x, F.y), vec2(F.z, F.w));
        vec2 F1 = vec2::min(vec2::min(vec2(F.x, F.y), vec2(F.z, F.w)), vec2(F01.y, F01.x));
        F.x = F1.x;
        F.y = F1.y;

        // add the last value
        vec2 F2 = vec2(vec2::dot(centerPos, centerPos), 1e+5);
        F.z = F2.x;
        F.w = F2.y;

        // shuffle into F the final 2 lowest values 
        vec2 F03 = vec2::max(vec2(F.x, F.y), vec2(F.z, F.w));
        vec2 F3 = vec2::min(vec2::min(vec2(F.x, F.y), vec2(F.z, F.w)), vec2(F03.y, F03.x));
        F.x = F3.x;
        F.y = F3.y;
        
        vec2 f12 = vec2(min(F.x, F.y), max<float>({F.x, F.y}));

        // normalize: 0.75^2 * 2.0  == 1.125
        return vec2::sqrt(f12) * (1.0 / 1.125);
    }

    // Classic 3x3 Cellular noise with F1 and F2 distances with support for multiple metrics.
    // @param scale Number of tiles, must be  integer for tileable results, range: [2, inf]
    // @param jitter Jitter factor for the cells, if zero then it will result in a square grid, range: [0, 1], default: 1.0
    // @param phase The phase for rotating the cells, range: [0, inf], default: 0.0
    // @param metric The distance metric used, can be euclidean, manhattan, chebyshev or triangular, range: [0, 3], default: 0
    // @param seed Seed to randomize result, range: [0, inf], default: 0.0
    // @return Returns the cellular distances, x = F1, y = F2, range: [0, 1].
    vec2 cellularNoisem(vec2 pos, vec2 scale, float jitter, float phase, uint metric, float seed) 
    {       
        const float kPI2 = 6.2831853071;
        pos *= scale;
        vec2 i = vec2::floor(pos);
        vec2 f = pos - i;
        
        const vec3 offset = vec3(-1.0, 0.0, 1.0);
        vec4 cells = vec4::mod(vec4(i.x, i.y, i.x, i.y) + vec4(offset.x, offset.x, offset.z, offset.z), vec4(scale.x, scale.y, scale.x, scale.y)) + seed;
        i = vec2::mod(i, scale) + seed;
        vec4 dx0, dy0, dx1, dy1;
        multiHash2D(vec4(cells.x, cells.y, i.x, cells.y), vec4(cells.z, cells.y, cells.x, i.y), dx0, dy0);
        multiHash2D(vec4(cells.z, cells.w, cells.z, i.y), vec4(cells.x, cells.w, i.x, cells.w), dx1, dy1);
        dx0 = vec4::sin(dx0 * kPI2 + phase) * 0.5f + 0.5f;
        dy0 = vec4::sin(dy0 * kPI2 + phase) * 0.5f + 0.5f;
        dx1 = vec4::sin(dx1 * kPI2 + phase) * 0.5f + 0.5f;
        dy1 = vec4::sin(dy1 * kPI2 + phase) * 0.5f + 0.5f;
        
        dx0 = vec4(offset.x, offset.y, offset.z, offset.x) + dx0 * jitter - vec4(f.x, f.x, f.x, f.x); // -1 0 1 -1
        dy0 = vec4(offset.x, offset.x, offset.x, offset.y) + dy0 * jitter - vec4(f.y, f.y, f.y, f.y); // -1 -1 -1 0
        dx1 = vec4(offset.z, offset.z, offset.x, offset.y) + dx1 * jitter - vec4(f.x, f.x, f.x, f.x); // 1 1 -1 0
        dy1 = vec4(offset.z, offset.y, offset.z, offset.z) + dy1 * jitter - vec4(f.y, f.y, f.y, f.y); // 1 0 1 1
        vec4 d0 = distanceMetric(dx0, dy0, metric);
        vec4 d1 = distanceMetric(dx1, dy1, metric);
        
        vec2 centerPos = (vec2::sin(multiHash2D(i) * kPI2 + phase) * 0.5f + 0.5f) * jitter - f; // 0 0
        vec4 F = vec4::min(d0, d1);
        // shuffle into F the 4 lowest values
        vec4 F0 = vec4::max(d0, d1);
        F = vec4::min(F, vec4(F0.w, F0.z, F0.y, F0.x));


        // shuffle into F the 2 lowest values 
        F.x = min(min(F.x, F.z), max<float>({F.y, F.w}));
        F.y = min(min(F.y, F.w), max<float>({F.x, F.z}));

        // add the last value
        vec2 F1 = vec2(distanceMetric(centerPos, metric), 1e+5);
        F.z = F1.x;
        F.w = F1.y;

        // shuffle into F the final 2 lowest values 
        vec2 F2 = vec2::max(vec2(F.x, F.y), vec2(F.z, F.w));
        vec2 F02 = vec2::min(vec2::min(vec2(F.x, F.y), vec2(F.z, F.w)), vec2(F2.y, F2.x));
        F.x = F02.x;
        F.y = F02.y;
        
        vec2 f12 = vec2(min(F.x, F.y), max<float>({F.x, F.y}));
        // normalize: 0.75^2 * 2.0  == 1.125
        return (metric == 0u ? vec2::sqrt(f12) : f12) * (1.0 / 1.125);
    }

    // Classic 3x3 Cellular noise with F1 and F2 distances and derivatives.
    // @param scale Number of tiles, must be  integer for tileable results, range: [2, inf]
    // @param jitter Jitter factor for the cells, if zero then it will result in a square grid, range: [0, 1], default: 1.0
    // @param seed Seed to randomize result, range: [0, inf], default: 0.0
    // @return x = value of the noise, yz = derivative of the noise, range: [-1, 1]
    vec3 cellularNoised(vec2 pos, vec2 scale, float jitter, float seed) 
    {       
        pos *= scale;
        vec2 i = vec2::floor(pos);
        vec2 f = pos - i;
        
        const vec3 offset = vec3(-1.0, 0.0, 1.0);
        vec4 cells = vec4::mod(vec4(i.x, i.y, i.x, i.y) + vec4(offset.x, offset.x, offset.z, offset.z), vec4(scale.x, scale.y, scale.x, scale.y)) + seed;
        i = vec2::mod(i, scale) + seed;
        vec4 dx0, dy0, dx1, dy1;
        multiHash2D(vec4(cells.x, cells.y, i.x, cells.y), vec4(cells.z, cells.y, cells.x, i.y), dx0, dy0);
        multiHash2D(vec4(cells.z, cells.w, cells.z, i.y), vec4(cells.x, cells.w, i.x, cells.w), dx1, dy1);
        
        dx0 = vec4(offset.x, offset.y, offset.z, offset.x) + dx0 * jitter - vec4(f.x, f.x, f.x, f.x); // -1 0 1 -1
        dy0 = vec4(offset.x, offset.x, offset.x, offset.y) + dy0 * jitter - vec4(f.y, f.y, f.y, f.y); // -1 -1 -1 0
        dx1 = vec4(offset.z, offset.z, offset.x, offset.y) + dx1 * jitter - vec4(f.x, f.x, f.x, f.x); // 1 1 -1 0
        dy1 = vec4(offset.z, offset.y, offset.z, offset.z) + dy1 * jitter - vec4(f.y, f.y, f.y, f.y); // 1 0 1 1
        vec4 d0 = dx0 * dx0 + dy0 * dy0; 
        vec4 d1 = dx1 * dx1 + dy1 * dy1; 
        
        vec2 centerPos = multiHash2D(i) * jitter - f; // 0 0
        float dCenter = vec2::dot(centerPos, centerPos);
        vec4 d = vec4::min(d0, d1);
        vec4 less = vec4::step(d1, d0);
        vec4 dx = vec4::mix(dx0, dx1, less);
        vec4 dy = vec4::mix(dy0, dy1, less);

        vec3 t1 = d.x < d.y ? vec3(d.x, dx.x, dy.x) : vec3(d.y, dx.y, dy.y);
        vec3 t2 = d.z < d.w ? vec3(d.z, dx.z, dy.z) : vec3(d.w, dx.w, dy.w);
        t2 = t2.x < dCenter ? t2 : vec3(dCenter, centerPos.x, centerPos.y);
        vec3 t = t1.x < t2.x ? t1 : t2;
        t.x = sqrt(t.x);
        // normalize: 0.75^2 * 2.0  == 1.125
        return  t * vec3(1.0, -2.0, -2.0) * (1.0 / 1.125);
    }

    // Classic 3x3 Cellular noise with F1 and F2 distances and derivatives.
    // @param scale Number of tiles, must be  integer for tileable results, range: [2, inf]
    // @param jitter Jitter factor for the cells, if zero then it will result in a square grid, range: [0, 1], default: 1.0
    // @param phase The phase for rotating the cells, range: [0, inf], default: 0.0
    // @param seed Seed to randomize result, range: [0, inf], default: 0.0
    // @return x = value of the noise, yz = derivative of the noise, range: [-1, 1]
    vec3 cellularNoisedp(vec2 pos, vec2 scale, float jitter, float phase, float seed) 
    {       
        const float kPI2 = 6.2831853071;
        pos *= scale;
        vec2 i = vec2::floor(pos);
        vec2 f = pos - i;
        
        const vec3 offset = vec3(-1.0, 0.0, 1.0);
        vec4 cells = vec4::mod(vec4(i.x, i.y, i.x, i.y) + vec4(offset.x, offset.x, offset.z, offset.z), vec4(scale.x, scale.y, scale.x, scale.y)) + seed;
        i = vec2::mod(i, scale) + seed;
        vec4 dx0, dy0, dx1, dy1;
        multiHash2D(vec4(cells.x, cells.y, i.x, cells.y), vec4(cells.z, cells.y, cells.x, i.y), dx0, dy0);
        multiHash2D(vec4(cells.z, cells.w, cells.z, i.y), vec4(cells.x, cells.w, i.x, cells.w), dx1, dy1);
        dx0 = vec4::sin(dx0 * kPI2 + phase) * 0.5f + 0.5f;
        dy0 = vec4::sin(dy0 * kPI2 + phase) * 0.5f + 0.5f;
        dx1 = vec4::sin(dx1 * kPI2 + phase) * 0.5f + 0.5f;
        dy1 = vec4::sin(dy1 * kPI2 + phase) * 0.5f + 0.5f;
        
        dx0 = vec4(offset.x, offset.y, offset.z, offset.x) + dx0 * jitter - vec4(f.x, f.x, f.x, f.x); // -1 0 1 -1
        dy0 = vec4(offset.x, offset.x, offset.x, offset.y) + dy0 * jitter - vec4(f.y, f.y, f.y, f.y); // -1 -1 -1 0
        dx1 = vec4(offset.z, offset.z, offset.x, offset.y) + dx1 * jitter - vec4(f.x, f.x, f.x, f.x); // 1 1 -1 0
        dy1 = vec4(offset.z, offset.y, offset.z, offset.z) + dy1 * jitter - vec4(f.y, f.y, f.y, f.y); // 1 0 1 1
        vec4 d0 = dx0 * dx0 + dy0 * dy0; 
        vec4 d1 = dx1 * dx1 + dy1 * dy1; 
        
        vec2 centerPos = (vec2::sin(multiHash2D(i) * kPI2 + phase) * 0.5f + 0.5f) * jitter - f; // 0 0
        float dCenter = vec2::dot(centerPos, centerPos);
        vec4 d = vec4::min(d0, d1);
        vec4 less = vec4::step(d1, d0);
        vec4 dx = vec4::mix(dx0, dx1, less);
        vec4 dy = vec4::mix(dy0, dy1, less);

        vec3 t1 = d.x < d.y ? vec3(d.x, dx.x, dy.x) : vec3(d.y, dx.y, dy.y);
        vec3 t2 = d.z < d.w ? vec3(d.z, dx.z, dy.z) : vec3(d.w, dx.w, dy.w);
        t2 = t2.x < dCenter ? t2 : vec3(dCenter, centerPos.x, centerPos.y);
        vec3 t = t1.x < t2.x ? t1 : t2;
        t.x = sqrt(t.x);
        // normalize: 0.75^2 * 2.0  == 1.125
        return  t * vec3(1.0, -2.0, -2.0) * (1.0 / 1.125);
    }

    // A variation of 3x3 Cellular noise that multiplies the minimum distance between the cells.
    // @param scale Number of tiles, must be  integer for tileable results, range: [2, inf]
    // @param jitter Jitter factor for the cells, if zero then it will result in a square grid, range: [0, 1], default: 1.0
    // @param seed Seed to randomize result, range: [0, inf], default: 0.0
    // @return Returns the metaballs distance from the cell edges, range: [0, 1]
    float metaballs(vec2 pos, vec2 scale, float jitter, float seed) 
    {       
        pos *= scale;
        vec2 i = vec2::floor(pos);
        vec2 f = pos - i;
        
        const vec3 offset = vec3(-1.0, 0.0, 1.0);
        vec4 cells = vec4::mod(vec4(i.x, i.y, i.x, i.y) + vec4(offset.x, offset.x, offset.z, offset.z), vec4(scale.x, scale.y, scale.x, scale.y)) + seed;
        i = vec2::mod(i, scale) + seed;
        vec4 dx0, dy0, dx1, dy1;
        multiHash2D(vec4(cells.x, cells.y, i.x, cells.y), vec4(cells.z, cells.y, cells.x, i.y), dx0, dy0);
        multiHash2D(vec4(cells.z, cells.w, cells.z, i.y), vec4(cells.x, cells.w, i.x, cells.w), dx1, dy1);

        dx0 = vec4(offset.x, offset.y, offset.z, offset.x) + dx0 * jitter - vec4(f.x, f.x, f.x, f.x); // -1 0 1 -1
        dy0 = vec4(offset.x, offset.x, offset.x, offset.y) + dy0 * jitter - vec4(f.y, f.y, f.y, f.y); // -1 -1 -1 0
        dx1 = vec4(offset.z, offset.z, offset.x, offset.y) + dx1 * jitter - vec4(f.x, f.x, f.x, f.x); // 1 1 -1 0
        dy1 = vec4(offset.z, offset.y, offset.z, offset.z) + dy1 * jitter - vec4(f.y, f.y, f.y, f.y); // 1 0 1 1
        vec4 d0 = dx0 * dx0 + dy0 * dy0; 
        vec4 d1 = dx1 * dx1 + dy1 * dy1; 
        
        vec2 centerPos = multiHash2D(i) * jitter - f; // 0 0
        
        float d = min(1.0f, vec2::dot(centerPos, centerPos));
        d = min(d, d * d0.x);
        d = min(d, d * d0.y);
        d = min(d, d * d0.z);
        d = min(d, d * d0.w);
        d = min(d, d * d1.x);
        d = min(d, d * d1.y);
        d = min(d, d * d1.z);
        d = min(d, d * d1.w);
        
        return sqrt(d);
    }

    float metaballsw(vec2 pos, vec2 scale, float jitter, float width, float smoothness, float seed) 
    {       
        float d = metaballs(pos, scale, jitter, seed);
        return smoothstep(width, width + smoothness, d);
    }

    // A variation of 3x3 Cellular noise that multiplies the minimum distance between the cells.
    // @param scale Number of tiles, must be  integer for tileable results, range: [2, inf]
    // @param jitter Jitter factor for the cells, if zero then it will result in a square grid, range: [0, 1], default: 1.0
    // @param phase The phase for rotating the cells, range: [0, inf], default: 0.0
    // @param seed Seed to randomize result, range: [0, inf], default: 0.0
    // @return Returns the metaballs distance from the cell edges, range: [0, 1]
    float metaballsp(vec2 pos, vec2 scale, float jitter, float phase, float seed) 
    {       
        const float kPI2 = 6.2831853071;
        pos *= scale;
        vec2 i = vec2::floor(pos);
        vec2 f = pos - i;
        
        const vec3 offset = vec3(-1.0, 0.0, 1.0);
        vec4 cells = vec4::mod(vec4(i.x, i.y, i.x, i.y) + vec4(offset.x, offset.x, offset.z, offset.z), vec4(scale.x, scale.y, scale.x, scale.y)) + seed;
        i = vec2::mod(i, scale) + seed;
        vec4 dx0, dy0, dx1, dy1;
        multiHash2D(vec4(cells.x, cells.y, i.x, cells.y), vec4(cells.z, cells.y, cells.x, i.y), dx0, dy0);
        multiHash2D(vec4(cells.z, cells.w, cells.z, i.y), vec4(cells.x, cells.w, i.x, cells.w), dx1, dy1);
        dx0 = vec4::sin(dx0 * kPI2 + phase) * 0.5f + 0.5f;
        dy0 = vec4::sin(dy0 * kPI2 + phase) * 0.5f + 0.5f;
        dx1 = vec4::sin(dx1 * kPI2 + phase) * 0.5f + 0.5f;
        dy1 = vec4::sin(dy1 * kPI2 + phase) * 0.5f + 0.5f;
        
        dx0 = vec4(offset.x, offset.y, offset.z, offset.x) + dx0 * jitter - vec4(f.x, f.x, f.x, f.x); // -1 0 1 -1
        dy0 = vec4(offset.x, offset.x, offset.x, offset.y) + dy0 * jitter - vec4(f.y, f.y, f.y, f.y); // -1 -1 -1 0
        dx1 = vec4(offset.z, offset.z, offset.x, offset.y) + dx1 * jitter - vec4(f.x, f.x, f.x, f.x); // 1 1 -1 0
        dy1 = vec4(offset.z, offset.y, offset.z, offset.z) + dy1 * jitter - vec4(f.y, f.y, f.y, f.y); // 1 0 1 1
        vec4 d0 = dx0 * dx0 + dy0 * dy0; 
        vec4 d1 = dx1 * dx1 + dy1 * dy1; 
        
        vec2 centerPos = (vec2::sin(multiHash2D(i) * kPI2 + phase) * 0.5f + 0.5f) * jitter - f; // 0 0
        
        float d = min(1.0f, vec2::dot(centerPos, centerPos));
        d = min(d, d * d0.x);
        d = min(d, d * d0.y);
        d = min(d, d * d0.z);
        d = min(d, d * d0.w);
        d = min(d, d * d1.x);
        d = min(d, d * d1.y);
        d = min(d, d * d1.z);
        d = min(d, d * d1.w);
        
        return sqrt(d);
    }

    float metaballswp(vec2 pos, vec2 scale, float jitter, float phase, float width, float smoothness, float seed) 
    {       
        float d = metaballsp(pos, scale, jitter, phase, seed);
        return smoothstep(width, width + smoothness, d);
    }

    // A variation of 3x3 Cellular noise that multiplies the minimum distance between the cells.
    // @param scale Number of tiles, must be  integer for tileable results, range: [2, inf]
    // @param jitter Jitter factor for the cells, if zero then it will result in a square grid, range: [0, 1], default: 1.0
    // @param phase The phase for rotating the cells, range: [0, inf], default: 0.0
    // @param metric The distance metric used, can be euclidean, manhattan, chebyshev or triangular, range: [0, 3], default: 0
    // @param seed Seed to randomize result, range: [0, inf], default: 0.0
    // @return Returns the metaballs distance from the cell edges, range: [0, 1]
    float metaballspm(vec2 pos, vec2 scale, float jitter, float phase, uint metric, float seed) 
    {       
        const float kPI2 = 6.2831853071;
        pos *= scale;
        vec2 i = vec2::floor(pos);
        vec2 f = pos - i;
        
        const vec3 offset = vec3(-1.0, 0.0, 1.0);
        vec4 cells = vec4::mod(vec4(i.x, i.y, i.x, i.y) + vec4(offset.x, offset.x, offset.z, offset.z), vec4(scale.x, scale.y, scale.x, scale.y)) + seed;
        i = vec2::mod(i, scale) + seed;
        vec4 dx0, dy0, dx1, dy1;
        multiHash2D(vec4(cells.x, cells.y, i.x, cells.y), vec4(cells.z, cells.y, cells.x, i.y), dx0, dy0);
        multiHash2D(vec4(cells.z, cells.w, cells.z, i.y), vec4(cells.x, cells.w, i.x, cells.w), dx1, dy1);
        dx0 = vec4::sin(dx0 * kPI2 + phase) * 0.5f + 0.5f;
        dy0 = vec4::sin(dy0 * kPI2 + phase) * 0.5f + 0.5f;
        dx1 = vec4::sin(dx1 * kPI2 + phase) * 0.5f + 0.5f;
        dy1 = vec4::sin(dy1 * kPI2 + phase) * 0.5f + 0.5f;
        
        dx0 = vec4(offset.x, offset.y, offset.z, offset.x) + dx0 * jitter - vec4(f.x, f.x, f.x, f.x); // -1 0 1 -1
        dy0 = vec4(offset.x, offset.x, offset.x, offset.y) + dy0 * jitter - vec4(f.y, f.y, f.y, f.y); // -1 -1 -1 0
        dx1 = vec4(offset.z, offset.z, offset.x, offset.y) + dx1 * jitter - vec4(f.x, f.x, f.x, f.x); // 1 1 -1 0
        dy1 = vec4(offset.z, offset.y, offset.z, offset.z) + dy1 * jitter - vec4(f.y, f.y, f.y, f.y); // 1 0 1 1
        vec4 d0 = distanceMetric(dx0, dy0, metric);
        vec4 d1 = distanceMetric(dx1, dy1, metric);
        
        vec2 centerPos = (vec2::sin(multiHash2D(i) * kPI2 + phase) * 0.5f + 0.5f) * jitter - f; // 0 0
        
        float d = min(1.0f, distanceMetric(centerPos, metric));
        d = min(d, d * d0.x);
        d = min(d, d * d0.y);
        d = min(d, d * d0.z);
        d = min(d, d * d0.w);
        d = min(d, d * d1.x);
        d = min(d, d * d1.y);
        d = min(d, d * d1.z);
        d = min(d, d * d1.w);
        
        return metric == 0u ? sqrt(d) : d;
    }

    float metaballswpm(vec2 pos, vec2 scale, float jitter, float phase, float width, float smoothness, uint metric, float seed) 
    {       
        float d = metaballspm(pos, scale, jitter, phase, metric, seed);
        return smoothstep(width, width + smoothness, d);
    }

    // @param scale Number of tiles, must be  integer for tileable results, range: [2, inf]
    // @param jitter Jitter factor for the cells, if zero then it will result in a square grid, range: [0, 1], default: 1.0
    // @param phase The phase for rotating the cells, range: [0, inf], default: 0.0
    // @param metric The distance metric used, can be euclidean, manhattan, chebyshev or triangular, range: [0, 3], default: 0
    // @param seed Seed to randomize result, range: [0, inf], default: 0.0
    // @return value of the noise
    vec3 crystals(vec2 pos, vec2 scale, float jitter, float phase, uint metric)
    {
        vec3 c0 = cellularNoisem(pos, scale, jitter, phase, metric, 0.0);
        vec3 c1 = cellularNoisem(pos, scale, jitter, phase, metric, 23.0);
        c0.x = 1.0 - c0.x;
        c1.x = 1.0 - c1.x;
        if (c0.x > c1.x)
        {
            vec3 temp = c0;
            c0 = c1;
            c1 = temp;
        }

        return vec3(c1.x - c0.x, c0.y - c1.y, c0.z - c1.z);
    }

    // @param scale Number of tiles, must be  integer for tileable results, range: [2, inf]
    // @param jitter Jitter factor for the cells, if zero then it will result in a square grid, range: [0, 1], default: 1.0
    // @param phase The phase for rotating the cells, range: [0, inf], default: 0.0
    // @param seed Seed to randomize result, range: [0, inf], default: 0.0
    // @return value of the noise, yz = derivative of the noise, range: [-1, 1]
    vec3 crystalsd(vec2 pos, vec2 scale, float jitter, float phase)
    {
        vec3 c0 = cellularNoisedp(pos, scale, jitter, phase, 0.0);
        vec3 c1 = cellularNoisedp(pos, scale, jitter, phase, 23.0);
        c0.x = 1.0 - c0.x;
        c1.x = 1.0 - c1.x;
        if (c0.x > c1.x)
        {
            vec3 temp = c0;
            c0 = c1;
            c1 = temp;
        }

        return vec3(c1.x - c0.x, c0.y - c1.y, c0.z - c1.z);
    }
}};