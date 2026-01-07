int temp_func(int *a, int *b, int *c, int *d)
{
    int i = a[0] - b[0];
    int z = i + c[0];

    d[0] = z;
    
    return z;
}