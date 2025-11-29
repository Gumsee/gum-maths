#pragma once
#include "mat.h"
#include "vec.h"

namespace Gum {
namespace Noise {
	extern int randomNumber;
    
    extern int setSeed(long seed);

    //Hashes
    extern unsigned int ihash1D(unsigned int q);
    extern uivec2 ihash2D(uivec2 q);
    extern uivec4 ihash4D(uivec4 q);
    extern float hash1D(float x);
    extern float hash1D(vec2 x);
    extern float hash1D(vec3 x);
    extern vec2 hash2D(vec2 x);
    extern vec3 hash3D(vec2 x);
    extern vec3 hash3D(vec3 x);
    extern vec4 hash4D(vec2 x);
    extern vec4 hash4D(vec4 x);
    extern vec2 multiHash2D(vec2 x);
    extern vec4 multiHash2D(vec4 cell);
    extern void multiHash2D(vec4 cell, vec4& hashX, vec4& hashY);
    extern vec4 multiHash2D(vec2 coords0, vec2 coords1);
    extern void multiHash2D(vec4 coords0, vec4 coords1, vec4& hashX, vec4& hashY);
    extern void multiHash3D(vec3 cell, vec3 cellPlusOne, vec4& lowHash, vec4& highHash);
    extern void smultiHash2D(vec4 cell, vec4& hashX, vec4& hashY);
    extern float distanceMetric(vec2 pos, uint metric);
    extern vec4 distanceMetric(vec4 px, vec4 py, uint metric);

    //Classic Noise

    extern float noiseInterpolate(const float x);
    extern vec2 noiseInterpolate(const vec2 x);
    extern vec3 noiseInterpolate(const vec3 x);
    extern vec4 noiseInterpolate(const vec4 x);
    extern vec2 noiseInterpolateDu(const float x);
    extern vec4 noiseInterpolateDu(const vec2 x);
    extern void noiseInterpolateDu(const vec3 x, vec3& u, vec3& du);
    extern float noise(float pos, float scale = 2.0f, float seed = randomNumber);
    extern float noise(vec2 pos, vec2 scale = vec2(2.0f), float seed = randomNumber);
    extern float noisep(vec2 pos, vec2 scale = vec2(2.0f), float phase = 0.0f, float seed = randomNumber);
    extern vec3 noised(vec2 pos, vec2 scale = vec2(2.0f), float seed = randomNumber);
    extern vec3 noisepd(vec2 pos, vec2 scale = vec2(2.0f), float phase = 0.0f, float seed = randomNumber);
    extern float noise3d(vec2 pos, vec2 scale = vec2(2.0f), float height = 0.0f, float seed = randomNumber);
    extern vec4 noised3d(vec2 pos, vec2 scale = vec2(2.0f), float time = 0.0f, float seed = randomNumber);
    extern vec2 multiNoise(vec4 pos, vec4 scale = vec2(2.0f), float phase = 0.0f, vec2 seed = randomNumber);
    extern float gridNoise(vec2 pos, vec2 scale = vec2(2.0f), float intensity = 0.75f, float time = 0.0f, float seed = randomNumber);
    extern float gridNoiset(vec2 pos, vec2 scale = vec2(2.0f), vec3 translate = vec3(0.5, -0.25, 0.15), float intensity = 0.75f, float time = 0.0f, float seed = randomNumber);
    extern vec3 dotsNoise(vec2 pos, vec2 scale = vec2(2.0f), float density = 1.0f, float size = 0.5f, float sizeVariation = 0.75f, float roundness = 1.0f, float seed = randomNumber);
    extern vec2 randomLines(vec2 pos, vec2 scale = vec2(2.0f), float count = 4.0f, float width = 1.0f, float jitter = 1.0f, vec2 smoothness = 0.0f, float phase = 0.0f, float seed = randomNumber);
    extern vec4 randomLinesv(vec2 pos, vec2 scale = vec2(2.0f), float count = 4.0f, float width = 1.0f, float jitter = 1.0f, vec2 smoothness = 0.0f, float phase = 0.0f, float colorVariation = 1.0f, float seed = randomNumber);
    

    //Gradient Noise
    extern float gradientNoise(vec2 pos, vec2 scale = vec2(2.0f), float seed = randomNumber);
    extern float gradientNoiset(vec2 pos, vec2 scale = vec2(2.0f), mat2 transform = mat2(), float seed = randomNumber);
    extern float gradientNoiser(vec2 pos, vec2 scale = vec2(2.0f), float rotation = 0.0f, float seed = randomNumber);
    extern vec3 gradientNoised(vec2 pos, vec2 scale = vec2(2.0f), float seed = randomNumber);
    extern vec3 gradientNoisedt(vec2 pos, vec2 scale = vec2(2.0f), mat2 transform = mat2(), float seed = randomNumber);
    extern vec3 gradientNoisedr(vec2 pos, vec2 scale = vec2(2.0f), float rotation = 0.0f, float seed = randomNumber);
    extern float gradientNoiseDisorder(vec2 pos, vec2 scale = vec2(2.0f), float disoder = 1.0f, float seed = randomNumber);


