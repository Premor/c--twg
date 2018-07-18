/*This source code copyrighted by Lazy Foo' Productions (2004-2015)
and may not be redistributed without written permission.*/

//Using SDL, SDL_image, standard IO, and strings
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <string>

//Screen dimension constants
const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;
const int CELL_W = SCREEN_WIDTH / 20; //делаем ячейки такими
const int CELL_H = SCREEN_HEIGHT / 20;//чтобы карта была 20х20
const int G = 5;



class Sprite {
	private:
		std::string picture;
		//int x,y,w,h;
		SDL_Texture* image;
		SDL_Rect* rect;		
		int center[2];
		bool visible;
	public:
		Sprite(SDL_Rect*,std::string,bool);
		void setImage(SDL_Texture* _image){image=_image;};
		SDL_Texture* getImage(){return image;};
		std::string getPicture(){return picture;};
		SDL_Rect* getRect(){return rect;};
};

class Player:public Sprite{
	private:
	int speed,vel_x,vel_y,move_vel_x,move_vel_y,number;
	bool airborne;
	SDL_Rect* future;
	public:
	Player(SDL_Rect*,std::string,bool);
};

class Cell:public Sprite{
	private:
	bool passable;
	public:
	Cell(SDL_Rect*,std::string,bool,bool);
};



Player::Player(SDL_Rect* _rect,std::string _picture="man.png",bool _visible=true):Sprite(_rect,_picture,_visible){
	speed = 8;
	vel_x=0;
	vel_y=0;
	move_vel_x=0;
	move_vel_y=0;

}

Cell::Cell(SDL_Rect* _rect,std::string _picture,bool pas=true,bool _visible=true):Sprite(_rect,_picture,_visible){
	passable = pas;
}

const int MAP_I =20;
const int MAP_J =20;

//Cell map[][];

//Starts up SDL and creates window
bool init();
SDL_Rect q;
q.x = 1;

Cell 
//Loads media
bool loadMedia();

//Frees media and shuts down SDL
void close();


Sprite::Sprite(SDL_Rect* _rect,std::string _picture,bool _visible=true){
	rect = _rect;
	visible = _visible;
	picture = _picture;
}


//Loads individual image as texture
SDL_Texture* loadTexture( std::string path );

//The window we'll be rendering to
SDL_Window* gWindow = NULL;

//The window renderer
SDL_Renderer* gRenderer = NULL;

//Current displayed texture
SDL_Texture* gTexture = NULL;



bool init()
{
	//Initialization flag
	bool success = true;

	//Initialize SDL
	if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
	{
		printf( "SDL could not initialize! SDL Error: %s\n", SDL_GetError() );
		success = false;
	}
	else
	{
		//Set texture filtering to linear
		if( !SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "1" ) )
		{
			printf( "Warning: Linear texture filtering not enabled!" );
		}

		//Create window
		gWindow = SDL_CreateWindow( "SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
		if( gWindow == NULL )
		{
			printf( "Window could not be created! SDL Error: %s\n", SDL_GetError() );
			success = false;
		}
		else
		{
			
			//Create renderer for window
			gRenderer = SDL_CreateRenderer( gWindow, -1, SDL_RENDERER_ACCELERATED );
			if( gRenderer == NULL )
			{
				printf( "Renderer could not be created! SDL Error: %s\n", SDL_GetError() );
				success = false;
			}
			else
			{
				//Initialize renderer color
				SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );

				//Initialize PNG loading
				int imgFlags = IMG_INIT_PNG;
				if( !( IMG_Init( imgFlags ) & imgFlags ) )
				{
					printf( "SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError() );
					success = false;
				}
			}
		}
	}

	return success;
}

bool loadMedia()
{
	//Loading success flag
	bool success = true;

	//Load PNG texture
	gTexture = loadTexture( "texture.png" );
	if( gTexture == NULL )
	{
		printf( "Failed to load texture image!\n" );
		success = false;
	}

	return success;
}

void close()
{
	//Free loaded image
	SDL_DestroyTexture( gTexture );
	gTexture = NULL;

	//Destroy window	
	SDL_DestroyRenderer( gRenderer );
	SDL_DestroyWindow( gWindow );
	gWindow = NULL;
	gRenderer = NULL;

	//Quit SDL subsystems
	IMG_Quit();
	SDL_Quit();
}

SDL_Texture* loadTexture( std::string path )
{
	//The final texture
	SDL_Texture* newTexture = NULL;

	//Load image at specified path
	SDL_Surface* loadedSurface = IMG_Load( path.c_str() );
	if( loadedSurface == NULL )
	{
		printf( "Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError() );
	}
	else
	{
		//Create texture from surface pixels
        newTexture = SDL_CreateTextureFromSurface( gRenderer, loadedSurface );
		if( newTexture == NULL )
		{
			printf( "Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError() );
		}

		//Get rid of old loaded surface
		SDL_FreeSurface( loadedSurface );
	}

	return newTexture;
}

void init_map(){
	SDL_Rect q;
	q.x=0;
	q.y=0;
	q.w=0;
	q.h=0;
		
	Cell buf_buf[MAP_I][MAP_J](&q,"rock.png");
	map = buf_buf;
	for(int i;i<MAP_I;i++){
		for(int j;j<MAP_J;j++){
		SDL_Rect buf;
		buf.x=i*CELL_W;
		buf.y=j*CELL_H;
		buf.w=CELL_W;
		buf.h=CELL_H;
		map[i][j]=Cell(&buf,"rock.png");
		}}



	for(int i;i<MAP_I;i++){
		for(int j;j<MAP_J;j++){
			map[i][j].setImage(loadTexture(map[i][j].getPicture()));}}
}

void render_map(){
	for(int i;i<MAP_I;i++){
		for(int j;j<MAP_J;j++){

			SDL_RenderCopy( gRenderer, map[i][j].getImage(), NULL, map[i][j].getRect() );

		}}
}

int main( int argc, char* args[] )
{
	//Start up SDL and create window
	if( !init() )
	{
		printf( "Failed to initialize!\n" );
	}
	else
	{

		//Load media
		if( !loadMedia() )
		{
			printf( "Failed to load media!\n" );
		}
		else
		{	
			//Main loop flag
			bool quit = false;

			SDL_Rect dest;
			dest.x = 100;
			dest.y = 100;
			dest.w = 100;
			dest.h = 100;
			init_map();

			//Event handler
			SDL_Event e;

			//While application is running
			while( !quit )
			{
				//Handle events on queue
				while( SDL_PollEvent( &e ) != 0 )
				{
					//User requests quit
					if( e.type == SDL_QUIT )
					{
						quit = true;
					}
				}

				//Clear screen
				SDL_RenderClear( gRenderer );

				render_map();
				//Render texture to screen
				//SDL_RenderCopy( gRenderer, gTexture, NULL, &dest );

				//Update screen
				SDL_RenderPresent( gRenderer );
			}
		}
	}

	//Free resources and close SDL
	close();

	return 0;
}