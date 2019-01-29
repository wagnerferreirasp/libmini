#include "object.h"

Object *newBlock(Point pos, Image *img)
{
	return newObject(pos, newPoint(img->width, img->height), newPoint(0, 0), img);
}

Object *newObject(Point pos, Point size, Point boundsPos, Image *img)
{
	Object *obj = (Object *)safeMalloc(sizeof(*obj));
	obj->bounds = newRectangle(pos.x, pos.y, size.x, size.y);
	obj->boundsPos = boundsPos;
	obj->image = img;
	obj->rects = NULL;
	return obj;
}

Object *newBlockSprite(Point pos, Image *spriteSheet, byte columns, byte lines)
{
	return newSprite(pos, newPoint(spriteSheet->width / columns, spriteSheet->height / lines), newPoint(0, 0),
		spriteSheet, columns, lines);
}

Object *newSprite(Point pos, Point size, Point boundsPos, Image *spriteSheet, byte columns, byte lines)
{
	Object *obj = (Object *)safeMalloc(sizeof(*obj));
	obj->bounds = newRectangle(pos.x, pos.y, size.x, size.y);
	obj->boundsPos = boundsPos;
	obj->image = spriteSheet;
	obj->rects = (Rectangle *)safeMalloc(columns * lines * sizeof(Rectangle));
	obj->columns = columns;
	obj->lines = lines;
	obj->imgIndex = 0;
	obj->imgTimer = 0;
	obj->animIndex = 0;
	int i, j, w = spriteSheet->width / columns, h = spriteSheet->height / lines;
	for (i = 0; i < columns; i++)
		for (j = 0; j < lines; j++)
			obj->rects[i + j * columns] = newRectangle(i * w, j * h, w, h);
	return obj;
}

Rectangle getBounds(Object *obj)
{
	return obj->bounds;
}

void setPosition(Object *obj, Point pos)
{
	obj->bounds.position = pos;
}

float getX(Object *obj)
{
    return obj->bounds.position.x;
}

float getY(Object *obj)
{
    return obj->bounds.position.y;
}

void setX(Object *obj, float x)
{
	obj->bounds.position.x = x;
}
void setY(Object *obj, float y)
{
	obj->bounds.position.y = y;
}

float getWidth(Object *obj)
{
    return obj->bounds.size.x;
}

float getHeight(Object *obj)
{
    return obj->bounds.size.y;
}

void setBounds(Object *obj, float x, float y, float width, float height)
{
	obj->boundsPos.x = x;
	obj->boundsPos.y = y;
	obj->bounds.size.x = width;
	obj->bounds.size.y = height;
}

void setCenter(Object *obj, Point pos)
{
	obj->bounds.position = newPoint(pos.x - obj->bounds.size.x / 2, pos.y - obj->bounds.size.y / 2);
}

void move(Object *obj, float xVar, float yVar)
{
	obj->bounds.position.x += xVar;
	obj->bounds.position.y += yVar;
}

void animate(Object *obj, byte *indices, byte size, byte interval)
{
	if (obj->rects == NULL) return;
	obj->imgTimer++;
	if (obj->imgTimer == interval)
	{
		if (indices)
		{
			if (obj->animIndex == size - 1) obj->animIndex = 0;
			else obj->animIndex++;
			obj->imgIndex = indices[obj->animIndex];
		}
		else
		{
			if (obj->imgIndex == obj->columns * obj->lines - 1) obj->imgIndex = 0;
			else obj->imgIndex++;
		}
		obj->imgTimer = 0;
	}
}

void drawObject(Object *obj)
{
	if (obj->image)
	{
		if (obj->rects)
		{
			Rectangle r = obj->rects[obj->imgIndex];
			drawSurfaceSection(obj->image->surface, r,
				roundFloat(obj->bounds.position.x - obj->boundsPos.x), roundFloat(obj->bounds.position.y - obj->boundsPos.y));
		}
		else drawSurface(obj->image->surface,
			roundFloat(obj->bounds.position.x - obj->boundsPos.x), roundFloat(obj->bounds.position.y - obj->boundsPos.y));
	}
}

void freeObject(Object *obj)
{
	if (obj->image) freeImage(obj->image);
	if (obj->rects) free(obj->rects);
	free(obj);
}
