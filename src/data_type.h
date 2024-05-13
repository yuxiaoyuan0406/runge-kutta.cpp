#pragma once

#include "common.h"

template <typename SizeType = uint32_t, typename ValueType = double>
class array
{
private:
    SizeType size;
    ValueType *val;

public:
    array(SizeType size) : size(size), val(new ValueType[size]) {}
    ~array() { delete[] val; }
};