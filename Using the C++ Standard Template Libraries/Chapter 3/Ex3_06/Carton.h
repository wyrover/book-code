// Carton.h
#ifndef CARTON_H
#define CARTON_H
#include "Box.h"

class  Carton : public Box
{
public:
    explicit Carton(size_t l = 1, size_t w = 1, size_t h = 1) : Box {l, w, h} {}
    double volume() const override
    {
        return 0.85 * Box::volume();
    }
};
#endif