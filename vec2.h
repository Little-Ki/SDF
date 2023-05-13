#pragma once
#include <math.h>

struct vec2
{
    float x, y;

    vec2() :x (0) , y(0) {}

    vec2(float x, float y) : x(x), y(y) {}
    
    vec2 operator+(const vec2 &o)
    {
        return {
            this->x + o.x,
            this->y + o.y};
    }

    vec2 operator-(const vec2 &o)
    {
        return {
            this->x - o.x,
            this->y - o.y};
    }

    float operator*(const vec2 &o)
    {
        return this->x * o.x + this->y * o.y;
    }

    vec2 operator*(const float &l)
    {
        return {
            this->x * l,
            this->y * l};
    }

    vec2 operator/(const float &l)
    {
        return {
            this->x / l,
            this->y / l};
    }

    void operator+=(const vec2 &o)
    {
        this->x += o.x;
        this->y += o.y;
    }
    
    void operator-=(const vec2 &o)
    {
        this->x -= o.x;
        this->y -= o.y;
    }

    
    void operator*=(const float &o)
    {
        this->x *= o;
        this->y *= o;
    }

    
    void operator/=(const float &o)
    {
        this->x /= o;
        this->y /= o;
    }

    float length()
    {
        return sqrtf(x * x + y * y);
    }

    vec2 normalized()
    {
        auto l = length();
        return {
            this->x / l,
            this->y / l};
    }

    void normal()
    {
        auto l = length();
        this->x /= l;
        this->y /= l;
    }
};

float cross(const vec2 &l, const vec2& r);

float dot(const vec2 &l, const vec2& r);

vec2 operator+(const vec2 &l, const vec2& r);

vec2 operator-(const vec2 &l, const vec2& r);

vec2 operator*(const vec2 &l, const float& r);

vec2 operator*(const float &l, const vec2& r);

vec2 operator/(const vec2 &l, const float& r);