    //Perlin
    extern float perlinNoise(vec2 pos, vec2 scale = vec2(2.0f), float seed = randomNumber);
    extern vec3 perlinNoised(vec2 pos, vec2 scale = vec2(2.0f), float seed = randomNumber);
    extern float organicNoise(vec2 pos, vec2 scale = vec2(2.0f), float density = 1.0f, vec2 phase = vec2(0.0f), float contrast = 0.0f, float highlights = 0.25f, float shift = 0.5f, float seed = randomNumber);


    //Voronoi
    extern vec3 voronoi(vec2 pos, vec2 scale = vec2(2.0f), float jitter = 1.0f, float phase = 0.0f, int seed = randomNumber);
    extern vec3 voronoiPattern(vec2 pos, vec2 scale = vec2(5.0f), float jitter = 1.0f, float variance = 1.0f, float factor = 1.0f, int seed = randomNumber);
    //extern vec3 cracks(vec2 pos, vec2 scale = vec2(2.0f), float jitter = 1.0f, float width = 1.0f, float smoothness = 0.0f, float warp = 0.0f, float warpScale = 2.0f, bool warpSmudge = false, float smudgePhase = 0.0f, float seed = randomNumber);
    //extern float cracks(vec2 pos, vec2 scale = vec2(2.0f), float jitter = 1.0f, float width = 1.0f, float smoothness = 0.0f, float warp = 0.0f, float warpScale = 2.0f, bool warpSmudge = false, float seed = randomNumber);
    

    //Hexagons
    extern float sdfHexagon(vec2 pos, float radius, bool isVertical);
    extern vec4 tileHexagonsRadii(vec2 scale, bool isVertical);
    extern vec4 tileHexagons(vec2 pos, vec2 scale = vec2(2.0f), bool isVertical = false);
    extern vec4 tileHexagons(vec2 pos, float& edgeDistance, vec2 scale = vec2(2.0f), bool isVertical = false);
    extern float noiseHexagons(vec2 pos, vec2 scale = vec2(2.0f), float size = 1.0f, vec2 jitter = 1.0f, bool isVertical = false, bool useCenter = false);
    //extern vec3 noiseHexagonsd(vec2 pos, vec2 scale = vec2(2.0f), float size = 1.0f, vec2 disorder = 1.0f, bool isVertical = false, bool useCenter = false);


    //Patterns
    extern vec3 checkerboard(const vec2 pos, const vec2 scale = vec2(2.0f), const vec2 smoothness = vec2(0.0f));
    extern vec3 checkerboard45(const vec2 pos, const vec2 scale = vec2(2.0f), const vec2 smoothness = vec2(1.0f));
    extern float wavePattern(vec2 pos, vec2 scale = vec2(2.0f), float width = 1.0f, float smoothness = 1.0f, float amplitude = 1.0f, float interpolate = 1.0f);
    extern float crossPattern(vec2 pos, vec2 scale = vec2(2.0f), vec2 smoothness = vec2(0.0f));
    extern float stairsPattern(vec2 pos, vec2 scale = vec2(2.0f), float width = 1.0f, float smoothness = 0.0f, float distance = 0.0f);
    //extern vec3 tileWeave(vec2 pos, vec2 scale = vec2(2.0f), float count = 2.0f, float width = 1.0f, float smoothness = 0.0f);
    //extern vec3 tileWeave(vec2 pos, vec2 scale = vec2(2.0f), float count = 2.0f, vec2 width = vec2(1.0f), float smoothness = 0.0f);

