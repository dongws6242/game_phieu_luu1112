#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>
#include <string>
#include<SDL2/SDL_mixer.h>

//Screen dimension constants
const int SCREEN_WIDTH = 900;
const int SCREEN_HEIGHT = 600;

const int FPS = 60;
const int FRAME_TIME = 1000 / FPS;

SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;
SDL_Texture* texture = NULL;
SDL_Surface* startButtonSurface = NULL;
SDL_Texture* exitButtonTexture = NULL;
SDL_Texture* startButtonTexture = NULL;
SDL_Surface* exitButtonSurface = NULL;
 SDL_Texture* Map1 = NULL;

//Xoay nhân vật
    bool isMoving; 
//Test Va chạm
    bool isTest;

class Mixer1 {
    public:
     
        // Initializes variables
        Mixer1();

        // Deallocates memory
        ~Mixer1();
        Mix_Music* music = NULL;
        static bool isPlayingMusic;  // biến kiểm soát trạng thái phát nhạc
        static bool isMusicPaused;  // biến kiểm soát trạng thái tạm dừng nhạc

        bool initAudio();
        bool loadAudio();
        void PauseMusic();
        void ResumeMusic();
		void closeMusic();
		void PlayMusic();
		void PlayMusicN();
        void playmusic1();
       
};
bool Mixer1::isPlayingMusic = true; // khởi tạo biến static
bool Mixer1::isMusicPaused = false; // khởi tạo biến static

Mixer1::Mixer1()   
{
     }
    Mixer1::~Mixer1(){
        closeMusic();
    }
bool Mixer1::initAudio()
{
    //Initialize SDL
    if (SDL_Init(SDL_INIT_AUDIO) < 0)
    {
        std::cout << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
        return false;
    }
    //Initialize SDL_mixer
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
    {
        std::cout << "SDL_mixer could not initialize! SDL_mixer Error: " << Mix_GetError() << std::endl;
        return false;
    }
    return true;
}

bool Mixer1::loadAudio()
{
     //Load music
    Mix_Music* music = Mix_LoadMUS("C:/FirstGame/Sound/SoundTrack.wav");
    if (music == NULL)
    {
        std::cout << "Failed to load music! SDL_mixer Error: " << Mix_GetError() << std::endl;
        return false;
    }
    //Play music infinitely
    Mix_PlayMusic(music, -1);
    return true;
}

void Mixer1::PauseMusic()
{
     if (Mix_PlayingMusic() == 1) // kiểm tra xem có đang phát nhạc hay không
    {
        Mix_PauseMusic(); // tạm dừng nhạc
        isPlayingMusic = false; // cập nhật biến kiểm soát trạng thái phát nhạc
        isMusicPaused = true; // cập nhật biến kiểm soát trạng thái tạm dừng nhạc
    }
}

void Mixer1::ResumeMusic()
{
   if (isMusicPaused) // kiểm tra xem nhạc đã tạm dừng hay chưa
    {
        Mix_ResumeMusic(); // tiếp tục phát nhạc
        isPlayingMusic = true; // cập nhật biến kiểm soát trạng thái phát nhạc
        isMusicPaused = false; // cập nhật biến kiểm soát trạng thái tạm dừng nhạc
    }
}
void Mixer1::closeMusic()
{
    //Free music and close SDL
    if (music != NULL)
    {
        Mix_FreeMusic(music);
        music = NULL;
    }
    Mix_CloseAudio();
    
}

void Mixer1::PlayMusic()
{
    
                if (isPlayingMusic) // nếu đang phát nhạc
                {
                    PauseMusic(); // tạm dừng nhạc
                }
                else // nếu đã tạm dừng nhạc
                {
                    ResumeMusic(); // tiếp tục phát nhạc
                }
}

void Mixer1::PlayMusicN(){
    // nếu đang phát nhạc và nhạc đã kết thúc
        if (isPlayingMusic && Mix_PlayingMusic() == 0)
        {
            Mix_PlayMusic(music, -1); // phát nhạc vô hạn lần
        }
}
void Mixer1::playmusic1(){
     Mix_PlayMusic(music, -1);
}

class animation {
    public:
		//Initializes variables
		animation();

		//Deallocates memory
		~animation();

		//Loads image at specified path
		bool loadAnimation( std::string path );
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

        void close();

		//Gets image dimensions
		int getWidth();
		int getHeight();

	private:
		//The actual hardware texture
		SDL_Texture* mAnimation;
          
		//Image dimensions
		int mWidth;
		int mHeight;

        
};

