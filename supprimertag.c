#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/xattr.h>
#include <errno.h>


#define LEN 1000

void formatError(){
  printf("Usage : supprimertag [name] [tag]\n");
  exit(1);
}

int main(int argc , char ** argv){
  int debug = 0;//afficher le mode debug si il y a un probleme
  if (argc < 3)
    formatError();
  
  int rc,i;
  char * path = argv[1];
  char tagname[LEN];//pas de malloc
  for(i=2; i< argc; i++){
    memset(tagname, 0,LEN * sizeof(char));

    strcat(tagname, "user.");
    strcat(tagname, argv[i]);
    
    if(debug) printf("%s\n", tagname);

    rc = removexattr(path,tagname);
    // Erreur possibles d'apres le manuel
    if(rc == -1){
      
      perror("setxattr");
      if(debug) printf("%d %d %d %d %d\n", errno, EEXIST, ENOSPC, EDQUOT, ENOTSUP);
      
      if(errno == ENODATA){
	printf("The named attribute does not exist\n");
      }
      if(errno == ENOTSUP){
	printf("Extended attributes are not supported by the filesystem, or are disabled\n");
      }
      
      //peut etre d'autre erreur à signaler
      
      exit(1);
    }
  }
  return 0;
}
