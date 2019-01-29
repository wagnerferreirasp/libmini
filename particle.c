#include "particle.h"

Particle *newParticle(Object *obj, float maxSpeed, float mass)
{
	Particle *part = (Particle *)safeMalloc(sizeof(*part));
	part->obj = obj;
	part->speed = newPoint(0,0);
	part->mass = mass;
	part->maxSpeed = maxSpeed;
	part->top = part->right = part->bottom = part->left = NULL;

	return part;
}

void setSpeed(Particle *part, float xSpeed, float ySpeed)
{
	part->speed.x = xSpeed;
	part->speed.y = ySpeed;

	float xSqr = part->speed.x * part->speed.x, ySqr = part->speed.y * part->speed.y, maxSqr = part->maxSpeed * part->maxSpeed;
	if (xSqr + ySqr > maxSqr)
	{
		float factor = (part->maxSpeed / sqrt(xSqr + ySqr));
		part->speed.x *= factor;
		part->speed.y *= factor;
	}
}

void setForces(Particle *part, float xForces, float yForces)
{
	float xAccel = xForces / part->mass, yAccel = yForces / part->mass;
	setSpeed(part, part->speed.x + xAccel, part->speed.y + yAccel);
}

void *setObstacleAttributes(void *_obs, bool particles, float *obsX, float *obsY, float *obsWidth, float *obsHeight)
{
	void *obs = NULL;

	if (particles) obs = ((Particle *)_obs)->obj;
	else obs = (Object*)_obs;

	*obsX = getX(obs), *obsY = getY(obs), *obsWidth = getWidth(obs), *obsHeight = getHeight(obs);

	return obs;
}

