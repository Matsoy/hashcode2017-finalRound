struct Cell
{
	enum e
	{
		Backbone = -2, // emetteur
		Void = -1, // vide
		Wall = 0, // mur
		Wireless = 1, // cellule cible
		Router = 2, // routeur
		ConnectedRouter = 3, // routeur connecte
		Cable = 4 // cable
	};
};