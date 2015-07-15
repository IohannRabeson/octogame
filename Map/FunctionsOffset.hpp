/*! Set the offsetX between 0 and mapSizeX and return the new position for the given position posDecorX */
static int getCircleOffset(int offsetX, int posDecorX, int screenSizeX, int mapSizeX)
{
	int newPosX = posDecorX;

	while (offsetX < 0)
	{
		offsetX += mapSizeX;
		newPosX -= mapSizeX;
	}
	while (offsetX >= mapSizeX)
	{
		offsetX -= mapSizeX;
		newPosX += mapSizeX;
	}
	int border = offsetX + screenSizeX + 20;
	if (border > mapSizeX)
	{
		if (posDecorX < (border % mapSizeX) + screenSizeX)
			newPosX += mapSizeX;
		else if (posDecorX < 40)
			newPosX += mapSizeX;
	}
	else if (offsetX < 60)
	{
		if (posDecorX > mapSizeX - 40)
			newPosX -= mapSizeX;
	}
	return newPosX;
}