//Walking animation
const int WALKING_ANIMATION_FRAMES = 8;
SDL_Rect gSpriteClips[ WALKING_ANIMATION_FRAMES ];
 animation gSpriteSheetTexture;
animation::animation()
{
	//Initialize
	mAnimation = NULL;
	mWidth = 0;
	mHeight = 0;
}

animation::~animation()
{
	//Deallocate
	free();
}

bool animation::loadAnimation( std::string path )
{
	//Get rid of preexisting texture
	free();

	//The final texture
	SDL_Texture* newAnimation = NULL;

	//Load image at specified path
	SDL_Surface* loadAnimation = IMG_Load( path.c_str() );
	if( loadAnimation == NULL )
	{
		printf( "Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError() );
	}
	else
	{
		//Color key image
		SDL_SetColorKey( loadAnimation, SDL_TRUE, SDL_MapRGB( loadAnimation->format, 0, 0xFF, 0xFF ) );

		//Create texture from surface pixels
        newAnimation = SDL_CreateTextureFromSurface( renderer, loadAnimation );
		if( newAnimation == NULL )
		{
			printf( "Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError() );
		}
		else
		{
			//Get image dimensions
			mWidth = loadAnimation->w;
			mHeight = loadAnimation->h;
		}


		//Get rid of old loaded surface
		SDL_FreeSurface( loadAnimation );
	}

	//Return success
	mAnimation = newAnimation;
	return mAnimation != NULL;
}

void animation::free()
{
	//Free texture if it exists
	if( mAnimation != NULL )
	{
		SDL_DestroyTexture( mAnimation );
		mAnimation = NULL;
		mWidth = 0;
		mHeight = 0;
	}
}

void animation::setColor( Uint8 red, Uint8 green, Uint8 blue )
{
	//Modulate texture rgb
	SDL_SetTextureColorMod( mAnimation, red, green, blue );
}

void animation::setBlendMode( SDL_BlendMode blending )
{
	//Set blending function
	SDL_SetTextureBlendMode( mAnimation, blending );
}
		
void animation::setAlpha( Uint8 alpha )
{
	//Modulate texture alpha
	SDL_SetTextureAlphaMod( mAnimation, alpha );
}

void animation::render(int x, int y, SDL_Rect* clip, double angle, SDL_Point* center, SDL_RendererFlip flip)
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
    if (isMoving) { // Nếu nhân vật đang di chuyển
        if (flip == SDL_FLIP_NONE) { // Nếu nhân vật đang quay về phía bên phải
            flipDirection = SDL_FLIP_HORIZONTAL; // Quay nhân vật về phía bên trái
        } else if (flip == SDL_FLIP_HORIZONTAL) { // Nếu nhân vật đang quay về phía bên trái
            flipDirection = SDL_FLIP_NONE; // Quay nhân vật về phía bên phải
        }
    } else { // Nếu nhân vật đang đứng yên
        flipDirection = flip; // Giữ nguyên hướng quay hiện tại của nhân vật
    }

    // Render to screen
    SDL_RenderCopyEx(renderer, mAnimation, clip, &renderQuad, angle, center, flipDirection);
}



int animation::getWidth()
{
	return mWidth;
}

int animation::getHeight()
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
	if( !gSpriteSheetTexture.loadAnimation( "C:/FirstGame/Adventures_figure/Walk.png" ) )
	{
		printf( "Failed to load walking animation texture!\n" );
		return false; }
	else
	{     for (int i=0; i<WALKING_ANIMATION_FRAMES; i++){
        //Set sprite clips
		gSpriteClips[ i ].x =   i*128;
		gSpriteClips[ i ].y =   0;
		gSpriteClips[ i ].w =  128;
		gSpriteClips[ i ].h = 128;
    }
	}
    return true;
}
enum class Direction { LEFT, RIGHT, DOWN,UP};

class Figure {
     public:
		//The dimensions of the dot
		static const int FIGURE_WIDTH = 128;
		static const int FIGURE_HEIGHT = 128;

		//Maximum axis velocity of the dot
		static const int FIGURE_VEL = 1;

		//Initializes the variables
		Figure();

		//Takes key presses and adjusts the dot's velocity
		void handleEvent( SDL_Event& e );

		//Moves the dot
		void move();

		//Shows the dot on the screen
		void render();

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


int currentFrame =0;

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

if (checkCollision(mFigure,WallOx,4) == true  or checkCollision(mFigure,WallOy,3) == true ){
    isTest = true;
}

}



void Figure::render(){
    // Render the sprite using the current frame
    gSpriteSheetTexture.render( mPosX, mPosY, &gSpriteClips[ currentFrame ] );
    }
