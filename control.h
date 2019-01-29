/** @file */

#ifndef MINI_CONTROL_H
#define MINI_CONTROL_H

#include <stdio.h>
#include "SDL/SDL_mixer.h"
#include "SDL/SDL_ttf.h"
#include "support.h"

#define Font TTF_Font
#define Color SDL_Color
#define Sound Mix_Chunk
#define Music Mix_Music

/// Total de canais de som (que determina a quantidade de sons simultâneos) disponibilizados para o jogo
#define SOUND_CHANNELS 5

/// Inicializa o subsistema de vídeo da SDL, juntamente com o sistema SDL_TTF
///
/// @param windowTitle Título para a janela do jogo
/// @param icon Ícone para a janela
/// @param size Tamanho, em pixels, da janela
/// @param fullScreen Determina se o jogo deve ser aberto em full screen
void initializeVideo(const char *windowTitle, const char *icon, Point size, bool fullScreen);

/// Inicializa o sistema de audio SDL_Mixer
void initializeAudio();

/// Inicializa o sistema de tratamento da entrada. Os códigos de teclas e botões do mouse estão definidos em 'support.h'
void initializeInput();

/// Executa o laço principal do jogo, chamando as funções de atualização e desenho aproximadamente 60 vezes por segundo
///
/// @param updateFunc Função de atualização
/// @param drawFunc Função de desenho
/// @param end Ponteiro para um booleano que deve ser setado para verdadeiro para finalizar o jogo
void runGameLoop(void (*updateFunc)(), void (*drawFunc)(), bool *end);

/// Finaliza todos os sistemas. Deve ser chamado após o término do laço principal do jogo
void finalize();

/// Define o modo full screen
///
/// @param fullScreen Verdadeiro para full screen, falso para janela
void setFullScreen(bool fullScreen);

/// Limpa a tela com a cor padrão (preto)
void clearScreen();

/// Limpa a tela com a cor especificada
///
/// @param color Cor para limpar a tela
void clearScreenColor(Color color);

/// Desenha uma imagem dada por uma SDL_Surface
///
/// @param surface A SDL_Surface a ser desenhada
/// @param x Coordenada x na tela para desenhar a imagem
/// @param y Coordenada y na tela para desenhar a imagem
void drawSurface(SDL_Surface *surface, short x, short y);

/// Desenha uma seção retangular de uma imagem
///
/// @param surface A imagem original
/// @param section Um retângulo representando a seção (coordenadas relativas à origem da imagem e largura e altura da seção) a ser desenhada
/// @param x Coordenada x na tela para desenhar a seção da imagem
/// @param y Coordenada y na tela para desenhar a seção da imagem
void drawSurfaceSection(SDL_Surface *surface, Rectangle section, short x, short y);

/// Cria uma nova fonte no sistema SDL_TTF
///
/// @param fileName Nome do arquivo da fonte (.ttf)
/// @param size Tamanho em pixels da fonte
/// @return Uma estrutura SDL_Font (encapsulada como Font)
Font *newFont(const char *fileName, int size);

/// Cria uma cor no padrão RGB
///
/// @param r Componente red (vermelho) da cor
/// @param g Componente green (verde) da cor
/// @param b Componente blue (azul) da cor
/// @return Uma cor encapsulada na estrutura Color
Color newColor(Uint8 r, Uint8 g, Uint8 b);

/// Desenha texto utilizando uma fonte do sistema SDL_TTF
///
/// @param font A fonte a ser usada
/// @param text Texto a ser renderizado
/// @param color Cor para desenhar o texto
/// @param pos Posição na tela onde desenhar o texto
void drawText(Font *font, const char *text, Color color, Point pos);

/// Retorna uma SDL_Surface contendo texto renderizado pelo sistema SDL_TTF. Usar essa função o lugar de drawText quando for preciso saber, por exemplo, o tamanho da imagem gerada
///
/// @param font A fonte a ser usada
/// @param text Texto a ser renderizado
/// @param color Cor para desenhar o texto
SDL_Surface *getDrawnText(Font *font, const char *text, Color color);

/// Libera a memória usada por uma fonte do sistema SDL_TTF
///
/// @param font A fonte a ser deletada
void freeFont(Font *font);

/// Define parâmetros para manipulação do teclado
///
/// @param heldDelay Intervalo em frames desde o pressionamento de uma tecla até que passe a ser considerado o evento tratado pela função isKeyHeld (tecla segurada)
/// @param heldInterval Intervalo em frames entre cada ocorrência do evento de tecla segurada, após ocorrência do primeiro
void setKeyboardParameters(Uint8 heldDelay, Uint8 heldInterval);

