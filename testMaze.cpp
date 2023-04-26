#include <SDL2/SDL.h>

const int SCREEN_WIDTH = 900;
const int SCREEN_HEIGHT = 600;
const int ROWS = 10;
const int COLUMNS = 15;

int main(int argc, char* args[])
{
    // Khởi tạo SDL2
    SDL_Init(SDL_INIT_VIDEO);

    // Tạo cửa sổ
    SDL_Window* window = SDL_CreateWindow("Grid", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);

    // Tạo renderer
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    // Tính toán kích thước của mỗi ô
    int cellWidth = SCREEN_WIDTH / ROWS;
    int cellHeight = SCREEN_HEIGHT / COLUMNS;

    // Tạo hình chữ nhật cho mỗi ô
    SDL_Rect grid[COLUMNS][ROWS];
    for (int i = 0; i < COLUMNS; i++) {
        for (int j = 0; j < ROWS; j++) {
            SDL_Rect rect = { j * cellWidth, i * cellHeight, cellWidth, cellHeight };
            grid[i][j] = rect;
        }
    }

    // Vẽ lưới
    for (int i = 0; i < COLUMNS; i++) {
        for (int j = 0; j < ROWS; j++) {
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
            SDL_RenderDrawRect(renderer, &grid[i][j]);
        }
    }

    // Vẽ lưới lên màn hình
    SDL_RenderPresent(renderer);

    // Chờ người dùng đóng cửa sổ
    bool quit = false;
    SDL_Event e;
    while (!quit) {
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                quit = true;
            }
        }
    }

    // Hủy renderer, cửa sổ và thoát SDL2
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