void moveParticle(Particle *part, List *obstacles, bool particles)
{
	int i;
	float epsilon = 0.0001f;

	if (obstacles)
	{
		float xVar = part->speed.x, yVar = part->speed.y,
			x = getX(part->obj), y = getY(part->obj), width = getWidth(part->obj), height = getHeight(part->obj),
			obsX, obsY, obsWidth, obsHeight;
		void *obs = NULL;
		
		Node *n;
		part->top = part->right = part->bottom = part->left = NULL;
		for (n = obstacles->head->next; n != obstacles->tail; n = n->next)
		{
			obs = setObstacleAttributes(n->item, particles, &obsX, &obsY, &obsWidth, &obsHeight);
			if (obs == part->obj) continue;

			if (x + width > obsX && obsX + obsWidth > x && obsY + obsHeight == y)
				part->top = obs;

			if (y + height > obsY && obsY + obsHeight > y && x + width == obsX)
				part->right = obs;

			if (x + width > obsX && obsX + obsWidth > x && y + height == obsY)
				part->bottom = obs;

			if (y + height > obsY && obsY + obsHeight > y && obsX + obsWidth == x)
				part->left = obs;
		}

		if (part->top && part->speed.y < epsilon) part->speed.y = yVar = 0;
		if (part->right && part->speed.x > -epsilon) part->speed.x = xVar = 0;
		if (part->bottom && part->speed.y > -epsilon) part->speed.y = yVar = 0;
		if (part->left && part->speed.x < epsilon) part->speed.x = xVar = 0;

		for (n = obstacles->head->next; n != obstacles->tail; n = n->next)
		{
			obs = setObstacleAttributes(n->item, particles, &obsX, &obsY, &obsWidth, &obsHeight);
			if (obs == part->obj) continue;
			Rectangle obsBounds = newRectangle(obsX, obsY, obsWidth, obsHeight);

			if (part->speed.x >-epsilon && part->speed.x < epsilon ) // X NULO
			{
				if (part->speed.y >-epsilon && part->speed.y < epsilon ) // X NULO E Y NULO
					break;
				else if (part->speed.y > epsilon) // X NULO E Y POSITIVO
				{
					Rectangle moveRec = newRectangle(x, y, width, height + yVar);

					if (intersects(obsBounds, moveRec) && y + height <= obsY)
					{
						// vai limitar Y
						part->speed.y = 0;
						setY(part->obj, obsY - height);
						part->bottom = obs;
					}
				}
				else // X NULO E Y NEGATIVO
				{
					Rectangle moveRec = newRectangle(x, y + yVar, width, height - yVar);

					if (intersects(obsBounds, moveRec) && obsY + obsHeight <= y)
					{
						// vai limitar Y
						part->speed.y = 0;
						setY(part->obj, obsY + obsHeight);
						part->top = obs;
					}
				}
			}
			else if (part->speed.x > epsilon) // X POSITIVO
			{
				if (part->speed.y >-epsilon && part->speed.y < epsilon ) // X POSITIVO E Y NULO
				{
					Rectangle moveRec = newRectangle(x, y, width + xVar, height);

					if (intersects(obsBounds, moveRec) && x + width <= obsX)
					{
						// vai limitar X
						part->speed.x = 0;
						setX(part->obj, obsX - width);
						part->right = obs;
					}
				}
				else if (part->speed.y > epsilon) // X POSITIVO E Y POSITIVO
				{
					Rectangle moveRec = newRectangle(x, y, width + xVar, height + yVar);

					if (intersects(obsBounds, moveRec))
					{
						if (obsX >= x + width)
						{
							// possivel limitar X
							if (obsY >= y + height)
							{
								// possivel limitar Y também, verificar qual limita
								float timeX = (obsX - x + width) / xVar;
								float timeY = (obsY - y + height) / yVar;

								if (timeX >= timeY)
								{
									// vai limitar X
									part->speed.x = 0;
									setX(part->obj, obsX - width);
									part->right = obs;
								}
								else if (y + height <= obsY)
								{
									// vai limitar Y
									part->speed.y = 0;
									setY(part->obj, obsY - height);
									part->bottom = obs;
								}
							}
							else
							{
								// vai limitar X
								part->speed.x = 0;
								setX(part->obj, obsX - width);
								part->right = obs;
							}
						}
						else if (y + height <= obsY)
						{
							// vai limitar Y
							part->speed.y = 0;
							setY(part->obj, obsY - height);
							part->bottom = obs;
						}
					}
				}
				else // X POSITIVO E Y NEGATIVO
				{
					Rectangle moveRec = newRectangle(x, y + yVar, width + xVar, height - yVar);

					if (intersects(obsBounds, moveRec))
					{
						if (obsX >= x + width)
						{
							// possivel limitar X
							if (obsY + obsHeight <= y)
							{
								// possivel limitar Y também, verificar qual limita
								float timeX = (obsX - x + width) / xVar;
								float timeY = (obsY + obsHeight - y) / yVar;

								if (timeX >= timeY)
								{
									// vai limitar X
									part->speed.x = 0;
									setX(part->obj, obsX - width);
									part->right = obs;
								}
								else
								{
									// vai limitar Y
									part->speed.y = 0;
									setY(part->obj, obsY + obsHeight);
									part->top = obs;
								}
							}
							else
							{
								// vai limitar X
								part->speed.x = 0;
								setX(part->obj, obsX - width);
								part->right = obs;
							}
						}
						else if (obsY + obsHeight <= y)
						{
							// vai limitar Y
							part->speed.y = 0;
							setY(part->obj, obsY + obsHeight);
							part->top = obs;
						}
					}
				}
			}
			else // X NEGATIVO
			{
				if (part->speed.y >-epsilon && part->speed.y < epsilon ) // X NEGATIVO E Y NULO
				{
					Rectangle moveRec = newRectangle(x + xVar, y, width - xVar, height);

					if (intersects(obsBounds, moveRec) && obsX + obsWidth <= x)
					{
						// vai limitar X
						part->speed.x = 0;
						setX(part->obj, obsX + obsWidth);
						part->left = obs;
					}
				}
				else if (part->speed.y > epsilon) // X NEGATIVO E Y POSITIVO
				{
					Rectangle moveRec = newRectangle(x + xVar, y, width - xVar, height + yVar);

					if (intersects(obsBounds, moveRec))
					{
						if (obsX + obsWidth <= x)
						{
							// possivel limitar X
							if (obsY >= y + height)
							{
								// possivel limitar Y também, verificar qual limita
								float timeX = (obsX + obsWidth - x) / xVar;
								float timeY = (obsY - y + height) / yVar;

								if (timeX >= timeY)
								{
									// vai limitar X
									part->speed.x = 0;
									setX(part->obj, obsX + obsWidth);
									part->left = obs;
								}
								else if (y + height <= obsY)
								{
									// vai limitar Y
									part->speed.y = 0;
									setY(part->obj, obsY - height);
									part->bottom = obs;
								}
							}
							else
							{
								// vai limitar X
								part->speed.x = 0;
								setX(part->obj, obsX + obsWidth);
								part->left = obs;
							}
						}
						else if (y + height <= obsY)
						{
							// vai limitar Y
							part->speed.y = 0;
							setY(part->obj, obsY - height);
							part->bottom = obs;
						}
					}
				}
				else // X NEGATIVO E Y NEGATIVO
				{
					Rectangle moveRec = newRectangle(x + xVar, y + yVar, width - xVar, height - yVar);

					if (intersects(obsBounds, moveRec))
					{
						if (obsX + obsWidth <= x)
						{
							// possivel limitar X
							if (obsY + obsHeight <= y)
							{
								// possivel limitar Y também, verificar qual limita
								float timeX = (obsX + obsWidth - x) / xVar;
								float timeY = (obsY + obsHeight - y) / yVar;

								if (timeX >= timeY)
								{
									// vai limitar X
									part->speed.x = 0;
									setX(part->obj, obsX + obsWidth);
									part->left = obs;
								}
								else
								{
									// vai limitar Y
									part->speed.y = 0;
									setY(part->obj, obsY + obsHeight);
									part->top = obs;
								}
							}
							else
							{
								// vai limitar X
								part->speed.x = 0;
								setX(part->obj, obsX + obsWidth);
								part->left = obs;
							}
						}
						else if (obsY + obsHeight <= y)
						{
							// vai limitar Y
							part->speed.y = 0;
							setY(part->obj, obsY + obsHeight);
							part->top = obs;
						}
					}
				}
			}
		}
	}
	move(part->obj, part->speed.x, part->speed.y);
}

void freeParticle(Particle *part)
{
	freeObject(part->obj);
	free(part);
}

