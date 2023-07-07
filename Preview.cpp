#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>
#include<SDL2/SDL_ttf.h>
#include <string>
#include<random>
#include<SDL2/SDL_mixer.h>
#include"mixergame.h"


//Screen dimension constants
const int SCREEN_WIDTH = 900;
const int SCREEN_HEIGHT = 600;

const int Figure_Size = 128;
const int  Icon = 64;
const int ICON_SIZE =32;
const int SumOfGoldTargets = 20;

//Walking animation
const int WALKING_ANIMATION_FRAMES = 8;
const int SumOfTargets = 4;


int Level =0;


int hammer , bag , drag;
int sum_BDH ;
int count_Player , count_AI;


int frame =0;
int frameLose =0;
int frameWin =0;
int frameBoss =0;
int currentFrame =0;

// The background scrolling offset
int scrollingOffset = 0;


// Khai báo biến cho nút "Start Game"
SDL_Rect startButtonRect ;
 // Khai báo biến cho nút "Exit Game"
SDL_Rect exitButtonRect;



SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;

 //Globally used font
TTF_Font *gFont = NULL;


SDL_Rect gSpriteClips[ WALKING_ANIMATION_FRAMES ];
SDL_Rect gDeadSpriteClips[WALKING_ANIMATION_FRAMES/2 ];
SDL_Rect gWinSpriteClips[7];
SDL_Rect gBossSpriteClips[4];

//Xoay nhân vật
bool isMoving; 
//Test 
bool isTest;


class LTexture {
    public:
		//Initializes variables
		LTexture();

		//Deallocates memory
		~LTexture();

        SDL_Texture* getTexture();

		//Loads image at specified path
		bool loadTexture( std::string path );

        //Creates image from font string
		bool loadFromRenderedText( std::string textureText, SDL_Color textColor );

		//Deallocates texture
		void free();

		//Set color modulation
		void setColor( Uint8 red, Uint8 green, Uint8 blue );

		//Set blending
		void setBlendMode( SDL_BlendMode blending );

		//Set alpha modulation
		void setAlpha( Uint8 alpha );
		
		//Renders texture at given point
		void render( int x, int y, SDL_Rect* clip = NULL, double angle = 0.0, SDL_Point* center = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE );

        void render_Map(  int x, int y, SDL_Rect* clip = NULL, double angle = 0.0, SDL_Point* center = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE);
        void Render_Box(SDL_Rect *Box);

        void close();

		//Gets image dimensions
		int getWidth();
		int getHeight();

	private:
		//The actual hardware texture
		SDL_Texture* mTexture;
          
		//Image dimensions
		int mWidth;
		int mHeight;

};

 LTexture gSpriteSheetTexture;
 LTexture gDeadSpriteTexture;
 LTexture gWinSpriteTexture;
 LTexture TargetTexture[SumOfTargets];

 LTexture FontScore;
 LTexture FontHighScore;
 LTexture FontBDH;
 LTexture YouWin;
 LTexture YouLose;

 LTexture Map1;
 LTexture BackGroundTexture;
 LTexture Boss1;
 LTexture Boss2;
 LTexture Boss3;
 LTexture Boss4;
 LTexture Fire1;
 LTexture Fire2;
 LTexture Fire3;
 LTexture Fire4;
 LTexture Map2;
 LTexture MapFinal;

 LTexture Drag;
 LTexture Hammer;
 LTexture Bag;


 LTexture MenuStartTexture;
 LTexture MenuExitTexture;


SDL_Texture* LTexture :: getTexture(){
    return mTexture;
}


LTexture::LTexture()
{
	//Initialize
	mTexture = NULL;
	mWidth = 0;
	mHeight = 0;
}


LTexture::~LTexture()
{
	//Deallocate
	free();
}


