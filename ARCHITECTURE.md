Architecture de notre programme
===============================

Architecture logicielle
--------------------------

Nos exécutables sont programmés en langage C avec des alias.  
Le but ici est de gérer les liens dans le fichier [liens.txt] et les tags avec [xattr]


Stockage des tags
----------------------
Le stockage des tags sera fait avec [xattr].  
Les attributs étendus sont des paires nom:valeur associées aux inœuds (fichiers, répertoires, liens symboliques, etc.). Ce sont des extensions des attributs normaux qui sont associés avec tous les inœuds du système.(cf. man xattr)

Les structures de donnees
-------------------------------
**utils.h**  
 * StructCharss (char **) (dans utils.h)  
 * StructLiens (struct memory, *Memory) (dans utils.h)  
**filtrertag.c**  
 * StructTags (struct type_tags, *Type_tags) (dans filtrertag.c)  
 * StructInode (struct memory2, *Memory2) (dans filtrertag.c) 


Mise a jour
----------------
La mise à jour des tags est faite via des appels systèmes de [xattr]. En effet, plusieurs actions peuvent être réaliser grâce à cette fonctionnalité :  
 * Ajouter et/ou Supprimer un tag à un fichier.  
 * Lister les fichiers content un tag.  
 * Faire le lien entre les tags dans un fichier [liens.txt] de la forme:  
    [pere1] [fils1]\n[pere2] [fils2]\n[...]\n.  
 * Lorsqu'un utilisateur veut procéder à une modification, ces actions là sont mises à jour.


Etre compatible avec SGF
---------------------------
 * **mv**: (rien à faire)
 * **cp**: avec alias pour copier [xattr].
 * **ln**: (rien à faire)
 * **rm**: (rien à faire)


Les actions
----------------
 * **Marquer un fichier** avec un tag :
    On ajoute un tag pour l'inode avec [setxattr()].
 * **Supprimer un tag** d'un fichier :
    On supprime le tag pour l'inode avec [removexattr()].
 * **Lister les tags** d'un fichier :
    On cherche les informations avec [listxattr()]  
    et les stocke dans [StructCharss] pour ensuite les afficher.
 * **Lister les fichiers** possédant une combinaison de tags :  
    On cherche des fichiers depuis le dossier [/home],  
    on utilise [StructTags] pour stocker les tags passés en paramètre,  
    [StructInode] pour les i-noeuds des fichiers déjà vu et  
    [StructLiens] pour les liens entre tags.
 * **Spécifier un lien hiérarchique** entre deux tags :
    Ce lien est ajouté dans le fichier [liens.txt].


Explications des structures
---------------------------------
 * **StructCharss** :
    C'est une structure [char **], un _tableau de string_
 * **StructLiens** :
    C'est une structure [struct memory] avec :
    * Deux _tableau de string_ pour les pères et les fils :
    avec pour chaque indice i correspondant au lien entre père et fils.
    * Gère des tableaux dynamiques: taille du tableau, nombre d'éléments
    * Un tableau qui indique l'emplacement d'un père pour un fils
 * **StructTags** :
    C'est une structure [struct type_tags] avec :
    * Un _string_ pour le tag 
    * Un _boolean_ pour savoir si il faut avoir ce tag ou non
    (et donc ["pas("un_tag")"] est changé en [un_tag] et [non])
    * On utilisera des [struct type_tags[LEN]]
 * **StructInode** :
    C'est une structure [struct memory2] avec :
    * Un tableau de [ino_t] dynamique simple et    
    * Des informations pour gérer un tableau dynamique.

Explications des algorithmes
---------------------------------

 * **A) Récupérer des tags d'un fichier:**
    * 1) On initialise un _tableau de string_ [StructCharss] avec la bonne taille
    * 2) On affiche les _strings_ avec ce tableau

 * **B) Récupérer les liens entre les tags:**
    * [tableau des pères], [tableau des fils] et [tableau d'optimisation]
    * 1) Récupérer les liens depuis le fichier [liens.txt] et les stocker dans
    [tableau des pères] et [tableau des fils]
    * 2) Créer le [tableau d'optimisation] :  
    Le [tableau d'optimisation] est un tableau d'int,  
    chaque indice i désigne l'emplacement du fils dans le [tableau des fils] et
    la valeur dans l'indice i est l'emplacement du père dans le [tableau des fils]
    (-1 si le père n'est pas un fils)

 * **C) Savoir si un fils [f] a comme père [p] (et ses ancêtres):**
    * 1) faire B) *ci-dessus*  
    * 2) Sinon, chercher l'emplacement [f_i] de [f] dans le [tableau des fils] et 
    un _tableau de boolean_ [pe] qui indique pour chaque i si l'emplacement i dans le
    [tableau des pères] vaut [p]  
    Si on ne trouve pas [f_i] ou au moins un [pe] alors il n'y a pas de liens existant alors c'est *faux*
    * 3) Soit dans le [tableau d'optimisation] et [p_j] la valeur dans l'indice [f_i]
        * si [p_j] vaut -1 alors on regarde dans [pe] a l'emplacement [p_i] si c'est *vrai* ou *faux*
        * si [p_j] ne vaut pas -1 alors on regarde aussi dans [pe] a l'emplacement [p_i]:
        si c'est *vrai* alors on a fini
        sinon [p_i] vaut [p_j] et on refait 4)
    * *Remarque* : Il ne compare pas [f] et [p]

 * **D) Filtrer les fichiers depuis le dossier [/home] :**
    * 1) Récupérer les tags passer en paramètre avec [StructTags] que l'on nomme [tags]
    * 2) Faire B), qui donne un [StructLiens] que l'on nomme [liens] et
    initialiser [StructInode] que l'on nommera [memoire i-noeuds]
    * 3) Parcourir depuis [/home] :  
        * Quand nous aurons un fichier :
            * Récupérer ses tags que l'on nomme [t], si pas de tag, alors *ne pas afficher*
            * sinon regarder son i-noeud et voir si il est dans [memoire i-noeuds],
            si il y est alors *ne pas afficher*
            * sinon comparer chaque tag dans [tags] si il est dans [t]:
                * si il ne doit pas y etre mais l'est ou il devrait mais ne l'es pas alors *ne pas afficher*
            * si on est ici alors le fichier rempli les conditions et donc on *affiche*

Obsolète
-------------
 * [cp.c], [ln.c] et [mv.c] en plus du stockage [names.txt] correspondent à la deuxième solution pour filtrer les tags *(voir RATE.md)*
 * Pour filtrer les fichiers, l'idée est de tout stocker dans [names.txt] et parcourir ce fichier à la place de [/home]
 * Pour [cp.c], [ln.c], [mv.c] et [marquertag.c] il fallait ajouter les chemins absolus des fichiers dans [names.txt]
 * Mais c'est obsolète et non terminé
 (ça ne fonctionne pas tout le temps pour [cp.c], [ln.c] et [mv.c], il manque des cas d'options, et cp ne fait pas [-p])


