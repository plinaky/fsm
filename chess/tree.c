#include "board.h"

#define UP 0
#define DN 1
#define LE 2
#define RI 3

/*
 * On construit un arbre de parties
 * au départ chaque noeud a :
 * un nombre de visites "view" à 0 (ou 1, si on compte la création)
 * un coût maximum (un coût nul consistant en la certitude quant à l'issue)
 * une espérance de gain algébrique : 0 est l'espérance d'une partie nulle 
 * un pointeur vers son prédécesseur
 * un pointeur vers son sucesseur
 * un pointeur vers son voisin droit
 * un pointeur vers son voisin gauche
 * une position
 * le mouvement qui a amené à cette position
 *
 * l'arbre des partie est en fait un graphe orienté car deux séquences
 * de coups différentes peuvent conduire à la même position.
 *
 * A l'insertition on veillera donc à ne pas insérer de duplicata d'une position donnée
 * ni de position identique poour la couleur opposée
 * ni de position symétrique par rapport à un axe vertical
 * ni de position symétrique par rotation de +- Pi /2 si applicable
 *
 * */

/*
 * On doit construire une fonction d'évaluation
 * qui utilise 
 * l'arbre des parties
 * ses capacités à calculer (jouer des parties non encore jouées)
 * d'autres données d'apprentissage qu'elle peut stocker
 *
 * En partie réelle, son travail s'effectuera en temps limité
 *
 * Elle a pour mission de trouver le meilleur coup
 * a défaut du coup gagnant à coup sûr,
 * elle établit génériquement la liste des coups triés
 * par leur espérance de gain.
 *
 * */

struct node {
	uint64_t views;
	uint64_t cost;
	int64_t  win;
	struct node *l[4];
	struct board b;
	uint16_t move;
};

#undef UP
#undef DN
#undef LE
#undef RI

