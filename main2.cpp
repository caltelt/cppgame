// g++ main2.cpp $(pkg-config --libs allegro-5.0)

#ifndef __INCLUDE__
#include "includes.h"
#endif

const float FPS = 60.0;
const int SCREEN_W = 640;
const int SCREEN_H = 480;
const int CHAR_SIZE = 32;

enum MYKEYS 
{
	KEY_UP, KEY_DOWN, KEY_LEFT, KEY_RIGHT
};

int main( int argc, char **argv )
{
	ALLEGRO_DISPLAY *display = NULL;
	ALLEGRO_EVENT_QUEUE *event_queue = NULL;
	ALLEGRO_TIMER *timer = NULL;
	
	bool redraw = true;
	bool key[4] = { false, false, false, false };

	srand( time( NULL ) );

	// character object (spot on screen, direction moving )
	Bullet *character = new Bullet( SCREEN_W / 2.0, SCREEN_H - ( CHAR_SIZE + 10 ), 0, 0 );

	Bullet *enemy = new Bullet( SCREEN_W / 2.0, ( CHAR_SIZE + 10 ), 0,0 );	

	//initialize allegro
	al_init();

	//initialize keyboard controlls
	al_install_keyboard();

	//create allegro related objects
	timer = al_create_timer( 1.0 / FPS );
	display = al_create_display( SCREEN_W, SCREEN_H );

	//set bitmaps
	character->setBitmap( CHAR_SIZE, CHAR_SIZE );
	enemy->setBitmap( CHAR_SIZE, CHAR_SIZE );


	//color bitmaps
	al_set_target_bitmap( character->getBitmap() );
	al_clear_to_color( al_map_rgb( 122,122,122 ) );
	
	al_set_target_bitmap( enemy->getBitmap() );
	al_clear_to_color( al_map_rgb( 255,0,255 ) );
	

	al_set_target_bitmap( al_get_backbuffer( display ) );

	event_queue = al_create_event_queue();

	al_register_event_source( event_queue, al_get_display_event_source( display ) ) ;
	al_register_event_source( event_queue, al_get_timer_event_source( timer ) );
	al_register_event_source( event_queue, al_get_keyboard_event_source() );

	al_clear_to_color( al_map_rgb( 0,0,0 ) );

	al_flip_display();
	
	al_start_timer( timer );

	bool doexit = false;

	while( !doexit )
	{
		ALLEGRO_EVENT ev;
		al_wait_for_event( event_queue, &ev );

		if( ev.type == ALLEGRO_EVENT_TIMER )
		{
			if( key[ KEY_UP ] )
			{
				character->changeY( -4 );
			}
			if( key[ KEY_DOWN] )
			{
				character->changeY( 4 );
			}
			if( key[ KEY_LEFT ] )
			{
				character->changeX( -4 );
			}
			if( key[ KEY_RIGHT ] )
			{
				character->changeX( 4 );
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
				case ALLEGRO_KEY_ESCAPE:
					doexit = true;
					break;
			}
		}
		
		if( redraw && al_is_event_queue_empty( event_queue ) )
		{
			redraw = false;
			al_clear_to_color( al_map_rgb( 0,0,0 ) );
			
			al_draw_bitmap( character->getBitmap(), character->getX(), character->getY(), 0 );
			al_draw_bitmap( enemy->getBitmap(), enemy->getX(), enemy->getY(), 0 );
			al_flip_display();
		}
	}
	
	al_destroy_bitmap( character->getBitmap() );
	al_destroy_bitmap( enemy->getBitmap() );
	al_destroy_display( display );
	al_destroy_event_queue( event_queue );
	
	return 0;
	
}
		
			
			
			
			
			
