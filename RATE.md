Filtrertag
==========
Reflexion
---------

La première idée est de parcourir depuis la racine [/] et de comparer les fichiers qui s'y trouvent.

La seconde est de parcourir le fichier [names.txt] et de comparer avec les tags.

Pour faire cela, on a fait une fonction qui prend un chemin absolu d'un fichier et des informations supplémentaires pour la fonction.
Ainsi il suffit de faire les deux parcours de fichier.


Déroulement
-----------


Pour commencer, la première idée consomme beaucoup de temps.De plus on rencontre certaine difficulté lors des phases de test.
Par exemple, un problème dans [/dev] qui produit une boucle infinie.

Suite à ça on a réfléchit à la possibilité d'envisager la deuxième idée.
Dans ce cas là, il faut entretenir [names.txt] pour les commandes [mv], [cp], [ln] et [marquertag] fonctionnent. 
Il faut ajouter les chemins absolue dans [names.txt].
Pour [marquertag], nous n'avons pas rencontrer de problème.
Pour mv, cp et ln, on voulait récupérer les noms des fichiers et les ajouter dans [names.txt], ce sont [mv.c] [cp.c] et [ln.c]. 
Malgré cette solution, on a envisagé de retravailler sur notre première idée.


Sur cette dernière, on a finalement compris d'où venait le problème de la bouclie infinie. Il fallait tout simplement pas chercher dans le dossier [/dev].
Le programme reste malgré tout assez lent, donc nous avons mis deux [define] dans [filtrertag.c]:

[PAS_AFFICHER_CACHER] : par défaut, n'affiche pas les dossiers et fichiers cachés

[FROM_HOME] : par defaut, on commence depuis [/home], même désactiver, on ne regarde pas [/dev]



On est donc sur la première idée.