bool LTexture::loadTexture( std::string path )
{
	//Get rid of preexisting texture
	free();

	//The final texture
	SDL_Texture* newTexture = NULL;

	//Load image at specified path
	SDL_Surface* loadTexture = IMG_Load( path.c_str() );
	if( loadTexture == NULL )
	{
		printf( "Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError() );
	}
	else
	{
		//Color key image
		SDL_SetColorKey( loadTexture, SDL_TRUE, SDL_MapRGB( loadTexture->format, 0, 0xFF, 0xFF ) );

		//Create texture from surface pixels
        newTexture = SDL_CreateTextureFromSurface( renderer, loadTexture );
		if( newTexture == NULL )
		{
			printf( "Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError() );
		}
		else
		{
			//Get image dimensions
			mWidth = loadTexture->w;
			mHeight = loadTexture->h;
		}

		//Get rid of old loaded surface
		SDL_FreeSurface( loadTexture );
	}

	//Return success
	mTexture = newTexture;
	return mTexture != NULL;
}


bool LTexture::loadFromRenderedText( std::string textureText, SDL_Color textColor )
{
	//Get rid of preexisting texture
	free();

	//Render text surface
	SDL_Surface* textSurface = TTF_RenderText_Solid( gFont, textureText.c_str(), textColor );
	if( textSurface == NULL )
	{
		printf( "Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError() );
	}
	else
	{
		//Create texture from surface pixels
        mTexture = SDL_CreateTextureFromSurface( renderer, textSurface );
		if( mTexture == NULL )
		{
			printf( "Unable to create texture from rendered text! SDL Error: %s\n", SDL_GetError() );
		}
		else
		{
			//Get image dimensions
			mWidth = textSurface->w;
			mHeight = textSurface->h;
		}

		//Get rid of old surface
		SDL_FreeSurface( textSurface );
	}
	
	//Return success
	return mTexture != NULL;
}


void LTexture::free()
{
	//Free texture if it exists
	if( mTexture != NULL )
	{
		SDL_DestroyTexture( mTexture );
		mTexture = NULL;
		mWidth = 0;
		mHeight = 0;
	}
}


void LTexture::setColor( Uint8 red, Uint8 green, Uint8 blue )
{
	//Modulate texture rgb
	SDL_SetTextureColorMod( mTexture, red, green, blue );
}


void LTexture::setBlendMode( SDL_BlendMode blending )
{
	//Set blending function
	SDL_SetTextureBlendMode( mTexture, blending );
}

		
void LTexture::setAlpha( Uint8 alpha )
{
	//Modulate texture alpha
	SDL_SetTextureAlphaMod( mTexture, alpha );
}


void LTexture::render(int x, int y, SDL_Rect* clip, double angle, SDL_Point* center, SDL_RendererFlip flip)
{
    // Set rendering space and render to screen
    SDL_Rect renderQuad = { x, y, mWidth, mHeight };

    // Set clip rendering dimensions
    if (clip != NULL) {
        renderQuad.w = clip->w;
        renderQuad.h = clip->h;
    }

                                                    // Determine flip direction based on current direction
    SDL_RendererFlip flipDirection = SDL_FLIP_NONE;
    if (isMoving) {                                 // Nếu nhân vật đang di chuyển
        if (flip == SDL_FLIP_NONE) {                // Nếu nhân vật đang quay về phía bên phải
            flipDirection = SDL_FLIP_HORIZONTAL;    // Quay nhân vật về phía bên trái
        } else if (flip == SDL_FLIP_HORIZONTAL) {   // Nếu nhân vật đang quay về phía bên trái
            flipDirection = SDL_FLIP_NONE;          // Quay nhân vật về phía bên phải
        }
    } else {                                        // Nếu nhân vật đang đứng yên
        flipDirection = flip;                        // Giữ nguyên hướng quay hiện tại của nhân vật
    }

    // Render to screen
    SDL_RenderCopyEx(renderer, mTexture, clip, &renderQuad, angle, center, flipDirection);
}


void LTexture::render_Map( int x, int y, SDL_Rect* clip, double angle, SDL_Point* center, SDL_RendererFlip flip )
{
	//Set rendering space and render to screen
	SDL_Rect renderQuad = { x, y, mWidth, mHeight };

	//Set clip rendering dimensions
	if( clip != NULL )
	{
		renderQuad.w = clip->w;
		renderQuad.h = clip->h;
	}

	//Render to screen
	SDL_RenderCopyEx( renderer, mTexture, clip, &renderQuad, angle, center, flip );
}



void LTexture::Render_Box(SDL_Rect* Box) {
        // sử dụng kích thước mặc định của texture để vẽ
        SDL_RenderCopy(renderer, mTexture, NULL, Box );
}


int LTexture::getWidth()
{
	return mWidth;
}


int LTexture::getHeight()
{
	return mHeight;
}


//Starts up SDL and creates window
bool init()
{
    //Initialize SDL
    if(SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        std::cout << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
        return false;
    }

    //Create window
    window = SDL_CreateWindow("Adventures", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if(window == NULL)
    {
        std::cout << "Window could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        return false;
    }

    //Create renderer
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if(renderer == NULL)
    {
        std::cout << "Renderer could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        return false;
    }


    //Initialize PNG loading (Tải ảnh PNG)
    int imgFlags = IMG_INIT_PNG;
    if(!(IMG_Init(imgFlags) & imgFlags))
    {
        std::cout << "SDL_image could not initialize! SDL_image Error: " << IMG_GetError() << std::endl;
        return false;
    }

    
    //Load sprite sheet texture
	if( !gSpriteSheetTexture.loadTexture( "C:/FirstGame/Adventures_figure/Walk.png" ) )
	{
		printf( "Failed to load walking animation texture!\n" );
		return false;
         }
	else
	{     for (int i=0; i<WALKING_ANIMATION_FRAMES; i++){
        //Set sprite clips
		gSpriteClips[ i ].x =   i*Figure_Size;
		gSpriteClips[ i ].y =   0;
		gSpriteClips[ i ].w =  Figure_Size;
		gSpriteClips[ i ].h = Figure_Size;
    }
	}


      //Load sprite sheet texture
	if( !gDeadSpriteTexture.loadTexture( "C:/FirstGame/Adventures_figure/Dead.png" ) )
	{
		printf( "Failed to load Dead animation texture!\n" );
		return false;
         }
	else
	{     for (int i=0; i<WALKING_ANIMATION_FRAMES/2; i++){
        //Set sprite clips
		gDeadSpriteClips[ i ].x =   i*Figure_Size;
		gDeadSpriteClips[ i ].y =   0;
		gDeadSpriteClips[ i ].w =  Figure_Size;
		gDeadSpriteClips[ i ].h = Figure_Size;
    }
	}


    if( !gWinSpriteTexture.loadTexture( "C:/FirstGame/Adventures_figure/Jump.png" ) )
	{
		printf( "Failed to load Dead animation texture!\n" );
		return false;
         }
	else
	{     for (int i=0; i < 7; i++){
        //Set sprite clips
		gWinSpriteClips[ i ].x =   i*Figure_Size;
		gWinSpriteClips[ i ].y =   0;
		gWinSpriteClips[ i ].w =  Figure_Size;
		gWinSpriteClips[ i ].h = Figure_Size;
    }
	}


    if (!TargetTexture[0].loadTexture("C:/FirstGame/Picture/Sad.png")){
        printf("Failed to load Target!\n");
        return false;
        }


     if (!TargetTexture[1].loadTexture("C:/FirstGame/Picture/Special.png")){
        printf("Failed to load Target!\n");
        return false;
    } 

    
     if (!TargetTexture[2].loadTexture("C:/FirstGame/Picture/Smile1.png")){
        printf("Failed to load Target!\n");
        return false;
    } 

    
     if (!TargetTexture[3].loadTexture("C:/FirstGame/Picture/Smile2.png")){
        printf("Failed to load Target!\n");
        return false;
    } 
    
    	//Load background texture
	if( !Map1.loadTexture( "C:/FirstGame/Picture/Map1.png" ) )
	{
		printf( "Failed to load Map texture!\n" );
		return  false;
	}


    	if( !Map2.loadTexture( "C:/FirstGame/Picture/Map2.png" ) )
	{
		printf( "Failed to load Map texture!\n" );
		return  false;
	}


    	if( !MapFinal.loadTexture( "C:/FirstGame/Picture/MapFinal.png" ) )
	{
		printf( "Failed to load Map texture!\n" );
		return  false;
	}


    if (!Boss1.loadTexture( "C:/FirstGame/Picture/Boss1.png") ) {
        printf( "Failed to load BOSS texture !\n" );
        return false;
    } 


    if (!Boss2.loadTexture( "C:/FirstGame/Picture/2.png") ) {
        printf( "Failed to load BOSS texture !\n" );
        return false;
    } 


    if (!Boss3.loadTexture( "C:/FirstGame/Picture/3.png") ) {
        printf( "Failed to load BOSS texture !\n" );
        return false;
    } 


    if (!Boss4.loadTexture( "C:/FirstGame/Picture/4.png") ) {
        printf( "Failed to load BOSS texture !\n" );
        return false;
    }

     if (!Fire1.loadTexture( "C:/FirstGame/Picture/Fire1.png") ) {
        printf( "Failed to load BOSS texture !\n" );
        return false;
    }

    if (!Fire2.loadTexture( "C:/FirstGame/Picture/Fire2.png") ) {
        printf( "Failed to load BOSS texture !\n" );
        return false;
    }

    if (!Fire3.loadTexture( "C:/FirstGame/Picture/Fire3.png") ) {
        printf( "Failed to load BOSS texture !\n" );
        return false;
    }

    if (!Fire4.loadTexture( "C:/FirstGame/Picture/Fire4.png") ) {
        printf( "Failed to load BOSS texture !\n" );
        return false;
    }


    if (!Bag.loadTexture( "C:/FirstGame/Picture/Bag.png") ) {
        printf( "Failed to load Bag texture !\n" );
        return false;
    }
    if (!Drag.loadTexture( "C:/FirstGame/Picture/Drag.png") ) {
        printf( "Failed to load BOSS texture !\n" );
        return false;
    }
    if (!Hammer.loadTexture( "C:/FirstGame/Picture/Hammer.png") ) {
        printf( "Failed to load BOSS texture !\n" );
        return false;
    }


    if (!BackGroundTexture.loadTexture( "C:/FirstGame/Picture/Background.png"))
    {
        printf( "Failed to load background texture!\n" );
        return false;
    }


     if (!MenuStartTexture.loadTexture( "C:/FirstGame/Picture/StartButton.png"))
    {
        printf( "Failed to load background texture!\n" );
        return false;
    }


     if (!MenuExitTexture.loadTexture( "C:/FirstGame/Picture/ExitButton.png"))
    {
        printf( "Failed to load background texture!\n" );
        return false;
    }


    //Initialize SDL_ttf
    if( TTF_Init() == -1 )
    {
        printf( "SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError() );
        return  false;
    }


    //Open the font
	gFont = TTF_OpenFont( "C:/FirstGame/Font.ttf", 20 );
	if( gFont == NULL )
	{
		printf( "Failed to load lazy font! SDL_ttf Error: %s\n", TTF_GetError() );
		return false;
	}

    return true;
} 


class Figure {
     public:

		//The dimensions of the dot
		static const int FIGURE_WIDTH = Figure_Size;
		static const int FIGURE_HEIGHT = Figure_Size;

		//Maximum axis velocity of the dot
		static const int FIGURE_VEL = 2;

		//Initializes the variables
		Figure();

		//Takes key presses and adjusts the dot's velocity
		void handleEvent( SDL_Event& e );

		//Moves the dot
		void move();

		//Shows the dot on the screen
		void render();

          // Getter function for mPosX
        int getPosX() const {
            return mPosX;
        }

        // Getter function for mPosY
        int getPosY() const {
            return mPosY;
        }

        // Setter function for mPosX
        void setPosX(int x) {
            mPosX = x;
        }

        // Setter function for mPosY
        void setPosY(int y) {
            mPosY = y;
        }


        //CreateMazeObject
        void CreateMaze(int level);
        

    private:
		//The X and Y offsets of the dot
		int mPosX, mPosY;

		//The velocity of the dot
		int mVelX, mVelY;

         //Collider 
        SDL_Rect mFigure;

};



Figure::Figure()
{
    //Initialize the offsets
    mPosX = -18;
    mPosY = -50;

	//Nhân vật
	mFigure.w = FIGURE_WIDTH;
	mFigure.h = FIGURE_HEIGHT;


    //Initialize the velocity
    mVelX = 0;
    mVelY = 0;
}


void Figure::handleEvent( SDL_Event& e )
{
        //If a key was pressed
	if( e.type == SDL_KEYDOWN && e.key.repeat == 0 )
    {
        //Adjust the velocity
        switch( e.key.keysym.sym )
        {
            case SDLK_UP: mVelY -= FIGURE_VEL;  break;
            case SDLK_DOWN: mVelY += FIGURE_VEL;  break;
            case SDLK_LEFT: mVelX -= FIGURE_VEL;
                    isMoving = true; // Cập nhật giá trị của thuộc tính isMoving
                    break;
            case SDLK_RIGHT: mVelX += FIGURE_VEL;
                     isMoving = false;
              break;
        }
    }
        //If a key was released
    else if( e.type == SDL_KEYUP && e.key.repeat == 0 )
    {
        //Adjust the velocity
        switch( e.key.keysym.sym )
        {
            case SDLK_UP: mVelY += FIGURE_VEL; break;
            case SDLK_DOWN: mVelY -= FIGURE_VEL; break;
            case SDLK_LEFT: mVelX += FIGURE_VEL; break;
            case SDLK_RIGHT: mVelX -= FIGURE_VEL; break;
        }
    }
}

void Figure::move()
{
     // Tạo tường thẳng đứng
        SDL_Rect WallOy[3];
        WallOy[0] = {SCREEN_WIDTH / 6 - 4 , SCREEN_HEIGHT / 4, 1, SCREEN_HEIGHT / 4};
        WallOy[1] = {SCREEN_WIDTH / 3, 0, 1, SCREEN_HEIGHT / 2};
        WallOy[2] = {SCREEN_WIDTH * 3 / 4, SCREEN_HEIGHT / 4 - 29, 1, SCREEN_HEIGHT / 2 + 30};

        // Tao đường thẳng nằm ngang
        SDL_Rect WallOx[4];
        WallOx[0] = {0, SCREEN_HEIGHT / 4, SCREEN_HEIGHT / 6 + 45, 1};
        WallOx[1] = { 64 + SCREEN_WIDTH / 4, SCREEN_HEIGHT / 8 + 50, SCREEN_WIDTH / 6 , 1};
        WallOx[2] = {SCREEN_WIDTH * 7 / 10, SCREEN_HEIGHT / 5, SCREEN_HEIGHT / 6, 1};
        WallOx[3] = {0, SCREEN_HEIGHT * 3 / 4, SCREEN_WIDTH * 3 / 4 + 45, 1};

    // Move the dot left or right
    mPosX += mVelX * FIGURE_VEL;
    mFigure.x = mPosX;

    // If the dot went too far to the left or right
    if( ( mPosX + FIGURE_WIDTH/3 - 15 < 0 ) || ( mPosX + FIGURE_WIDTH/2 > SCREEN_WIDTH )  )
    {
        // Move back
        mPosX -= mVelX * FIGURE_VEL;
        mFigure.x = mPosX;
    }
    else
    {
        // Update the current frame based on the direction of movement
        if( mVelX > 0 )
        {
            // Moving right, so update the frame to the next frame in the animation
            currentFrame = ( currentFrame + 1 ) % WALKING_ANIMATION_FRAMES;
        }
        else if( mVelX < 0 )
        {
            // Moving left, so update the frame to the previous frame in the animation
            currentFrame = ( currentFrame - 1 + WALKING_ANIMATION_FRAMES ) % WALKING_ANIMATION_FRAMES;
        }
    }

    // Move the dot up or down
    mPosY += mVelY * FIGURE_VEL;
    mFigure.y = mPosY;

    // If the dot went too far up or down
    if( ( mPosY + FIGURE_HEIGHT/2 - 10 < 0 ) || ( mPosY + FIGURE_HEIGHT > SCREEN_HEIGHT )  )
    {
        // Move back
        mPosY -= mVelY * FIGURE_VEL;
        mFigure.y = mPosY;
    } else{
            if( mVelY > 0 )
                {
                    // Moving down, so update the frame to the next frame in the animation
                    currentFrame = ( currentFrame + 1 ) % (WALKING_ANIMATION_FRAMES );
                }
                else if( mVelY < 0 )
                {
                    // Moving up, so update the frame to the previous frame in the animation
                    currentFrame = ( currentFrame - 1 + WALKING_ANIMATION_FRAMES  ) % (WALKING_ANIMATION_FRAMES );
                }

    }
}


void Figure::render(){
    // Render the sprite using the current frame
    gSpriteSheetTexture.render( mPosX, mPosY, &gSpriteClips[ currentFrame ] );
    }


bool checkCollision(SDL_Rect a, SDL_Rect b[], int numWalls)
{      
    for (int i = 0; i < numWalls; i++)
    {
      
        SDL_Rect currentWall = b[i];

        // The sides of the rectangles
        int leftA, leftB;
        int rightA, rightB;
        int topA, topB;
        int bottomA, bottomB;

        // Calculate the sides of rect A
        leftA = a.x;
        rightA = a.x + a.w;
        topA = a.y;
        bottomA = a.y + a.h;

        // Calculate the sides of rect B
        leftB = currentWall.x;
        rightB = currentWall.x + currentWall.w;
        topB = currentWall.y;
        bottomB = currentWall.y + currentWall.h;

        // If any of the sides from A are outside of B
        if (bottomA <= topB)
        {   
             
            continue;
           
        }

        if (topA >= bottomB)
        {  
            continue;
        }

        if (rightA <= leftB)
        {
             
            continue;
        }

        if (leftA >= rightB)
        {    
            continue;
        }
        // If none of the sides from A are outside B
        return true;
    }

    // If we get here, there is no collision
    return false;
} 
/* bool checkCollision(SDL_Rect a, SDL_Rect b[], int numWalls)
{      
    for (int i = 0; i < numWalls; i++)
    {
        SDL_Rect currentWall = b[i];

        // Calculate the sides of rect A
        int leftA = a.x;
        int rightA = a.x + a.w;
        int topA = a.y;
        int bottomA = a.y + a.h;

        // Calculate the sides of rect B
        int leftB = currentWall.x;
        int rightB = currentWall.x + currentWall.w;
        int topB = currentWall.y;
        int bottomB = currentWall.y + currentWall.h;

        // Check for collision
        if ((leftA > leftB && leftA < rightB) &&
            ((topA > topB && topA < bottomB) ||
             (bottomA > topB && bottomA < bottomB)) ||
            (rightA > leftB && rightA < rightB) &&
            ((topA > topB && topA < bottomB) ||
             (bottomA > topB && bottomA < bottomB)) ||
            (leftB > leftA && leftB < rightA) &&
            ((topB > topA && topB < bottomA) ||
             (bottomB > topA && bottomB < bottomA)) ||
            (rightB > leftA && rightB < rightA) &&
            ((topB > topA && topB < bottomA) ||
             (bottomB > topA && bottomB < bottomA)) ||
            (topA == topB && rightA == rightB && bottomA == bottomB))
        {
            return true; // Collision detected
        }
    }

    // If we get here, there is no collision
    return false;
}
*/


void Figure::CreateMaze(int level) {

      // Tạo tường thẳng đứng
        SDL_Rect WallOy[3];
        WallOy[0] = {SCREEN_WIDTH / 6 - 4 , SCREEN_HEIGHT / 4, 5, SCREEN_HEIGHT  / 4};
        WallOy[1] = {SCREEN_WIDTH / 3, 0, 5, (SCREEN_HEIGHT -64) / 2};
        WallOy[2] = {SCREEN_WIDTH * 3 / 4, SCREEN_HEIGHT / 4 - 29, 5, SCREEN_HEIGHT / 2 + 30};


        // Tao đường thẳng nằm ngang
        SDL_Rect WallOx[4];
        WallOx[0] = {0, SCREEN_HEIGHT / 4, SCREEN_HEIGHT / 6 + 45, 5};
        WallOx[1] = {  64 + SCREEN_WIDTH / 4, SCREEN_HEIGHT / 8 +50 , SCREEN_WIDTH / 6, 5};
        WallOx[2] = {SCREEN_WIDTH * 7 / 10, SCREEN_HEIGHT / 5, SCREEN_HEIGHT / 6, 5};
        WallOx[3] = {0, SCREEN_HEIGHT * 3 / 4, SCREEN_WIDTH * 3 / 4 + 45, 5};


    if (level == 1) {
        
        // Đặt màu 
       // SDL_SetRenderDrawColor(renderer, 135, 206, 235, 255);

        for (int i = 0; i < 3; i++) {
            Map1.Render_Box(&WallOy[i]);
            Map1.Render_Box(&WallOx[i]);
            // SDL_RenderDrawRect(renderer, &WallOy[i]);
            //SDL_RenderDrawRect(renderer, &WallOx[i]);
        }
           // SDL_RenderDrawRect(renderer, &WallOx[3]);
           Map1.Render_Box(&WallOx[3]);

        // Đặt lại màu vẽ ban đầu
        //SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        
         SDL_RenderPresent(renderer);

        // Scroll background
            scrollingOffset--;
        if (scrollingOffset < -Map2.getWidth()) {
            scrollingOffset = 0;
        }

        // Clear screen
        SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
        SDL_RenderClear(renderer);

        //Render background
        Map2.render_Map( scrollingOffset, 0 );
    
            // Vẽ background tiếp theo để che phủ toàn bộ màn hình
        Map2.render_Map(scrollingOffset + Map2.getWidth(), 0);


    } else {
        if (level == 0) {

            // Scroll background
            scrollingOffset--;
            
            if (scrollingOffset < -Map1.getWidth()) {
                scrollingOffset = 0;
            }

            // Clear screen
            SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
            SDL_RenderClear(renderer);

            //Render background
            Map1.render_Map( scrollingOffset, 0 );
        
                // Vẽ background tiếp theo để che phủ toàn bộ màn hình
            Map1.render_Map(scrollingOffset + Map1.getWidth(), 0);

        }
        if (level == 2){
            // Scroll background
            scrollingOffset--;
            if (scrollingOffset < -MapFinal.getWidth()) {
                scrollingOffset = 0;
            }

            // Clear screen
            SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
            SDL_RenderClear(renderer);

            //Render background
            MapFinal.render_Map( scrollingOffset, 0 );
        
                // Vẽ background tiếp theo để che phủ toàn bộ màn hình
            MapFinal.render_Map(scrollingOffset + MapFinal.getWidth(), 0);

        }

    }
    if (checkCollision(mFigure,WallOx,4) == true  or checkCollision(mFigure,WallOy,3) == true ){
                
               isTest = true;
      }

}
    


bool CheckBDH(int a, int b){
  bool res ;
  if (a == 1 and b == 2) {
    res = false;
  } else if (a == 1 and b == 3) {
    res = true;
  } else if (a == 2 and b == 1) {
    res = true;
  } else if (a == 2 and b == 3) {
    res = false;
  } else if (a == 3 and b == 1) {
    res = false;
  } else if (a == 3 and b == 2) {
    res = true;
  }
  return res;
}



int checkCollision1(SDL_Rect a, SDL_Rect b[], int size) {
       
    for (int i = 0; i < size; i++) {
        if (SDL_HasIntersection(&a, &b[i])) {
            return i;
        }
    }
    return -1;
}


bool checkCollision_2Wall(SDL_Rect a, SDL_Rect b)
{   
    // Calculate the sides of rect A
    int leftA, leftB;
    int rightA, rightB;
    int topA, topB;
    int bottomA, bottomB;

    // Calculate the sides of rect A
    leftA = a.x;
    rightA = a.x + a.w;
    topA = a.y;
    bottomA = a.y + a.h;

    // Calculate the sides of rect B
    leftB = b.x;
    rightB = b.x + b.w;
    topB = b.y;
    bottomB = b.y + b.h;

    // If any of the sides from A are outside of B
    if (bottomA <= topB || topA >= bottomB || rightA <= leftB || leftA >= rightB)
    {   
        return false;
    }

    // If none of the sides from A are outside B
    return true;
}


int  Random(int a,int b){
        // Khởi tạo engine
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(a, b);

    // Sinh số nguyên ngẫu nhiên trong khoảng [a, b]
    int random_number = dist(gen);

    return random_number;
}


 // Hàm tạo tọa độ ngẫu nhiên, cách nhau một khoảng bằng ICON_SIZE
SDL_Point randomPoint() {
    int x = Random(1, 27)*ICON_SIZE ;
    int y = Random(1, 17)*ICON_SIZE ;
    return {x, y};
}


void generateNewPoints(std::vector<SDL_Point>& points, int SumOfGoldTargets) {
    points.clear(); //clear all
    for (int i = 0; i < SumOfGoldTargets ; i++) {

        SDL_Point point = randomPoint();
        // Kiểm tra xem tọa độ đã được sử dụng chưa
        bool used = false;

        for (SDL_Point p : points) {
            if (p.x == point.x && p.y == point.y) {
                used = true;
                break;
            }
        }
        
        // Nếu tọa độ chưa được sử dụng thì thêm vào vector
        if (!used) {
            points.push_back(point);
        } else {
            i--;
        }
    }
}

                
//Frees media and shuts down SDL
void close()
{
    //Destroy renderer
    SDL_DestroyRenderer(renderer);
    renderer = NULL;

    //Destroy window
    SDL_DestroyWindow(window);
    window = NULL;
    //Free global font
	TTF_CloseFont( gFont );
	gFont = NULL;
   

    //Free loaded images
    FontScore.free();
    FontHighScore.free();
    FontBDH.free();
    YouWin.free();
    YouLose.free();


    MenuStartTexture.free();
    MenuExitTexture.free();
	gSpriteSheetTexture.free();
    gDeadSpriteTexture.free();
    gWinSpriteTexture.free();


    Boss1.free();
    Boss2.free();
    Boss3.free();
    Boss4.free();
    Fire1.free();
    Fire2.free();
    Fire3.free();
    Fire4.free();
    Bag.free();
    Drag.free();
    Hammer.free();
    Map1.free();
    Map2.free();
    MapFinal.free();
    BackGroundTexture.free();


for (int i=0; i<SumOfTargets;i++){
    TargetTexture[i].free();
}
 
    //Quit SDL subsystems
    IMG_Quit();
    SDL_Quit();
    TTF_Quit();
}


int main(int argc, char* argv[])
{       
    int score = 1;
    int HighScore =1;
    bool CheckScore;

    // Tạo một vector chứa các tọa độ ngẫu nhiên
    std::vector<SDL_Point> points;
               

    startButtonRect= {SCREEN_WIDTH/2 ,SCREEN_HEIGHT/2 + 45,200, 50}  ;  
    exitButtonRect = {SCREEN_WIDTH/2 ,SCREEN_HEIGHT/2 + 110,200, 50}  ;
             

    Mixer1 SoundTrack;
    Mixer1 Ting;
    Mixer1 Fight;
    Mixer1 Click;
    Mixer1 SoundLoseGame;

       
    SoundTrack.initAudio();
    Ting.initAudio();
    Click.initAudio();
    SoundLoseGame.initAudio();
        
       
    SoundTrack.loadAudio("C:/FirstGame/Sound/SoundTrack.wav");
    SoundTrack.playMusicLoop();
        

    if(!init())
    {
        std::cout << "Failed to initialize!" << std::endl;
        return -1;
    }
    if(!SoundTrack.initAudio() or !Ting.initAudio() or !Click.initAudio() or !SoundLoseGame.initAudio() ){
        std::cout << "failed" << std::endl;
    }


    SDL_Color ColorHighScore = { 255, 255, 255 };
    std::string textHighScore = "HIGHSCORE : ";
    std::string myHighScore = std::to_string(HighScore);
    textHighScore += myHighScore;
    if (!FontHighScore.loadFromRenderedText(textHighScore.c_str(), ColorHighScore))

		{
			printf( "Failed to render text texture!\n" );
		}
   
    
    //Main loop flag
    bool quit = false;

    //Event handler
    SDL_Event e;

    //While application is running
    while(!quit)
    {             
        //Handle events on queue
        while(SDL_PollEvent(&e) != 0)
        {   
                    //User requests quit
            if (e.type == SDL_QUIT)
            {
                quit = true;
            }
           
            if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_p)
            {
               SoundTrack.resumeMusic();
            }
            
            if (e.type == SDL_MOUSEBUTTONDOWN)
            {   
                // Lấy vị trí chuột
                int mouseX, mouseY;
                SDL_GetMouseState(&mouseX, &mouseY);
            
                if (mouseX >= startButtonRect.x && mouseX <= startButtonRect.x + startButtonRect.w && mouseY >= startButtonRect.y && mouseY <= startButtonRect.y + startButtonRect.h)
                {   

                    isTest = false;
                    bool quit1 = false;
                    score = 1;
                    drag = 0;
                    hammer = 0;
                    bag = 0;
                    sum_BDH = 5;
                    count_AI =0;
                    count_Player =0;
                    Level =1;
                    generateNewPoints(points, SumOfGoldTargets);
                  

                    SDL_Event e1;
                    Figure figure;
                    // Khởi tạo giá trị ban đầu cho timer
                    Uint32 timer = SDL_GetTicks();
    
                    while (!quit1) {
                     
                        // Di chuyển đối tượng figure
                        figure.move();

                    
                        // Vẽ các texture
                        
                        SDL_Rect Pos = { figure.getPosX()   ,figure.getPosY()   , 56, Figure_Size};
                        SDL_Rect Boss = {-Figure_Size*1/4 - 10 ,SCREEN_HEIGHT - Figure_Size - 20 ,Figure_Size*3/2,Figure_Size*3/2};
                        SDL_Rect Fire_Level1 = {-Figure_Size*1/4 - 10 + 128 ,SCREEN_HEIGHT - Figure_Size - 20 +45 ,Figure_Size,Figure_Size};

                        SDL_Rect HAMMER = {SCREEN_WIDTH *5/8 , SCREEN_HEIGHT/2, Icon, Icon};
                        SDL_Rect BAG =    {SCREEN_WIDTH*5/8 , SCREEN_HEIGHT/2 - 2* Icon,Icon, Icon};
                        SDL_Rect DRAG = {SCREEN_WIDTH*5/8, SCREEN_HEIGHT /2 + 2*Icon, Icon, Icon};

                        SDL_Rect BossOne = {0,0,Figure_Size,Figure_Size};
                        SDL_Rect BossTwo = {SCREEN_WIDTH/2,SCREEN_HEIGHT/2,Figure_Size,Figure_Size};
                        SDL_Rect BossThree = {250,450,Figure_Size,Figure_Size};

                        //Render text
                        SDL_Color textColor = { 255, 255, 0 };
                        std::string text = "SCORE : ";
                        std::string myScore = std::to_string(score);

                        text += myScore;
                        if (!FontScore.loadFromRenderedText(text.c_str(), textColor))

                        {
                            printf( "Failed to render text texture!\n" );
                        }

                        // Xử lý sự kiện

        while (SDL_PollEvent(&e1) != 0)
        {
                     
            if (e1.type == SDL_QUIT) {
                quit1 = true;
                isTest = true;
                if(HighScore <= score){
                    HighScore = score;
                }
                
            }

            if (e1.type == SDL_KEYDOWN && e1.key.keysym.sym == SDLK_p)
                {
                SoundTrack.loadAudio("C:/FirstGame/Sound/SoundTrack.wav");
                SoundTrack.resumeMusic();
                }

            // Xử lý sự kiện cho đối tượng figure
            figure.handleEvent(e1);


            if ( e1.type == SDL_MOUSEBUTTONDOWN && checkCollision_2Wall(Pos,Boss) && Level == 1) {
                int mouseX1 = e1.button.x;
                int mouseY1 = e1.button.y;
                
                SDL_GetMouseState(&mouseX1, &mouseY1);

                Click.loadAudio("C:/FirstGame/Sound/Click.wav");

                if (mouseX1 >= HAMMER.x && mouseX1 <= HAMMER.x + HAMMER.w && mouseY1 >= HAMMER.y && mouseY1 <= HAMMER.y + HAMMER.h)
                {
                    hammer++;
                    // 1 là búa 
                    int CheckHammer = Random(2,3);
                    
                        if (CheckBDH(1,CheckHammer) == 3 ){
                            count_Player++;
                            score += 5;
                        } else {
                            count_AI++;
                                if (score > 5){
                                score-=5;
                            } else {
                                score = 0;
                            }
                        }

                    Click.playMusicOnce();
                    
                }
                    if (mouseX1 >= BAG.x && mouseX1 <= BAG.x + BAG.w && mouseY1 >= BAG.y && mouseY1 <= BAG.y + BAG.h)
                {
                    bag++;
                    // 2 là bao
                                                
                    int CheckBag = Random(1,3);
                    while (CheckBag == 2){
                    CheckBag = Random(1,3);
                    }
                    
                        if (CheckBDH(2,CheckBag) == true ){
                            count_Player++;
                            score+=5;
                        } else {
                            count_AI++;
                            if (score > 5){
                                score-=5;
                            } else {
                                score = 0;
                            }
                                
                    }

                    Click.playMusicOnce();
                        
                }
                if (mouseX1 >= DRAG.x && mouseX1 <= DRAG.x + DRAG.w && mouseY1 >= DRAG.y && mouseY1 <= DRAG.y + DRAG.h)
                {
                    drag++;
                    // 3 là kéo
                        int CheckDrag = Random(1,2);
                    
                        if (CheckBDH(3,CheckDrag) == true ){
                            count_Player++;
                            score+=5;
                        } else {
                            count_AI++;
                                if (score > 5){
                                score-=5;
                            } else {
                                score = 0;
                            }  
                    }

                    Click.playMusicOnce();
                    
                }
                sum_BDH = 5 - drag - hammer - bag;
                if (sum_BDH < 0){
                    sum_BDH = 0;
                }
                if (score >= HighScore){
                    HighScore = score;
                    std::string textHighScore = "HIGHSCORE : " + std::to_string(HighScore);
                    if (!FontHighScore.loadFromRenderedText(textHighScore.c_str(), ColorHighScore)) {
                        printf( "Failed to render text texture!\n" );
                    }
                }
                
                    
            }  
            if ( e1.type == SDL_MOUSEBUTTONDOWN && Level == 2 ) {
            int mouseX1 = e1.button.x;
            int mouseY1 = e1.button.y;        
            SDL_GetMouseState(&mouseX1, &mouseY1);
            
            if (mouseX1 >= 0 && mouseX1 <= SCREEN_WIDTH && mouseY1 >= 0 && mouseY1 <= SCREEN_HEIGHT) {
                        if (mouseX1 >= BossOne.x && mouseX1 <= BossOne.x + BossOne.w && mouseY1 >= BossOne.y && mouseY1 <= BossOne.y + BossOne.h){
                        SDL_RenderCopy(renderer,Boss1.getTexture(),NULL,&BossOne);
                            score += 30;
                            if(checkCollision_2Wall(Pos,BossOne)  ){
                            isTest = true;
                            } 
                            
                        }
                        else if (mouseX1 >= BossTwo.x && mouseX1 <= BossTwo.x + BossTwo.w && mouseY1 >= BossTwo.y && mouseY1 <= BossTwo.y + BossTwo.h){
                        SDL_RenderCopy(renderer,Boss1.getTexture(),NULL,&BossTwo);
                        score +=30;
                        if(checkCollision_2Wall(Pos,BossTwo)  ){
                            isTest = true;
                            } 
                    }  else if (mouseX1 >= BossThree.x && mouseX1 <= BossThree.x + BossThree.w && mouseY1 >= BossThree.y && mouseY1 <= BossThree.y + BossThree.h){
                        SDL_RenderCopy(renderer,Boss1.getTexture(),NULL,&BossThree);
                        score +=30;
                        if(checkCollision_2Wall(Pos,BossThree)  ){
                            isTest = true;
                            } 
                    }
                    score -= 20;
                    if (score < 0 ){
                        score =0;
                    }
                        
                    FontScore.render_Map( SCREEN_WIDTH/2, 0 );
                    FontHighScore.render_Map( SCREEN_WIDTH/2 + Figure_Size, 0 );
                        SDL_RenderPresent(renderer);
                    
                }
 
            }
                        
        }

            
    if ( Level == 1 ){
       // Tạo tường thẳng đứng
        SDL_Rect WallOy[3];
        WallOy[0] = {SCREEN_WIDTH / 6 - 4 , SCREEN_HEIGHT / 4, 5, SCREEN_HEIGHT  / 4};
        WallOy[1] = {SCREEN_WIDTH / 3, 0, 5, (SCREEN_HEIGHT -64) / 2};
        WallOy[2] = {SCREEN_WIDTH * 3 / 4, SCREEN_HEIGHT / 4 - 29, 5, SCREEN_HEIGHT / 2 + 30};


        // Tao đường thẳng nằm ngang
        SDL_Rect WallOx[4];
        WallOx[0] = {0, SCREEN_HEIGHT / 4, SCREEN_HEIGHT / 6 + 45, 5};
        WallOx[1] = {  64 + SCREEN_WIDTH / 4, SCREEN_HEIGHT / 8 +50 , SCREEN_WIDTH / 6, 5};
        WallOx[2] = {SCREEN_WIDTH * 7 / 10, SCREEN_HEIGHT / 5, SCREEN_HEIGHT / 6, 5};
        WallOx[3] = {0, SCREEN_HEIGHT * 3 / 4, SCREEN_WIDTH * 3 / 4 + 45, 5}; 

         for (int i = 0; i < 3; i++) {
            Map1.Render_Box(&WallOy[i]);
            Map1.Render_Box(&WallOx[i]);
            // SDL_RenderDrawRect(renderer, &WallOy[i]);
            //SDL_RenderDrawRect(renderer, &WallOx[i]);
        }
           // SDL_RenderDrawRect(renderer, &WallOx[3]);
           Map1.Render_Box(&WallOx[3]);

        // Đặt lại màu vẽ ban đầu
        //SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        
         SDL_RenderPresent(renderer);

        // Scroll background
            scrollingOffset--;
        if (scrollingOffset < -Map2.getWidth()) {
            scrollingOffset = 0;
        }

        // Clear screen
        SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
        SDL_RenderClear(renderer);

        //Render background
        Map2.render_Map( scrollingOffset, 0 );
    
            // Vẽ background tiếp theo để che phủ toàn bộ màn hình
        Map2.render_Map(scrollingOffset + Map2.getWidth(), 0);

        SDL_Color black = {0x00, 0x00, 0x00};
        std::string textBDH = "Remaining turns : ";

        if(checkCollision_2Wall(Pos,Boss) ) 
        {
            figure.CreateMaze(0);

			FontScore.render_Map( SCREEN_WIDTH/2, 0 );
            FontHighScore.render_Map( SCREEN_WIDTH/2 + Figure_Size, 0 );
            if (score >= HighScore){
                HighScore =score;
            }

           
             SDL_RenderCopy(renderer,Hammer.getTexture(),NULL, &HAMMER);
             SDL_RenderCopy(renderer,Bag.getTexture(),NULL, &BAG);
             SDL_RenderCopy(renderer,Drag.getTexture(),NULL, &DRAG);

            //SDL_Rect Figure_Boss= {SCREEN_WIDTH * 3/4 , SCREEN_HEIGHT/2 , Figure_Size, Figure_Size};
            SDL_Rect Boss_Figure = {SCREEN_WIDTH *1/4   , SCREEN_HEIGHT/2 - Figure_Size + 35 , Figure_Size *3/2, Figure_Size*3/2};
            SDL_Rect Fire = {SCREEN_WIDTH *1/4 + 128   , SCREEN_HEIGHT/2 - Figure_Size + 80 , Figure_Size, Figure_Size};

             // Render the sprite using the current frame
            gSpriteSheetTexture.render( SCREEN_WIDTH * 3/4,  SCREEN_HEIGHT *2/5, &gSpriteClips[ currentFrame ] );
            
             // Tính toán thời gian đã trôi qua kể từ lần cuối cập nhật frameBoss
            Uint32 current_time = SDL_GetTicks();
            Uint32 elapsed_time = current_time - timer;

            // Nếu đã trôi qua đủ 0.5 giây, cập nhật frameBoss và đặt lại giá trị của timer
            if (elapsed_time >= 200) {
                frameBoss++;
                timer = current_time;
            }
                    if (frameBoss %4 == 1){
                SDL_RenderCopy(renderer,Boss1.getTexture(),NULL,&Boss_Figure);
                SDL_RenderCopy(renderer,Fire1.getTexture(),NULL,&Fire);
            } else if (frameBoss %4 == 2){
                SDL_RenderCopy(renderer,Boss2.getTexture(),NULL,&Boss_Figure);
                SDL_RenderCopy(renderer,Fire2.getTexture(),NULL,&Fire);
            } else if (frameBoss %4 == 3){
                SDL_RenderCopy(renderer,Boss3.getTexture(),NULL,&Boss_Figure);
                SDL_RenderCopy(renderer,Fire3.getTexture(),NULL,&Fire);
            } else if (frameBoss %4 == 0){
                SDL_RenderCopy(renderer,Boss4.getTexture(),NULL,&Boss_Figure);
                SDL_RenderCopy(renderer,Fire4.getTexture(),NULL,&Fire);
            }
            //SDL_RenderCopy(renderer,Boss1.getTexture(),NULL,&Boss_Figure);
           
        
             //Render text
            std::string Turns = std::to_string(sum_BDH);
            textBDH +=Turns ;

            if (!FontBDH.loadFromRenderedText(textBDH.c_str(), black )) {
                printf( "Failed to render text texture!\n" );
            }
             //Render current frame
            FontBDH.render_Map( SCREEN_WIDTH/4, SCREEN_HEIGHT/2 - Figure_Size );

                
            if (sum_BDH == 0) 
            {
            SDL_RenderClear(renderer);
            
            FontScore.render_Map(SCREEN_WIDTH / 2, 0);
            FontHighScore.render_Map(SCREEN_WIDTH /2 +Figure_Size,0);

            if(count_Player     < count_AI ) {

                Uint32 startTime = SDL_GetTicks(); // Lấy thời gian bắt đầu
                Uint32 elapsedTime = 0;

                while (elapsedTime < 2500) {
                elapsedTime = SDL_GetTicks() - startTime;

                // Vẽ nội dung màn hình
                SDL_RenderClear(renderer);
                Map1.render(0,0);
                std::string lose = "YOU LOSE ";
                if (!YouLose.loadFromRenderedText(lose.c_str(), textColor)) {
                    printf("Failed to render text texture!\n");
                }
                FontScore.render_Map(SCREEN_WIDTH / 2, 0);
                FontHighScore.render_Map(SCREEN_WIDTH /2 +Figure_Size,0);
                YouLose.render_Map(SCREEN_WIDTH / 2 - Figure_Size/4, SCREEN_HEIGHT / 2);
                
                frameLose++;
                gDeadSpriteTexture.render(SCREEN_WIDTH / 2 - Figure_Size/4 , SCREEN_HEIGHT/2 + 16 ,&gDeadSpriteClips[ frameLose/32 ]);

                if (frameLose/32 >=4 ){
                    frameLose =0;
                }

                SDL_RenderPresent(renderer);
            }

            quit1 = true ;
            break;

        }
        else{
            while  (count_Player   > count_AI) {
                
                Uint32 startTime = SDL_GetTicks(); // Lấy thời gian bắt đầu
                Uint32 elapsedTime = 0;

                while (elapsedTime < 2000) {
                elapsedTime = SDL_GetTicks() - startTime;

                // Vẽ nội dung màn hình
                    SDL_RenderClear( renderer );
                    score+= 50;
                    
                std::string win = "YOU WIN ";
                if (!YouWin.loadFromRenderedText(win.c_str(), textColor)) {
                    printf("Failed to render text texture!\n");
                }
                Map2.render(0,0);
                YouWin.render_Map(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);
                FontScore.render_Map( SCREEN_WIDTH/2, 0 );
                FontHighScore.render_Map( SCREEN_WIDTH/2 + Figure_Size, 0 );
                    if (score >= HighScore){
                        HighScore =score;
                    }
                
                frameWin++;
                gWinSpriteTexture.render(SCREEN_WIDTH / 2, SCREEN_HEIGHT/2 + 16 ,&gWinSpriteClips[ frameWin/14 ]);
                if (frameWin/14 >=7 ){
                    frameWin =0;
                }

                SDL_RenderPresent(renderer);
                }

                CheckScore = true;
                Level = 2;
                break;      
                    }


             } 
        }
    }
        else {
            // Vẽ các đối tượng khác lên renderer
           
            figure.render();
                   
            SDL_Rect TypeTargets[points.size()];
            SDL_Rect TypeTargets1[points.size()];    

            for (int i = 0; i < points.size(); i++) {

            TypeTargets[i] = {points[i].x, points[i].y, ICON_SIZE*4/5, ICON_SIZE*4/5};
            TypeTargets1[i] = {points[i].x, points[i].y, ICON_SIZE/4, ICON_SIZE/4};


            SDL_RenderCopy(renderer, TargetTexture[2].getTexture(), NULL, &TypeTargets[i]);
            }

            int hitIndex = checkCollision1(Pos, TypeTargets1, points.size());
             Ting.loadAudio("C:/FirstGame/Sound/Ting.wav");

             if (hitIndex >= 0)
              {  // Có va chạm
           
                if (checkCollision_2Wall(Pos,TypeTargets1[hitIndex])){
                    Ting.playMusicOnce();
                } 

            // Xóa texture va chạm
            points.erase(points.begin() + hitIndex);
            score++;
            if (score >= HighScore){
                HighScore = score;
                std::string textHighScore = "HIGHSCORE : " + std::to_string(HighScore);
                if (!FontHighScore.loadFromRenderedText(textHighScore.c_str(), ColorHighScore)) {
                    printf( "Failed to render text texture!\n" );
                }
            }

            // Xóa cảnh vẽ hiện tại
            //SDL_RenderClear(renderer);

            // Vẽ lại các texture còn lại
            //SDL_Rect TypeTargets[points.size()];
            //for (int i = 0; i < points.size(); i++) {
            //TypeTargets[i] = {points[i].x, points[i].y, ICON_SIZE, ICON_SIZE};
            //SDL_RenderCopy(renderer, TargetTexture[2].getTexture(), NULL, &TypeTargets[i]);
            // }

            // Cập nhật renderer
            SDL_RenderPresent(renderer);
        }  

        
	   // std::string text = "SCORE : ";
        //std::string myScore = std::to_string(score);

        //text += myScore;
        //if (!FontScore.loadFromRenderedText(text.c_str(), textColor))

		//{
		//	printf( "Failed to render text texture!\n" );
		//}
                    	
        FontScore.render_Map( SCREEN_WIDTH/2, 0 );         
        FontHighScore.render_Map( SCREEN_WIDTH/2 + Figure_Size, 0 );
                
         // Tính toán thời gian đã trôi qua kể từ lần cuối cập nhật frameBoss
        Uint32 current_time = SDL_GetTicks();
        Uint32 elapsed_time = current_time - timer;

        // Nếu đã trôi qua đủ 0.5 giây, cập nhật frameBoss và đặt lại giá trị của timer
        if (elapsed_time >= 200) {
            frameBoss++;
            timer = current_time;
        }
        if (frameBoss %4 == 1){
        SDL_RenderCopy(renderer,Boss1.getTexture(),NULL,&Boss);
        SDL_RenderCopy(renderer,Fire2.getTexture(),NULL,&Fire_Level1);
        } else if (frameBoss %4 == 2){
            SDL_RenderCopy(renderer,Boss2.getTexture(),NULL,&Boss);
            SDL_RenderCopy(renderer,Fire3.getTexture(),NULL,&Fire_Level1);
        } else if (frameBoss %4 == 3){
            SDL_RenderCopy(renderer,Boss3.getTexture(),NULL,&Boss);
             SDL_RenderCopy(renderer,Fire4.getTexture(),NULL,&Fire_Level1);
        } else if (frameBoss %4 == 0){
            SDL_RenderCopy(renderer,Boss4.getTexture(),NULL,&Boss);
             SDL_RenderCopy(renderer,Fire1.getTexture(),NULL,&Fire_Level1);
        }
        
        } 
              // Cập nhật renderer

                    SDL_RenderPresent(renderer);

                    } 
                    
         if ( Level == 2){
           
            if (CheckScore){
                 score = 100 + 5 *(count_Player - count_AI);;
                 CheckScore = false;
            }
           
            if (HighScore <=score ){
                HighScore = score;
            }
            
               // SDL_RenderClear(renderer);
               figure.CreateMaze(Level);
                // Render the sprite using the current frame
                figure.render();
                //gSpriteSheetTexture.render( figure.getPosX(), figure.getPosY(), &gSpriteClips[ currentFrame ] );
             
                FontScore.render_Map( SCREEN_WIDTH/2, 0 );
                FontHighScore.render_Map( SCREEN_WIDTH/2 + Figure_Size, 0 );
                
                SDL_RenderPresent(renderer);
            }
            
          
        while  (isTest == true ){

        SoundLoseGame.loadAudio("C:/FirstGame/Sound/LoseGame.wav");
        SoundLoseGame.playMusicOnce();

        Uint32 startTime = SDL_GetTicks(); // Lấy thời gian bắt đầu
        Uint32 elapsedTime = 0;

        while (elapsedTime < 2000) {

        elapsedTime = SDL_GetTicks() - startTime;

        // Vẽ nội dung màn hình
        SDL_RenderClear(renderer);
        Map1.render(0,0);
        std::string lose = "YOU LOSE ";
        FontScore.render_Map( SCREEN_WIDTH/2, 0 );
        FontHighScore.render_Map( SCREEN_WIDTH/2 + Figure_Size, 0 );
        if (!YouLose.loadFromRenderedText(lose.c_str(), textColor)) {
            printf("Failed to render text texture!\n");
        }
        YouLose.render_Map(SCREEN_WIDTH / 2 - Figure_Size/4, SCREEN_HEIGHT / 2);


        frame++;
        gDeadSpriteTexture.render(SCREEN_WIDTH / 2 - Figure_Size/4 , SCREEN_HEIGHT/2 + 16 ,&gDeadSpriteClips[ frame/32 ]);

        if (frame/32 >=4 ){
            frame =0;
        }

        SDL_RenderPresent(renderer);

    }
    quit1 = true ;
    break;
    }
    if (score <= 0 )
    {
        isTest = true;
        } 
         
                } 
                
             }  

            // Kiểm tra xem chuột có nằm trên nút "Exit Game" không
            if (mouseX >= exitButtonRect.x && mouseX <= exitButtonRect.x + exitButtonRect.w && mouseY >= exitButtonRect.y && mouseY <= exitButtonRect.y + exitButtonRect.h)
            {
                // Thực hiện chức năng của nút "Exit Game"
                quit = true;

                }
         
            }    
        }
        //Clear screen
        SDL_RenderClear(renderer);
        BackGroundTexture.Render_Box(NULL);
        MenuStartTexture.Render_Box(&startButtonRect);
        MenuExitTexture.Render_Box(&exitButtonRect);
   
        //Update screen
        SDL_RenderPresent(renderer);
       
   
}


SoundTrack.closeMusic();
Ting.close();
Ting.closeMusic();
SoundTrack.close();
Click.close();
Click.closeMusic();
SoundLoseGame.close();
SoundLoseGame.closeMusic();
close();

return 0;
}