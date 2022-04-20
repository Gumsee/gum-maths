#include "ColorFunctions.h"
#include "Maths.h"
#include <iostream>

namespace Gum {
namespace Maths
{
    vec3 HSVToRGB(vec3 hsv)
    {
        float h = hsv.x;
        float s = hsv.y / 100.0f;
        float v = hsv.z / 100.0f;

        if(s <= 0.0f)  { return vec3(v); }
        if(h >= 360.0f) { h = 0.0f; };
        h /= 60.0f;
        float ff = h - (int)h;
        float p = v * (1.0f - s);
        float q = v * (1.0f - (s * ff));
        float t = v * (1.0f - (s * (1.0f - ff)));

        switch((int)h) 
        { 
            case 0:  return vec3(v, t, p);
            case 1:  return vec3(q, v, p);
            case 2:  return vec3(p, v, t);
            case 3:  return vec3(p, q, v);
            case 4:  return vec3(t, p, v);
            default: return vec3(v, p, q);
        }
    }

    vec3 RGBToHSV(vec3 rgb)
    {
        rgb = rgb*255.0f;
        float hue, saturation, brightness;
        float cmax = std::max(std::max(rgb.x, rgb.y), rgb.z);
        float cmin = std::min(std::min(rgb.x, rgb.y), rgb.z);

        brightness = cmax / 255.0f;
        if (cmax != 0)  { saturation = (cmax - cmin) / cmax; }
        else            { saturation = 0; }

        if (saturation == 0) { hue = 0; }
        else {
            float redc =    (cmax - rgb.x) / (cmax - cmin);
            float greenc =  (cmax - rgb.y) / (cmax - cmin);
            float bluec =   (cmax - rgb.z) / (cmax - cmin);
            if (rgb.x == cmax)      { hue = bluec - greenc; }
            else if (rgb.y == cmax) { hue = 2.0f + redc - bluec; }
            else                    { hue = 4.0f + greenc - redc; }
            hue = hue / 6.0f;
            if (hue < 0) { hue = hue + 1.0f; }
        }
        return vec3(hue, saturation, brightness);
    }
    vec3 RGBToHSB(vec3 rgb) { return RGBToHSV(rgb); }

    vec3 HEXToRGB(std::string hex)
    {
        int r, g, b;
        sscanf(hex.c_str(), "%02x%02x%02x", &r, &g, &b);
        return vec3(r,g,b);
    }
    vec3 HEXToRGB(int hex)
    {
        vec3 rgb;
        rgb.x = ((hex >> 16) & 0xFF) / 255.0;  // Extract the RR byte
        rgb.y = ((hex >> 8) & 0xFF) / 255.0;   // Extract the GG byte
        rgb.z = ((hex) & 0xFF) / 255.0;        // Extract the BB byte
        return rgb;
    }
    std::string RGBToHEX(vec3 rgb)
    {
        std::string ret;
        rgb = rgb * 255.0f;

        char r[255];	
        sprintf(r, "%.2X", (int)rgb.x);
        ret.append(r );

        char g[255];	
        sprintf(g, "%.2X", (int)rgb.y);
        ret.append(g );

        char b[255];	
        sprintf(b, "%.2X", (int)rgb.z);
        ret.append(b );
        
        return ret;
    }
}}