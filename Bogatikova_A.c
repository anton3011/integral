#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define MAXIT 100000

double f2(double u, double x);
double integral2(double (*f)(double, double), double a, double b, double e);
double f1(double x);
double urav(double *x, double (*f)(double), double a, double b, double e);

double integral2(double (*f)(double, double), double a, double b, double e)
{
    double fa, fb, fc, h, c, I1, I2 = 0, r;
    int i, j = 0, n = 4;
    fa = (*f)(a, b);
    fb = (*f)(b, b);
    h = (b - a) / 2;
    c = a + h;
    fc = (*f)(c, b);
    I1 = h * fa /2 + h * fc + h * fb / 2; 
    for(i = 0; i < 25; i++)
    {
        c = a + h/2;
        I2 = 0;
        for(j = 0;j < n/2; j++)
        {
            fc = (*f)(c, b);
            I2 = fc * h / 2 + I2;
            c = c + h;
        }
        I2 = I2 + I1/2;
        if(fabs(I2 - I1) < e)
        {
            r = I2;
            return r;
        }
        I1 = I2;
        n = n * 2;
        h = h / 2; 
    }
    return 0;
}

double f1(double x)
{
    double y = 0;
    double (*f)(double, double);
    f = f2;
    y = integral2(f, 0 , x, 1e-11);
    return y - 0.1 - x * x;
}

double f2(double u, double x)
{
    double y = 0;
    y = sqrt(u * x * x / (1. + x * x) + 1. / (1. + u * u * u * u));
    return y;
}

double urav(double *x, double (*f)(double), double a, double b, double e)
{
    double fa, fb, c, fc;
    int i;
    fa = (*f)(a);
    fb = (*f)(b);
    if(fabs(fa) < e)
    {
        *x = a;
        return 0;
    }
    if(fabs(fb) < e)
    {
        *x = b;
        return 0;
    }
    if(((fa > 0) && (fb > 0)) || ((fa < 0) && (fb < 0)))
        return -1;
    for(i = 0; i < MAXIT; i++)
    {
        if(fabs(b - a) < e)
        {
            *x = a;
            return i;
        }
        c = (a + b) * 0.5;
        fc = (*f)(c);
        if(fabs(fc) < e)
        {
            *x = c;
            return i;
        }
        if((a >= c) && (a <= c))
        {
            *x = c;
            return i;
        }
        if((b >= c) && (b <= c))
        {
            *x = c;
            return i;
        }
        if(fb * fc < 0)
        {
            a = c;
            fa = fc;
        }
        else
        {
            b = c;
            fb = fc;
        }
    }
    return -1;
}

int main(int argc, char *argv[])
{
    double (*f)(double);
    int it = 0;
    double x1 = 0, x2 = 0, a, b, e, fx1 = 0, fx2 = 0;
    if(!((argc == 2) && (sscanf(argv[1], "%lf", &e) == 1)))
    {
        printf("Usage: %s e\n",argv[0]);
        return 1;
    }
    f = f1;
    a = 0.0;
    b = 0.5;
    it = urav(&x1, f, a, b, e);
    if(it < 0)
    {
        printf("Решение не найдено\n");
        return 2;
    }
    fx1 = (*f)(x1);
    printf ("%s : X = %e Res = %e Its = %d\n",argv[0], x1, fx1, it);
    a = 0.5;
    b = 1.5;
    it = 0;
    it = urav(&x2, f, a, b, e);
    if(it < 0)
    {
        printf("Решение не найдено\n");
        return 2;
    }
    fx2 = (*f)(x2);
    printf ("%s : X = %e Res = %e Its = %d\n",argv[0], x2, fx2, it);
    return 0;
}