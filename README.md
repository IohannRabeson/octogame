# An Octonaut Odyssey

## Clone

    $> git clone git@github.com:IohannRabeson/octogame.git
    $> git submodule init
    $> git submodule update

Des explications à propos des sous modules: [issues 2](http://git-scm.com/book/fr/v1/Utilitaires-Git-Sous-modules)

## Mettre a jour le depot

	$> git pull origin master
	$> git submodule update
	$> make complete

## Compilation, installation, et tout ça

### Dependances
	- SFML 2.3
	- [hatedepend](https://github.com/IohannRabeson/hatedepend)
	- [cppcheck](https://github.com/danmar/cppcheck/)
	- Tout ce qui est requis pour [libocto](https://github.com/IohannRabeson/octolib#requierements)

### Compiler le projet

Pour compiler la librairie, les tests et les outils et le jeu:

	$> make complete

### Les tests
Hatedepend vous previendra de certains probleme liee au inclusions.  
Vous pouvez lancer l'analyse statique avec cppcheck en tapant:

	$> make static-check

## Executer le projet

### Sous mac

Pas de soucis pour l'instant, tout devrait se faire normalement.

### Sous linux

Il est possible que les fichiers des librairies dynamique SFML soit introuvable.  
Dans ce cas, il est necessaire d'utiliser ldconfig apres l'installation en regle de la sfml:

	$> ldconfig /usr/local/bin/

## Configuration de l'execution via le fichier default.conf

Le fichier default.conf permet le parametrage du programme.  
Vous pouvez y modifier differente chose, comme le package de ressource a utiliser, le premier
etat a lancer, etc...  
La documentation a ce sujet est dans la classe Application.

## Travis
Travis effectue differents tests a chaque fois qu'un pull request est ouverte ou mise a jour.
Les compilations se feront avec clang, en mode debug et release.
Hatedepend n'est pas necessaire a travis, pour ne pas avoir a l'installer, il est desactive.
