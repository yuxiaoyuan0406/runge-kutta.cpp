#include "data_type.h"
#include "cmath"

real my_func(real x)
{
    return sin(2 * M_PIf64 * 200 * x);
}

int main() {
    array<array<real>> v(3);

    // f(v[0], 10000);
    v[0] = array<real>::linear(0., 1/(128. * 1000), 20000);

    v[1] = array<real>::generate(v[0], std::function<real(real)>(my_func));
    // v[1] = v[0] + v[0];
    // v[2] = array<real>::generate(v[0], std::function<real(real)>(my_func));
    v[2] = v[1] * v[1];

    // for (size_t i = 0; i < 100; i++)
    // {
    //     printf("%ld\t%e\t%e\t%e\n", i, v[0][i], v[1][i], v[2][i]);
    // }

    FILE* f = fopen("data.txt","w");
    v[2].save(f);
    fclose(f);
    f = fopen("time.txt", "w");
    v[0].save(f);
    fclose(f);

    return 0;
}
