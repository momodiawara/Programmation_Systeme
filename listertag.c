#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/xattr.h>
#include <errno.h>
#include "utils.h"

//int debut =  0;

void formatError(){
  printf("Usage: listertag [name]\n");
  exit(1);
}
void noTag(){
  printf("No tag\n");
  exit(0);
}

int main(int argc, char ** argv){

  //** traitement parametre
  if(argc != 2)
    formatError();
  
  char * name = argv[1];

  //** taille total des tags
  ssize_t len = get_tags_len(name);
  if(len == 0)
    noTag();
    
  //** avoir les tags
  char tags[len];
  memset(tags, 0, len);
 
  list_file_tags(name, tags, len);

  //** nb tag(s)
  int num = get_number_of_tags(tags, len);

  printf("tag");
  if(num == 1)
    printf(":\n");
  else
    printf("s:\n");

  //** traiter les tags 
  //(on peut afficher directement, c'est un exemple pour filtrertag)
#define LEN 10000

  char ** tab = create_charss(num, LEN);
  split_tags(tags, tab, num);//supprime user.

  //** afficher les tags
  for(int i = 0; i< num; i++){
    printf("%s ", tab[i]);//espace a la fin pas tres grave
  }
  printf("\n");
  
  free_charss(num, tab);
  
  return 0;
}
