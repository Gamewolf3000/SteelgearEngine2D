void Update(float dt, GameUnit &in myUnit)
{
	SGPoint playerPos = player.GetPosition();
	float xMove;
	float yMove;

	if(playerPos.xPos > myUnit.GetPosition().xPos)
	{
		xMove = 250 * dt;
	}
	else
	{
		xMove = -250 * dt;
	}

	if(playerPos.yPos > myUnit.GetPosition().yPos)
	{
		yMove = 250 * dt;
	}
	else
	{
		yMove = -250 * dt;
	}

	myUnit.Move(xMove, yMove);
}