#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include<SDL2/SDL_mixer.h>
#include <iostream>
#include <string>

//Screen dimension constants
const int SCREEN_WIDTH = 900;
const int SCREEN_HEIGHT = 600;

SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;
SDL_Texture* texture = NULL;
Mix_Music* music = NULL;

bool isPlayingMusic = true; // biến kiểm soát trạng thái phát nhạc
bool isMusicPaused = false; // biến kiểm soát trạng thái tạm dừng nhạc


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

    //Initialize PNG loading
    int imgFlags = IMG_INIT_PNG;
    if(!(IMG_Init(imgFlags) & imgFlags))
    {
        std::cout << "SDL_image could not initialize! SDL_image Error: " << IMG_GetError() << std::endl;
        return false;
    }
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

//Loads media
bool loadMedia()
{
    //Load PNG image
    SDL_Surface* loadedSurface = IMG_Load("C:/FirstGame/BackGround.png");
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
    //Load music
    Mix_Music* music = Mix_LoadMUS("C:/FirstGame/SoundTrack.wav");
    if (music == NULL)
    {
        std::cout << "Failed to load music! SDL_mixer Error: " << Mix_GetError() << std::endl;
        return false;
    }

    //Get rid of old loaded surface
    SDL_FreeSurface(loadedSurface);
    //Play music infinitely
    Mix_PlayMusic(music, -1);
    return true;
}
void PauseMusic()
{
    if (Mix_PlayingMusic() == 1) // kiểm tra xem có đang phát nhạc hay không
    {
        Mix_PauseMusic(); // tạm dừng nhạc
        isPlayingMusic = false; // cập nhật biến kiểm soát trạng thái phát nhạc
        isMusicPaused = true; // cập nhật biến kiểm soát trạng thái tạm dừng nhạc
    }
}
void ResumeMusic()
{
    if (isMusicPaused) // kiểm tra xem nhạc đã tạm dừng hay chưa
    {
        Mix_ResumeMusic(); // tiếp tục phát nhạc
        isPlayingMusic = true; // cập nhật biến kiểm soát trạng thái phát nhạc
        isMusicPaused = false; // cập nhật biến kiểm soát trạng thái tạm dừng nhạc
    }
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
    //Free music and close SDL
    Mix_FreeMusic(music);
    music = NULL;

    //Quit SDL subsystems
    Mix_Quit();
    IMG_Quit();
    SDL_Quit();
}

int main(int argc, char* argv[])
{
    if(!init())
    {
        std::cout << "Failed to initialize!" << std::endl;
        return -1;
    }

    if(!loadMedia())
    {
        std::cout << "Failed to load media!" << std::endl;
        return -1;
    }
   Mix_PlayMusic(music, -1);

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
        }

        // nếu đang phát nhạc và nhạc đã kết thúc
        if (isPlayingMusic && Mix_PlayingMusic() == 0)
        {
            Mix_PlayMusic(music, -1); // phát nhạc vô hạn lần
        }
                

//Clear screen
SDL_RenderClear(renderer);

//Render texture to screen
SDL_RenderCopy(renderer, texture, NULL, NULL);

//Update screen
SDL_RenderPresent(renderer);
}
close();

return 0;
}
