finite state machines en c
============

introduction
============

- Une machine d'état est une question d'être et pas d'avoir
La question à laquelle elle répond est : que suis-je et non qu'ai-je ?
Par "Que suis-je", comprendre : dans quel état suis-je ?
Une structure de donnée est une question d'avoir.

- En c, on dirait spontanément qu'une structure de donnée est ASSOCIEE à une fsm, pas qu'elle lui appartient.

- En cpp une classe qui possédantles données et qui hériterait de la classe fsm, créerait une collision entre les deux concepts : un objet conteneur composite étant dans un certain état.

- Mais conceptuellement ça n'a pas à être le cas : l'état c'est une seule variable, par exemple un enum. Ce qui doit être associé aux états ou aux transitions d'une fsm, ce sont des actions.


états vs. transitions
============

- Sur quel critère théorique doit on décider si les actions effectuées "par" une fsm le sont lors des transitions entre les états ou dans les états.

- Si le traitement transition dure ? à cause d'I/O, à cause de timers ?

- Si l'état d'arrivée dépend du résultat du traitement d'une transition ?

- Le plus simple c'est que les noeuds de la fsm soient des traitements et que les traitements soient des noeuds.


gestion du temps, pourquoi non.
============

- Spontanément certains imaginent un orchestrateur qui calcule l'état suivant et l'exécute, et attend un certain délai avant de recommencer.
Mais la fonction de wait pourrait avoir un délai qui dépend de l'état particulier de la fsm.
En fait ce délai d'orchestrateur n'est pas une caractéristique intrinsèque de la fsm.

- Le cas que je viens de décrire est un cas particulier où on a un délai unique et commun à chaque transition d'état.
On peut tout à fait imaginer que chaque état gère un ou plusieurs "wait" de durées différenciées et éventuellement modifiables.

- L'appel à une fonction wait dont être considéré comme un appel à une primitive non fournie par la fsm, au sens où elle ne l'implémente pas et où tous les scénarios d'utilisations de cette primitive sont envisageables.

- Si on supprime les I/O et les Wait, une fsm s'exécute à vitesse "infinie".
Une machine d'état s'exécute à vitesse infinie s'il n'y a pas de wait, mais les fonctions d'attente ne sont pas une caractéristique interne.
C'est une possibilité (comme les I/O qui comprennent souvent des wait d'ailleurs)
A ce stade une fsm est essentiellement une liste d'états qui effectuent des traitements.


event, pourquoi non
============

- Quid d'évenements que la fsm peut envoyer et recevoir ? (Pourquoi non)
- Même si elle utilise presque sûrement un système d'évènements pour intéragir avec l'extérieur, une fsm n'a pas besoin d'implémenter elle-même un gestionnaire d'évèvement.
- Elle a juste besoin de pouvoir appeler un changement d'état


idée de la recursivite mutuelle (belle, mais non. elle fait faire un pas vers la solution)
============

- Si une fsm n'était qu'une liste de fonctions qui ont ceci de particulier que le dernier traitement qu'elles effectuent est d'appeler la fonctions d'un état quitte à ce que ce soit elle-même ?

- Objection :
Mais si on veut simplement attendre dans un état 

- Réponse : il s'agit d'un wait avant la transition.
Si cette hypothèse est vraie une fsm est un ensemble fini de fonctions dont la dernière instruction de chacune est d'appeler une des fonctions de cet ensemble.

- C'est comme une récursivité multiple. Ou un jeu de flipper.
On me dira qu'il faut décider quel sera ce prochain état en fonction de données et cette dernière instruction se transforme alors en gros switch case spaghetti.

- Mais dans ce cas on peut l'écrire dans une fonction calculate_next_state qui devient elle même un état de la fsm.

- Il y a un problème de taille de stack si on a des apels récursifs à l'infini.

- Idée pour éviter de stacker infiniment les appels : inliner les fonctions d'état, et une fonction void state_alpha(void) non inlinée qui les wrappe.

- L'idée pour éviter de stacker infiniment les appels consistant à inliner les fonctions d'état, et une fonction void state_alpha(void) non inlinée qui les wrappe ne fonctionne par que accident et en optim -O2.

- En effet la spec de gcc indique clairement qu'il j'aire mal l'inlining de fonctions "nested", et le tes de compil sur un exemple simple avec -Winline montre que deux inlinings sur trois échouent.
Intéressant de voir que le code de l'exemple ne marche qu'optimisé.


posseder les donneées ?
============

- Il est intéressant de noter qu'une structure de données exclusivement associée à une fsm, même si elle n'est pas la "propriété" de cette fsm au sens de l'héritage ou de l'appartenance, est protégée des incohérences du simple fait que la fsm ne peut être que dans un seul état : c'est comme les propositions conditionnelles exclusives du logicien Pierce.
- L'essence de la machine d'état c'est l'exclusion mutuelle stricte des états.


fsm code template ou norme de codage ?
============