    //Cellular
    extern vec2 cellularNoise(vec2 pos, vec2 scale = vec2(2.0f), float jitter = 1.0f, float seed = randomNumber);
    extern vec2 cellularNoisep(vec2 pos, vec2 scale = vec2(2.0f), float jitter = 1.0f, float phase = 0.0f, float seed = randomNumber);
    extern vec2 cellularNoisem(vec2 pos, vec2 scale = vec2(2.0f), float jitter = 1.0f, float phase = 0.0f, uint metric = 0, float seed = randomNumber);
    extern vec3 cellularNoised(vec2 pos, vec2 scale = vec2(2.0f), float jitter = 1.0f, float seed = randomNumber);
    extern vec3 cellularNoisedp(vec2 pos, vec2 scale = vec2(2.0f), float jitter = 1.0f, float phase = 0.0f, float seed = randomNumber);
    extern float metaballs(vec2 pos, vec2 scale = vec2(2.0f), float jitter = 1.0f, float seed = randomNumber);
    extern float metaballsw(vec2 pos, vec2 scale = vec2(2.0f), float jitter = 1.0f, float width = 1.0f, float smoothness = 0.0f, float seed = randomNumber);
    extern float metaballsp(vec2 pos, vec2 scale = vec2(2.0f), float jitter = 1.0f, float phase = 0.0f, float seed = randomNumber);
    extern float metaballswp(vec2 pos, vec2 scale = vec2(2.0f), float jitter = 1.0f, float phase = 0.0f, float width = 1.0f, float smoothness = 0.0f, float seed = randomNumber);
    extern float metaballspm(vec2 pos, vec2 scale = vec2(2.0f), float jitter = 1.0f, float phase = 0.0f, uint metric = 0, float seed = randomNumber);
    extern float metaballswpm(vec2 pos, vec2 scale = vec2(2.0f), float jitter = 1.0f, float phase = 0.0f, float width = 1.0f, float smoothness = 0.0f, uint metric = 0, float seed = randomNumber);
    extern vec3 crystals(vec2 pos, vec2 scale = vec2(2.0f), float jitter = 1.0f, float phase = 0.0f, uint metric = 0);
    extern vec3 crystalsd(vec2 pos, vec2 scale = vec2(2.0f), float jitter = 1.0f, float phase = 0.0f);


    //FBM
    float fbm(vec2 pos, vec2 scale = vec2(2.0f), int octaves = 1, float shift = 0.0f, float timeShift = 0.0f, float gain = 0.5f, float lacunarity = 8, float octaveFactor = 0.0f, float seed = randomNumber);
    vec3 fbmd(vec2 pos, vec2 scale = vec2(2.0f), int octaves = 1, vec2 shift = 0.0f, float timeShift = 0.0f, float gain = 0.5f, vec2 lacunarity = 8, float slopeness = 0.5f, float octaveFactor = 0.0f, float seed = randomNumber);
    vec3 fbmd(vec2 pos, vec2 scale = vec2(2.0f), int octaves = 1, float shift = 0.0f, float timeShift = 0.0f, float gain = 0.5f, float lacunarity = 8, float slopeness = 0.5f, float octaveFactor = 0.0f, float seed = randomNumber);
    vec3 fbmd(vec2 pos, vec2 scale = vec2(2.0f), int octaves = 1, vec2 shift = 0.0f, float timeShift = 0.0f, float gain = 0.5f, float lacunarity = 8, float slopeness = 0.5f, float octaveFactor = 0.0f, float seed = randomNumber);
    //vec3 fbmdPerlin(vec2 pos, vec2 scale, int octaves, vec2 shift, mat2 transform, float gain, vec2 lacunarity, float slopeness, float octaveFactor, bool negative, float seed);
    //vec3 fbmdPerlin(vec2 pos, vec2 scale, int octaves, vec2 shift, float axialShift, float gain, vec2 lacunarity, float slopeness, float octaveFactor, bool negative, float seed);
    //float fbmPerlin(vec2 pos, vec2 scale = vec2(2.0f), int octaves = 1, float shift = 0.0f, float axialShift = 0.0f, float gain = 0.5f, float lacunarity = 8, uint mode = 0, float factor = 1.0f, float offset = 0.0f, float octaveFactor = 0.0f, float seed = randomNumber);
    vec4 fbmVoronoi(vec2 pos, vec2 scale = vec2(2.0f), int octaves = 1, float shift = 0.0f, float timeShift = 0.0f, float gain = 0.5f, float lacunarity = 8, float octaveFactor = 0.0f, float jitter = 1.0f, float interpolate = 0.0f, float seed = randomNumber);
    float fbmGridt(vec2 pos, vec2 scale = vec2(2.0f), int octaves = 1, float shift = 0.0f, float timeShift = 0.0f, float gain = 0.5f, float lacunarity = 8, vec3 translate = vec3(0.5, -0.25, 0.15), float warpStrength = 0.5f, float octaveFactor = 0.0f, float seed = randomNumber);
    float fbmGrid(vec2 pos, vec2 scale = vec2(2.0f), int octaves = 1, float shift = 0.0f, float timeShift = 0.0f, float gain = 0.5f, float lacunarity = 8, float warpStrength = 0.5f, float octaveFactor = 0.0f, float seed = randomNumber);
    float fbmMetaballs(vec2 pos, vec2 scale = vec2(2.0f), int octaves = 1, float shift = 0.0f, float timeShift = 0.0f, float gain = 0.5f, float lacunarity = 8, float octaveFactor = 0.0f, float jitter = 1.0f, float interpolate = 0.0f, vec2 width = 1.0f, float seed = randomNumber);
    vec4 fbmMulti(vec2 pos, vec2 scale = vec2(2.0f), float lacunarity = 8, int octaves = 1, float phase = 0.0f, float seed = randomNumber);
}}
