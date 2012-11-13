// g++ main2.cpp $(pkg-config --libs allegro-5.0)

//enemy bullets ln 240

#ifndef __INCLUDE__
#include "includes.h"
#endif

using namespace std;

const float FPS = 60.0;
const int SCREEN_W = 640;
const int SCREEN_H = 480;
const int CHAR_SIZE = 16;
const int ENEMY_SIZE = 32;
const int IMG_SIZE = 20;
const int BULLET_SIZE = 6;
//const int BULLET_CENTER = BULLET_SIZE / 2;

Bullet *HEALTHBAR;
Bullet *HEALTHBAR_BACKGROUND;
Bullet *HEALTHBAR_BACKGROUND_BACKGROUND;

int HEALTH = 100;
int LEVEL = 1;
int SCORE = 0;

Bullet *character;
bool dead = false;

vector<Enemy> enemies;
vector<Bullet> enemy_bullets;

enum MYKEYS 
{
	KEY_UP, KEY_DOWN, KEY_LEFT, KEY_RIGHT, KEY_SPACE
};


void char_hit()
{
	HEALTH -= 10;
	
	if( HEALTH < 0 )
	{
		dead = true;
	}
	else
	{
		ALLEGRO_BITMAP *health_bmp = al_create_bitmap( HEALTH, 30 );
		HEALTHBAR->setBitmap( health_bmp );
	
		al_set_target_bitmap( HEALTHBAR->getBitmap() );
		al_clear_to_color( al_map_rgb( 0, 122, 122 ) );
	}
}

void add_enemy( void )
{
	Enemy *bullet = new Enemy( rand() % ( SCREEN_W - ENEMY_SIZE ) , -( ENEMY_SIZE ), 0, 1 );
	
	int bullet_type = rand() % 2;
	
	ALLEGRO_BITMAP *bullet_bmp = al_create_bitmap( ENEMY_SIZE, ENEMY_SIZE );
	bullet->setBitmap( bullet_bmp );
	
	al_set_target_bitmap( bullet->getBitmap() );
	if( bullet_type == 0 )
	{
		al_clear_to_color( al_map_rgb( 77,77,77 ) );
	}
	else if( bullet_type == 1 )
	{
		al_clear_to_color( al_map_rgb( 177, 177, 177 ) );
	}
	
	//bullet->print();
	bullet->set_bullet_type( bullet_type );
	
	enemies.push_back( *bullet );
}

void circle_bullets( int enemy_number )
{
	int center_x = enemies[ enemy_number ].getX() + ( ENEMY_SIZE / 2 );
	int center_y = enemies[ enemy_number ].getY() + ( ENEMY_SIZE / 2 );
	
	float dx;
	float dy;
	
	for( int i = 0; i < 6; i++ )
	{
		dx = rand() % 4 - 2;
		dy = rand() % 4 - 2;
		
		if( dx >= 0 ) dx++;
		if( dy >= 0 ) dy++;
		
		Bullet *bullet = new Bullet( center_x, center_y, dx, dy );
		
		ALLEGRO_BITMAP *bullet_bmp = al_create_bitmap( BULLET_SIZE, BULLET_SIZE );
		bullet->setBitmap( bullet_bmp );
	
		al_set_target_bitmap( bullet->getBitmap() );
		al_clear_to_color( al_map_rgb( 255,0,0 ) );
		
		enemy_bullets.push_back( *bullet );	
	}
}

void straight_bullet( int enemy_number )
{
	int center_x = enemies[ enemy_number ].getX() + ( ENEMY_SIZE / 2 );
	int center_y = enemies[ enemy_number ].getY() + ( ENEMY_SIZE / 2 );
	
	float dx;
	float dy;
	float length;
	
	dx = character->getX() - center_x;
	dy = character->getY() - center_y;
	
	length = sqrt ( pow( dx, 2 ) + pow( dy, 2 ) );
	
	//cout << dy / length << "\n" << ( dx / length ) << endl;
	int multiple = (rand() % 2) + 1;
	length = length / multiple;
	
	Bullet *bullet = new Bullet( center_x, center_y, dx / length, dy / length );
	
	ALLEGRO_BITMAP *bullet_bmp = al_create_bitmap( BULLET_SIZE, BULLET_SIZE );
	bullet->setBitmap( bullet_bmp );

	al_set_target_bitmap( bullet->getBitmap() );
	al_clear_to_color( al_map_rgb( 0,255,0 ) );
	
	enemy_bullets.push_back( *bullet );	
}

