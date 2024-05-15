#pragma once
#include "common.h"

template <typename ValueType = real>
class array
{
private:
    size_type size;
    ValueType *val;
    
public:
    /// @brief Initializer.
    /// @param size Size of the array.
    array(size_type size) : size(size), val(new ValueType[size]) {}

    /// @brief Default initializer.
    array() : size(0), val(nullptr) {}

    /// @brief Copy initializer.
    /// @param other
    array(const array& other) : size(other.size), val(new ValueType[other.size])
    {
        std::copy(other.val, other.val + other.size, val);
    }

    /// @brief De-initializer.
    ~array() { delete[] val; }

    /// @brief Reload of operator [], access member via array-like syntax.
    /// @param index The index accessing.
    /// @return The reference to obj.val[index].
    ValueType& operator[](size_type index)
    {
        if (index >= this->size) {
            throw std::out_of_range("Index out of range");
        }
        return this->val[index];
    }
    
    /// @brief Reload of operator [], access const member via array-like syntax.
    /// @param index The index accessing.
    /// @return The reference to obj.val[index].
    const ValueType& operator[](size_type index) const
    {
        if (index >= this->size) {
            throw std::out_of_range("Index out of range");
        }
        return this->val[index];
    }

    /// @brief Reload of operator =.
    /// @param other The other array.
    /// @return Return reference to this object.
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

    /// @brief Save this object to file.
    /// @param f File handler.
    /// @return The count of value saved.
    size_type save(FILE *f);

#define DEFINE_ARRAY_OPERATOR(op)                                        \
    template <typename V>                                                \
    friend array<V> operator op(const array<V> &a1, const array<V> &a2); \
    template <typename V>                                                \
    friend array<V> operator op(const array<V> &a, const V & b);         \
    template <typename V>                                                \
    friend array<V> operator op(const V & b, const array<V> &a);

    DEFINE_ARRAY_OPERATOR(+)
    DEFINE_ARRAY_OPERATOR(-)
    DEFINE_ARRAY_OPERATOR(*)
    DEFINE_ARRAY_OPERATOR(/)
#undef DEFINE_ARRAY_OPERATOR

    template <typename V1, typename V2>
    static array<V1> linear(
        const V1& start_val, 
        const V2& step, 
        const size_type& step_count
    ) {
        array<V1> result(step_count);

        for (size_type i = 0; i < step_count; i++)
            result[i] = start_val + step * i;
        return result;
    }

    template <typename V1, typename V2>
    static array<V2> generate(
        const array<V1>& x, 
        std::function<V2(V1)> f
    ) {
        array<V2> result(x.size);
        for (size_type i = 0; i < x.size; i++)
            result[i] = f(x[i]);
        return result;
    }
};

#define DEFINE_ARRAY_OPERATOR_IMPL(op)                                   \
    template <typename V>                                                \
    array<V> operator op(const array<V> &a1, const array<V> &a2)         \
    {                                                                    \
        if (a1.size != a2.size)                                          \
            throw std::length_error("Array must be of the same length"); \
        array<V> result(a1.size);                                        \
        for (size_type i = 0; i < a1.size; i++)                          \
            result[i] = a1[i] op a2[i];                                  \
        return result;                                                   \
    }                                                                    \
    template <typename V>                                                \
    inline array<V> operator op(const array<V> &a, const V & b)          \
    {                                                                    \
        array<V> result(a.size);                                         \
        for (size_type i = 0; i < a.size; i++)                           \
            result[i] = a[i] op b;                                       \
        return result;                                                   \
    }                                                                    \
    template <typename V>                                                \
    inline array<V> operator op(const V & b, const array<V> &a)          \
    {                                                                    \
        array<V> result(a.size);                                         \
        for (size_type i = 0; i < a.size; i++)                           \
            result[i] = b op a[i];                                       \
        return result;                                                   \
    }

DEFINE_ARRAY_OPERATOR_IMPL(+)
DEFINE_ARRAY_OPERATOR_IMPL(-)
DEFINE_ARRAY_OPERATOR_IMPL(*)
DEFINE_ARRAY_OPERATOR_IMPL(/)
#undef DEFINE_ARRAY_OPERATOR_IMPL

template <typename ValueType>
inline size_type array<ValueType>::save(FILE *f)
{
    if (f == nullptr)
        return 0;
    
    size_type i;

    for (i = 0; i < this->size; i++)
    {
        fprintf(f, "%e\n", this->val[i]);
    }
   
    return i;

}
