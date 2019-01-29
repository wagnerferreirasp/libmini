#include "support.h"

void checkIndex(List *, int);
void clearItems(List *list, void (*freeItem)(void *));

List *newList()
{
	List *list = (List *)safeMalloc(sizeof(List));
	list->head = (Node *)safeMalloc(sizeof(Node));
	list->tail = (Node *)safeMalloc(sizeof(Node));
	list->head->item = list->head->prev = list->tail->item = list->tail->next = NULL;
	list->head->next = list->tail;
	list->tail->prev = list->head;
	list->size = 0;

	return list;
}

void addItem(List *list, void *item)
{
    Node *newNode = (Node *)safeMalloc(sizeof(Node));
    newNode->item = item;
    newNode->next = list->tail;
    newNode->prev = list->tail->prev;
    list->tail->prev->next = newNode;
    list->tail->prev = newNode;
    list->size++;
}

void insertItem(List *list, int index, void *item)
{
	checkIndex(list, index);

	Node *aux = list->head, *newNode = (Node *)safeMalloc(sizeof(Node));
	newNode->item = item;
	int i = 0;
	while (i++ < index)
		aux = aux->next;
	newNode->prev = aux;
	newNode->next = aux->next;
	aux->next->prev = newNode;
	aux->next = newNode;
	list->size++;
}

void removeItem(List *list, int index, void (*freeItem)(void *))
{
	checkIndex(list, index);

	Node *aux = list->head->next;
	int i = 0;
	while (i++ < index)
		aux = aux->next;
	aux->prev->next = aux->next;
	aux->next->prev = aux->prev;
	if (freeItem) freeItem(aux->item);
	free(aux);
	list->size--;
}

void removeNode(List *list, Node *node, void (*freeItem)(void *))
{
	node->prev->next = node->next;
	node->next->prev = node->prev;
	if (freeItem) freeItem(node->item);
	free(node);
	list->size--;
}

void *getItem(List *list, int index)
{
	checkIndex(list, index);

	Node *aux = list->head->next;
	int i = 0;
	while (i++ < index)
		aux = aux->next;
	return aux->item;
}

void clearList(List *list, void (*freeItem)(void *))
{
	clearItems(list, freeItem);
	list->head->next = list->tail;
	list->tail->prev = list->head;
	list->size = 0;
}

void freeList(List *list, void (*freeItem)(void *))
{
	clearItems(list, freeItem);
	freeListAux(list);
}

void freeListAux(List *list)
{
	free(list->head);
	free(list->tail);
	free(list);
}

Point newPoint(float x, float y)
{
	Point p;
	p.x = x; p.y = y;
	return p;
}
int roundFloat(float f)
{
	if (f < 0) return (int)(f - 0.5f);
	else return (int)(f + 0.5f);
}
int randomNumber(int from, int to)
{
	return roundFloat((to - from) * ((float)rand()/RAND_MAX) + from);
}

Rectangle newRectangle(float x, float y, float width, float height)
{
	Rectangle r;
	r.position = newPoint(x, y);
	r.size = newPoint(width, height);
	return r;
}
bool intersects(Rectangle a, Rectangle b)
{
	return
		a.position.x + a.size.x > b.position.x && b.position.x + b.size.x > a.position.x &&
		a.position.y + a.size.y > b.position.y && b.position.y + b.size.y > a.position.y;
}

Image *newImage(const char *fileName)
{
	Image *img = (Image *)safeMalloc(sizeof(*img));
	img->surface = IMG_Load(fileName);
	SDL_Surface *opt = SDL_DisplayFormatAlpha(img->surface);
	if (opt)
	{
		SDL_FreeSurface(img->surface);
		img->surface = opt;
	}
	SDL_SetAlpha(img->surface, SDL_RLEACCEL | SDL_SRCALPHA, SDL_ALPHA_OPAQUE);
	img->width = img->surface->w;
	img->height = img->surface->h;
	return img;
}
void freeImage(Image *img)
{
	SDL_FreeSurface(img->surface);
	free(img);
}

void *safeMalloc(size_t size)
{
	void *p = malloc(size);
	if (p == NULL)
	{
		printf("Falta de memória!\n");
		exit(EXIT_FAILURE);
	}
	return p;
}

void checkIndex(List *list, int index)
{
	if (index > list->size-1 || index < 0)
	{
		printf("Invalid index!\n");
		exit(EXIT_FAILURE);
	}
}

void clearItems(List *list, void (*freeItem)(void *))
{
	Node *n = list->head->next;
	while (n != list->tail)
	{
		Node *next = n->next;
		if (freeItem) freeItem(n->item);
		free(n);
		n = next;
	}
}