void move_enemies( void )
{
	for( int i = 0; i < enemies.size(); i++ )
	{
		enemies[i].changeY( enemies[i].getDY() );
		
		if( enemies[i].ready_to_fire() == 1 )
		{
			switch( enemies[i].get_bullet_type() )
			{
				case 0: 
					circle_bullets( i );
					break;
				case 1:
					straight_bullet( i );
					break;
			}
		}
	}
}

void init( )
{
	HEALTH = 110;
	char_hit();
	LEVEL = 1;
	
	enemies.clear();
	enemy_bullets.clear();
	
	character->setX( SCREEN_W / 2 );
	character->setY( SCREEN_H - CHAR_SIZE - 10 );
	
	dead = false;
}

int main( int argc, char **argv )
{
	ALLEGRO_DISPLAY *display = NULL;
	ALLEGRO_EVENT_QUEUE *event_queue = NULL;
	ALLEGRO_TIMER *timer = NULL;
	
	bool redraw = true;
	bool key[5] = { false, false, false, false, false };

	srand( time( NULL ) );	

	// character object (spot on screen, direction moving )
	Bullet *c = new Bullet( SCREEN_W / 2.0, SCREEN_H - ( CHAR_SIZE + 10 ), 0, 0 );
	character = c;
	
	//Bullet *enemy = new Bullet( SCREEN_W / 2.0, ( CHAR_SIZE + 10 ), 0,0 );	

	//initialize allegro
	al_init();

	//initialize keyboard controls
	al_install_keyboard();

	//create allegro related objects
	timer = al_create_timer( 1.0 / FPS );
	display = al_create_display( SCREEN_W, SCREEN_H );

	//set bitmaps
	//character->setBitmap( IMG_SIZE, IMG_SIZE );
	ALLEGRO_BITMAP *img = al_create_bitmap( CHAR_SIZE, CHAR_SIZE );
	//ALLEGRO_BITMAP *img = al_load_bitmap( "/home/nick/workspacecpp/cppgame/mush.tga" );
	character->setBitmap( img );
	
	//enemy->setBitmap( CHAR_SIZE, CHAR_SIZE );


	//color bitmaps
	al_set_target_bitmap( character->getBitmap() );
	al_clear_to_color( al_map_rgb( 255,255,0 ) );
	

	al_set_target_bitmap( al_get_backbuffer( display ) );

	event_queue = al_create_event_queue();

	al_register_event_source( event_queue, al_get_display_event_source( display ) ) ;
	al_register_event_source( event_queue, al_get_timer_event_source( timer ) );
	al_register_event_source( event_queue, al_get_keyboard_event_source() );

	al_clear_to_color( al_map_rgb( 0,0,0 ) );

	al_flip_display();
	
	al_start_timer( timer );


	//background stars
	vector<Bullet> stars;
	for( int i = 0; i < 50; i++ )
	{
		Bullet *bullet = new Bullet( rand() % SCREEN_W, rand() % SCREEN_H, 0, ( rand() % 2 ) + 1 );
		
		int bullet_size = rand() % 5;
		ALLEGRO_BITMAP *bullet_bmp = al_create_bitmap( bullet_size, bullet_size );
		bullet->setBitmap( bullet_bmp );
		
		al_set_target_bitmap( bullet->getBitmap() );
		al_clear_to_color( al_map_rgb( 255,255,255 ) );
		
		stars.push_back( *bullet );
	}

	//vector of bullets character fires
	vector<Bullet> bullets;
	vector<Bullet> newBullets;
	
	vector<Bullet> newEnemy_bullets;
	
	int bullet_counter = 0;
	int enemy_counter = 0;
	
	///////////////////////
	//set health bar
	
	Bullet *health = new Bullet( 10, 10, 0, 0 );
	HEALTHBAR = health;
	
	ALLEGRO_BITMAP *health_bmp = al_create_bitmap( 100, 30 );
	HEALTHBAR->setBitmap( health_bmp );
	
	al_set_target_bitmap( HEALTHBAR->getBitmap() );
	al_clear_to_color( al_map_rgb( 0, 122, 122 ) );
	
	// background_background
	health = new Bullet( 8, 8, 0, 0 );
	HEALTHBAR_BACKGROUND_BACKGROUND = health;
	
	health_bmp = al_create_bitmap( 104, 34 );
	HEALTHBAR_BACKGROUND_BACKGROUND->setBitmap( health_bmp );
	
	al_set_target_bitmap( HEALTHBAR_BACKGROUND_BACKGROUND->getBitmap() );
	al_clear_to_color( al_map_rgb( 255, 255, 255 ) );
	
	//background
	health = new Bullet( 10, 10, 0, 0 );
	HEALTHBAR_BACKGROUND = health;
	
	health_bmp = al_create_bitmap( 100, 30 );
	HEALTHBAR_BACKGROUND->setBitmap( health_bmp );
	
	al_set_target_bitmap( HEALTHBAR_BACKGROUND->getBitmap() );
	al_clear_to_color( al_map_rgb( 0, 0, 0 ) );
	
	
	
	
	bool hit = false;
	
	bool doexit = false;
	
	//loop for game
	while( !doexit )
	{
		ALLEGRO_EVENT ev;
		al_wait_for_event( event_queue, &ev );

		if( ev.type == ALLEGRO_EVENT_TIMER )
		{			
			if( key[ KEY_UP ] )
			{
				if( character->getY() > CHAR_SIZE )
				{
					character->changeY( -4 );;
				}
			}
			if( key[ KEY_DOWN] )
			{
				if( character->getY() < SCREEN_H - CHAR_SIZE )
				{
					character->changeY( 4 );
				}
			}
			if( key[ KEY_LEFT ] )
			{
				if( character->getX() > 0 )
				{
					character->changeX( -4 );
				}
			}
			if( key[ KEY_RIGHT ] )
			{
				if( character->getX() < SCREEN_W - CHAR_SIZE )
				{
					character->changeX( 4 );
				}
			}
			if( key[ KEY_SPACE ] )
			{
				bullet_counter++;
				if( bullet_counter > 5 )
				{
					int bullet_dy = -5;
					int bullet_dx = 0;
					if( key[ KEY_UP ] )
					{
						bullet_dy = -7;
					}
					else if( key[ KEY_DOWN ] )
					{
						bullet_dy = -3;
					}
					
					if( key[ KEY_LEFT ] )
					{
						bullet_dx = 0;
					}
					else if ( key[ KEY_RIGHT ] )
					{
						bullet_dx = 0;
					}
						
					Bullet *bullet = new Bullet( character->getX() + ( 0.5 * CHAR_SIZE ) - ( 0.5 * BULLET_SIZE ), character->getY(), bullet_dx, bullet_dy );
				
					ALLEGRO_BITMAP *bullet_bmp = al_create_bitmap( BULLET_SIZE, BULLET_SIZE );
					bullet->setBitmap( bullet_bmp );
	
					al_set_target_bitmap( bullet->getBitmap() );
					al_clear_to_color( al_map_rgb( 122,122,255 ) );
					
					bullets.push_back( *bullet );
					
					bullet_counter = 0;
				}
			}
			
			//	TIMER DINGS
			
			for( int i = 0; i < bullets.size(); i++ )
			{
				bullets[ i ].changeY( bullets[i].getDY() );
				bullets[ i ].changeX( bullets[i].getDX() );
			}
			
			for( int i = 0; i < stars.size(); i++ )
			{
				stars[i].changeY( stars[i].getDY() );
				
				if( stars[i].getY() > SCREEN_H )
				{
					stars[i].setX( rand() % SCREEN_W );
					stars[i].setY( 1 );
					stars[i].setDY( ( rand() % 4 ) + 1 );
				}	
			}
			
			enemy_counter++;
			
			//add enemies
			if( enemy_counter > ( 100 / LEVEL ) )
			{
				add_enemy();
				enemy_counter = 0;
				//cout << "NOW" << endl;
			}			
			
			for( int i = 0; i < enemies.size(); i++ )
			{
				enemies[i].prepare_to_fire( 10 );
			}
			move_enemies();
			
			for( int i = 0; i < enemy_bullets.size(); i++ )
			{
				enemy_bullets[i].changeX( enemy_bullets[i].getDX() );
				enemy_bullets[i].changeY( enemy_bullets[i].getDY() );
			}
			
			//for getting rid of old bullets
			if( bullets.size() > 0 && bullets[0].getY() < 0 )
			{
				newBullets.clear();
				for( int i = 0; i < bullets.size(); i++ )
				{
					if( bullets[i].getY() > 0 )
					{
						newBullets.push_back( bullets[i] );
					}
					else
					{
						al_destroy_bitmap( bullets[i].getBitmap() );
					}
				}
				bullets.clear();
				for( int i = 0; i < newBullets.size(); i++ )
				{
					bullets.push_back( newBullets[i] );
				}
			}
			
			if( enemy_bullets.size() > 1 )
			{
				bool current_hit = false;
				newEnemy_bullets.clear();
				//vector<Bullet> newEnemy_bullets;
				for( int i = 0; i < enemy_bullets.size(); i++ )
				{
					if( enemy_bullets[i].getY() < 0 || enemy_bullets[i].getY() > SCREEN_H || 
						enemy_bullets[i].getX() < 0 || enemy_bullets[i].getX() > SCREEN_W )
					{
						al_destroy_bitmap( enemy_bullets[i].getBitmap() );
					}
					else
					{						
						if( enemy_bullets[i].getX() + BULLET_SIZE > character->getX() && 
							enemy_bullets[i].getX() < character->getX() + CHAR_SIZE )
						{
							//cout << "HI" << endl;
							if( enemy_bullets[i].getY() + BULLET_SIZE > character->getY() && enemy_bullets[i].getY() < character->getY() + CHAR_SIZE )
							{
								hit = true;
								current_hit = true;
								char_hit();
							}
						}
						
						if( !current_hit )
						{
							newEnemy_bullets.push_back( enemy_bullets[i] );
						}
					}
					current_hit = false;
				}
				enemy_bullets.clear();
				for( int i = 0; i < newEnemy_bullets.size(); i++ )
				{
					enemy_bullets.push_back( newEnemy_bullets[i] );
				}
			}
				
			
			redraw = true;
			
		}
		else if( ev.type == ALLEGRO_EVENT_KEY_DOWN )
		{
			switch( ev.keyboard.keycode )
			{
				case ALLEGRO_KEY_UP:
					key[ KEY_UP ] = true;
					break;
				case ALLEGRO_KEY_DOWN:
					key[ KEY_DOWN ] = true;
					break;
				case ALLEGRO_KEY_RIGHT:
					key[ KEY_RIGHT ] = true;
					break;
				case ALLEGRO_KEY_LEFT:
					key[ KEY_LEFT ] = true;
					break;
				case ALLEGRO_KEY_SPACE:
					key[ KEY_SPACE ] = true;
					break;
			}
		}
		else if( ev.type == ALLEGRO_EVENT_KEY_UP )
		{
			switch( ev.keyboard.keycode )
			{
				case ALLEGRO_KEY_UP:
					key[ KEY_UP ] = false;
					break;
				case ALLEGRO_KEY_DOWN:
					key[ KEY_DOWN ] = false;
					break;
				case ALLEGRO_KEY_RIGHT:
					key[ KEY_RIGHT ] = false;
					break;
				case ALLEGRO_KEY_LEFT:
					key[ KEY_LEFT ] = false;
					break;
				case ALLEGRO_KEY_SPACE:
					key[ KEY_SPACE ] = false;
					break;
				case ALLEGRO_KEY_ESCAPE:
					doexit = true;
					break;
			}
		}
		
		if( redraw && al_is_event_queue_empty( event_queue ) )
		{
			redraw = false;
			
			if( dead ) init();
			
			al_set_target_bitmap( al_get_backbuffer( display ) );
			
			if( hit )
			{
				al_clear_to_color( al_map_rgb( 150,0,0 ) );	
				hit = false;
			}
			else
			{
				al_clear_to_color( al_map_rgb( 0,0,0 ) );
			}
					
			
			for( int i = 0; i < bullets.size(); i++ )
			{
				al_draw_bitmap( bullets[i].getBitmap(), bullets[i].getX(), bullets[i].getY(), 0 );
			}
			
			for( int i = 0; i < stars.size(); i++ )
			{
				al_draw_bitmap( stars[i].getBitmap(), stars[i].getX(), stars[i].getY(), 0 );
			}
			
			al_draw_bitmap( character->getBitmap(), character->getX(), character->getY(), 0 );
			
			
			for( int i = 0; i < enemy_bullets.size(); i++ )
			{
				al_draw_bitmap( enemy_bullets[i].getBitmap(), enemy_bullets[i].getX(), enemy_bullets[i].getY(), 0 );
			}
			
			//add enemies
			for( int i = 0; i < enemies.size(); i++ )
			{
				//enemies[i].print();
				al_draw_bitmap( enemies[i].getBitmap(), enemies[i].getX(), enemies[i].getY(), 0 );
			}
			
			al_draw_bitmap( HEALTHBAR_BACKGROUND_BACKGROUND->getBitmap(), HEALTHBAR_BACKGROUND_BACKGROUND->getX(), HEALTHBAR_BACKGROUND_BACKGROUND->getY(), 0 );
			al_draw_bitmap( HEALTHBAR_BACKGROUND->getBitmap(), HEALTHBAR_BACKGROUND->getX(), HEALTHBAR_BACKGROUND->getY(), 0 );
			al_draw_bitmap( HEALTHBAR->getBitmap(), HEALTHBAR->getX(), HEALTHBAR->getY(), 0 );
			
			al_flip_display();
		}
	}
	
	al_destroy_bitmap( character->getBitmap() );
	al_destroy_display( display );
	al_destroy_event_queue( event_queue );
	
	return 0;
	
}
		
			
			
			
			
			
