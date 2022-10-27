#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/xattr.h>
#include <sys/stat.h>
#include <unistd.h>
#include <assert.h>
#include <errno.h>
#include <fcntl.h>
#include <time.h>
#include <sys/wait.h>
#include <sys/select.h>
#include <dirent.h>


#define LIENS_FILE "liens.txt"

int debug = 0;

#define TAG_LEN 1000

//ouverture d'un fichier 
FILE * open_file(char * name, char * mode){
  FILE * res = fopen(name, mode);
  if(res == NULL){
    perror("fopen");
    exit(1);
  }
  return res;
}

//ouvertur
FILE * open_file_read(char * name){
  return open_file(name, "r");
}
//savoir si un inode est dans le rep courant
int isPresent(int inode){
  
  DIR *d = opendir(".");
  if(d == NULL){
    perror("ouverture");
    exit(1);
  }
  
  struct dirent * ent;
  while((ent = readdir(d))){
    if(ent->d_ino == inode){
      closedir(d);
      return 1;
    }
  }
  closedir(d);
  return 0;
}


#define LIENS_FILE "liens.txt"


//ajoute un lien dans le fichier [liens.txt]
void add_lien_liens(char * pere, char * fils){
  FILE * f = open_file(LIENS_FILE, "a+");
  fprintf(f, "%s %s\n", pere, fils);
  fclose(f);
}

void add_file_name(char * s){
#define LEN2 4096
  //TODO: names.txt n'existe pas?
  //printf("ADD\n");
  FILE * f2 = fopen("names.txt", "r+");
  
  if(f2 == NULL){
    perror("fopen");
    return;
  }

  fseek(f2, 0, SEEK_SET);

  char p[LEN2];
  memset(p, 0, LEN2);

  while(fscanf(f2, "%s\n", p) > 0){
    if(p[0] == '\0'){
      fseek(f2, -2, SEEK_CUR);//0 et \n
      fprintf(f2, "%s\n%c\n", s, 0);
      fclose(f2);
      return;
    }
    if(strcmp(p, s) == 0){
      fclose(f2);
      return;
    }
    memset(p, 0, LEN2);
  }
  //printf("ici\n");
  fprintf(f2, "%s\n%c\n", s, 0);
  fclose(f2);
}

int est_racine(){
  struct stat statbuf;
  int rc = lstat("./", &statbuf);//courant
  if(rc == -1){
    perror("stat");
    exit(1);
  }

  struct stat statbuf2;
  rc = lstat("../", &statbuf2);//courant
  if(rc == -1){
    perror("stat");
    exit(1);
  }

  return statbuf.st_ino == statbuf2.st_ino;
  
}

char* nom_du_repertoire_courant(){
  if(est_racine()){
    int LEN = 512;
    char * res = malloc(LEN);
    memset(res, 0, LEN);
    return res;
  }

  struct stat statbuf;
  int rc = lstat("./", &statbuf);//courant
  if(rc == -1){
    perror("stat");
    exit(1);
  }

  DIR * d = opendir ("../");
  if(d == NULL){
    perror("opendir");
    exit(1);
  }

  char * name;
  struct dirent *r = readdir(d);
  while(r != NULL){
    if(r-> d_ino == statbuf.st_ino){
      name = r-> d_name;
      break;
    }
    r = readdir(d);
  }
  
  chdir("../");
  char * res = nom_du_repertoire_courant();
  char * racine = "/";
  strcat(res, racine);
  strcat(res, name);
  closedir(d);
  chdir(name);
  return res;
}

void path_from_root(char * file, char * res){
  
  char * name;

  if(file[0] == '/'){
    strcat(res, file);
  } else {
    name = nom_du_repertoire_courant();
    if(name == NULL) return;
  }

  
  strcat(res, name);
  strcat(res, "/");
  strcat(res, file);

  free(name);

}


//STRUCT

