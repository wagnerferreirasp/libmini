/** @file */

#ifndef MINI_OBJECT_H
#define MINI_OBJECT_H

#include "control.h"

/// Estrutura que representa um objeto de jogo, em geral definido por uma posição, caixa de colisão e imagem
typedef struct {
	/// Limites do objeto, isto é, sua caixa de colisão
	Rectangle bounds;
	
	/// Conjunto de retângulos usado para desenho de sprite sheets. Será nulo se o objeto não tiver uma sprite sheet
	Rectangle *rects;
	
	/// Posição relativa da caixa de colisão em relação à imagem do objeto
	Point boundsPos;
	
	/// Imagem do objeto
	Image *image;
	
	/// Colunas da sprite sheet
	byte columns;
	
	/// Linhas da sprite sheet
	byte lines;
	
	/// Índice da imagem atual na sprite sheet. Os índices são contados da esquerda para a direita, de cima para baixo, sendo que o primeiro é 0
	byte imgIndex;
	
	/// Contador de frames usado para a animação do objeto
	byte imgTimer;
	
	/// Variável auxiliar para a animação do objeto
	byte animIndex;
} Object;

/// Cria um objeto simples com posição e imagem. Sua caixa de colisão terá o mesmo tamanho da imagem
///
/// @param pos Posição do objeto
/// @param img Imagem do objeto
/// @return O objeto gerado
Object *newBlock(Point pos, Image *img);

/// Cria um objeto com posição, imagem e caixa de colisão especificadas
///
/// @param pos Posição do objeto
/// @param size Tamanho da caixa de colisão do objeto
/// @param boundsPos Posição relativa da caixa de colisão em relação à imagem do objeto
/// @param img Imagem do objeto
/// @return O objeto gerado
Object *newObject(Point pos, Point size, Point boundsPos, Image *img);

/// Cria um objeto com posição e sprite sheet. A caixa de colisão terá o tamanho de uma imagem da sprite sheet
///
/// @param pos Posição do objeto
/// @param spriteSheet Sprite sheet do objeto
/// @param columns Número de colunas da sprite sheet dada
/// @param lines Número de linhas da sprite sheet dada
/// @return O objeto gerado
Object *newBlockSprite(Point pos, Image *spriteSheet, byte columns, byte lines);

/// Cria um objeto com posição, sprite sheet e caixa de colisão especificadas
///
/// @param pos Posição do objeto
/// @param size Tamanho da caixa de colisão do objeto
/// @param boundsPos Posição relativa da caixa de colisão em relação à imagem do objeto
/// @param spriteSheet Sprite sheet do objeto
/// @param columns Número de colunas da sprite sheet dada
/// @param lines Número de linhas da sprite sheet dada
/// @return O objeto gerado
Object *newSprite(Point pos, Point size, Point boundsPos, Image *spriteSheet, byte columns, byte lines);

/// Retorna a caixa de colisão de um objeto
///
/// @param obj Objeto cuja caixa de colisão deve ser retornada
/// @return A caixa de colisão do objeto
Rectangle getBounds(Object *obj);

/// Define a posição de um objeto
///
/// @param obj Objeto a ser posicionado
/// @param pos Nova posição do objeto
void setPosition(Object *obj, Point pos);

/// Retorna a coordenada x da posição de um objeto
///
/// @param obj Objeto cuja posição deve ser retornada
/// @return A coordenada x do objeto
float getX(Object *obj);

/// Retorna a coordenada y da posição de um objeto
///
/// @param obj Objeto cuja posição deve ser retornada
/// @return A coordenada y do objeto 
float getY(Object *obj);

/// Define a coordenada x da posição de um objeto
///
/// @param obj Objeto cuja posição deve ser alterada
/// @param x Nova coordenada x do objeto
void setX(Object *obj, float x);

/// Define a coordenada y da posição de um objeto
///
/// @param obj Objeto cuja posição deve ser alterada
/// @param y Nova coordenada y do objeto
void setY(Object *obj, float y);

/// Retorna a largura da caixa de colisão de um objeto
///
/// @param obj Objeto cuja largura deve ser retornada
/// @return A largura do objeto 
float getWidth(Object *obj);

/// Retorna a altura da caixa de colisão de um objeto
///
/// @param obj Objeto cuja altura deve ser retornada
/// @return A altura do objeto 
float getHeight(Object *obj);

/// Redefine a caixa de colisão de um objeto
///
/// @param obj O objeto a ser redefinido
/// @param x Nova coordenada x da caixa de colisão em relação à imagem do objeto
/// @param y Nova coordenada y da caixa de colisão em relação à imagem do objeto
/// @param width Nova largura da caixa de colisão
/// @param height Nova altura da caixa de colisão
void setBounds(Object *obj, float x, float y, float width, float height);

/// Coloca o centro da caixa de colisão de um objeto numa dada posição
///
/// @param obj Objeto a ser posicionado
/// @param pos Nova posição do objeto
void setCenter(Object *obj, Point pos);

/// Move um objeto em relação à sua posição atual
///
/// @param obj Objeto a ser movido
/// @param xVar Variação em x da posição
/// @param yVar Variação em y da posição
void move(Object *obj, float xVar, float yVar);

/// Faz a animação de um objeto com sprite sheet. Essa função deve ser chamada a cada iteração do loop principal, para uma animação contínua
///
/// @param obj Objeto a ser animado
/// @param indices Vetor com a sequência de índices da sprite sheet a serem usados. Se for nulo, serão usados índices em ordem crescente a partir de 0
/// @param size Tamanho do vetor 'indices'. Se 'indices' for nulo, esse parâmetro será ignorado
/// @param interval Intervalo em frames entre cada passo da animação (cada alteração de imagem) 
void animate(Object *obj, byte *indices, byte size, byte interval);

/// Desenha um objeto na tela
///
/// @param obj Objeto a ser desenhado
void drawObject(Object *obj);

/// Libera a memória usada por um objeto
///
/// @param obj Objeto a ser deletado
void freeObject(Object *obj);

#endif

