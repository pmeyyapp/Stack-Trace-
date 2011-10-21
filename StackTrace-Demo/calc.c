#include <stdlib.h> /* atoi() */
#include <stdio.h>  /* (f)printf */
FILE *stacktrace_fp;
int stacktrace_foo()
{
int dummy;
return &dummy;
}
#include <assert.h> /* assert() */

int add(int x, int y)
 {
int sai_a;
    int carry = 0;
    int result = 0;
    int i;

    for(i = 0; i < 32; ++i) 
   {
        int a = (x >> i) & 1;
        int b = (y >> i) & 1;
        result |= ((a ^ b) ^ carry) << i;
        carry = (a & b) | (b & carry) | (carry & a);
    }

int sai_b=(int)stacktrace_foo();
fprintf(stacktrace_fp,"add-%d\n",(int)&sai_a-sai_b);
    return result;
}

int negate(int x)
{
int sai_a;
int sai_b=(int)stacktrace_foo();
fprintf(stacktrace_fp,"negate-%d\n",(int)&sai_a-sai_b);
    return add(~x, 1);
}

int subtract(int x, int y) 
{
int sai_a;
int sai_b=(int)stacktrace_foo();
fprintf(stacktrace_fp,"subtract-%d\n",(int)&sai_a-sai_b);
    return add(x, negate(y));
}

int is_even(int n) 
{
int sai_a;
int sai_b=(int)stacktrace_foo();
fprintf(stacktrace_fp,"is_even-%d\n",(int)&sai_a-sai_b);
    return !(n & 1);
}

int divide_by_two(int n) 
{
int sai_a;
int sai_b=(int)stacktrace_foo();
fprintf(stacktrace_fp,"divide_by_two-%d\n",(int)&sai_a-sai_b);
    return n >> 1;
}

int multiply_by_two(int n) 
{
int sai_a;
int sai_b=(int)stacktrace_foo();
fprintf(stacktrace_fp,"multiply_by_two-%d\n",(int)&sai_a-sai_b);
    return n << 1;
}

int multiply(int x, int y) 
{
int sai_a;
    int result = 0;

    if(x < 0 && y < 0)
 {
int sai_b=(int)stacktrace_foo();
fprintf(stacktrace_fp,"multiply-%d\n",(int)&sai_a-sai_b);
        return multiply(negate(x), negate(y));
    }

    if(x >= 0 && y < 0) 
{
        return multiply(y, x);
    }

    while(y > 0) 
{
        if(is_even(y)) 
{
int sai_a;
                x = multiply_by_two(x);
                y = divide_by_two(y);
int sai_b=(int)stacktrace_foo();
fprintf(stacktrace_fp,"is_even-%d\n",(int)&sai_a-sai_b);
        } 
else 
{
                result = add(result, x);
                y = add(y, -1);
        }
    }

    return result;
}

int main(int argc, char **argv) 
{
stacktrace_fp=fopen("stacktrace_tmp.txt","w+");
int sai_a;
    int from = -100, to = 100;
    int i, j;

    for(i = from; i <= to; ++i) 
{
        assert(0 - i == negate(i));
        assert(((i % 2) == 0) == is_even(i));
        assert(i * 2 == multiply_by_two(i));
        if(is_even(i)) 
{
int sai_a;
                assert(i / 2 == divide_by_two(i));
int sai_b=(int)stacktrace_foo();
fprintf(stacktrace_fp,"is_even-%d\n",(int)&sai_a-sai_b);
        }
    }

    for(i = from; i <= to; ++i) 
{
        for(j = from; j <= to; ++j) 
{
                assert(i + j == add(i, j));
                assert(i - j == subtract(i, j));
                assert(i * j == multiply(i, j));
        }
    }

int sai_b=(int)stacktrace_foo();
fprintf(stacktrace_fp,"main-%d\n",(int)&sai_a-sai_b);
fclose(stacktrace_fp);
    return 0;
}

