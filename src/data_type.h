#pragma once
#include "common.h"

template <typename SizeType = uint32_t, typename ValueType = double>
class array
{
private:
    SizeType size;
    ValueType *val;
public:
    /// @brief Initializer.
    /// @param size Size of the array.
    array(SizeType size) : size(size), val(new ValueType[size]) {}

    array() : size(0), val(nullptr) {}

    array(const array& other) : size(other.size), val(new ValueType[other.size])
    {
        std::copy(other.val, other.val + other.size, val);
    }

    /// @brief De-initializer.
    ~array() { delete[] val; }

    ValueType& operator[](SizeType index)
    {
        if (index >= this->size) {
            throw std::out_of_range("Index out of range");
        }
        return this->val[index];
    }
    
    const ValueType& operator[](SizeType index) const
    {
        if (index >= this->size) {
            throw std::out_of_range("Index out of range");
        }
        return this->val[index];
    }

    array& operator=(const array& other)
    {
        if (this != &other) {
            ValueType* new_val = new ValueType[other.size];
            std::copy(other.val, other.val + other.size, new_val);

            delete[] val;
            size = other.size;
            val = new_val;
        }
        return *this;
    }

    // array operator+(const array& a1, const array& a2)
    // {
    //     if (a1.size != a2.size) {
    //         throw std::length_error("Array must be of the same length");
    //     }

    //     array<SizeType, ValueType> result(a1.size);
    //     for (SizeType i = 0; i < a1.size; i++)
    //     {
    //         result[i] = a1[i] + a2[i];
    //     }
    //     return result;
    // }


    SizeType save(FILE *f);
    
    template <typename S, typename V>
    friend void f(array<S, V> & arr, S);
};

template <typename SizeType, typename ValueType>
array<SizeType, ValueType> operator+(const array<SizeType, ValueType>& a1, const array<SizeType, ValueType>& a2)
{
    if (a1.size != a2.size) {
        throw std::length_error("Array must be of the same length");
    }

    array<SizeType, ValueType> result(a1.size);
    for (SizeType i = 0; i < a1.size; i++)
    {
        result[i] = a1[i] + a2[i];
    }
    return result;
}


// template <typename SizeType = uint32_t, typename ValueType = double>
// class vector
// {
// private:
//     SizeType size;
//     array<SizeType, ValueType> *member;
// public:
//     vector() : size(0), member(nullptr) {}
//     vector(SizeType size) : size(size), member(new array<SizeType, ValueType>[size]) {}

//     ~vector() { delete[] member; }
// };

template <typename SizeType, typename ValueType>
inline SizeType array<SizeType, ValueType>::save(FILE *f)
{
    if (f == nullptr)
        return 0;
    
    SizeType i;

    for (i = 0; i < this->size; i++)
    {
        fprintf(f, "%e\n", this->val[i]);
    }
    
    return i;
}
