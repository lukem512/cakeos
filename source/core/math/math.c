/* CakeOS */

int abs(int x)
{
int t = x >> 31; 
// t is -1 if x is negative otherwise it is 0

return t ^ (x + t);
}

double sqrt (double x)
{
  double res;
  __asm__ __volatile__ ("fsqrt" : "=t" (res) : "0" (x));
  return res;
}
