#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>
#include <string>

//Screen dimension constants
const int SCREEN_WIDTH = 900;
const int SCREEN_HEIGHT = 600;

SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;
SDL_Texture* texture = NULL;

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

    return true;
}

//Loads media
bool loadMedia()
{
    //Load PNG image
    SDL_Surface* loadedSurface = IMG_Load("C:/FisrtGame/BackGround.png");
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

    //Get rid of old loaded surface
    SDL_FreeSurface(loadedSurface);

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

    //Quit SDL subsystems
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

//Clear screen
SDL_RenderClear(renderer);

//Render texture to screen
SDL_RenderCopy(renderer, texture, NULL, NULL);

//Update screen
SDL_RenderPresent(renderer);
}}
close();

return 0;
}