void Figure::CreateMaze(int level) {
    if (level == 1) {
        // Tạo tường thẳng đứng
        SDL_Rect WallOy[3];
        WallOy[0] = {SCREEN_WIDTH / 6 - 4 , SCREEN_HEIGHT / 4, 1, SCREEN_HEIGHT  / 4};
        WallOy[1] = {SCREEN_WIDTH / 3, 0, 1, (SCREEN_HEIGHT -64) / 2};
        WallOy[2] = {SCREEN_WIDTH * 3 / 4, SCREEN_HEIGHT / 4 - 29, 1, SCREEN_HEIGHT / 2 + 30};

        // Tao đường thẳng nằm ngang
        SDL_Rect WallOx[4];
        WallOx[0] = {0, SCREEN_HEIGHT / 4, SCREEN_HEIGHT / 6 + 45, 1};
        WallOx[1] = {  64 + SCREEN_WIDTH / 4, SCREEN_HEIGHT / 8 +50 , SCREEN_WIDTH / 6, 1};
        WallOx[2] = {SCREEN_WIDTH * 7 / 10, SCREEN_HEIGHT / 5, SCREEN_HEIGHT / 6, 1};
        WallOx[3] = {0, SCREEN_HEIGHT * 3 / 4, SCREEN_WIDTH * 3 / 4 + 45, 1};

        // Đặt màu vẽ thành màu đỏ
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);

        for (int i = 0; i < 3; i++) {
            SDL_RenderDrawRect(renderer, &WallOy[i]);
            SDL_RenderDrawRect(renderer, &WallOx[i]);
        }
        SDL_RenderDrawRect(renderer, &WallOx[3]);

        // Đặt lại màu vẽ ban đầu
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

        SDL_RenderPresent(renderer);
    }
}



//Loads media
bool loadMedia()
{
    //Load PNG image
    SDL_Surface* loadedSurface = IMG_Load("C:/FirstGame/Picture/BackGround.png");
    if(loadedSurface == NULL)
    {
        std::cout << "Unable to load image! SDL Error: " << SDL_GetError() << std::endl;
        return false;
    }

    //Create texture from surface pixels
    texture = SDL_CreateTextureFromSurface(renderer, loadedSurface);
    if(texture == NULL)
    {
        std::cout << "Unable to create texture! SDL Error: " << SDL_GetError() << std::endl;
        return false;
    }
    SDL_Surface* startButtonSurface = IMG_Load("C:/FirstGame/Picture/StartButton.png");
    if(startButtonSurface == NULL ){
        std :: cout << "failed : " <<SDL_GetError() << std::  endl;
        return false; 
    }
    //Create texture from surface pixels
        startButtonTexture = SDL_CreateTextureFromSurface(renderer, startButtonSurface);
     if(startButtonTexture == NULL)
    {
        std::cout << "Unable to create texture! SDL Error: " << SDL_GetError() << std::endl;
        return false;
    }
    
    SDL_Surface* exitButtonSurface = IMG_Load("C:/FirstGame/Picture/ExitButton.png");
     if(exitButtonSurface == NULL ){
        std :: cout << "failed : " <<SDL_GetError() << std::  endl;
        return false; 
    }
    //Create texture from surface pixels
        exitButtonTexture = SDL_CreateTextureFromSurface(renderer, exitButtonSurface);
     if(exitButtonTexture == NULL)
    {
        std::cout << "Unable to create texture! SDL Error: " << SDL_GetError() << std::endl;
        return false;
    }

        //Load PNG image
    SDL_Surface* loadedMap = IMG_Load("C:/FirstGame/Picture/Map1.png");
    if(loadedSurface == NULL)
    {
        std::cout << "Unable to load image! SDL Error: " << SDL_GetError() << std::endl;
        return false;
    }
    //Create texture from surface pixels
     Map1 = SDL_CreateTextureFromSurface(renderer, loadedMap);
    if(Map1 == NULL)
    {
        std::cout << "Unable to create texture! SDL Error: " << SDL_GetError() << std::endl;
        return false;
    }
    //Get rid of old loaded surface
    SDL_FreeSurface(loadedSurface);
    SDL_FreeSurface(startButtonSurface);
    SDL_FreeSurface(exitButtonSurface);
    SDL_FreeSurface(loadedMap);
    return true;
}
//Frees media and shuts down SDL
void close()
{
    //Free texture
    SDL_DestroyTexture(texture);
    texture = NULL;

    //Destroy renderer
    SDL_DestroyRenderer(renderer);
    renderer = NULL;

    //Destroy window
    SDL_DestroyWindow(window);
    window = NULL;
    SDL_DestroyTexture(startButtonTexture);
    startButtonTexture = NULL;
    SDL_DestroyTexture(exitButtonTexture);
    exitButtonTexture = NULL;
    SDL_DestroyTexture(Map1);
    Map1 = NULL;
    //Free loaded images
	gSpriteSheetTexture.free();
    //Quit SDL subsystems
    Mix_Quit();
    IMG_Quit();
    SDL_Quit();
}

