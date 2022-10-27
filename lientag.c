#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/xattr.h>
#include <errno.h>
#include "utils.h"


int main(int argc, char ** argv){
  
  if(argc != 3){
    printf("Usage: lientag [pere] [fils]\n");
    exit(1);
  }

  add_lien_liens(argv[1], argv[2]);

  return 0;
}
