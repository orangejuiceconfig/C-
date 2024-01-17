#include <SDL2/SDL.h>
#include <cmath>
#include <iostream>
#include <fstream>
using namespace std;

const int numberOfVertices = 8;
const int WINDOW_HEIGHT = 740, WINDOW_WIDTH = 900;
float speed = 0.5;
float lastsotationspeed = 0.5;





void recordRotationSpeed(float speed)
{
    if (speed != 0.0)
    {
        lastsotationspeed = speed;
    }
}

struct Vec3
{
    float x, y, z;
};

void translate(Vec3 &point, float offsetsX, float offsetsY, float offsetsZ)
{
    point.x += offsetsX;
    point.y += offsetsY;
    point.z += offsetsZ;
}

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

void handleKeyPress(SDL_Event &event, Vec3 *cubeVertices, float &speed, float &lastsotationspeed, float &deltaX, float &deltaY, float &deltaZ, bool &running)
{
    const Uint8 *keyboard_state_array = SDL_GetKeyboardState(NULL);

    if (event.key.keysym.sym == SDLK_d)
    {
        for (int i = 0; i < numberOfVertices; ++i)
        {
            deltaZ = 1.25;
            cubeVertices[i].x *= deltaZ;
            cubeVertices[i].y *= deltaZ;
            cubeVertices[i].z *= deltaZ;
        }
    }
    else if (event.key.keysym.sym == SDLK_a)
    {
        for (int i = 0; i < numberOfVertices; ++i)
        {
            deltaZ = 0.25;
            cubeVertices[i].x *= deltaZ;
            cubeVertices[i].y *= deltaZ;
            cubeVertices[i].z *= deltaZ;
        }
    }

    if (event.key.keysym.sym == SDLK_w) // Increase rotation speed by 0.08
    {
        speed += 0.08;
        lastsotationspeed = speed;
    }

    if (speed > 3.0) // Limit the maximum rotation speed to 3.0
        speed = 3.0;
    else if (event.key.keysym.sym == SDLK_s)
    {
        speed -= 0.08;
        lastsotationspeed = speed;
    }

    if (speed < -3.0) // Limit the maximum rotation speed to -3.0
        speed = -3.0;
    else if (event.key.keysym.sym == SDLK_SPACE && event.type == SDL_KEYUP) // Toggle stopped
    {
        if (speed == 0)
        {
            speed = lastsotationspeed;
        }
        else
        {
            speed = 0;
        }
    }

    if (event.type == SDL_QUIT)
        running = false;
    else if (event.type == SDL_KEYDOWN)
    {
        if (event.key.keysym.sym == SDLK_ESCAPE)
        {
            running = false;
        }
    }

    if (event.type == SDL_KEYDOWN)
    {
        if (keyboard_state_array[SDL_SCANCODE_UP] && !(keyboard_state_array[SDL_SCANCODE_DOWN]))
        {
            deltaX -= 5.0;
        }
        else if (!keyboard_state_array[SDL_SCANCODE_UP] && keyboard_state_array[SDL_SCANCODE_DOWN])
        {
            deltaX += 5.0;
        }
        if (keyboard_state_array[SDL_SCANCODE_RIGHT] && !keyboard_state_array[SDL_SCANCODE_LEFT])
        {
            deltaY += 5.0;
        }
        else if (!keyboard_state_array[SDL_SCANCODE_RIGHT] && keyboard_state_array[SDL_SCANCODE_LEFT])
        {
            deltaY -= 5.0;
        }
    }
}

void handleDrawAndRender(SDL_Renderer *renderer, Vec3 *cubeVertices, float &speed, float &lastsotationspeed, float &deltaX, float &deltaY, float &deltaZ, bool &running)
{
    // Calculate elapsed time since the previous frame
    Uint32 currentTicks = SDL_GetTicks();
    static Uint32 prevTicks = currentTicks; // Declare prevTicks as static to persist its value between function calls
    float deltaTime = (currentTicks - prevTicks) / 1000.0; // Convert milliseconds to seconds
    prevTicks = currentTicks;

    // Clear the window to black
    SDL_SetRenderDrawColor(renderer, 30, 30, 30, 0xFF);
    SDL_RenderClear(renderer);

    // Set the draw color to be white
    SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);

    // Apply rotation to cube vertices
    for (int i = 0; i < numberOfVertices; ++i)
    {
        rotate(cubeVertices[i], speed * deltaTime, speed * deltaTime, speed * deltaTime);
    }

    // Draw the faces of the cube
    for (int i = 0; i < 4; ++i)
    {
        int next = (i + 1) % 4;
        float posX = WINDOW_WIDTH / 2 + deltaY;
        float posY = WINDOW_HEIGHT / 2 + deltaX;

        SDL_RenderDrawLine(renderer, cubeVertices[i].x + posX, cubeVertices[i].y + posY,
                           cubeVertices[next].x + posX, cubeVertices[next].y + posY);
        SDL_RenderDrawLine(renderer, cubeVertices[i + 4].x + posX, cubeVertices[i + 4].y + posY,
                           cubeVertices[next + 4].x + posX, cubeVertices[next + 4].y + posY);
        SDL_RenderDrawLine(renderer, cubeVertices[i].x + posX, cubeVertices[i].y + posY,
                           cubeVertices[i + 4].x + posX, cubeVertices[i + 4].y + posY);
    }

    // Present the backbuffer
    SDL_RenderPresent(renderer);
}

int main()
{
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window *window = SDL_CreateWindow("Rotating Cube", 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, 0);
    bool running = true;
    Vec3 cubeVertices[numberOfVertices] = {
        {-100, -100, -100}, {-100, 100, -100}, {100, 100, -100}, {100, -100, -100},
        {-100, -100, 100}, {-100, 100, 100}, {100, 100, 100}, {100, -100, 100}};

    float deltaX = 0, deltaY = 0, deltaZ = 1.5;
    while (running)
    {
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
	  handleKeyPress(event, cubeVertices, speed, lastsotationspeed, deltaX, deltaY, deltaZ, running);
        }
	
        handleDrawAndRender(renderer, cubeVertices, speed, lastsotationspeed, deltaX, deltaY, deltaZ, running);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}

