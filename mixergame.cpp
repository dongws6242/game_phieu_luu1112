
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include<string>
#include<iostream>
#include"mixergame.h"
#include<SDL2/SDL_mixer.h>



bool Mixer1::isPlayingMusic = true; // khởi tạo biến static
bool Mixer1::isMusicPaused = false; // khởi tạo biến static
Mix_Music* music = NULL;
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
void Mixer1::close(){
    //Quit SDL subsystems
    Mix_Quit();
}
