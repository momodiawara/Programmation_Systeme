CC=gcc

CFLAGS=-Wall -g -std=c11

LDLIBS= -lm 

ALL =  utils.o marquertag listertag supprimertag lientag afficherlien filtrertag tmp_filtrertag ln mv cp 

all : $(ALL)

cp : cp.c utils.o 
mv : mv.c utils.o
ln : ln.c utils.o

tmp_filtrertag : tmp_filtrertag.c utils.o


utils.o : utils.c


marquertag : marquertag.c utils.o 
supprimertag : supprimertag.c utils.o 
listertag : listertag.c utils.o 
filtrertag : filtrertag.c utils.o 
lientag : lientag.c utils.o 
afficherlien : afficherlien.c utils.o


clean : 
	rm -rf *~ 
cleanall :
	rm -rf *~  *.o *.exe.stackdump $(ALL)
