#pragma once
#include <string>
#include <Engine/OGLGame.h>

#include "GameObject.h"
#include "Rect.h"


/**
*  An OpenGL Game based on ASGE.
*/
class BreakoutGame :
	public ASGE::OGLGame
{
public:
	BreakoutGame();
	~BreakoutGame();
	virtual bool init() override;

private:
	void keyHandler(const ASGE::SharedEventData data);
	void clickHandler(const ASGE::SharedEventData data);
	void setupResolution();
	void respawn();
	void paddleMovement(float dt_sec);
	void ballMovement(float dt_sec);
	void collision(const ASGE::GameTime & us);

	virtual void update(const ASGE::GameTime &) override;
	virtual void render(const ASGE::GameTime &) override;

	int  key_callback_id = -1;	        /**< Key Input Callback ID. */
	int  mouse_callback_id = -1;        /**< Mouse Input Callback ID. */

	//General game variables
	bool in_menu = true;
	int score = 0;
	int lives = 3;

	//Block variables
	int x_pos = 20;
	int y_pos = 35;
	int block_row = 1;
	int block_array_size = 48;
	int number_of_blocks = block_array_size;

	//Gem Variables
	int gem_array_size = 3;
	int number_of_gems = gem_array_size;
	int gem_chance = 0;;

	vector2 ball_direction = { 2,3 };

	//Add your GameObjects
	GameObject paddle;
	ASGE::Sprite* paddle_sprite = nullptr;
	rect paddle_box;

	GameObject ball;
	ASGE::Sprite* ball_sprite = nullptr;
	rect ball_box;

	GameObject blocks[48];
	ASGE::Sprite* block_sprite;
	rect block_box;

	GameObject gems[3];
	ASGE::Sprite* gem_sprite;
	rect gem_box;

};