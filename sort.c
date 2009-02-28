#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>

extern char **roms;

int list_num;

static int compare (void const *a, void const *b)
{

   char const *const *pa = a;
   char const *const *pb = b;

   return strcmp (*pa, *pb);
}

int sort (void)
{
   qsort (roms, (sizeof roms)*list_num / sizeof *roms, sizeof *roms, compare);

   return 0;
}