- Dès lors doit on prototyper une fsm générique qui soit réutilisable, ou compter sur la bonne volonté des codeurs pour bien entendre ce qu'on a dit ci-dessus ?

- En cpp on peut envisager qu'une fsm soit un ensemble de méthode privées d'une classe qui s'appellent

- Si on ne fait pas d'abstraction les fsm en c sont essentiellement une manière de coder : des fonctions statiques locales qui s'appellent.

- Si on fait de la généricité, il faut pouvoir enregistrer ou désenregistrer des fonctions, d'une fsm, sans doute décrire son état, mais c'est décevant, tant l'exemple récursif est léger.

- L'idée qu'il faille rendre atomique la modification de la variable d'état et l'appel de la fonction suivante m'embête un peu.

- Mais si on ne communique que par les éléments d'une structure de données partagée avec l'extérieur, cette protection semble nécessaire.


connaître son propre etat ?
============

- Dans ce que je décris, en allant à l'os, une fsm n'a pas besoin de connaître son état grâce à un champ (typ. un enum), puisqu'elle exécute la fonction qu'elle exécute et se fiche de le savoir.

- Une entité extérieure pourrait avoir besoin d'interroger cet état autrement qu'en consultant la trace des transitions passées.

- Mais ce sont des infomations annexes : si la fsm est très petite ou qu'elle a reçue une preuve formelle par ailleurs, on peut ne pas vouloir du surcoût induit par la présence de ces informations.


visibilité
============

Il semble nécessaire que les fonctions d'état soient privées.
On pourrait même souhaiter que la machine d'état ne communique avec l'extérieur que par la structure de données qu'elle contrôle et pas par ses fonctions d'état. Même pas un get state (car comme indiqué une fsm peut être quasi ignorante de son état.


question sous jascente : décrire une fsm.
============

- Une fsm est l'implementation d'un objet mathématique qu'on appelle en général un graphe orienté fini (a priori connexe).

- On peut la coder comme semble de noeuds numérotés de à à n-1 (des états) et une matrice de transition.

- On peut aussi la décrire en mémoire comme un graphe : des noeuds qui ont des pointeurs vers des noeuds (possiblement un noeud peut pointer sur lui même).

- L'idée de la récursivité mutuelle jointe à celle de non posséssion de donnée, m'a conduit à cette idée, qu'il n'est même pas nécessaire de décrire le graphe des appels dans une structure autre que l'implémentation du graphe des appels (ce serait une manière de faire de la surqualité, on devrait maintenir la cohérence entre cette structure et l'état effectif).

- L'exclusivité mutuelle des états provident du fait qu'une instance de fsm est nécessairement mono threadée, et qu'un thread ne fait qu'une chose à la fois.

- Pas besoin, donc, d'une fonction générique calculate next state car le calcul va dépendre de l'état.

- Autant les mettre dans les fonctions d'état (quitte à factoriser une partie du calcul).


fsm hiérarchiques
============

- La question des fsm hiérarchiques me semble un faux problème : l'emboîtement n'est que dans nos têtes.

- On peut à la rigureur se poser une question d'allocation dynamique ou pas d'une fsm si vraiment on souhaite que la fsm locale à l'intérieur d'un état d'une fsm globale n'existe en mémoire que si on s'y trouve.

- Mais les fonctions qui vont être associées aux pointeurs de fonctions de l'abstraction doivent exister et être créés, ce qui milite contre l'abstraction, et pour la vue opérative.


données (suite et fin)
============

- Au maximum une fsm est donc propriétaire d'un sémaphore ou d'un mutex qui contrôle la structure d'échange avec l'extérieur.

- Comme je veux rendre les choses portables et fonctionnellement séparées, la détention de données, le threading, le mutexing, et l'eventing, sont déportés dans d'autres templates de code.

- La solution propose du fsm pur, elle peut importer n'importe qu'elle solution de sémaphoring.


avantage par rapport au simple switch case
============

- Le switch case peut devenir un gigantesque morceau de code spaghetti s'il est centralisé. L'avantge de la solution proposées ici n'est pas (a priori) d'efficacité, mais sémantique, esthétique.

- La macro FSM_NEXT permet qu'un état particulier décide de l'état suivant autant qu'il est possible, plutôt que sa décision ne soit rappatriée dans un unique switch case où ses critères de décisions sont exposés aux autres.

- Plus généralement le but est aussi d'encourager le développement de petites fsm ayant une localité sémantique, et le développement de fsm séparées d'un rang hiérarchique supérieur mais qui reste simple et uni sémantiquement pour les comportements macroscopiques. Plutôt qu'une fsm unique gigantesque où des portions de code rentrent en visibilité réciproque alors qu'elles ne devraient pas.


compilation
============

	gcc -o fsm fsm.c -Winline
	gcc -o fsm fsm.c -Winline -pg
	gcc -o fsm fsm.c -Winline -O2

