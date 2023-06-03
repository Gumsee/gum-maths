#pragma once
#include "vec.h"

template<typename T, int S>
struct tbbox
{
    tvec<T, S> pos, size;
    tbbox()
    {
        this->pos = tvec<T, S>(0);
        this->size = tvec<T, S>(0);
    }

    tbbox(tvec<T, S> pos, tvec<T, S> size)
    {
        this->pos = pos;
        this->size = size;
    }

    tvec<T, S> getPos()  { return pos; }
    tvec<T, S> getSize() { return size; }
    tvec<T, S> getDimensions() 
    { 
        return tvec<T, S>::abs(pos) + tvec<T, S>::abs(size); 
    }


    template<typename TT, int SS>
    void operator=(const tbbox<TT, SS>& other)
    {
        this->pos = other.pos;
        this->size = other.size;
    }
};

typedef tbbox<float, 2> bbox2;
typedef tbbox<int,   2> bbox2i;
typedef tbbox<float, 3> bbox3;
typedef tbbox<int,   3> bbox3i;