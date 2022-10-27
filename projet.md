SYSTÈME
==================

**L3 Informatique**


Il est important de bien lire le sujet jusqu'au bout et de bien y réfléchir
avant de se lancer dans la programmation du projet.


## Sujet : système de gestion de fichiers avec tags

Le but du projet est d'ajouter une couche par-dessus le système de
gestion de fichiers (SGF), permettant de gérer les fichiers grâce à des *tags*.

Un *tag* est une chaîne de caractères alphanumériques (par exemple "systeme" ou
"L3") qui permet de donner un renseignement sur un fichier. On veut
ensuite pouvoir trouver des fichiers dans le SGF  en fonction de leurs tags.

Ce qu'on doit pouvoir faire:

* marquer un fichier avec un tag;
* effacer un tag d'un fichier;
* lister les tags d'un fichier;
* lister les fichiers possédant une combinaison de tags [voir plus bas
  le paragraphe **Gestion de tags** qui explique cette notion], un fichier possédant
  plusieurs liens physiques ne doit apparaître dans cette liste qu'une
  fois;
* spécifier un lien hiérarchique entre deux tags.

On impose que ce système soit compatible avec votre SGF, en
particulier:

* utilisation de `mv` : si on modifie le nom ou on déplace un fichier, ses tags suivent;
* utilisation de `cp` : si on copie un fichier, la copie possèdera les
  mêmes tags que le fichier d'origine *au moment de la copie* (ensuite
  les deux fichiers sont indépendants l'un de l'autre)
* les tags marquent un *fichier*, pas un *lien*, donc:
  * utilisation de `ln` : si on fait un lien physique sur un fichier, les tags
    concernent le fichier (qui est unique, indépendemment de la quantité de
    liens physiques); si on fait un lien symbolique, on s'attend à ce que les
    tags listés pour le lien soient les tags marquant le fichier référencé,
  * utilisation de `rm` : si on supprime un lien physique sur un fichier :
    * s'il possédait d'autres liens physiques, les tags marquent encore ces
      liens,
    * sinon ce fichier ne doit bien sûr plus apparaître quand on liste les
      fichiers marqués par le tag


## Gestion des tags

Une *combinaison* de tags est une conjonction de tags et de complémentaires de
tags, avec au moins un tag non complémenté, exemple: `"systeme" et "L3" et pas("2019")` (où `et` est l'opérateur de conjonction et `pas` l'opérateur
de négation).

Les tags sont organisés de façon hiérarchique: un tag peut contenir
plusieurs sous-tags, par exemple le tag "couleur" pourra se décliner
en "rouge", "bleu" et "vert", le tag "bleu" pouvant lui-même se
décliner en "bleu ciel", "bleu marine" et "cobalt". Un fichier marqué
par le tag "cobalt" est alors automatiquement considéré comme marqué
par le tag "bleu" et par le tag "couleur". D'un autre côté, un fichier
peut être marqué par le tag "couleur", sans pour autant être marqué
par un des tags "bleu", "rouge" ou "vert".

La recherche des fichiers `"couleur" et pas("bleu")` permet ainsi de
récupérer des noms de tous les fichiers marqués "couleur" qui ne sont
pas marqués "bleu".

## C'est trop dur, comment on fait?

