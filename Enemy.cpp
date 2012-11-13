#include "Bullet.cpp"
#include <vector>

using namespace std;

class Enemy : public Bullet
{
private:
	int HEALTH;
	int BULLET_TYPE;
	int COUNTER;
	vector<Bullet> bullets;
	
public:

	Enemy( float x, float y, float dx, float dy  ) : Bullet( x, y, dx, dy )
	{
		HEALTH = 0;
		BULLET_TYPE = 0;
	}
	
	//ALLEGRO_BITMAP* Bullet::getBitmap( void )
	//{
		//return bullet;
	//}
	
	vector<Bullet> return_bullets( void )
	{
		return bullets;
	}
	
	void set_bullet_type( int type )
	{
		BULLET_TYPE = type;
	}
	
	int get_bullet_type( void )
	{
		return BULLET_TYPE;
	}
	
	void change_health( int health_dx )
	{
		HEALTH -= health_dx;
	}
	
	int get_health( void )
	{
		return HEALTH;
	}
	
	void prepare_to_fire( int i )
	{
		COUNTER += i;
	}
	
	int ready_to_fire( void )
	{
		if( COUNTER > 1000 )
		{
			COUNTER = 0;
			return 1;
		}
		return 0;
	}
	
	
};
