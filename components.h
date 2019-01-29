/** @file */

#ifndef MINI_COMPONENTS_H
#define MINI_COMPONENTS_H

#include "object.h"

/// Representa o estado inicial de um botão (Button)
#define BUTTON_STATE_UP 0

/// Representa o estado de um botão (Button) quando o mouse está pousado sobre ele
#define BUTTON_STATE_OVER 1

/// Representa o estado de um botão (Button) quando o mouse está sobre ele e com o botão esquerdo pressionado
#define BUTTON_STATE_DOWN 2

/// Representa o estado de um botão (Button) quando o mouse sai da área dele ainda com o botão esquerdo pressionado
#define BUTTON_STATE_DOWN_OUT 3

/// Representa o estado de um botão (Button) quando o mouse completa um clique sobre ele
#define BUTTON_STATE_CLICKED 4

/// Total de teclas que são processadas pelos campos de texto (TextField)
#define NUM_VALID_KEYS 48

/// Intervalo em frames (o jogo roda aproximadamente a 60 frames por segundo) entre cada piscada do cursor de um campo de texto  (TextField)
#define CURSOR_BLINK_INTERVAL 40

/// Estrutura representando um botão
typedef struct {
	/// Objeto que representa o botão
	Object *obj;
	
	/// Texto do botão
	char *text;
	
	/// Superfície contendo o texto renderizado do botão
	SDL_Surface *textImg;
	
	/// Posição relativa do texto em relação à origem do botão (canto superior esquerdo)
	Point textPos;
	
	/// Estado atual do botão, assumindo os valores BUTTON_STATE_UP, BUTTON_STATE_OVER, BUTTON_STATE_DOWN, BUTTON_STATE_DOWN_OUT ou  BUTTON_STATE_CLICKED
	byte state;
} Button;

/// Estrutura representando um campo de texto
typedef struct {
	/// Objeto que representa o campo de texto
	Object *box;
	
	/// Objeto que representa o cursor
	Object *cursor;
	
	/// Fonte usada para o texto
	Font *font;
	
	/// Texto do campo
	char *text;
	
	/// Superfície contendo o texto renderizado
	SDL_Surface *textImg;
	
	/// Cor do texto
	Color textColor;
	
	/// Comprimento (número de caracteres) atual do texto
	byte length;
	
	/// Comprimento máximo permitido para o texto
	byte maxLength;
	
	/// Variável usada para controlar a animação do cursor
	byte cursorTimer;
	
	/// Determina se o cursor deve ser desenhado no frame atual
	bool showCursor;
} TextField;

/// Cria um botão com os parâmetros especificados
///
/// @param pos Posição da tela onde o botão será desenhado
/// @param img Imagem contendo os três estados do botão: normal, mouse pousado sobre e clicado
/// @param font Fonte usada para o texto do botão
/// @param text Texto do botão
/// @param color Cor do texto do botão
/// @return Um ponteiro para uma estrutura Button
Button *newButton(Point pos, Image *img, Font *font, char *text, Color color);

/// Cria um campo de texto com os parâmetros especificados
///
/// @param pos Posição da tela onde o campo de texto será desenhado
/// @param box Imagem do campo de texto
/// @param cursor Imagem do cursor usado no campo de texto
/// @param maxLength Máximo de caracteres que podem ser digitados no campo de texto
/// @param font Fonte para o texto
/// @param color Cor do texto
/// @return Um ponteiro para uma estrutura TextField
TextField *newTextField(Point pos, Image *box, Image *cursor, byte maxLength, Font *font, Color color);

/// Realiza toda a lógica de atualização do botão, verificando se foi clicado, se o mouse está pousado sobre ele, etc.
///
/// @param btn O botão a ser atualizado
void updateButton(Button *btn);

/// Realiza toda a lógica de atualização do campo de texto, atualizando o texto em si e fazendo a animação do cursor
///
/// @param txt O campo de texto a ser atualizado
void updateTextField(TextField *txt);

/// Desenha um botão na tela
///
/// @param btn O botão a ser desenhado
void drawButton(Button *btn);

/// Desenha um campo de texto na tela
///
/// @param txt O campo de texto a ser desenhado
void drawTextField(TextField *txt);

/// Libera a memória usada por um botão
///
/// @param btn O botão a ser deletado
void freeButton(Button *btn);

/// Libera a memória usada por um campo de texto
///
/// @param txt O campo de texto a ser deletado
void freeTextField(TextField *txt);

#endif

