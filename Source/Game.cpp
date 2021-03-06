#include <string>

#include <Engine/Keys.h>
#include <Engine/Input.h>
#include <Engine/InputEvents.h>
#include <Engine/Sprite.h>

#include "Game.h"

/**
*   @brief   Default Constructor.
*   @details Consider setting the game's width and height
             and even seeding the random number generator.
*/
BreakoutGame::BreakoutGame()
{


}

/**
*   @brief   Destructor.
*   @details Remove any non-managed memory and callbacks.
*/
BreakoutGame::~BreakoutGame()
{
	this->inputs->unregisterCallback(key_callback_id);
	this->inputs->unregisterCallback(mouse_callback_id);


}

/**
*   @brief   Initialises the game.
*   @details The game window is created and all assets required to
			 run the game are loaded. The keyHandler and clickHandler
			 callback should also be set in the initialise function.
*   @return  True if the game initialised correctly.
*/
bool BreakoutGame::init()
{
	setupResolution();
	if (!initAPI())
	{
		return false;
	}


	srand(time(NULL));
	toggleFPS();
	renderer->setWindowTitle("Breakout!");

	// input handling functions
	inputs->use_threads = false;

	key_callback_id = inputs->addCallbackFnc(
		ASGE::E_KEY, &BreakoutGame::keyHandler, this);
	
	mouse_callback_id =inputs->addCallbackFnc(
		ASGE::E_MOUSE_CLICK, &BreakoutGame::clickHandler, this);

	if (!paddle.addSpriteComponent(renderer.get(),
		".\\Resources\\Textures\\puzzlepack\\png\\paddleBlue.png"))
	{
		return false;
	}
	paddle_sprite = paddle.spriteComponent()->getSprite();
	paddle_sprite->xPos((game_width - paddle_sprite->width() )/ 2);
	paddle_sprite->yPos(game_height - 50);

	if (!ball.addSpriteComponent(renderer.get(),
		".\\Resources\\Textures\\puzzlepack\\png\\ballBlue.png"))
	{
		return false;
	}
	ball_sprite = ball.spriteComponent()->getSprite();
	
	
	for (int i = 0; i < block_array_size; i++)

	{
		if (block_row % 2 == 0)
		{
			if (i % 2 == 0)
			{
				if (!blocks[i].addSpriteComponent(renderer.get(),
					".\\Resources\\Textures\\puzzlepack\\png\\element_red_rectangle_glossy.png"))
				{
					blocks[i].addSpriteComponent(renderer.get(),
						".\\Resources\\Textures\\puzzlepack\\png\\element_red_rectangle_glossy.png");
				}
			}
			else
				if (!blocks[i].addSpriteComponent(renderer.get(),
					".\\Resources\\Textures\\puzzlepack\\png\\element_blue_rectangle_glossy.png"))
				{
					blocks[i].addSpriteComponent(renderer.get(),
						".\\Resources\\Textures\\puzzlepack\\png\\element_blue_rectangle_glossy.png");
				}
		}
		else
		{
			if (i % 2 == 0)
			{
				if (!blocks[i].addSpriteComponent(renderer.get(),
					".\\Resources\\Textures\\puzzlepack\\png\\element_blue_rectangle_glossy.png"))
				{
					blocks[i].addSpriteComponent(renderer.get(),
						".\\Resources\\Textures\\puzzlepack\\png\\element_blue_rectangle_glossy.png");
				}
			}
			else
				if (!blocks[i].addSpriteComponent(renderer.get(),
					".\\Resources\\Textures\\puzzlepack\\png\\element_red_rectangle_glossy.png"))
				{
					blocks[i].addSpriteComponent(renderer.get(),
						".\\Resources\\Textures\\puzzlepack\\png\\element_red_rectangle_glossy.png");
				}

		}


		block_sprite = blocks[i].spriteComponent()->getSprite();
		block_sprite->xPos(x_pos);
		block_sprite->yPos(y_pos);

		x_pos += (block_sprite->width() + 10);

		if (x_pos + block_sprite->width() >= game_width)
		{
			block_row++;
			x_pos = 20;
			y_pos = block_row * 35;
		}
		
		blocks[i].visibility = true;
		
	}

	initGems();
	respawn();

	return true;
}

void BreakoutGame::initGems()
{
	for (int i = 0; i < gem_array_size; i++)
	{
		if (!gems[i].addSpriteComponent(renderer.get(),
			".\\Resources\\Textures\\puzzlepack\\png\\element_yellow_diamond_glossy.png"))
		{
			gems[i].addSpriteComponent(renderer.get(),
				".\\Resources\\Textures\\puzzlepack\\png\\element_yellow_diamond_glossy.png");
		}

		gems[i].visibility = false;
	}
}