typedef struct memory{
  char ** peres;
  char ** fils;
  ssize_t size;
  ssize_t open;
  int * pere_du_fils;//ne pas initialiser: faire init_pere_du_fils_comparaison()
}memory, *Memory;

#define MEMORY_SIZE 256
Memory init_memory(){
  Memory m = malloc(sizeof(memory));

  char ** node_id = malloc(MEMORY_SIZE);
  char ** data = malloc(MEMORY_SIZE);

  //si on ajoute directement dans [m], il faut tester son [malloc] avant (mais pas le cas ici)
  if(m == NULL || node_id == NULL 
     || data == NULL){
    perror("malloc");//on exit() autant afficher
    exit(1);//RIEN A FAIRE: FIN
  }
  
  memset(node_id, 0, MEMORY_SIZE);
  memset(data, 0, MEMORY_SIZE);

  m-> peres = node_id;
  m-> fils = data;
  m-> size = MEMORY_SIZE;
  m-> open = 0;
  return m;
}


void memory_put_pere_at(Memory m, char * pere, ssize_t i){
  m-> peres[i] = calloc(1, strlen(pere) + 1);
  if(m-> peres[i] == NULL){
    perror("calloc");
    exit(1);
  }
  memmove(m-> peres[i], pere, strlen(pere));
}

void memory_put_fils_at(Memory m, char * fils, ssize_t i){
  m-> fils[i] = calloc(1, strlen(fils) + 1);
  if(m-> fils[i] == NULL){
    perror("calloc");
    exit(1);
  }
  memmove(m-> fils[i], fils, strlen(fils));
}


char * pere_at(char ** list_peres, ssize_t i){
  return list_peres[i];
}

char * fils_at(char ** list_fils, ssize_t i){
  return list_fils[i];
}


//ajouter comme nouvelle valeur
void add_new_value_memory(Memory m, char * pere, char * fils){
  
  //cas ok
  if(m-> open < m-> size){

    memory_put_pere_at(m, pere, m-> open);

    memory_put_fils_at(m, fils, m-> open);
    
    m-> open += 1;//A FAIRE A LA FIN: DES CALCULS SONT FAIT AVEC!
    return;
  }

  m-> size = m-> size * 2;//changement de la taille

  //sinon depassement
  m-> peres = realloc(m-> peres, m-> size);
  m-> fils = realloc(m-> fils, m-> size);

  if(m-> peres == NULL || m-> fils == NULL){
    perror("realloc");//exit(): autant afficher
    exit(1);//RIEN A FAIRE: FIN
  }

  //et recommence! (impossible de loop: assez de place, sauf erreur programmeur)
  add_new_value_memory(m, pere, fils);
}

Memory get_liens(){
  if(debug) printf("ici\n");
  
  FILE * file = open_file(LIENS_FILE, "r");
  Memory m = init_memory();
  
  int total = TAG_LEN + 1; 

  char p[total];
  char f[total];
  memset(p, 0, total);
  memset(f, 0, total);
  
  while(fscanf(file, "%s %s\n", p, f) > 0){
    add_new_value_memory(m, p, f);
    memset(p, 0, total);
    memset(f, 0, total);
  }

  return m;
}


void print_memory(Memory m){
  for(int i = 0; i < m-> open; i++){
    printf("%s %s\n", pere_at(m-> peres, i), fils_at(m-> fils, i));
  }
}


void init_pere_du_fils_comparaison(Memory m){
  m-> pere_du_fils = malloc(m->open * sizeof(int));

  //initialisation
  for(int i = 0; i < m-> open; i++){
    int pere_pas_un_fils = 1;
    for(int j = 0; j < m-> open; j++){
      if(strcmp(m-> fils[j], m-> peres[i]) == 0){
        pere_pas_un_fils = 0;
        m-> pere_du_fils[i] = j;
        break;
      }
    }
    if(pere_pas_un_fils) m-> pere_du_fils[i] = -1;
  }
}





