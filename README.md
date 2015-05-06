# octogame [![Build Status](https://magnum.travis-ci.com/IohannRabeson/octogame.svg?token=oSgYDG8ZHmxB1gxGNZxP&branch=master)](https://magnum.travis-ci.com/IohannRabeson/octogame)

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

Pour les dependances, rien n'est fait. Vous devez les installer a la mano
pour l'instant.  
Mais c'est prevu, voir [issues 2](https://github.com/IohannRabeson/octogame/issues/2)

Les Makefile attendent que les librairies soient accessibles par le compilateur
ce qui est le cas sous mac si on installe la sfml sans oublier
d'utiliser sa regle make install.  
Voir ce tutoriel officiel pour l'installe de la sfml avec CMake:
http://www.sfml-dev.org/tutorials/2.2/compile-with-cmake.php

Pour Boost, utilisez brew:

    $> brew install boost --c++11
    
Pour Doxygen, utilisez brew:

    $> brew install doxygen --with-graphviz

## Compiler le projet

Pour compiler la librairie, les tests et les outils:

	$> make complete

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
