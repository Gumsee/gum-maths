#include "../Noise.h"

namespace Gum {
namespace Noise {
    unsigned int ihash1D(unsigned int q)
    {
        // hash by Hugo Elias, Integer Hash - I, 2017
        q = q * 747796405u + 2891336453u;
        q = (q << 13u) ^ q;
        return q * (q * q * 15731u + 789221u) + 1376312589u;
    }

    uivec2 ihash2D(uivec2 q)
    {
        // hash by Hugo Elias, Integer Hash - I, 2017
        q = q * 747796405u + 2891336453u;
        q = (q << 13u) ^ q;
        return q * (q * q * 15731u + 789221u) + 1376312589u;
    }

    uivec4 ihash4D(uivec4 q)
    {
        // hash by Hugo Elias, Integer Hash - I, 2017
        q = q * 747796405u + 2891336453u;
        q = (q << 13u) ^ q;
        return q * (q * q * 15731u + 789221u) + 1376312589u;
    }

    float hash1D(float x)
    {
        // based on: pcg by Mark Jarzynski: http://www.jcgt.org/published/0009/03/02/
        uint state = uint(x * 8192.0) * 747796405u + 2891336453u;
        uint word = ((state >> ((state >> 28u) + 4u)) ^ state) * 277803737u;
        return float((word >> 22u) ^ word) * (1.0 / float(0xffffffffu));;
    }

    float hash1D(vec2 x)
    {
        // hash by Inigo Quilez, Integer Hash - III, 2017
        uivec2 q = uivec2(x * 8192.0);
        q = ((q >> 1u) ^ uivec2(q.y, q.x)) * 1103515245u;
        uint n = 1103515245u * (q.x ^ (q.y >> 3u));
        return float(n) * (1.0 / float(0xffffffffu));
    }

    float hash1D(vec3 x)
    {
        // based on: pcg3 by Mark Jarzynski: http://www.jcgt.org/published/0009/03/02/
        uivec3 v = uivec3(x * 8192.0) * 1664525u + 1013904223u;
        v += uivec3(v.y, v.z, v.x) * uivec3(v.z, v.x, v.y);
        v ^= v >> 16u;
        return float(v.x + v.y * v.z) * (1.0 / float(0xffffffffu));
    }

    vec2 hash2D(vec2 x)
    {
        // based on: Inigo Quilez, Integer Hash - III, 2017
        uivec2 y = uivec2(x * 8192.0);
        uivec2 z = uivec2(0u, 3115245u);
        uivec4 q = uivec4(y.x, y.y, y.y, y.x) + uivec4(z.x, z.x, z.y, z.y);
        q = ((q >> 1u) ^ uivec4(q.y, q.x, q.w, q.z)) * 1103515245u;
        uivec2 n = (uivec2(q.x, q.z) ^ (uivec2(q.y, q.w) >> 3u)) * 1103515245u;
        return vec2(n) * (1.0 / float(0xffffffffu));
    }

    vec3 hash3D(vec2 x) 
    {
        // based on: pcg3 by Mark Jarzynski: http://www.jcgt.org/published/0009/03/02/
        uivec3 v = uivec3(vec3(x.x, x.y, x.x) * 8192.0) * 1664525u + 1013904223u;
        v += uivec3(v.y, v.z, v.x) * uivec3(v.z, v.x, v.y);
        v ^= v >> 16u;

        v.x += v.y * v.z;
        v.y += v.z * v.x;
        v.z += v.x * v.y;
        return vec3(v) * (1.0 / float(0xffffffffu));
    }

    vec3 hash3D(vec3 x) 
    {
        // based on: pcg3 by Mark Jarzynski: http://www.jcgt.org/published/0009/03/02/
        uivec3 v = uivec3(x * 8192.0) * 1664525u + 1013904223u;
        v += uivec3(v.y, v.z, v.x) * uivec3(v.z, v.x, v.y);
        v ^= v >> 16u;

        v.x += v.y * v.z;
        v.y += v.z * v.x;
        v.z += v.x * v.y;
        return vec3(v) * (1.0 / float(0xffffffffu));
    }

    vec4 hash4D(vec2 x)
    {
        // based on: pcg4 by Mark Jarzynski: http://www.jcgt.org/published/0009/03/02/
        uivec4 v = uivec4(vec4(x.x, x.y, x.y, x.x) * 8192.0) * 1664525u + 1013904223u;

        v += uivec4(v.y, v.z, v.x, v.y) * uivec4(v.w, v.x, v.y, v.z);
        v.x += v.y * v.w;
        v.y += v.z * v.x;
        v.z += v.x * v.y;
        v.w += v.y * v.z;
        
        v.x += v.y * v.w;
        v.w += v.y * v.z;
        
        v ^= v >> 16u;

        return vec4(v ^ (v >> 16u)) * (1.0 / float(0xffffffffu));
    }

