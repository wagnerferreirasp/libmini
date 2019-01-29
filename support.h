/** @file */

#ifndef MINI_SUPPORT_H
#define MINI_SUPPORT_H

#include <stdlib.h>
#include "SDL/SDL.h"
#include "SDL/SDL_image.h"

#define bool unsigned char
#define false 0
#define true 1
#define byte char
#define PI 3.1415926536f

#define Key SDLKey
#define KEY_UP SDLK_UP
#define KEY_RIGHT SDLK_RIGHT
#define KEY_DOWN SDLK_DOWN
#define KEY_LEFT SDLK_LEFT
#define KEY_A SDLK_a
#define KEY_B SDLK_b
#define KEY_C SDLK_c
#define KEY_D SDLK_d
#define KEY_E SDLK_e
#define KEY_F SDLK_f
#define KEY_G SDLK_g
#define KEY_H SDLK_h
#define KEY_I SDLK_i
#define KEY_J SDLK_j
#define KEY_K SDLK_k
#define KEY_L SDLK_l
#define KEY_M SDLK_m
#define KEY_N SDLK_n
#define KEY_O SDLK_o
#define KEY_P SDLK_p
#define KEY_Q SDLK_q
#define KEY_R SDLK_r
#define KEY_S SDLK_s
#define KEY_T SDLK_t
#define KEY_U SDLK_u
#define KEY_V SDLK_v
#define KEY_W SDLK_w
#define KEY_X SDLK_x
#define KEY_Y SDLK_y
#define KEY_Z SDLK_z
#define KEY_0 SDLK_0
#define KEY_1 SDLK_1
#define KEY_2 SDLK_2
#define KEY_3 SDLK_3
#define KEY_4 SDLK_4
#define KEY_5 SDLK_5
#define KEY_6 SDLK_6
#define KEY_7 SDLK_7
#define KEY_8 SDLK_8
#define KEY_9 SDLK_9
#define KEY_NUMPAD_0 SDLK_KP0
#define KEY_NUMPAD_1 SDLK_KP1
#define KEY_NUMPAD_2 SDLK_KP2
#define KEY_NUMPAD_3 SDLK_KP3
#define KEY_NUMPAD_4 SDLK_KP4
#define KEY_NUMPAD_5 SDLK_KP5
#define KEY_NUMPAD_6 SDLK_KP6
#define KEY_NUMPAD_7 SDLK_KP7
#define KEY_NUMPAD_8 SDLK_KP8
#define KEY_NUMPAD_9 SDLK_KP9
#define KEY_SPACE SDLK_SPACE
#define KEY_BACKSPACE SDLK_BACKSPACE
#define KEY_ENTER SDLK_RETURN
#define KEY_ESC SDLK_ESCAPE
#define KEY_F1 SDLK_F1
#define KEY_F2 SDLK_F2
#define KEY_F3 SDLK_F3
#define KEY_F4 SDLK_F4
#define KEY_F5 SDLK_F5
#define KEY_F6 SDLK_F6
#define KEY_F7 SDLK_F7
#define KEY_F8 SDLK_F8
#define KEY_F9 SDLK_F9
#define KEY_F10 SDLK_F10
#define KEY_F11 SDLK_F11
#define KEY_F12 SDLK_F12

#define MOUSE_DOUBLE_CLICK_DELAY 8
#define BUTTON_LEFT 1
#define BUTTON_MIDDLE 2
#define BUTTON_RIGHT 3

/// Um ponto com duas coordenadas de ponto flutuante
typedef struct {
	/// Coordenada x
	float x;

	/// Coordenada y
	float y;
} Point;

/// Um retângulo com posição e tamanho de ponto flutuante
typedef struct {
	/// Contém as coordenadas x e y do retângulo
	Point position;

	/// Contém a largura (size.x) e altura (size.y) do retângulo
	Point size;
} Rectangle;

/// Estrutura representando uma imagem, que encapsula uma superfície SDL
typedef struct {
	/// Superfície encapsulada
	SDL_Surface *surface;

	/// Largura da imagem
	short width;

	/// Altura da imagem
	short height;
} Image;

