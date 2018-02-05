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

	virtual void update(const ASGE::GameTime &) override;
	virtual void render(const ASGE::GameTime &) override;

	int  key_callback_id = -1;	        /**< Key Input Callback ID. */
	int  mouse_callback_id = -1;        /**< Mouse Input Callback ID. */

	bool in_menu = true;
	int direction = 0;
	int move_speed = 150;
	int array_size = 50;
	int i = 0;

	//Add your GameObjects
	GameObject paddle;
	ASGE::Sprite* paddle_sprite = nullptr;

	GameObject ball;
	ASGE::Sprite* ball_sprite = nullptr;

	GameObject blocks[50];
	ASGE::Sprite* block_sprite;


};