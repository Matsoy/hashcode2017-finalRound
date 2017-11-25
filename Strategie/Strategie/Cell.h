struct Cell
{
	enum e
	{
		Backbone = -2,
		Void = -1, // vide
		Wall = 0, // mur
		Wireless = 1, // cellule cible
		Router = 2, // router
		ConnectedRouter = 3,
		Cable = 4 // cable
	};
};