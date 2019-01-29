/** @file */

#ifndef MINI_PARTICLE_H
#define MINI_PARTICLE_H

#include "object.h"
#include <math.h>

/// Estrutura que representa um objeto com propriedades físicas, que pode ser usado para movimentação baseada em forças e para tratar colisões
typedef struct{
	/// Estrutura básica do objeto
	Object *obj;

	/// Velocidade atual do objeto, decomposta nos eixos x e y
	Point speed;

	/// Massa do objeto
	float mass;

	/// Velocidade máxima permitida para o objeto
	float maxSpeed;

	/// Objeto com o qual este está colidindo acima (se não houver colisão acima, será nulo)
	Object *top;

	/// Objeto com o qual este está colidindo à direita (se não houver colisão à direita, será nulo)
	Object *right;

	/// Objeto com o qual este está colidindo abaixo (se não houver colisão abaixo, será nulo)
	Object *bottom;

	/// Objeto com o qual este está colidindo à esquerda (se não houver colisão à esquerda, será nulo)
	Object *left;
} Particle;

/// Cria uma nova partícula (objeto com propriedades físicas)
///
/// @param obj Objeto básico que essa partícula representará
/// @param maxSpeed Velocidade máxima permitida para a partícula
/// @param mass Massa da partícula
/// @return A partícula gerada
Particle *newParticle(Object *obj, float maxSpeed, float mass);

/// Define a velocidade de uma partícula
///
/// @param part Partícula cuja velocidade deve ser alterada
/// @param xSpeed Nova componente x da velocidade
/// @param ySpeed Nova componente y da velocidade
void setSpeed(Particle *part, float xSpeed, float ySpeed);

/// Define as forças que agem sobre a partícula. Essas forças são usadas para calcular a aceleração e, a partir dessa, alterar a velocidade da partícula. Consecutivas chamadas a essa função resultarão na soma das forças especificadas
///
/// @param part Partícula sendo afetada pela(s) força(s)
/// @param xForces Componente x da(s) força(s) sobre a partícula
/// @param yForces Componente y da(s) força(s) sobre a partícula
void setForces(Particle *part, float xForces, float yForces);

/// Movimenta uma partícula usando sua velocidade atual. Se a lista 'obstacles' não for nula, será efetuada checagem de colisão e resposta para cada objeto nessa lista, que pode conter Particles ou Objects
///
/// @param part Partícula a ser movimentada
/// @param obstacles Lista de obstáculos a serem considerados para tratamento de colisão
/// @param particles Deve ser verdadeiro se os itens da lista são do tipo Particle, falso para itens do tipo Object
void moveParticle(Particle *part, List *obstacles, bool particles);

/// Libera a memória usada por uma partícula
///
/// @param part Partícula a ser deletada
void freeParticle(Particle *part);

#endif
