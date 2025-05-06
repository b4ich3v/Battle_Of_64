#pragma once

template<class T1 = int, class T2 = int>

struct Pair
{
public:

    T1 first;
    T2 second;

    Pair() = default;

    Pair(const T1& a, const T2& b);

    bool operator == (const Pair& other) const;

    bool operator != (const Pair& other) const;

    bool operator < (const Pair& other) const;

    bool operator <=(const Pair& other) const;

    bool operator > (const Pair& other) const;

    bool operator >= (const Pair& other) const;

};

template<class T1, class T2>

Pair<T1, T2>::Pair(const T1& a, const T2& b):
    first(a), second(b) {}

template<class T1, class T2>

bool Pair<T1, T2>::operator == (const Pair& other) const
{

    return first == other.first && second == other.second;

}

template<class T1, class T2>

bool Pair<T1, T2>::operator != (const Pair& other) const
{

    return !(*this == other);

}

template<class T1, class T2>

bool Pair<T1, T2>::operator < (const Pair& other) const
{

    if (first < other.first) return true;
    if (other.first < first) return false;

    return second < other.second;

}

template<class T1, class T2>

bool Pair<T1, T2>::operator <= (const Pair& other) const
{

    return !(other < *this);

}

template<class T1, class T2>

bool Pair<T1, T2>::operator > (const Pair& other) const
{

    return other < *this;

}

template<class T1, class T2>

bool Pair<T1, T2>::operator >= (const Pair& other) const
{

    return !(*this < other);

}
