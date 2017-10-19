# Partie Arbitre
____________________
__***Vérification cohérence de l'output :***__
- 0 <= nombre de cellules connectées à l'antenne < nombre de lignes * nombre de colonnes
- lignes en-dessous:
	- 0 <= 1er nombre < nombre de lignes : coordonnées X de la cellule connectée
	- 0 <= 2eme nombre < nombre de colonnes: coordonnées Y de la cellule connectée
- 0 <= nombre de routeurs < nombre de lignes * nombre de colonnes
- lignes en-dessous:
	- 0 <= 1er nombre < nombre de lignes : coordonnées X du routeur
	- 0 <= 2eme nombre < nombre de colonnes: coordonnées Y du routeur

__***+ :***__
- pas de routeur sur un mur
- les routeurs sont bien connectés à l'antenne 
- les câbles sont donnés dans l'ordre, en partant de l'antenne
- budget pas dépassé

__***Si plusieurs fichiers output en entrée, affichage :***__
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

# Partie Stratégie
____________________
__***Vérification cohérence de l'input :***__
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