//UTILISER APRES AVOIR FAIT: init_pere_du_fils_comparaison()
int est_pere_du_fils(Memory m, char * pere, char * fils){
  int fi = -1;
  for(int i = 0; i < m-> open; i++){//cherche le fils
    if(strcmp(m-> fils[i], fils) == 0){
      fi = i;
      break;
    }
  }

  if(fi == -1) return 0;//n'est pas un fils

  int pe[m-> open];
  int pe_in = -1;
  for(int i = 0; i < m-> open; i++){//cherche le fils
    if(strcmp(m-> peres[i], pere) == 0){
      pe[i] = 1;

      pe_in = 1;
    } else {
      pe[i] = 0;
    }
  }

  if(pe_in == -1) return 0;//n'est pas un pere


  for(int i = 0; i < m-> open; i++){//boucle total possible, plus signifie cycle

    if(m-> pere_du_fils[fi] == -1){//le pere n'est pas un fils
      return pe[fi];
    }

    if(pe[fi]){
      return 1;//on a trouve
    }

    //sinon voir le grand pere du pere
    fi = m-> pere_du_fils[fi];
  }

  return 0;//rien du tout
}



char ** create_charss(int size, int len){
  char ** res = malloc(sizeof(char*) * len);//pointeurs
  for(int i = 0; i< len; i++){
    res[i] = malloc(sizeof(char) * size);//pointeur
    memset(res[i], 0, size);
  }
  return res;
}

void free_charss(int len, char ** s){
  for(int i = 0; i < len; i++)
    free(s[i]);
  free(s);
}


//retourne la taille pour les tags
int get_tags_len(char * name){
  ssize_t rc = 0;
  rc = listxattr(name, NULL, 0);
  if(rc == -1){
    perror("getxattr"); 
    if(debug) printf("%d %d %d %d %d\n", errno, EEXIST, ENOSPC, EDQUOT, ENOTSUP);
    return -1;
  }
  return rc;
}

//list_file_tag: retourne les tags du fichier [name]
int list_file_tags(const char * name, char * tags, int size){
  int rc;
  
  rc = listxattr(name, tags, size);

  //printf("%s\n", tags);

  if(rc == -1){
    perror("listxattr"); 
    if(debug) printf("%d %d %d %d %d\n", errno, EEXIST, ENOSPC, EDQUOT, ENOTSUP);
    return -1;
  }
  return 1;

  //get_number_of_tags(tags, size);
  
}

int get_number_of_tags(char * tags, int len){
  int i = 0;
  if(debug) printf("STA get_number_of_tags()\n");
  while(len > 0 && *tags != 0){
    i++;

    if(debug) printf("%s\n", tags);

    //cas du 1er
    int diff = strlen(tags) + 1;
    len -= diff;

    //regarder ce que sa fait
    if(debug) printf("%d %s %d %d\n", len, tags, tags[0], diff);

    tags = tags + diff;
    
  }
  if(debug) printf("END get_number_of_tags(): %d\n", i);
  return i;
  
}

void split_tags(const char * tags, char ** arrays, int array_len){
  //printf("%d\n", array_len);
  for(int i = 0; i < array_len; i++){
    //strstr(): ne pas afficher dans notre cas "user."
    sprintf(arrays[i], "%s", strstr(tags, ".") + 1);
    tags = tags + strlen(tags) + 1;

  }

}

//retourne 1 si le lien est une reference sinon 0
int estReference2(char *s){
  struct stat buf;
  int ok = stat(s,&buf);
  return (ok == -1)?0:1;
}

int est_racine2(){
  struct stat current;
  int i = stat(".",&current);
  if(i<0){
    perror("ouverture");
    exit(1);
  }
  struct stat pere;
  int j=stat("..",&pere);
  if(j<0){
    perror("ouverture");
    exit(1);
  }
  return (current.st_ino == pere.st_ino); 
}