/**
*   @brief   Sets the game window resolution
*   @details This function is designed to create the window size, any 
             aspect ratio scaling factors and safe zones to ensure the 
			 game frames when resolutions are changed in size.
*   @return  void
*/
void BreakoutGame::setupResolution()
{
	// how will you calculate the game's resolution?
	// will it scale correctly in full screen? what AR will you use?
	// how will the game be framed in native 16:9 resolutions?
	// here are some abritrary values for you to adjust as you see fit
	// https://www.gamasutra.com/blogs/KenanBolukbasi/20171002/306822/Scaling_and_MultiResolution_in_2D_Games.php
	game_width = 640;
	game_height = 920;
}

/**
*   @brief   Processes any key inputs
*   @details This function is added as a callback to handle the game's
			 keyboard input. For this game, calls to this function
			 are thread safe, so you may alter the game's state as you
			 see fit.
*   @param   data The event data relating to key input.
*   @see     KeyEvent
*   @return  void
*/
void BreakoutGame::keyHandler(const ASGE::SharedEventData data)
{
	auto key = static_cast<const ASGE::KeyEvent*>(data.get());
	
	if (key->key == ASGE::KEYS::KEY_ESCAPE)
	{
		signalExit();
	}

	if (key->key == ASGE::KEYS::KEY_ENTER)
	{
		in_menu = false;
	}
	if (key->action == ASGE::KEYS::KEY_PRESSED)
	{
		if (key->key == ASGE::KEYS::KEY_A)
		{
			paddle.set_vel_x(-1);
		}
		if (key->key == ASGE::KEYS::KEY_D)
		{
			paddle.set_vel_x(1);
		}
	}
	else if (key->action == ASGE::KEYS::KEY_RELEASED)
	{
		paddle.set_vel_x(0);
	}

}

/**
*   @brief   Processes any click inputs
*   @details This function is added as a callback to handle the game's
		     mouse button input. For this game, calls to this function
             are thread safe, so you may alter the game's state as you
             see fit.
*   @param   data The event data relating to key input.
*   @see     ClickEvent
*   @return  void
*/
void BreakoutGame::clickHandler(const ASGE::SharedEventData data)
{
	auto click = static_cast<const ASGE::ClickEvent*>(data.get());

	double x_pos, y_pos;
	inputs->getCursorPos(x_pos, y_pos);
}


/**
*   @brief   Updates the scene
*   @details Prepares the renderer subsystem before drawing the
		     current frame. Once the current frame is has finished
		     the buffers are swapped accordingly and the image shown.
*   @return  void
*/
void BreakoutGame::update(const ASGE::GameTime& us)
{

	auto dt_sec = us.delta_time.count() / 1000.0;


	//make sure you use delta time in any movement calculations!
	if (!in_menu)
	{
		paddleMovement(dt_sec);
		ballMovement(dt_sec);
		collision(us);
		gemMovement(dt_sec);
	}
}

/**
*   @brief   Renders the scene
*   @details Renders all the game objects to the current frame.
	         Once the current frame is has finished the buffers are
			 swapped accordingly and the image shown.
*   @return  void
*/
void BreakoutGame::render(const ASGE::GameTime &)
{
	renderer->setFont(0);

	if (in_menu)
	{
		renderer->renderText("Press Enter to continue",
			(game_width / 2) - 160, game_height / 2, ASGE::COLOURS::WHITE);
	}
	else if (number_of_blocks <= 0)
	{
		renderer->renderText("Congratulations",
			(game_width / 2) - 160, game_height / 2, ASGE::COLOURS::WHITE);
	}
	else if (lives <= 0)
	{
		renderer->renderText("You Lose",
			(game_width / 2) - 160, game_height / 2, ASGE::COLOURS::WHITE);

	}
	else
	{
	
		renderer->renderSprite(*paddle_sprite);
		renderer->renderSprite(*ball_sprite);

		std::string score_str = "Score: " + std::to_string(score);
		renderer->renderText(score_str.c_str(),
			20, game_height - 20, ASGE::COLOURS::WHITE);
		std::string lives_str = "Lives: " + std::to_string(lives);
		renderer->renderText(lives_str.c_str(),
			20, game_height - 40, ASGE::COLOURS::WHITE);
		std::string gem_str = "Gem Chance: " + std::to_string(gem_chance);
		renderer->renderText(gem_str.c_str(),
			20, game_height - 60, ASGE::COLOURS::WHITE);

		for (int j = 0; j < block_array_size; j++)
		{
			if (blocks[j].visibility == true)
			{
				renderer->renderSprite(*blocks[j].spriteComponent()->getSprite());
			}
		}

		for (int j = 0; j < gem_array_size; j++)
		{
			if (gems[j].visibility == true)
			{
				renderer->renderSprite(*gems[j].spriteComponent()->getSprite());
			}
		}
	
	}
}

