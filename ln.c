#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include "utils.h"


int main(int argc, char ** argv){

      int forme = 0;
      int start = -1;
      char dir_target_directory[10000];
      memset(dir_target_directory, 0, 10000);
      char * dossier;

      for(int i = 1; i < argc; i++){
	char * temp = argv[i];
	if(temp[0] == '-'){
          ssize_t tmp_len = strlen(temp);
          if(tmp_len < 1) continue;
          
          if(temp[1] == '-'){
            if(strcmp(temp, "--no-target-directory")==0){
              forme = 1;
            }
            
            char tmp[tmp_len + 1];
            memset(tmp, 0, tmp_len + 1);
            if(tmp_len > strlen("--target-directory=x")){
              
              memmove(tmp, temp, strlen("--target-directory"));

              memmove(dir_target_directory,
                      temp +strlen("--target-directory") +1,
                       strlen(temp +strlen("--target-directory") +1));
            }

            if(strcmp(tmp, "--target-directory")==0){
              forme = 4;
            } else {
              memset(dir_target_directory, 0, 10000);
            }

            continue;
          }

	  for(int j = 1; j < strlen(temp); j++){
	    
	    if(temp[j] == 'T'){
	      forme = 1;
	    
	      goto SWITCHTIME;
	    }
	    if(temp[j] == 't'){
	      forme = 4;
	    }
	  }
	}
	else {
	  if(start == -1) start = i;
	}
      }
      
      if(forme == 0){//soit 1, soit 2, soit 3
	if(argc - start > 1){
	  if(argc - start == 2){//peut etre aue c'est 1
	    add_name(argv[argc - 1]);
	  }
	  forme = 3;
	} else {
	  forme = 2;
	}
      }

    SWITCHTIME:
      
      switch(forme){
      case 1:
	add_name(argv[argc - 1]); break;
      case 2:
	add_name(get_name(argv[argc - 1])); break;
      case 4:
        dossier = argv[start];
        if(dir_target_directory[0] != 0){
          dossier = dir_target_directory;
          start--;
        }
	for(int i = start + 1; i < argc; i++){
	  add_name_from_rep(argv[i], dossier);
	}
	break;
      case 3:
	for(int i = start; i < argc - 1; i++){
	  add_name_from_rep(argv[i], argv[argc - 1]);
	}
	break;
      
      default: break;
      }


  execvp("/bin/ln", argv);
  perror("execl");
  return 1;
}
