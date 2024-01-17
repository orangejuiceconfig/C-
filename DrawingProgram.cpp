#include <SDL2/SDL.h>

using namespace std;

const int WINDOW_HEIGHT = 760, WINDOW_WIDTH = 900;
float size = 20;
SDL_Point lineStart, lineEnd;
bool drawingLine = false;

int Draw(SDL_Renderer *renderer, float x, float y, int size) {
    SDL_Color color = {99, 99, 99, 99};
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);

    // Draw a filled rectangle centered around the specified coordinates
    SDL_Rect rect = { static_cast<int>(x - size / 2), static_cast<int>(y - size / 2), size, size };
    SDL_RenderFillRect(renderer, &rect);
    return 0;
}

int main() {
    SDL_Init(SDL_INIT_VIDEO);

    SDL_Window *window = SDL_CreateWindow("Draw", 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, 0);

    bool running = true;
    float mouseX = WINDOW_WIDTH / 2, mouseY = WINDOW_HEIGHT / 2;

    while (running) 
      {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
	  if (event.type == SDL_MOUSEMOTION) {
	    mouseX = static_cast<float>(event.motion.x);
	    mouseY = static_cast<float>(event.motion.y);
	  }
	  if (event.key.keysym.sym == SDLK_SPACE)
	    {
	      lineStart.x = event.button.x;
	      lineStart.y = event.button.y;
	      drawingLine = true;
	    }
	  if (event.type == SDL_MOUSEBUTTONUP) {
	    if (event.button.button == SDL_BUTTON_RIGHT && drawingLine) {
	      lineEnd.x = event.button.x;
	      lineEnd.y = event.button.y;
	      drawingLine = false;
	    }
	  }
	  if (event.type == SDL_KEYDOWN) {
	    if (event.key.keysym.sym == SDLK_PLUS || event.key.keysym.sym == SDLK_EQUALS)
	      {
                size += 5;
            }
            if (event.key.keysym.sym == SDLK_MINUS) {
                size -= 5;
            }
	    
	    if (event.key.keysym.sym == SDLK_ESCAPE) {
	      running = false;
	    }
	  } else if (event.type == SDL_QUIT) {
	    running = false;
            }  
        }

        SDL_SetRenderDrawColor(renderer, 0x0, 0x0, 0x0, 0xFF);
        SDL_RenderClear(renderer);

        // Drawing logic here
        Draw(renderer, mouseX, mouseY, size);

        // Present the backbuffer
        SDL_RenderPresent(renderer);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}

