#include "ColorFunctions.h"
#include "Maths.h"
#include <iostream>

namespace Gum {
namespace Maths
{
    rgb HSVToRGB(hsv val)
    {
        float h = val.h;
        float s = val.s / 100.0f;
        float v = val.v / 100.0f;

        if(h >= 360.0f)
            h -= 360.0f;
        
        h /= 60.0f;
        float ff = h - (int)h;
        float p = v * (1.0f - s);
        float q = v * (1.0f - (s * ff));
        float t = v * (1.0f - (s * (1.0f - ff)));

        switch((int)h) 
        { 
            case 0:  return rgb(v * 255.0f, t * 255.0f, p * 255.0f);
            case 1:  return rgb(q * 255.0f, v * 255.0f, p * 255.0f);
            case 2:  return rgb(p * 255.0f, v * 255.0f, t * 255.0f);
            case 3:  return rgb(p * 255.0f, q * 255.0f, v * 255.0f);
            case 4:  return rgb(t * 255.0f, p * 255.0f, v * 255.0f);
            default: return rgb(v * 255.0f, p * 255.0f, q * 255.0f);
        }
    }

    hsv RGBToHSV(rgb val)
    {
        float redc   = val.r / 255.0f;
        float greenc = val.g / 255.0f;
        float bluec  = val.b / 255.0f;

        float cmax = std::max(std::max(redc, greenc), bluec);
        float cmin = std::min(std::min(redc, greenc), bluec);
        float delta = cmax - cmin;

        float hue = 0;
        if      (cmax == redc)   { hue = delta ==  0.0f ? 0.0f : std::fmod((greenc - bluec)/delta, 6.0f); }
        else if (cmax == greenc) { hue = delta == -2.0f ? 0.0f : (bluec - redc)/delta + 2.0f; }
        else                     { hue = delta == -4.0f ? 0.0f : (redc - greenc)/delta + 4.0f; }
        hue *= 60;
        if(hue < 0)
            hue += 360;

        float saturation = (cmax == 0 ? 0 : delta / cmax) * 100;
        float brightness = cmax * 100.0f;
        return hsv(hue, saturation, brightness);
    }

    rgb HEXToRGB(std::string hex)
    {
        unsigned short r, g, b;
        sscanf(hex.c_str(), "#%2hx%2hx%2hx", &r, &g, &b);
        return rgb(r,g,b);
    }

    rgba HEXToRGBA(std::string hex)
    {
        unsigned short r, g, b, a;
        switch(hex.length())
        {
            case 4: sscanf(hex.c_str(), "#%2hx%2hx%2hx", &r, &g, &b); a = 255; break;
            case 5: sscanf(hex.c_str(), "#%2hx%2hx%2hx%2hx", &r, &g, &b, &a); break;
            case 7: sscanf(hex.c_str(), "#%2hx%2hx%2hx", &r, &g, &b); a = 255; break;
            case 9: sscanf(hex.c_str(), "#%2hx%2hx%2hx%2hx", &r, &g, &b, &a); break;
            default: std::cerr << "Failed to convert hex to rgba, given hex str: " << hex << std::endl;
        }

        return rgba(r,g,b,a);
    }

    rgba HEXToRGBA(int hex)
    {
        rgba ret;
        ret.r = (hex >> 16) & 0xFF;  // Extract the RR byte
        ret.g = (hex >> 8)  & 0xFF;  // Extract the GG byte
        ret.b = (hex)       & 0xFF;  // Extract the BB byte
        return ret;
    }

    std::string RGBToHEX(rgb val)
    {
        char r[4], g[4], b[4];
        sprintf(r, "%.2hX", (unsigned short)val.r);
        sprintf(g, "%.2hX", (unsigned short)val.g);
        sprintf(b, "%.2hX", (unsigned short)val.b);

        return std::string("#") + r + g + b + "FF";
    }

    std::string RGBAToHEX(rgba val)
    {
        char r[8], g[8], b[8], a[8];
        sprintf(r, "%.2hX", (unsigned short)val.r);
        sprintf(g, "%.2hX", (unsigned short)val.g);
        sprintf(b, "%.2hX", (unsigned short)val.b);
        sprintf(a, "%.2hX", (unsigned short)val.a);

        return std::string("#") + r + g + b + a;
    }

}}