#ifndef utility_hpp_INCLUDED
#define utility_hpp_INCLUDED

#include <SFML/Graphics.hpp>
#include <cmath>

template <typename T>
inline float length(const sf::Vector2<T> vec)
{
    return std::sqrt(vec.x*vec.x + vec.y*vec.y);
}

template <typename T>
inline float dot(const sf::Vector2<T> vec1, const sf::Vector2<T> vec2)
{
    return vec1.x*vec2.x + vec1.y*vec2.y;
}

template <typename T>
inline void clamp(T& val, const T minVal, const T maxVal)
{
    val = std::min(std::max(val, minVal), maxVal);
}

#endif // utility_hpp_INCLUDED

