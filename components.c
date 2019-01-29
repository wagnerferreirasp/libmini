#include "components.h"

const Key VALID_KEYS[] = {KEY_A, KEY_B, KEY_C, KEY_D, KEY_E, KEY_F, KEY_G, KEY_H, KEY_I, KEY_J, KEY_K, KEY_L, KEY_M, KEY_N, KEY_O, KEY_P, KEY_Q, KEY_R, KEY_S, KEY_T, KEY_U, KEY_V, KEY_W, KEY_X, KEY_Y, KEY_Z, KEY_0, KEY_1, KEY_2, KEY_3, KEY_4, KEY_5, KEY_6, KEY_7, KEY_8, KEY_9, KEY_NUMPAD_0, KEY_NUMPAD_1, KEY_NUMPAD_2, KEY_NUMPAD_3, KEY_NUMPAD_4, KEY_NUMPAD_5, KEY_NUMPAD_6, KEY_NUMPAD_7, KEY_NUMPAD_8, KEY_NUMPAD_9, KEY_SPACE, KEY_BACKSPACE};
const char VALID_CHARS[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ01234567890123456789 ";

Button *newButton(Point pos, Image *imgs, Font *font, char *text, Color color)
{
	Button *btn = (Button *)safeMalloc(sizeof(*btn));
	btn->obj = newBlockSprite(pos, imgs, 1, 3);
	btn->text = text;
	if (text) btn->textImg = getDrawnText(font, text, color);
	else btn->textImg = NULL;
	if (text)
		btn->textPos = newPoint((getBounds(btn->obj).size.x - btn->textImg->w) / 2, (getBounds(btn->obj).size.y - btn->textImg->h) / 2);
	btn->state = BUTTON_STATE_UP;
	return btn;
}

TextField *newTextField(Point pos, Image *box, Image *cursor, byte maxLength, Font *font, Color color)
{
	TextField *txt = (TextField *)safeMalloc(sizeof(*txt));
	txt->box = newBlock(pos, box);
	Point curPos = newPoint(pos.x + 3, pos.y + 3);
	txt->cursor = newBlock(curPos, cursor);
	txt->font = font;
	txt->text = (char *)safeMalloc((maxLength + 1) * sizeof(char));
	txt->text[0] = 0;
	txt->textImg = NULL;
	txt->textColor = color;
	txt->length = 0;
	txt->maxLength = maxLength;
	txt->cursorTimer = 0;
	txt->showCursor = true;
	return txt;
}

void updateButton(Button *btn)
{
	if (btn->state == BUTTON_STATE_CLICKED)
		btn->state = BUTTON_STATE_UP;
	
	switch (btn->state)
	{
		case BUTTON_STATE_UP:
			if (isMouseOver(getBounds(btn->obj)))
			{
				btn->obj->imgIndex = 1;
				btn->state = BUTTON_STATE_OVER;
			} break;
		case BUTTON_STATE_OVER:
			if (!isMouseOver(getBounds(btn->obj)))
			{
				btn->obj->imgIndex = 0;
				btn->state = BUTTON_STATE_UP;
			}
			else if (isMousePressed(BUTTON_LEFT))
			{
				btn->obj->imgIndex = 2;
				btn->state = BUTTON_STATE_DOWN;
			} break;
		case BUTTON_STATE_DOWN:
			if (!isMouseOver(getBounds(btn->obj)))
			{
				btn->obj->imgIndex = 0;
				btn->state = BUTTON_STATE_DOWN_OUT;
			}
			else if (isMouseReleased(BUTTON_LEFT))
			{
				btn->obj->imgIndex = 0;
				btn->state = BUTTON_STATE_CLICKED;
			} break;
		case BUTTON_STATE_DOWN_OUT:
			if (isMouseOver(getBounds(btn->obj)))
			{
				btn->obj->imgIndex = 2;
				btn->state = BUTTON_STATE_DOWN;
			}
			else if (isMouseReleased(BUTTON_LEFT))
			{
				btn->obj->imgIndex = 0;
				btn->state = BUTTON_STATE_UP;
			} break;
	}
}

void updateTextField(TextField *txt)
{
	int i;
	bool changed = false;
	for (i = 0; i < NUM_VALID_KEYS; i++)
	{
		if (isKeyPressed(VALID_KEYS[i]) || isKeyHeld(VALID_KEYS[i]))
		{
			if (i == NUM_VALID_KEYS - 1)
			{
				if (txt->length > 0)
					txt->text[--(txt->length)] = 0;
				changed = true;
			}
			else if (txt->length < txt->maxLength)
			{
				char digit[2];
				digit[0] = VALID_CHARS[i];
				digit[1] = 0;
				strcat(txt->text, digit);
				txt->length++;
				changed = true;
			}
			break;
		}
	}
	if (changed)
	{
		if (txt->textImg) SDL_FreeSurface(txt->textImg);
		if (txt->length > 0) txt->textImg = getDrawnText(txt->font, txt->text, txt->textColor);
		else txt->textImg = NULL;
		setPosition(txt->cursor, 
			newPoint(getBounds(txt->box).position.x + (txt->textImg ? txt->textImg->w : 0) + 3, getBounds(txt->box).position.y + 3));
		txt->showCursor = true;
		txt->cursorTimer = 0;
	}
	txt->cursorTimer++;
	if (txt->cursorTimer == CURSOR_BLINK_INTERVAL)
	{
		txt->showCursor = !(txt->showCursor);
		txt->cursorTimer = 0;
	}
}

void drawButton(Button *btn)
{
	drawObject(btn->obj);
	if (btn->text)
		drawSurface(btn->textImg, getBounds(btn->obj).position.x + btn->textPos.x, getBounds(btn->obj).position.y + btn->textPos.y);
}

void drawTextField(TextField *txt)
{
	drawObject(txt->box);
	if (txt->textImg) drawSurface(txt->textImg, getBounds(txt->box).position.x + 3, getBounds(txt->box).position.y + 3);
	if (txt->showCursor) drawObject(txt->cursor);
}

void freeButton(Button *btn)
{
	freeObject(btn->obj);
	if (btn->text) SDL_FreeSurface(btn->textImg);
	free(btn);
}

void freeTextField(TextField *txt)
{
	freeObject(txt->box);
	freeObject(txt->cursor);
	SDL_FreeSurface(txt->textImg);
	TTF_CloseFont(txt->font);
	free(txt->text);
	free(txt);
}

