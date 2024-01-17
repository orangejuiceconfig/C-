#include <SDL2/SDL.h>
#include <cmath>

const int WINDOW_HEIGHT = 740, WINDOW_WIDTH = 740;
double ROTATION_SPEED = 0.5;

struct Vec3
{
    float x, y, z;
};

void rotate(Vec3 &point, float angleX, float angleY, float angleZ)
{
    float tempX = point.x;
    float tempY = point.y;

    // Rotate around X-axis
    point.y = tempY * cos(angleX) - point.z * sin(angleX);
    point.z = tempY * sin(angleX) + point.z * cos(angleX);

    // Rotate around Y-axis
    float tempZ = point.z;
    point.x = tempX * cos(angleY) + tempZ * sin(angleY);
    point.z = -tempX * sin(angleY) + tempZ * cos(angleY);

    // Rotate around Z-axis
    tempX = point.x;
    point.x = tempX * cos(angleZ) - point.y * sin(angleZ);
    point.y = tempX * sin(angleZ) + point.y * cos(angleZ);
}

// Function to translate (move) the cube by a given offset
void translate(Vec3 cubeVertices[], int numVertices, const Vec3 &offset)
{
    for (int i = 0; i < numVertices; ++i)
    {
        cubeVertices[i].x += offset.x;
        cubeVertices[i].y += offset.y;
        cubeVertices[i].z += offset.z;
    }
}

int main()
{
    SDL_Init(SDL_INIT_VIDEO);

    SDL_Window *window = SDL_CreateWindow("Rotating and Translating Cube", 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, 0);

    bool running = true;

    Vec3 cubePosition = {0.0, 0.0, 0.0}; // Initial position of the cube
    Vec3 cubeVertices[8] = {
        {-100, -100, -100}, {-100, 100, -100}, {100, 100, -100}, {100, -100, -100},
        {-100, -100, 100}, {-100, 100, 100}, {100, 100, 100}, {100, -100, 100}};

    Uint32 prevTicks = SDL_GetTicks();

    while (running)
    {
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            if (event.key.keysym.sym == SDLK_w)
            {
                ROTATION_SPEED += 0.08; // Increase rotation speed by 0.08

                // Limit the maximum rotation speed to 3.0
                if (ROTATION_SPEED > 3.0)
                {
                    ROTATION_SPEED = 3.0;
                }
            }
            if (event.key.keysym.sym == SDLK_s)
            {
                ROTATION_SPEED -= 0.08; // Decrease rotation speed by 0.08

                // Limit the maximum rotation speed to -3.0
                if (ROTATION_SPEED < -3.0)
                {
                    ROTATION_SPEED = -3.0;
                }
            }
            if (event.key.keysym.sym == SDLK_SPACE && ROTATION_SPEED > 0)
            {
                ROTATION_SPEED = 0.0; // Stop cube
            }
            else if (event.key.keysym.sym == SDLK_SPACE && ROTATION_SPEED == 0)
            {
                ROTATION_SPEED = 0.5;
            }
            else if (event.key.keysym.sym == SDLK_SPACE && ROTATION_SPEED < 0)
            {
                ROTATION_SPEED = -0.5;
            }

            if (event.type == SDL_QUIT)
            {
                running = false;
            }
            else if (event.type == SDL_KEYDOWN)
            {
                if (event.key.keysym.sym == SDLK_ESCAPE)
                {
                    running = false;
                }
            }
        }

        // Calculate elapsed time since the previous frame
        Uint32 currentTicks = SDL_GetTicks();
        float deltaTime = (currentTicks - prevTicks) / 1000.0; // Convert milliseconds to seconds
        prevTicks = currentTicks;

        // Clear the window to black
        SDL_SetRenderDrawColor(renderer, 30, 30, 30, 0xFF);
        SDL_RenderClear(renderer);

        // Set the draw color to be white
        SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);

        // Apply rotation to cube vertices
        for (int i = 0; i < 8; ++i)
        {
            rotate(cubeVertices[i], ROTATION_SPEED * deltaTime, ROTATION_SPEED * deltaTime, ROTATION_SPEED * deltaTime);
        }

        // Move the cube to its new position
        translate(cubeVertices, 8, cubePosition);

        // Draw the faces of the cube
        for (int i = 0; i < 4; ++i)
        {
            int next = (i + 1) % 4;
            SDL_RenderDrawLine
	      (renderer, cubeVertices[i].x + WINDOW_WIDTH / 2, cubeVertices[i].y + WINDOW_HEIGHT / 2,
	       cubeVertices[next].x + WINDOW_WIDTH / 2, cubeVertices[next].y + WINDOW_HEIGHT / 2);
	    SDL_RenderDrawLine(renderer, cubeVertices[i + 4].x + WINDOW_WIDTH / 2, cubeVertices[i + 4].y + WINDOW_HEIGHT / 2,
			       cubeVertices[next + 4].x + WINDOW_WIDTH / 2, cubeVertices[next + 4].y + WINDOW_HEIGHT / 2);
	    SDL_RenderDrawLine(renderer, cubeVertices[i].x + WINDOW_WIDTH / 2, cubeVertices[i].y + WINDOW_HEIGHT / 2,
			       cubeVertices[i + 4].x + WINDOW_WIDTH / 2, cubeVertices[i + 4].y + WINDOW_HEIGHT / 2);
	}

	// Present the backbuffer
	SDL_RenderPresent(renderer);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
     SDL_Quit();

    return 0;
}
