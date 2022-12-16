Buts
==========

* parser du code c et c++
* lister les fonctions écrites dans les fichiers source d'un répertoire donné et ses sous-répertoires
* établir le graphe d'appels
* établir la liste des fonctions écrites et non appelées
* établir une métrique qui mesure la cohérence sémantique du code de manière formelle
plus précisément la distance de parcours d'aborescence des appels de fonction

Moyens
==========

* parcourir une arborescence, ouvrir et lire les fichiers c cpp, h, hpp
* établir une représentation en mémoire de l'arborescence
* identifier les déclarations de fonctions
* identifier les implémentations de fonctions
* interpréter leur signature (mots clés, templates, classes abstraites, ...)
* trouver le main et suivre les appels dans la liste des signatures
* utiliser les directives #include, mais pas makefile et CMakeFiles

Représentation des données
==========
* répertoire
* fichier
* arbre
* graphe orienté
* signature de fonction
* type

on peut faire une liste chaînée de fonctions pour établir leur liste, en utilisant list.h
on peut stocker les localisation dans l'arborescence d'une fonction, ou on fait un deuxième passage pour le graphe d'appels
la matrice d'adjacence est de taille connue si on fait une deuxième passe



