#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/xattr.h>
#include <errno.h>
#include "utils.h"


void formatError(){
  printf("Usage: marquertag [name] [tag]\n");
  exit(1);
}

int main(int argc, char ** argv){

  int debug = 0;

  if(argc < 3)
    formatError();

  int rc,i;
  int taille;
  
#define LEN 10000
  char tagname[LEN];

  int message_pas_afficher = 1;

  for(i=2;i< argc ;i++){
    memset(tagname, 0, LEN * sizeof(char));//qui est 1
    strcat(tagname, "user.");
    strcat(tagname, argv[i]);

    if(debug) printf("%s\n", tagname);

    taille = strlen(tagname);
    if(taille > 10 && tagname[5] == 'p' && tagname[6] == 'a'
       && tagname[7] == 's' && tagname[8] == '('
       && tagname[taille -1] == ')'){
      if(message_pas_afficher){
        printf("format [pas(][...][)] non accepter\n");
        message_pas_afficher = 0;
      }
      continue;
    }
    rc = setxattr(argv[1], tagname, NULL, 0, 0);
    
    if(rc == -1){
      perror("setxattr");
      if(debug) printf("%d %d %d %d %d\n", errno, EEXIST, ENOSPC, EDQUOT, ENOTSUP);
      exit(1);
    }/*
    char res[LEN];
    memset(res, 0, LEN);
    path_from_root(argv[1], res);
    add_file_name(res);*/
  }
  return 0;
}