/// Nó de lista encadeada
typedef struct Node {
	/// Item armazenado no nó
	void *item;

	/// Ponteiro para o próximo nó da lista
	struct Node *prev;

	/// Ponteiro para o nó anterior da lista
	struct Node *next;
} Node;

/// Descritor de lista genérica duplamente encadeada
typedef struct {
	/// Cabeça da lista
	Node *head;

	/// Cauda da lista
	Node *tail;

	/// Total de nós não vazios na lista
	int size;
} List;

/// Cria uma lista genérica vazia
///
/// @return Uma lista genérica vazia
List *newList();

/// Adiciona um item a uma lista. Esse item deve ser dado como ponteiro void
///
/// @param list Lista onde o item será adicionado
/// @param item Item a adicionar
void addItem(List *list, void *item);

/// Insere um item numa posição específica de uma lista
///
/// @param list Lista onde o item será inserido
/// @param index Posição da lista onde o item será inserido. A posição logo após a cabeça tem índice 0
/// @param item Item a ser inserido
void insertItem(List *list, int index, void *item);

/// Remove o item de uma dada posição de uma lista
///
/// @param list Lista de onde o item será removido
/// @param index Posição da lista onde o item será removido. A posição logo após a cabeça tem índice 0
/// @param freeItem Função de liberação do item. Se for nula, o item não será deletado
void removeItem(List *list, int index, void (*freeItem)(void *));

/// Remove um nó de lista
///
/// @param list Lista de onde o nó será removido
/// @param node Nó a ser removido. Deve ser não-nulo
/// @param freeItem Função de liberação do item. Se for nula, o item não será deletado
void removeNode(List *list, Node *node, void (*freeItem)(void *));

/// Retorna um ponteiro para o item de uma lista na posição dada
///
/// @param list Lista onde procurar o item
/// @param index Posição do item na lista. A posição logo após a cabeça tem índice 0
void *getItem(List *list, int index);

/// Remove todos os itens de uma lista
///
/// @param list Lista a ser limpa
/// @param freeItem Função de liberação do item. Se for nula, os itens não serão deletados
void clearList(List *list, void (*freeItem)(void *));

/// Remove todos os itens de uma lista e libera a memória usada por ela
///
/// @param list Lista a ser deletada
/// @param freeItem Função de liberação do item. Se for nula, os itens não serão deletados
void freeList(List *list, void (*freeItem)(void *));

/// Libera a memória usada por uma lista vazia. Deve ser chamada quando os itens já foram removidos
///
/// @param list Lista a ser deletada
void freeListAux(List *list);

/// Cria um ponto
///
/// @param x Coordenada x do ponto
/// @param y Coordenada y do ponto
/// @return Um ponto com as coordenadas especificadas
Point newPoint(float x, float y);

/// Arredonda um float para o inteiro mais próximo
///
/// @param f O float a ser arredondado
/// @return Resultado do arredondamento
int roundFloat(float f);

/// Gera um inteiro aletório entre 'from' e 'to'
///
/// @param from Mínimo inteiro que pode ser gerado. Pode ser negativo
/// @param to Máximo inteiro que pode ser gerado. Pode ser negativo
/// @return Número gerado
int randomNumber(int from, int to);

/// Cria um retângulo
///
/// @param x Coordenada x do retângulo
/// @param y Coordenada y do retângulo
/// @param width Largura do retângulo
/// @param height Altura do retângulo
/// @return Um retângulo com os atributos especificados
Rectangle newRectangle(float x, float y, float width, float height);

/// Retorna se um retângulo intercepta outro
///
/// @param a Um retângulo
/// @param b Outro retângulo
/// @return Verdadeiro se há intersecção. Falso caso contrário
bool intersects(Rectangle a, Rectangle b);

/// Cria uma imagem
///
/// @param fileName Nome do arquivo de imagem (.bmp, .png, entre outros)
/// @return A imagem carregada
Image *newImage(const char *fileName);

/// Libera a memória usada por uma imagem (estrutura Image)
///
/// @param img Imagem a ser deletada
void freeImage(Image *img);

/// Função que aloca espaço na memória e encerra o programa caso haja erro
///
/// @param size Espaço em bytes a ser alocado
/// @return Ponteiro para o espaço alocado
void *safeMalloc(size_t size);

#endif

