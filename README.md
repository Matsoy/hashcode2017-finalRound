Hash Code 2017, Final Round - Router placement
===========

*Given a building plan, decide where to put wireless routers and how to connect them to the fiber backbone to maximize coverage and minimize cost.*

# Equipe : 
----------
- Nicolas Ehresmann (Arbitre)
- Mehdi Haddad (Stratégie)
- Arthur Outhier (Stratégie)
- Neil Rittner (Arbitre)
- Mathieu Soyer (Stratégie)


# Contexte :
------------
L'objectif de ce projet est d'optimiser le placement de routeurs sur une carte donnée, pour faire
en sorte de couvrir un maximum de cellules cibles, le tout avec un budget donné.
Cependant, il vaut veiller à respecter quelques règles : 
- Tous les routeurs doivent être connectés au Backbone donné.
- Tous les routeurs doivent être placés sur des cellules connectées (c'est à dire câblées).
- Les routeurs ne doivent pas être posés sur des murs.
- La Wifi ne traverse pas les murs.
- Veiller à ce que les routeurs et les câbles soient bien posés dans les limites de la carte.

Enfin, le calcul du score respecte l'équation suivante:
`Score = 1000 * T + (B - (N * Pb + M * Pr))`. Avec : 
- `T` : Nombre de cellules cibles couvertes.
- `B` : Budget initial
- `N` : Nombre de câbles construits
- `Pb` : Prix d'un câble
- `M` : Nombre de routeurs placés
- `Pr` : Prix d'un routeur


# Découpage du projet
---------------------
Pour ce projet, nous devions travailler sur une partie Arbitre et une partie Stratégie.
Ci-dessous est repris le découpage que nous avons effectué pour définir les 2 parties.

## Partie Arbitre
-----------------
***Vérification cohérence de l'output :***
- 0 <= nombre de cellules connectées à l'antenne < nombre de lignes * nombre de colonnes
- lignes en-dessous:
	- 0 <= 1er nombre < nombre de lignes : coordonnées X de la cellule connectée
	- 0 <= 2eme nombre < nombre de colonnes: coordonnées Y de la cellule connectée
- 0 <= nombre de routeurs < nombre de lignes * nombre de colonnes
- lignes en-dessous:
	- 0 <= 1er nombre < nombre de lignes : coordonnées X du routeur
	- 0 <= 2eme nombre < nombre de colonnes: coordonnées Y du routeur

***+ :***
- pas de routeur sur un mur
- les routeurs sont bien connectés à l'antenne 
- les câbles sont donnés dans l'ordre, en partant de l'antenne
- budget pas dépassé

***Si plusieurs fichiers output en entrée, affichage :***
- pourcentage max de couvertures
- score max
- score min
- score moyen
- score médian
- écart-type du score
- variance du score
- pour chaque fichier, affichage:
	- nombre de cellules couvertes
	- nombre de routeurs
	- nombre de câbles
	- budget

## Partie Stratégie
-------------------
***Vérification cohérence de l'input :***
- 1ere ligne
	- 1 <= 1er nombre <= 1000 : nombre de lignes
	- 1 <= 2eme nombre <= 1000 : nombre de colonnes
	- 1 <= 3eme nombre <= 10 : rayon des routeurs
- 2eme ligne
	- 1 <= 1er nombre <= 5 : prix d'un câble
	- 1 <= 2eme nombre <= 100 : prix d'un routeur
	- 1 <= 3eme nombre <= 10^9 : budget maximum
- 3eme ligne
	- 0 <= 1er nombre < nombre de lignes : coordonnées X de l'antenne
	- 0 <= 2eme nombre < nombre de colonnes: coordonnées Y de l'antenne

# Meilleurs scores obtenus
--------------------------

| Carte           | Score    
| --------------- | ----------
| Charleston_road | 21 962 856
| Rue_de_londres  | 58 356 153
| Opera           | 174 527 508
| Lets_go_higher  | 290 192 787

**Total** : :boom: 545 039 304 :boom:
