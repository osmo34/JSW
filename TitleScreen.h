#pragma once
#include "Sprite.h"
class TitleScreen : public Sprite
{
public:
	using Sprite::Sprite;
	~TitleScreen();
	void update(const float dt);
	bool getTitleScreenStatus();
	
private:
	int TEXTURE_SIZE_X = 1280;
	int TEXTURE_SIZE_Y = 720;
	void setStartPosition();
	void input();
	bool isOnTitleScreen = true;
};

