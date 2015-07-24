/*! Set the offsetX between 0 and mapSizeX and return the new position for the given position posDecorX */
static int getCircleOffset(int offsetX, int posDecorX, int screenSizeX, int mapSizeX)
{
	int countMap = static_cast<int>(offsetX / mapSizeX);
	int newPosDecorX = countMap * mapSizeX + posDecorX;
	int borderLeft = offsetX + screenSizeX / 2.f + mapSizeX / 2.f;
	int borderRight = offsetX + screenSizeX / 2.f - mapSizeX / 2.f;
	if (newPosDecorX - mapSizeX >= borderRight && newPosDecorX <= (countMap + 1) * mapSizeX)
		return (newPosDecorX - mapSizeX);
	else if (newPosDecorX + mapSizeX <= borderLeft && newPosDecorX >= countMap * mapSizeX)
		return (newPosDecorX + mapSizeX);
	return (newPosDecorX);
}
