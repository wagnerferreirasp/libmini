#include "control.h"

SDL_Surface *screen;
SDL_Rect screenRect;
Uint8 *keys, *prevKeys, keyHeldDelay, keyHeldInterval, mouseHeldDelay, mouseHeldInterval;
Uint8 mouse, prevMouse;
int numKeys, *keyTimers, mouseX, mouseY, *mouseTimers, ms;
bool *mouseDouble;

void initializeVideo(const char *windowTitle, const char *icon, Point size, bool fullScreen)
{
	if (SDL_Init(SDL_INIT_AUDIO | SDL_INIT_VIDEO) < 0)
	{
		printf("Erro ao iniciar: %s\n", SDL_GetError());
		exit(1);
	}
	screenRect.x = 0; screenRect.y = 0; screenRect.w = size.x; screenRect.h = size.y;
	screen = SDL_SetVideoMode(screenRect.w, screenRect.h, 0, SDL_ANYFORMAT | SDL_SWSURFACE | (fullScreen ? SDL_FULLSCREEN : 0));
	SDL_WM_SetCaption(windowTitle, windowTitle);

	if(icon)
	{
		SDL_Surface *icone = IMG_Load(icon);
		SDL_WM_SetIcon(icone, NULL);
	}

	TTF_Init();
}

void initializeAudio()
{
	Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096);
	Mix_AllocateChannels(SOUND_CHANNELS);
}

void initializeInput()
{
	keys = SDL_GetKeyState(&numKeys);
	prevKeys = (Uint8 *)safeMalloc(numKeys);
	keyTimers = (int *)calloc(sizeof(int), numKeys);
	keyHeldDelay = 40;
	keyHeldInterval = 5;
	mouse = SDL_GetMouseState(&mouseX, &mouseY);
	mouseTimers = (int *)calloc(sizeof(int), 3);
	mouseDouble = (bool *)calloc(sizeof(bool), 3);
	mouseHeldDelay = 40;
	mouseHeldInterval = 5;
}

bool startFrame()
{
	ms = SDL_GetTicks();

	memcpy(prevKeys, keys, numKeys);
	prevMouse = mouse;

	SDL_Event event;
	while (SDL_PollEvent(&event))
		if (event.type == SDL_QUIT) return true;
	mouse = SDL_GetMouseState(&mouseX, &mouseY);

	int i = 0;
	for (i = 0; i < numKeys; i++)
		if (isKeyDown(i)) keyTimers[i]++;
		else keyTimers[i] = 0;

	for (i = 0; i < 3; i++)
	{
		if (isMousePressed(i + 1))
		{
			if (mouseTimers[i] >= -MOUSE_DOUBLE_CLICK_DELAY) mouseDouble[i] = true;
			mouseTimers[i] = 0;
		}
		else if (isMouseDown(i + 1))
			mouseTimers[i]++;
		else if (isMouseReleased(i + 1))
			mouseTimers[i] = 0;
		else mouseTimers[i]--;
	}

	return false;
}

void endFrame()
{
	SDL_Flip(screen);
	int frame = SDL_GetTicks() - ms, i;
	if (frame < 17) SDL_Delay(17 - frame);
	for (i = 0; i < 3; i++)
		mouseDouble[i] = false;
}

void runGameLoop(void (*updateFunc)(), void (*drawFunc)(), bool *end)
{
	*end |= startFrame();
	while (!(*end))
	{
		updateFunc();
		drawFunc();
		endFrame();
		*end |= startFrame();
	}
}

void finalize()
{
	free(prevKeys);
	Mix_CloseAudio();
	TTF_Quit();
	SDL_Quit();
}

void setFullScreen(bool fullScreen)
{
	screen = SDL_SetVideoMode(screenRect.w, screenRect.h, 0, SDL_ANYFORMAT | SDL_SWSURFACE | (fullScreen ? SDL_FULLSCREEN : 0));
}

void clearScreen()
{
	SDL_FillRect(screen, &screenRect, SDL_MapRGB(screen->format, 0, 0, 0));
}
void clearScreenColor(Color color)
{
	SDL_FillRect(screen, &screenRect, SDL_MapRGB(screen->format, color.r, color.g, color.b));
}

