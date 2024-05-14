#include "data_type.h"
#include "cmath"

void f(array<uint32_t, real> & arr, uint32_t size) 
{
    array tmp(size);

    for (uint32_t i = 0; i < size; i++)
        tmp[i] = i * 1e-6;
    
    arr = tmp;
}

int main() {
    array<uint32_t, array<uint32_t, real>> v(2);

    f(v[0], 100);

    for (size_t i = 0; i < 100; i++)
    {
        printf("%ld, %e\n", i, v[0][i]);
    }
    

    return 0;
}