char* nom_du_repertoire_courant2(){
  
  struct stat current;
  stat(".",&current);
  
  DIR* dir = opendir("..");
  struct dirent* cur;

  while((cur = readdir(dir)) != NULL) {
    struct stat inside;
    char* name = malloc(sizeof(char) * (strlen(cur -> d_name) + 4));
    memcpy(name,"../",3);
    memcpy(name+3,cur -> d_name,strlen(cur -> d_name)+1);
    stat(name,&inside);
    if( inside.st_ino == current.st_ino &&
	current.st_dev == inside.st_dev) {
      char* copy = malloc(sizeof(char) * strlen(cur -> d_name));
      strcpy(copy,cur -> d_name);
      closedir(dir);
      return copy;
    }
    
  }
  closedir(dir);
  return NULL;
}

#define LEN 10000

char * get_name(char * path){
  char * res = path;
  for(int i = 0; i < strlen(path); i++){
    if(path[i] == '/')
      res = path + i + 1;
  }
  return res;
}
//ajouter le chemin_absolu de name au fichier names.txt 
void add_name(char * name){
  char res[LEN];
  memset(res, 0, LEN);
  path_from_root(name, res);
  add_file_name(res);
}
//TODO: bien faire
void add_name_from_rep(char * name, char * rep){

  int num = strlen(name) + strlen(rep) + 2;
  char tmp_name[num];
  memset(tmp_name, 0, num);

  DIR * dir = NULL;
  dir = opendir(rep);
  if(dir != NULL){// rep est bien un repertoir
    
    strcat(tmp_name, rep);
    if(rep[strlen(rep)-1] != '/'){
      strcat(tmp_name, "/");
    }
    strcat(tmp_name, get_name(name));
    
    char res[LEN];
    memset(res, 0, LEN);
    
    path_from_root(tmp_name, res);
    add_file_name(res);
  } else {
    //perror("opendir");
  }
}


//ancien dans ln.c adapter
void chemin_absolu(char *chemin_courant_tmp, char * dossier_de_base, char * dossier_a_ajouter){
  char chemin_courant[10000];
  memset(chemin_courant, 0, 10000);
  strcat(chemin_courant, dossier_de_base);
  strcat(chemin_courant, chemin_courant_tmp);

  //printf("%s\n", chemin_courant);
  DIR *dir  = opendir(chemin_courant);
  ssize_t path_name_size = strlen(chemin_courant_tmp);
  ssize_t all_path_name_size = strlen(chemin_courant);
  if(dir == NULL){
    perror("dir");
    return;//on ignore: pas exit!
  }

  struct dirent *ent;
  struct stat buf;
  int ok;
  char *s;

  char chemin_courant2[10000];
  memset(chemin_courant2, 0, 10000);
  strcat(chemin_courant2, dossier_a_ajouter);
  ssize_t path_name_size2 = strlen(chemin_courant2);

  while( (ent = readdir(dir)) != NULL){
    s = ent->d_name;
    strcat(chemin_courant_tmp,s);
    strcat(chemin_courant,s);
    ok = stat(chemin_courant,&buf);
    if( ok != -1 && strcmp(s, ".") != 0 && strcmp(s, "..") != 0){
      
      if (S_ISDIR(buf.st_mode)){
	//répertoire
        strcat(chemin_courant_tmp,"/");
	chemin_absolu(chemin_courant_tmp, dossier_de_base, dossier_a_ajouter);

      } else if(S_ISREG(buf.st_mode)){
	
        
        strcat(chemin_courant2, chemin_courant_tmp);
        printf("%s\n", chemin_courant2);
        add_name(chemin_courant2);
      }

    }
    chemin_courant_tmp[path_name_size] = 0;
    chemin_courant[all_path_name_size] = 0;
    chemin_courant2[path_name_size2] = 0;
  }
  
  closedir(dir);
}

void add_file_from_directory(char * curr_dir, char * next_dir){
  char chemin_courant[10000];
  memset(chemin_courant, 0, 10000);
  
  chemin_absolu(chemin_courant, curr_dir, next_dir);
}





