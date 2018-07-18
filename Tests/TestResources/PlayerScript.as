void Update(float dt, GameUnit &in myUnit)
{
	if(input.IsDown(0))
	{
		myUnit.Move(-500 * dt, 0);
	}
	if(input.IsDown(1))
	{
		myUnit.Move(500 * dt, 0);
	}

	if(input.IsDown(2))
	{
		myUnit.Move(0, -500 * dt);
	}
	if(input.IsDown(3))
	{
		myUnit.Move(0, 500 * dt);
	}
}