Le premier point est que ce projet est un projet de réflexion (notamment: sur
l'architecture logicielle et les fonctionnalités du système d'exploitation que
vous souhaitez utiliser pour l'implémentation), qui doit aboutir à du code, et
pas un projet de programmation où il faut réfléchir à certains moments. Donc
commencez par réfléchir!

Le deuxième est que ce projet est tout à fait faisable avec les outils
que vous avez (ou que vous aurez, ça dépend quand vous lisez cet
énoncé). Chaque membre de l'équipe enseignante a proposé une solution
technique différente pour ce projet et il y a beaucoup de façons de
l'aborder. Nous vous mettons ici quelques questions que vous devez
vous poser pour parvenir au bout du projet :

* Les tags sont-ils globaux (le tag est lié au SGF, dès qu'un
utilisateur marque un fichier, ce fichier est marqué pour tout le
monde) ou locaux (chaque utilisateur possède son propre jeu de tags) ?
**C'est à vous de décider.**
* Les tags doivent-ils être stockés dans les i-nœuds ou dans une
  structure extérieure ? **C'est à vous de décider.**
* La gestion des tags doit-elle se faire comme réaction à une
  modification du SGF, ou dès que l'utilisateur demande à modifier le
  SGF ? **C'est à vous de décider.**

La notation ne dépend pas de vos choix et les difficultés
d'implémentation des divers choix sont du même niveau. Certains choix
vous entraîneront dans l'utilisation d'outils externes (que vous
devrez trouver et comprendre), d'autres choix dans une réflexion plus
poussée sur les structures de données à utiliser.


## Modalités d'exécution (et de test)

Votre projet peut mettre en péril l'intégrité des données de celui qui
l'utilise, c'est pourquoi nous vous fournissons une image ISO d'un système Linux
sur lequel il doit fonctionner, et nous vous conseillons fortement de ne le
faire fonctionner que sur une machine virtuelle.

Le projet sera testé sur une machine virtuelle exécutant l'image ISO
fournie. Il est tout à fait admis qu'il nécessite l'intervention de
l'utilisateur `root` pour une phase d'installation de votre programme, mais
ensuite tout doit pouvoir se faire en tant qu'utilisateur quelconque.

L'image iso que nous vous proposons est la suivante:
[ubuntu-18.04.3-desktop-amd64.iso](http://releases.ubuntu.com/18.04.3/ubuntu-18.04.3-desktop-amd64.iso). Vous
pouvez tester que le téléchargement s'est bien passé ici:
[sums](http://releases.ubuntu.com/18.04.3/).

**Attention: nous devons encore étudier la possibilité d'utiliser une
machine virtuelle sur les machines de la salle de TP, les paragraphes
précédents sont susceptibles d'être mis à jour.**

## Modalités de rendu

Le projet est à faire par équipes de 2 ou 3 étudiants. Aucune exception ne sera
tolérée et nous avons une préférence pour les équipes de 3. La composition de
chaque équipe devra être envoyée par mail à l'enseignante responsable
du cours de systèmes **au plus tard le 1er mars 2020**, avec copie à
chaque membre de l'équipe.

Chaque équipe doit créer un dépôt `git` privé sur
le [gitlab de l'UFR](https://gaufre.informatique.univ-paris-diderot.fr/) **dès
le début de la phase de codage** et y donner accès en tant que `Reporter` à
tous les enseignants du cours de Système : Ines Klimann, Dominique Poulalhon,
Claude Stolze et Stefano Zacchiroli. Le dépôt devra contenir un fichier
`AUTHORS` donnant la liste des membres de l'équipe (nom, prénom, numéro
étudiant et pseudo(s) sur le gitlab).

En plus du programme demandé, vous devez fournir un `Makefile` utilisable, un
mode d'emploi, et un fichier `ARCHITECTURE` (idéalement en format Markdown,
donc avec extensions `.md`) expliquant la stratégie adoptée pour répondre au
sujet (notamment l'architecture logicielle, les choix faits en terme de où
sont stockés les tags et quand/comment ils sont mis à jours, les structures de
données et les algorithmes implémentés).

En cas de question et si la réponse n'est pas contenue dans le présent
document, merci de poser la question sur le forum `moodle` dédié du
cours de systèmes. Seules les réponses postées sur ce forum feront foi
au moment de la soutenance.

Les seules interdictions strictes sont les suivantes : plagiat (d'un
autre projet ou d'une source extérieure à la licence), utilisation de
la fonction `system` de la `stdlib`.


## Soutenances

La soutenance se fera à partir du code contenu sur le gitlab, sur une
machine virtuelle exécutant le système dont l'image ISO a été fournie.
Elle aura lieu après la fin des cours du semestre. Sa date précise
sera ultérieurement affichée sur `moodle`.

On attend de tous les groupes venant soutenir qu'ils soient capables
de lancer une machine virtuelle avec l'image ISO fournie et de charger
dessus leur programme en clônant le dépôt git associé, de le compiler
et de procéder à l'éventuelle phase d'installation. L'ensemble de
ces actions doit être rapide.