void drawSurface(SDL_Surface *surface, short x, short y)
{
	SDL_Rect r = {x, y, 0, 0};
	SDL_BlitSurface(surface, NULL, screen, &r);
}
void drawSurfaceSection(SDL_Surface *surface, Rectangle section, short x, short y)
{
	SDL_Rect r = {x, y, 0, 0}, src = {section.position.x, section.position.y, section.size.x, section.size.y};
	SDL_BlitSurface(surface, &src, screen, &r);
}

Font *newFont(const char *fileName, int size)
{
	return TTF_OpenFont(fileName, size);
}
Color newColor(Uint8 r, Uint8 g, Uint8 b)
{
	Color c; c.r = r; c.g = g; c.b = b;
	return c;
}
void drawText(Font *font, const char *text, Color color, Point pos)
{
	SDL_Surface *s = getDrawnText(font, text, color);
	SDL_Rect r = {pos.x, pos.y, 0, 0};
	SDL_BlitSurface(s, NULL, screen, &r);
	SDL_FreeSurface(s);
}
SDL_Surface *getDrawnText(Font *font, const char *text, Color color)
{
	return TTF_RenderUTF8_Blended(font, text, color);
}
void freeFont(Font *font)
{
	TTF_CloseFont(font);
}

void setKeyboardParameters(Uint8 heldDelay, Uint8 heldInterval)
{
	keyHeldDelay = heldDelay;
	keyHeldInterval = heldInterval;
}
bool isKeyDown(Key key)
{
	return keys[key];
}
bool isKeyPressed(Key key)
{
	return keys[key] & !(prevKeys[key]);
}
bool isKeyReleased(Key key)
{
	return prevKeys[key] & !(keys[key]);
}
bool isKeyHeld(Key key)
{
	return (keyTimers[key] >= keyHeldDelay) && ((keyTimers[key] - keyHeldDelay) % keyHeldInterval == 0);
}

void setMouseParameters(Uint8 heldDelay, Uint8 heldInterval)
{
	mouseHeldDelay = heldDelay;
	mouseHeldInterval = heldInterval;
}
Point getMousePosition()
{
	Point p;
	p.x = mouseX; p.y = mouseY;
	return p;
}
bool isMouseOver(Rectangle rect)
{
	Point p = getMousePosition();
	return p.x >= rect.position.x && p.x < rect.position.x + rect.size.x &&
		p.y >= rect.position.y && p.y < rect.position.y + rect.size.y;
}
bool isMouseDown(Uint8 button)
{
	return mouse & SDL_BUTTON(button);
}
bool isMousePressed(Uint8 button)
{
	return (mouse & SDL_BUTTON(button)) && !(prevMouse & SDL_BUTTON(button));
}
bool isMouseReleased(Uint8 button)
{
	return (prevMouse & SDL_BUTTON(button)) && !(mouse & SDL_BUTTON(button));
}
bool isMouseHeld(Uint8 button)
{
	return mouseTimers[button-1] >= mouseHeldDelay && ((mouseTimers[button-1] - mouseHeldDelay) % mouseHeldInterval == 0);
}
bool isMouseDoubleClicked(Uint8 button)
{
	return mouseDouble[button-1];
}

Sound *newSound(const char *fileName)
{
	return Mix_LoadWAV(fileName);
}
void playSound(Sound *sound, float volume)
{
	Mix_VolumeChunk(sound, volume * MIX_MAX_VOLUME);

	byte i;
	for (i = 0; i < SOUND_CHANNELS; i++)
		if (!Mix_Playing(i))
		{
			Mix_PlayChannel(i, sound, 0);
			return;
		}
}
void freeSound(Sound *sound)
{
	Mix_FreeChunk(sound);
}

Music *newMusic(const char *fileName)
{
	return Mix_LoadMUS(fileName);
}
void playMusic(Music *music, float volume)
{
	Mix_VolumeMusic(volume * MIX_MAX_VOLUME);
	Mix_PlayMusic(music, -1);
}
void setMusicVolume(Music *music, float volume)
{
	Mix_VolumeMusic(volume * MIX_MAX_VOLUME);
}
void toggleMusicPlaying(Music *music)
{
	if (Mix_PausedMusic()) Mix_ResumeMusic();
	else Mix_PauseMusic();
}
void freeMusic(Music *music)
{
	Mix_HaltMusic();
	Mix_FreeMusic(music);
}

