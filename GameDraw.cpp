#include "GameManager.h"

GameDraw::GameDraw() {}
GameDraw::~GameDraw() {}

void GameDraw::draw(sf::RenderWindow *window, LevelObjects &levelObjects, Game &game) {
	switch (game) {

	case Game::TITLE_SCREEN:
		levelObjects.titleScreen->draw(window);
		break;
	case Game::GAME:		
		draw(levelObjects.spritesBackgroundLayer, window);
		draw(levelObjects.spritesStatic, window);
		draw(levelObjects.levelStaticObjects, window);
		draw(levelObjects.levelStaticPlatforms, window);
		draw(levelObjects.travelators, window);
		draw(levelObjects.levelStaticStairs, window);
		draw(levelObjects.enemiesMoving, window);
		draw(levelObjects.enemiesStatic, window);
		draw(levelObjects.pickups, window);
		draw(levelObjects.ropes, window);
		draw(levelObjects.player, window);
		break;
	case Game::DEAD:
		break;		   
	}
}