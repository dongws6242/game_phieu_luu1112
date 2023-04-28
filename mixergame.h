#ifndef MIXERGAME_H
#define MIXERGAME_H

class Mixer1 {
    public:
     
        // Initializes variables
        Mixer1();

        // Deallocates memory
        ~Mixer1();
        
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
        void close();
       
};

#endif
