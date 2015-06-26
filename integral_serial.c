#include <stdlib.h>
#include <stdio.h>
#include <math.h>

double f(double);
double integrate(double ,double, int);

int main(int argc, char *argv[])
{
  int N = 1000000;
  double x0 = 0;
  double x1 = 1;
  double dx = (x1 - x0) / N;
  double integral;

  integral = integrate(x0, dx, N);
  printf("value of the integral is %10.15f\n", integral);

  return 0;
}

double f(double x)
{
  return sin(x) * cos(x * 4.0) * sqrt(x * x + 1) / (2 * x * x + 1);
}

double integrate(double x0, double dx, int n_iter)
{
  double integral, x;
  integral = 0;

  for (int i = 0; i < n_iter; i++)
  {
    x = x0 + dx * i;
    integral += f(x) * dx;
  }

  return integral;
}