// Handles spawning the ball
void BreakoutGame::respawn()
{

	auto x = (rand() % 10 + 1) - 5;
	auto y = (rand() % 1 - 10);

	ball_direction.x = x;
	ball_direction.y = y;
	ball_direction.normalise();

	ball_sprite->xPos((game_width - ball_sprite->width()) / 2);
	ball_sprite->yPos(game_height - 80);

}

// Handles paddle movement
void BreakoutGame::paddleMovement(float dt_sec)
{
	auto paddle_pos = paddle_sprite->xPos();

	if (paddle_sprite->xPos() <= 0)
	{
		paddle.set_vel_x(paddle.get_vel_x() * -1);
	}
	if (paddle_sprite->xPos() + paddle_sprite->width() >= game_width)
	{
		paddle.set_vel_x(paddle.get_vel_x() * -1);
	}

	paddle_pos += paddle.get_vel_x() * paddle.speed* dt_sec;

	paddle_sprite->xPos(paddle_pos);
}

// Handles ball movement
void BreakoutGame::ballMovement(float dt_sec)
{
	auto ball_x_pos = ball_sprite->xPos();
	auto ball_y_pos = ball_sprite->yPos();


	if (ball_box.isInside(paddle_box))
	{
		ball_y_pos -= 10;
		ball_direction.y *= -1;
	}

	if (ball_x_pos + ball_sprite->width() >= game_width || ball_x_pos <= 0)
	{
		ball_direction.x *= -1;
	}

	if (ball_y_pos <= 0)
	{
		ball_direction.y *= -1;
	}

	ball_x_pos += ball.speed * ball_direction.x * dt_sec;
	ball_y_pos += ball.speed * ball_direction.y * dt_sec;


	ball_sprite->xPos(ball_x_pos);
	ball_sprite->yPos(ball_y_pos);

	if (ball_y_pos + ball_sprite->height() >= game_height)
	{
		lives--;
		respawn();
	}



}

// Handles all collisions
void BreakoutGame::collision(const ASGE::GameTime& us)
{
	ball_box = ball.spriteComponent()->getBoundingBox();
	paddle_box = paddle.spriteComponent()->getBoundingBox();
	auto dt_sec = us.delta_time.count() / 1000.0;
	for (int i = 0; i <= block_array_size; i++)
	{

		block_box = blocks[i].spriteComponent()->getBoundingBox();
		if (ball_box.isInside(block_box) && blocks[i].visibility == true)
		{
			gem_chance += rand() % (us.game_time.count() / 500);

			if (number_of_gems > 0)
			{
				if (gem_chance >= 50)
				{
					gemSpawn();
				}
			}

			ball_direction.y *= -1;
			blocks[i].visibility = false;
			score += 1000;
			number_of_blocks--;
			break;	
		}

	}
}

//Handles gem spawning
void BreakoutGame::gemSpawn()
{
	

	if(gems[number_of_gems-1].visibility == false)
	{ 
			gem_chance = 0;						
			gem_sprite = gems[number_of_gems-1].spriteComponent()->getSprite();	
			gem_sprite->xPos(((game_width - gem_sprite->width()) / 100) * (rand() % 100 + 1));
			gem_sprite->yPos(-50);
			gems[number_of_gems-1].visibility = true;
			number_of_gems--;
		
	}		
}

//Handles gem movement and collision
void BreakoutGame::gemMovement(float dt_sec)
{
	for (int i = 0; i < gem_array_size; i++)
	{
		if (gems[i].visibility == true)
		{
			if (gems[i].spriteComponent()->getSprite()->yPos() > game_height)
			{
				gems[i].visibility = false;
				number_of_gems++;
			}
			gems[i].set_vel_y(1);

			ASGE::Sprite* gem_sprite = gems[i].spriteComponent()->getSprite();
			gem_box = gems[i].spriteComponent()->getBoundingBox();

			auto gem_pos = gem_sprite->yPos();

			gem_pos += gems[i].get_vel_y() * (gems[i].speed / 2) * dt_sec;

			gem_sprite->yPos(gem_pos);

			if (paddle_box.isInside(gem_box) && gems[i].visibility == true)
			{
				gems[i].visibility = false;
				number_of_gems++;
				score += 50000;
			}
		}
	}
}