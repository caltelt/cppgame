//g++ main.cpp $(pkg-config --libs allegro-5.0)

#ifndef __INCLUDES__
#include "includes.h"
#endif


const float FPS = 60;
const int SCREEN_W = 640;
const int SCREEN_H = 480;
const int BOUNCER_SIZE = 32;

int main(int argc, char **argv){
	srand( time(NULL) );
	int bulletLength = 4;
	Bullet *bullets[ 4 ];
	
   ALLEGRO_DISPLAY *display = NULL;
   ALLEGRO_EVENT_QUEUE *event_queue = NULL;
   ALLEGRO_TIMER *timer = NULL;
 
   //Bullet *bouncer = new Bullet( SCREEN_W / 2.0 - BOUNCER_SIZE, SCREEN_H / 2.0 - BOUNCER_SIZE / 2.0, -4.0, -4.0 );
   
   bool redraw = true;
	
	for( int i = 0; i < bulletLength; i++ )
	{
		bullets[i] = new Bullet( SCREEN_W / 2.0 + ( i * 10 ), SCREEN_H / 2.0 - BOUNCER_SIZE / 2.0, -4.0, -4.0 );
	}

   if(!al_init()) 
   {
      fprintf(stderr, "failed to initialize allegro!\n");
      return -1;
   }

   timer = al_create_timer(1.0 / FPS);
   if(!timer) 
   {
      fprintf(stderr, "failed to create timer!\n");
      return -1;
   }

   display = al_create_display(SCREEN_W, SCREEN_H);
   if(!display) 
   {
      fprintf(stderr, "failed to create display!\n");
      al_destroy_timer(timer);
      return -1;
   }

	for( int i = 0; i < bulletLength; i++ )
	{
		bullets[i]->setBitmap( BOUNCER_SIZE, BOUNCER_SIZE );
		
		if( !bullets[i]->getBitmap() ) 
		{
			fprintf(stderr, "failed to create bouncer bitmap!\n");
			al_destroy_display(display);
			al_destroy_timer(timer);
			return -1;
		}
	}

	for( int i = 0; i < bulletLength; i++ )
	{
		al_set_target_bitmap( bullets[i]->getBitmap() );
	}
	
  // al_set_target_bitmap( bouncer->getBitmap() );

   al_clear_to_color(al_map_rgb(255, 0, 255));

   al_set_target_bitmap(al_get_backbuffer(display));

   event_queue = al_create_event_queue();
   if(!event_queue) 
   {
		fprintf(stderr, "failed to create event_queue!\n");
      
		for( int i = 0; i < bulletLength; i++ )
		{
			al_destroy_bitmap( bullets[i]->getBitmap() );
		}
		
      //al_destroy_bitmap( bouncer->getBitmap() );
      al_destroy_display(display);
      al_destroy_timer(timer);
      return -1;
   }

   al_register_event_source(event_queue, al_get_display_event_source(display));

   al_register_event_source(event_queue, al_get_timer_event_source(timer));

   al_clear_to_color(al_map_rgb(0,0,0));

   al_flip_display();

   al_start_timer(timer);

   while(1)
   {
      ALLEGRO_EVENT ev;
      al_wait_for_event(event_queue, &ev);
      
		for( int i = 0; i < bulletLength; i++ )
		{
			if(ev.type == ALLEGRO_EVENT_TIMER) 
			{
				if( bullets[i]->getX() < 0 || bullets[i]->getX() > SCREEN_W - BOUNCER_SIZE) 
				{
					bullets[i]->setDX( - ( bullets[i]->getDX() ) );
				}
			
				if( bullets[i]->getY() < 0 || bullets[i]->getY() > SCREEN_H - BOUNCER_SIZE) 
				{
					bullets[i]->setDY( -(bullets[i]->getDY() ) );
				}
				
				bullets[i]->setX( bullets[i]->getX() + bullets[i]->getDX() );
				bullets[i]->setY( bullets[i]->getY() + bullets[i]->getDY() );
				
				redraw = true;
			}
			else if(ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE) 
			{
				break;
			}
			
			if(redraw && al_is_event_queue_empty(event_queue)) 
			{
				redraw = false;
				
				al_clear_to_color(al_map_rgb(0,0,0));
				
				al_draw_bitmap( bullets[i]->getBitmap(), bullets[i]->getX(), bullets[i]->getY(), 0);
				
				al_flip_display();
			}
		}
	}

	for( int i = 0; i < bulletLength; i++ )
	{
		al_destroy_bitmap( bullets[i]->getBitmap() );
	}
   al_destroy_timer(timer);
   al_destroy_display(display);
   al_destroy_event_queue(event_queue);

   return 0;
}
