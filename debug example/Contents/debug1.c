/*** Use GDB to Debug this program ***/

#include <stdio.h>

void main(void)
{ int i, j;
   
  j = 54321;
  for (i= 10; i>=0; i--)
  { j = j/i;
    printf("j = %d\n", j);
  }   
}
