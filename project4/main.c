#include <stdio.h>
#include <stdlib.h>
#include "p4machine.h"

struct point {
  double x;
  double y;
};

int main()
{
  // a few test operations...
  p4heap *h = p4heap_create();
  printf("p4heap_num_allocs %d\n", p4heap_num_allocs(h));
  struct point *p = p4malloc(h,sizeof(struct point),0);
  printf("p4heap_num_allocs %d\n", p4heap_num_allocs(h));
  p->x = 1;
  p->y = 2;
  printf("p %p\n",p);
  printf("p->x %f\n",p->x);
  printf("p->y %f\n",p->y);
  p4free(h,p);
  printf("p4heap_num_allocs %d\n", p4heap_num_allocs(h));
  free(h);
  printf("done\n");
  return 0;
}
