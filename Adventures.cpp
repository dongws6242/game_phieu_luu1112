#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>
#include<SDL2/SDL_ttf.h>
#include <string>
#include<random>
#include"mixergame.h"


//Screen dimension constants
 const int SCREEN_WIDTH = 900;
 const int SCREEN_HEIGHT = 600;
 int score = 0;

 // Khai báo biến cho nút "Start Game"
SDL_Rect startButtonRect ;
 // Khai báo biến cho nút "Exit Game"
SDL_Rect exitButtonRect;
               
SDL_Window* window = NULL;
 SDL_Renderer* renderer = NULL;
 //Globally used font
TTF_Font *gFont = NULL;

 //Walking animation
const int WALKING_ANIMATION_FRAMES = 8;
const int SumOfTargets = 4;
// The background scrolling offset
int scrollingOffset = 0;

SDL_Rect gSpriteClips[ WALKING_ANIMATION_FRAMES ];

const int ICON_SIZE =32;
const int SumOfGoldTargets = 20;

SDL_Rect SumTargets[SumOfGoldTargets];

//Xoay nhân vật
    bool isMoving; 
//Test Va chạm
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
        void renderBackground();
        void renderMenuStart();
        void renderMenuExit();
        void renderTargets(SDL_Rect *Targets);

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
 LTexture TargetTexture[SumOfTargets];
 LTexture FontScore;

 LTexture Map1;
 LTexture BackGroundTexture;


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
    SDL_RenderCopyEx(renderer, mTexture, clip, &renderQuad, angle, center, flipDirection);
}

void LTexture::renderBackground(){
    SDL_RenderCopy(renderer, mTexture, NULL,NULL);
}

void LTexture::renderMenuStart(){
        // Hiển thị các nút menu
        SDL_RenderCopy(renderer, mTexture, NULL, &startButtonRect);
}
void LTexture::renderMenuExit(){
             SDL_RenderCopy(renderer, mTexture, NULL, &exitButtonRect);

}
void LTexture::renderTargets(SDL_Rect* Targets) {
        // sử dụng kích thước mặc định của texture để vẽ
        SDL_RenderCopy(renderer, mTexture, NULL, Targets);
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
		gSpriteClips[ i ].x =   i*128;
		gSpriteClips[ i ].y =   0;
		gSpriteClips[ i ].w =  128;
		gSpriteClips[ i ].h = 128;
    }
	}
    if (!TargetTexture[0].loadTexture("C:/FirstGame/Picture/Sad.png")){
        printf("Failed to load Target!\n");
        return false;}

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

          // Getter function for mPosX
        int getPosX() const {
            return mPosX;
        }

        // Getter function for mPosY
        int getPosY() const {
            return mPosY;
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

int checkCollision1(SDL_Rect a, SDL_Rect b[], int size) {
    for (int i = 0; i < size; i++) {
        if (SDL_HasIntersection(&a, &b[i])) {
            return i;
            score++;
        }
    }
    return -1;
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

int  Random(int a,int b){
        // Khởi tạo engine
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(a, b);

    // Sinh số nguyên ngẫu nhiên trong khoảng [a, b]
    int random_number = dist(gen);

    return random_number;
}


            // Hàm tạo tọa độ ngẫu nhiên, cách nhau một khoảng bằng 32
SDL_Point randomPoint() {
    int x = Random(1, 27)*ICON_SIZE ;
    int y = Random(1, 17)*ICON_SIZE ;
    return {x, y};
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

                    // Scroll background
                    --scrollingOffset;
                    if (scrollingOffset < -Map1.getWidth()) {
                        scrollingOffset = 0;
                    }

                    // Clear screen
                    SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
                    SDL_RenderClear(renderer);

                    //Render background
				    Map1.render( scrollingOffset, 0 );
				

                    // Vẽ background tiếp theo để che phủ toàn bộ màn hình
                    Map1.render(scrollingOffset + Map1.getWidth(), 0);
        

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
    MenuStartTexture.free();
    MenuExitTexture.free();
	gSpriteSheetTexture.free();
    Map1.free();
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
                 // Tạo một vector chứa các tọa độ ngẫu nhiên
            std::vector<SDL_Point> points;

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
   
                startButtonRect.x = SCREEN_WIDTH/2 ;    // Tọa độ x
                startButtonRect.y = SCREEN_HEIGHT/2 + 45 ;    // Tọa độ y
                startButtonRect.w = 200;    // Chiều rộng
                startButtonRect.h = 50;     // Chiều cao

                exitButtonRect.x = SCREEN_WIDTH/2 ;     // Tọa độ x
                exitButtonRect.y = SCREEN_HEIGHT/2 + 110;     // Tọa độ y
                exitButtonRect.w = 200;     // Chiều rộng
                exitButtonRect.h = 50;      // Chiều cao


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
    mixer1.playmusic1();;

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
                    figure.CreateMaze(1);

                    // Xử lý sự kiện
                    while (SDL_PollEvent(&e1) != 0) {
                        if (e1.type == SDL_QUIT) {
                            quit1 = true;
                            isTest = true;
                        }
                        // Xử lý sự kiện cho đối tượng figure
                        figure.handleEvent(e1);
                    }

                    // Di chuyển đối tượng figure
                    figure.move();

                    // Vẽ các đối tượng khác lên renderer
                    figure.render();
                   
         // Vẽ các texture
                    SDL_Rect Pos = {figure.getPosX(),figure.getPosY(), 128, 128};

                    SDL_Rect TypeTargets[points.size()];   
            
                    for (int i = 0; i < points.size(); i++) {

                     TypeTargets[i] = {points[i].x, points[i].y, ICON_SIZE, ICON_SIZE};

                    SDL_RenderCopy(renderer, TargetTexture[2].getTexture(), NULL, &TypeTargets[i]);
                        }
            int hitIndex = checkCollision1(Pos, TypeTargets, points.size());

            if (hitIndex >= 0) {  // Có va chạm
                // Xóa texture va chạm
                points.erase(points.begin() + hitIndex);
                score++;
                // Xóa cảnh vẽ hiện tại
                SDL_RenderClear(renderer);

                // Vẽ lại các texture còn lại
                SDL_Rect TypeTargets[points.size()];
                for (int i = 0; i < points.size(); i++) {
                    TypeTargets[i] = {points[i].x, points[i].y, ICON_SIZE, ICON_SIZE};
                    SDL_RenderCopy(renderer, TargetTexture[2].getTexture(), NULL, &TypeTargets[i]);
                }
               
                    // Cập nhật renderer
                    SDL_RenderPresent(renderer);
            }

             //Render text
		SDL_Color textColor = { 100, 200, 165 };
		std::string text = "SCORE : ";
        std::string myScore = std::to_string(score);

        text += myScore;
        if (!FontScore.loadFromRenderedText(text.c_str(), textColor))

		{
			printf( "Failed to render text texture!\n" );
		}

                    	//Render current frame
				FontScore.render( SCREEN_WIDTH/2, 0 );
                
          
                    // Cập nhật renderer
                    SDL_RenderPresent(renderer);

                    // Kiểm tra điều kiện thoát vòng lặp
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
        BackGroundTexture.renderBackground();
        MenuStartTexture.renderMenuStart();
        MenuExitTexture.renderMenuExit();
   
        //Update screen
        SDL_RenderPresent(renderer);
       
   
}

mixer1.closeMusic();
mixer1.close();
close();

return 0;
}
