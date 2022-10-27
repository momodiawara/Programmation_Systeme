#ifndef UTILS_H
#define UTILS_H

//ajoute les tags du fichier [name] dans [tags]
int list_file_tags(const char * name, char * tags, int tags_size);

void add_lien_liens(char * pere, char * fils);
void add_file_name(char * s);
void path_from_root(char * file, char * res);

struct memory;
typedef struct memory * Memory;

Memory get_liens();

void init_pere_du_fils_comparaison(Memory m);


//UTILISER APRES AVOIR FAIT: init_pere_du_fils_comparaison()
int est_pere_du_fils(Memory m, char * pere, char * fils);

void print_memory(Memory m);


int get_tags_len(char * name);

int get_number_of_tags(char * tags, int tags_len);

void split_tags(const char * tags, char *arrays[], int array_len);

//retourne un tableau de string
//de taille len et chaque string: size
char ** create_charss(int size, int len);

void free_charss(int len, char ** s);


char * get_name(char * path);
void add_name(char * name);
void add_name_from_rep(char * name, char * rep);

void add_file_from_directory(char * curr_dir, char * next_dir);

#endif