    vec4 hash4D(vec4 x)
    {
        // based on: pcg4 by Mark Jarzynski: http://www.jcgt.org/published/0009/03/02/
        uivec4 v = uivec4(x * 8192.0) * 1664525u + 1013904223u;

        v += uivec4(v.y, v.z, v.x, v.y) * uivec4(v.w, v.x, v.y, v.z);
        v.x += v.y * v.w;
        v.y += v.z * v.x;
        v.z += v.x * v.y;
        v.w += v.y * v.z;
        
        v.x += v.y * v.w;
        v.y += v.z * v.x;
        v.z += v.x * v.y;
        v.w += v.y * v.z;

        v ^= v >> 16u;

        return vec4(v ^ (v >> 16u)) * (1.0 / float(0xffffffffu));
    }


    // generates 2 random numbers for the coordinate
    vec2 multiHash2D(vec2 x)
    {
        uivec2 q = uivec2(x);
        uint h0 = ihash1D(ihash1D(q.x) + q.y);
        uint h1 = h0 * 1933247u + ~h0 ^ 230123u;
        return vec2(h0, h1)  * (1.0 / float(0xffffffffu));
    }

    // generates a random number for each of the 4 cell corners
    vec4 multiHash2D(vec4 cell)
    {
        uivec4 i = uivec4(cell);
        uivec4 hash = ihash4D(ihash4D(uivec4(i.x, i.z, i.x, i.z)) + uivec4(i.y, i.y, i.w, i.w));
        return vec4(hash) * (1.0 / float(0xffffffffu));
    }

    // generates 2 random numbers for each of the 4 cell corners
    void multiHash2D(vec4 cell, vec4& hashX, vec4& hashY)
    {
        uivec4 i = uivec4(cell);
        uivec4 hash0 = ihash4D(ihash4D(uivec4(i.x, i.z, i.x, i.z)) + uivec4(i.y, i.y, i.w, i.w));
        uivec4 hash1 = ihash4D(hash0 ^ 1933247u);
        hashX = vec4(hash0) * (1.0 / float(0xffffffffu));
        hashY = vec4(hash1) * (1.0 / float(0xffffffffu));
    }

    // generates 2 random numbers for each of the 2D coordinates
    vec4 multiHash2D(vec2 coords0, vec2 coords1)
    {
        uivec4 i = uivec4(coords0.x, coords0.y, coords1.x, coords1.y);
        uivec4 hash = ihash2D(ihash2D(uivec2(i.x, i.z)) + uivec2(i.y, i.w));
        hash = uivec4(hash.x, hash.x, hash.y, hash.y);
        hash.y = hash.y * 1933247u + ~hash.y ^ 230123u;
        hash.w = hash.w * 1933247u + ~hash.w ^ 230123u;
        return vec4(hash) * (1.0 / float(0xffffffffu));
    }

    // generates 2 random numbers for each of the four 2D coordinates
    void multiHash2D(vec4 coords0, vec4 coords1, vec4& hashX, vec4& hashY)
    {
        uivec4 hash0 = ihash4D(ihash4D(uivec4(coords0.x, coords0.z, coords1.x, coords1.z)) + uivec4(coords0.y, coords0.w, coords1.y, coords1.w));
        uivec4 hash1;
        hash1.x = hash0.x * 1933247u + ~hash0.x ^ 230123u;
        hash1.y = hash0.y * 1933247u + ~hash0.y ^ 230123u;
        hash1.z = hash0.z * 1933247u + ~hash0.z ^ 230123u;
        hash1.w = hash0.w * 1933247u + ~hash0.w ^ 230123u;
        hashX = vec4(hash0) * (1.0 / float(0xffffffffu));
        hashY = vec4(hash1) * (1.0 / float(0xffffffffu));
    } 

    // generates a random number for each of the 8 cell corners
    void multiHash3D(vec3 cell, vec3 cellPlusOne, vec4& lowHash, vec4& highHash)
    {
        uivec4 cells = uivec4(cell.x, cell.y, cellPlusOne.x, cellPlusOne.y);  
        uivec4 hash = ihash4D(ihash4D(vec4(cells.x, cells.z, cells.x, cells.z)) + vec4(cells.y, cells.y, cells.w, cells.w));
        
        lowHash = vec4(ihash4D(hash + uint(cell.z))) * (1.0 / float(0xffffffffu));
        highHash = vec4(ihash4D(hash + uint(cellPlusOne.z))) * (1.0 / float(0xffffffffu));
    }

    void smultiHash2D(vec4 cell, vec4& hashX, vec4& hashY)
    {
        multiHash2D(cell, hashX, hashY);
        hashX = hashX * 2.0 - 1.0; 
        hashY = hashY * 2.0 - 1.0;
    }
}}
