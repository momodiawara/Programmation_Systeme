
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include <assert.h>
#include <string.h>

#include "utils.h"



//LES DEFINE *****

//ne pas afficher les fichiers et dossiers cachee
#define PAS_AFFICHER_CACHER 1

//afficher depuis home, racine sinon
#define FROM_HOME 1











// ****************** tmp_lister.c


struct memory2{
  ino_t * mem;
  ssize_t free;
  ssize_t len;  
};

void init_memory2(struct memory2 * mem2){
  #define MEM2_LEN 256
  mem2-> mem = malloc(MEM2_LEN * sizeof(ino_t));
  if(mem2-> mem == NULL){
    perror("malloc");
    exit(1);
  }
  mem2-> free = 0;
  mem2-> len = MEM2_LEN;
}

void add_memory2(struct memory2 * mem2, ino_t val){
  if(mem2-> len > mem2-> free){

    mem2-> mem[mem2-> free] = val;
    mem2-> free += 1;
    return;
  }
  
  mem2-> len *= 2;

  //le sizeof!
  mem2-> mem = realloc(mem2-> mem, mem2-> len * sizeof(ino_t));
  if(mem2-> mem == NULL){
    perror("realloc");
    exit(1);
  }
  add_memory2(mem2, val);
}

int is_in_memory2_or_add(struct memory2 * mem2, ino_t val){

  for(int i = 0; i< mem2->  free; i++){

    if(mem2-> mem[i] == val){
      return 1;
    }
  }
  add_memory2(mem2, val);

  return 0;
}





typedef struct type_tags{
  char * word;
  int dif;//diff mode
} type_tags, * Type_tags;





#define OK 1
#define NO 0
#define ERR -1

int same_family(Memory memory, char * pere, char * fils){
  if(strcmp(pere, fils) == 0) return 1;
  return est_pere_du_fils(memory, pere, fils);
}

//return 0 if no tag or wrong tag
//1 else
//-1 error
int op_function(Type_tags tags, int tags_len, Memory memory, char * name, struct memory2 * mem2){

  //verifier si deja vu

  struct stat buf;
  int rc;

  rc = stat(name, &buf);
  if(rc == -1){
    //perror("stat");
    return ERR;
  }

  
  
  //verifier tag

  //** total size
  ssize_t len = get_tags_len(name);
  if(len == -1){
    return ERR;
  }

  if(len == 0){
    //printf("no tag");
    return ERR;//no tag
  }

  if(is_in_memory2_or_add(mem2, buf.st_ino)){
    return NO;
  }

  //** get tags
  char curr_tags[len];
  memset(curr_tags, 0, len);
 
  rc = list_file_tags(name, curr_tags, len);
  if(rc == -1) return ERR;

  //** tags number
  int num = get_number_of_tags(curr_tags, len);


  //** tags
  #define LEN 10000

  char ** tab = create_charss(num, LEN);
  split_tags(curr_tags, tab, num);//supprime user.

  int is_bad;

  //for each type
  for(int j = 0; j < tags_len; j++){
    char * pere = tags[j].word;
    
    is_bad = 1;
    //** pour chaque tag
    for(int i = 0; i< num; i++){
      char * fils = tab[i];

      if(tags[j].dif){//diff mode
        is_bad = 0;//out == good
        //try for each

        if(same_family(memory, pere, fils)){//bad

          free_charss(num, tab); return NO;
        }
      } else if(same_family(memory, pere, fils)) {//normal

        is_bad = 0;
        break;
      }
    }
    if(is_bad){//bad
      free_charss(num, tab); return NO;
    }
  }

  free_charss(num, tab);
  return OK;
}




void make_type_tags(type_tags * t, int i, char * m){
  int d = 0;//TODO

  int size = strlen(m);

  //less than 5: not: pas()
  if(size > 5 && m[0] == 'p' && m[1] == 'a' && m[2] == 's' 
              && m[3] == '(' && m[size -1] == ')'){

    d = 1;
    m[size -1] = 0;//last
    m = m + 4;//first
  }

  t[i].word = m;//TODO
  t[i].dif = d;
}



void print_tags(Type_tags tags, int len){
  for(int i = 0; i < len; i++){
    printf("%d %s\n", tags[i].dif, tags[i].word);
  }
}


// **************** fin tmp_lister.c





void chemin_absolu6(char *chemin_courant, int (*pf)(Type_tags tags, int tags_len, Memory memory, char * name, struct memory2 * mem2),
                     Type_tags tags, int tags_len, Memory memory, struct memory2 * mem2){
  //printf("%s\n", chemin_courant);

  //depuis racine erreur
  if(strcmp(chemin_courant, "/dev/") == 0) return;



  DIR *dir  = opendir(chemin_courant);
  ssize_t path_name_size = strlen(chemin_courant);

  if(dir == NULL){
    return;//on ignore: pas exit!
  }

  struct dirent *ent;
  struct stat buf;
  int ok;
  char *s;

  while( (ent = readdir(dir)) != NULL){
    s = ent->d_name;

    //les fichers cachees
    if(PAS_AFFICHER_CACHER && s[0] == '.') continue;



    strcat(chemin_courant,s);



    ok = stat(chemin_courant,&buf);

    if( ok != -1 && strcmp(s, ".") != 0 && strcmp(s, "..") != 0){
      
      if (S_ISDIR(buf.st_mode)){
	//répertoire
        strcat(chemin_courant,"/");
	chemin_absolu6(chemin_courant, pf, tags, tags_len, memory, mem2);

      } else if(S_ISREG(buf.st_mode) || S_ISLNK(buf.st_mode)){
	//fichier ordinaire || lien  symbolique
        if(pf(tags, tags_len, memory, chemin_courant, mem2) == OK)
          printf("%s\n", chemin_courant);
      }

    }
    chemin_courant[path_name_size] = 0;
  } 
  closedir(dir);
}

int main(int argc, char ** argv){
   char path[LEN];
   memset(path, 0, LEN);
   if(FROM_HOME){
     strcat(path, "/home/");
   } else {
     strcat(path, "/");
   }
   argc--; argv++;//delete useless


  type_tags tags[argc];
  for(int i = 0; i < argc; i++){
    make_type_tags(tags, i, argv[i]);
  }

  Memory memory = get_liens();
  init_pere_du_fils_comparaison(memory);

  struct memory2 mem2;
  init_memory2(&mem2);

  chemin_absolu6(path ,op_function, tags, argc, memory, &mem2);
}