/// Retorna se uma dada tecla está pressionada no frame atual
///
/// @param key Código da tecla a ser verificada
/// @return Verdadeiro se a tecla 'key' está pressionada
bool isKeyDown(Key key);

/// Retorna se uma dada tecla foi pressionada no frame atual (não estava pressionada no frame anterior)
///
/// @param key Código da tecla a ser verificada
/// @return Verdadeiro se a tecla 'key' foi pressionada nesse frame
bool isKeyPressed(Key key);

/// Retorna se uma dada tecla foi solta no frame atual (estava pressionada no frame anterior)
///
/// @param key Código da tecla a ser verificada
/// @return Verdadeiro se a tecla 'key' foi solta nesse frame
bool isKeyReleased(Key key);

/// Retorna se uma dada tecla está sendo segurada (mais detalhes na função setKeyboardParameters)
///
/// @param key Código da tecla a ser verificada
/// @return Verdadeiro se a tecla 'key' está sendo segurada
bool isKeyHeld(Key key);

/// Define parâmetros para manipulação do mouse
///
/// @param heldDelay Intervalo em frames desde o pressionamento de um botão até que passe a ser considerado o evento tratado pela função isMouseHeld (botão segurado)
/// @param heldInterval Intervalo em frames entre cada ocorrência do evento de botão segurado, após ocorrência do primeiro
void setMouseParameters(Uint8 heldDelay, Uint8 heldInterval);

/// Retorna a posição atual do mouse (relativa ao canto superior esquerdo da janela do jogo)
///
/// @return Um ponto (estrutura Point) com as coordenadas do mouse
Point getMousePosition();

/// Retorna se o mouse está sobre uma dada área retangular
///
/// @param rect Área a ser considerada
bool isMouseOver(Rectangle rect);

/// Retorna se um dado botão do mouse está pressionado no frame atual
///
/// @param button Código do botão a ser verificado
/// @return Verdadeiro se o botão 'button' está pressionado
bool isMouseDown(Uint8 button);

/// Retorna se um dado botão do mouse foi pressionado no frame atual (não estava pressionado no frame anterior)
///
/// @param button Código do botão a ser verificado
/// @return Verdadeiro se o botão 'button' foi pressionado nesse frame
bool isMousePressed(Uint8 button);

/// Retorna se um dado botão do mouse foi solto no frame atual (estava pressionado no frame anterior)
///
/// @param button Código do botão a ser verificado
/// @return Verdadeiro se o botão 'button' está pressionado
bool isMouseReleased(Uint8 button);

/// Retorna se um dado botão do mouse está sendo segurado (mais detalhes na função setMouseParameters)
///
/// @param button Código do botão a ser verificado
/// @return Verdadeiro se o botão 'button' está sendo segurado
bool isMouseHeld(Uint8 button);

/// Retorna se um dado botão do mouse completou um duplo-clique no frame atual
///
/// @param button Código do botão a ser verificado
/// @return Verdadeiro se o botão 'button' completou duplo-clique nesse frame
bool isMouseDoubleClicked(Uint8 button);

/// Cria um som no sistema SDL_Mixer
///
/// @param fileName Nome do arquivo de som (.wav, .ogg, entre outros)
/// @return Um som numa estrutura Mix_Chunk (encapsulada como Sound)
Sound *newSound(const char *fileName);

/// Toca um som com o volume dado
///
/// @param sound Som a ser tocado
/// @param volume Volume para tocar o som (valores entre 0 e 1, onde 0 é mudo e 1 é o volume máximo)
void playSound(Sound *sound, float volume);

/// Libera a memória usada por um som (estrutura Sound)
///
/// @param sound Som a ser deletado
void freeSound(Sound *sound);

/// Cria uma nova música no sistema SDL_Mixer
///
/// @param fileName Nome do arquivo de música (.mp3, .ogg entre outros)
/// @return Uma música numa estrutura Mix_Music (encapsulada como Music)
Music *newMusic(const char *fileName);

/// Toca uma música repetidamente com o dado volume
///
/// @param music Música a ser tocada
/// @param volume Volume para tocar a música (valores entre 0 e 1, onde 0 é mudo e 1 é o volume máximo)
void playMusic(Music *music, float volume);

/// Altera o volume de uma música, podendo ser executado enquanto a música é tocada
///
/// @param music Música cuja volume deve ser alterado
/// @param volume Novo volume para a música
void setMusicVolume(Music *music, float volume);

/// Pausa ou retoma a execução de uma música
///
/// @param music Música a ser pausada/retomada
void toggleMusicPlaying(Music *music);

/// Libera a memória usada por uma música (estrutura Music)
///
/// @param music Música a ser deletada
void freeMusic(Music *music);

#endif

