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
	void spawn();

	virtual void update(const ASGE::GameTime &) override;
	virtual void render(const ASGE::GameTime &) override;

	int  key_callback_id = -1;	        /**< Key Input Callback ID. */
	int  mouse_callback_id = -1;        /**< Mouse Input Callback ID. */

	bool in_menu = true;
	int array_size = 48;
	


	int x_pos = 20;
	int y_pos = 35;
	int block_row = 1;
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


};