int main(int argc, char* argv[])
{
        Mixer1 mixer1;
        mixer1.initAudio();
        mixer1.loadAudio();

    if(!init())
    {
        std::cout << "Failed to initialize!" << std::endl;
        return -1;
    }
    if(!mixer1.initAudio()){
        std::cout << "failed" << std::endl;
    }
    if(!loadMedia())
    {
        std::cout << "Failed to load media!" << std::endl;
        return -1;
    }
    mixer1.playmusic1();;

    //Main loop flag
    bool quit = false;

    //Event handler
    SDL_Event e;

    //While application is running
    while(!quit)
    {        // Khai báo biến cho nút "Start Game"
                SDL_Rect startButtonRect;
                startButtonRect.x = SCREEN_WIDTH/2 ;    // Tọa độ x
                startButtonRect.y = SCREEN_HEIGHT/2 + 45 ;    // Tọa độ y
                startButtonRect.w = 200;    // Chiều rộng
                startButtonRect.h = 50;     // Chiều cao

                // Khai báo biến cho nút "Exit Game"
                SDL_Rect exitButtonRect;
                exitButtonRect.x = SCREEN_WIDTH/2 ;     // Tọa độ x
                exitButtonRect.y = SCREEN_HEIGHT/2 + 110;     // Tọa độ y
                exitButtonRect.w = 200;     // Chiều rộng
                exitButtonRect.h = 50;      // Chiều cao

        //Handle events on queue
        while(SDL_PollEvent(&e) != 0)
        {
            //User requests quit//User requests quit
            if(e.type == SDL_QUIT)
            {
            quit = true;
            }
            // nếu nhấn phím p
            else if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_p)
            {    mixer1.PlayMusic();
                    }                   
            if (e.type == SDL_MOUSEBUTTONDOWN)
        {   
            // Lấy vị trí chuột
            int mouseX, mouseY;
            SDL_GetMouseState(&mouseX, &mouseY);
            if (mouseX >= startButtonRect.x && mouseX <= startButtonRect.x + startButtonRect.w && mouseY >= startButtonRect.y && mouseY <= startButtonRect.y + startButtonRect.h)
            {
                bool quit1 = false;
                SDL_Event e1;
                Figure figure;

                while (!quit1) {
                    // Vẽ lại màn hình ở đầu vòng lặp
                    SDL_SetRenderDrawColor( renderer, 0xFF, 0xFF, 0xFF, 0xFF );
                    SDL_RenderClear( renderer );
                    SDL_SetRenderDrawColor( renderer, 0x00, 0x00, 0x00, 0xFF );
                      //Render texture to screen
                    SDL_RenderCopy(renderer, Map1, NULL, NULL);
                    figure.render();
                    figure.CreateMaze(1);
                    

                    while (SDL_PollEvent(&e1) != 0) {
                        if (e1.type == SDL_QUIT) {
                            quit1 = true;
                            isTest = true;
                        }
                        // Xử lý sự kiện cho hình
                        figure.handleEvent( e1 );
                    }
                    figure.move();
                                
                    //Update screen
                    SDL_RenderPresent( renderer );
                    quit1 = isTest;
                                
                }
                isTest = false;
                
             }
           
            // Kiểm tra xem chuột có nằm trên nút "Exit Game" không
            if (mouseX >= exitButtonRect.x && mouseX <= exitButtonRect.x + exitButtonRect.w && mouseY >= exitButtonRect.y && mouseY <= exitButtonRect.y + exitButtonRect.h)
            {
                // Thực hiện chức năng của nút "Exit Game"
                quit = true;
                }
            
        }    
        }
          mixer1.PlayMusicN();
        //Clear screen
        SDL_RenderClear(renderer);
        //Render texture to screen
        SDL_RenderCopy(renderer, texture, NULL, NULL);
          // Hiển thị các nút menu
        SDL_RenderCopy(renderer, startButtonTexture, NULL, &startButtonRect);
        SDL_RenderCopy(renderer, exitButtonTexture, NULL, &exitButtonRect);
        //Update screen
        SDL_RenderPresent(renderer);
   
}
mixer1.closeMusic();
close();

return 0;
}
