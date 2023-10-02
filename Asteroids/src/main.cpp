#include "SDL2/SDL_render.h"
#include "asteroid.h"
#include "background.h"
#include "constants.h"
#include "player.h"
#include <SDL2/SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <cmath>
#include <iostream>
#include <random>
#include <string>
#include <vector>

// NOTE: Globals
std::random_device rd;
std::mt19937 gen(rd());
bool gameIsRunning = false;
int lastFrameTime = 0;
int frameCount = 0;
float timeCount = 0.0f;
float frameRate = 0.0f;
SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;
Player *player;
int numberOfStars = 1000;
std::vector<std::string> backgroundImages;
std::vector<Asteroid> asteroids;
SDL_Texture *backgroundTexture = NULL;
SDL_Event event;
TTF_Font *font = nullptr;
SDL_Texture *asteroidTexture = nullptr;

void setup() {
  // Scan the assets/backgrounds directory for png files
  backgroundImages = Background::getBackgroundImages();

  // Select a random background image
  std::uniform_int_distribution<> dis(0, backgroundImages.size() - 1);
  std::string randomBackgroundImage = backgroundImages[dis(gen)];

  // Create background surface
  SDL_Surface *backgroundSurface = NULL;
  backgroundSurface =
      SDL_CreateRGBSurface(0, SCREEN_WIDTH, SCREEN_HEIGHT, 32, 0, 0, 0, 0);

  // Create a temporary SDL_Renderer to draw on the background surface
  SDL_Renderer *tempRenderer = SDL_CreateSoftwareRenderer(backgroundSurface);

  // Draw image onto the surface
  SDL_Surface *backgroundImage = IMG_Load(randomBackgroundImage.c_str());
  if (!backgroundImage) {
    std::cout << "Failed to load background image: " << IMG_GetError()
              << std::endl;
  }
  SDL_Texture *texture =
      SDL_CreateTextureFromSurface(tempRenderer, backgroundImage);
  SDL_FreeSurface(backgroundImage);

  SDL_RenderCopy(tempRenderer, texture, NULL, NULL);
  SDL_DestroyTexture(texture);

  // Draw stars onto the surface
  Background background(tempRenderer, SCREEN_WIDTH, SCREEN_HEIGHT);
  background.drawStars(numberOfStars);

  // Create texture from surface pixels
  backgroundTexture = SDL_CreateTextureFromSurface(renderer, backgroundSurface);

  // Free the surface and the temporary renderer as we no longer need them
  SDL_FreeSurface(backgroundSurface);
  SDL_DestroyRenderer(tempRenderer);

  // Create player
  player = new Player(renderer);

  // Push 10 asteroids to the vector
  for (int i = 0; i < 10; i++) {
    asteroids.push_back(
        Asteroid(renderer, SCREEN_WIDTH / 2.0, SCREEN_HEIGHT / 2.0));
  }
}

bool initialiseWindow(void) {
  if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
    std::cout << "SDL could not initialise! SDL_Error: " << SDL_GetError()
              << std::endl;
    return false;
  }
  window = SDL_CreateWindow("Asteroids", SDL_WINDOWPOS_UNDEFINED,
                            SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH,
                            SCREEN_HEIGHT, SDL_WINDOW_SHOWN);

  if (!window) {
    std::cout << "SDL could not create window! SDL_Error: " << SDL_GetError()
              << std::endl;
  }
  renderer = SDL_CreateRenderer(window, -1, 0);

  if (!renderer) {
    std::cout << "SDL could not create renderer! SDL_Error: " << SDL_GetError()
              << std::endl;
  }

  if (TTF_Init() == -1) {
    std::cout << "Could not initailize SDL2_ttf, error: " << TTF_GetError()
              << std::endl;
  } else {
    std::cout << "SDL2_ttf system ready to go!" << std::endl;
  }

  font = TTF_OpenFont("assets/fonts/Asteroids.ttf", 32);

  if (font == nullptr) {
    std::cout << "Could not load font" << std::endl;
    return false;
  }

  return true;
}

void processInput(void) {
  const Uint8 *state = SDL_GetKeyboardState(NULL);
  while (SDL_PollEvent(&event)) {
    switch (event.type) {
    case SDL_QUIT:
      gameIsRunning = false;
      break;
    case SDL_KEYDOWN:
      switch (event.key.keysym.sym) {
      case SDLK_ESCAPE:
        gameIsRunning = false;
        break;
      case SDLK_SPACE:
        player->Fire();
        break;
      case SDLK_r:
        player->Reset();
        break;
      }
      break;
    case SDL_KEYUP:
      switch (event.key.keysym.sym) {
      case SDLK_UP:
        player->StopAccelerating();
        break;
      case SDLK_LEFT:
        player->StopRotating();
        break;
      case SDLK_RIGHT:
        player->StopRotating();
        break;
      case SDLK_SPACE:
        player->StopFiring();
        break;
      }
      break;
    }
  }

  if (state[SDL_SCANCODE_UP]) {
    player->Accelerate();
  }
  if (state[SDL_SCANCODE_LEFT]) {
    player->RotateLeft();
  }
  if (state[SDL_SCANCODE_RIGHT]) {
    player->RotateRight();
  }
}

void calculateFramerate(float deltaTime) {
  frameCount++;
  timeCount += deltaTime;

  if (timeCount >= 1.0f) {              // If more than one second has passed
    frameRate = frameCount / timeCount; // Calculate the frame rate
    timeCount -= 1.0f;                  // Subtract one second from time count
    frameCount = 0;                     // Reset frame count
  }
}

void updateGame(void) {
  float currentTime = SDL_GetTicks();
  float deltaTime = (currentTime - lastFrameTime) / 1000.0;

  lastFrameTime = currentTime;

  // Update player
  player->Update(deltaTime, renderer);

  // Check for collision between asteroids
  for (int i = 0; i < asteroids.size(); i++) {

    // Update the asteroid
    asteroids[i].Update(deltaTime);

    // Check for collision between player and asteroid
    if (player->isAlive) {
      if (asteroids[i].CheckPlayerCollision(player->centerX, player->centerY)) {
        player->isAlive = false;
      }
    }

    for (int j = i + 1; j < asteroids.size(); j++) {
      if (asteroids[i].CheckAsteroidCollision(&asteroids[j])) {
        // Calculate the vector between the centers of the asteroids
        float dx = asteroids[j].centerX - asteroids[i].centerX;
        float dy = asteroids[j].centerY - asteroids[i].centerY;

        // Normalize the vector
        float distance = sqrt(dx * dx + dy * dy);
        dx /= distance;
        dy /= distance;

        // Calculate the velocity of each asteroid along the line connecting
        // their centers
        float velocity_i = dx * asteroids[i].dx + dy * asteroids[i].dy;
        float velocity_j = dx * asteroids[j].dx + dy * asteroids[j].dy;

        // Swap the velocities of the asteroids along this line
        float temp = velocity_i;
        velocity_i = velocity_j;
        velocity_j = temp;

        // Update the velocities of the asteroids
        asteroids[i].dx += dx * (velocity_i - velocity_j);
        asteroids[i].dy += dy * (velocity_i - velocity_j);
        asteroids[j].dx += dx * (velocity_j - velocity_i);
        asteroids[j].dy += dy * (velocity_j - velocity_i);

        // Adjust the position of one asteroid so they don't overlap
        float overlap =
            0.5 * (distance - asteroids[i].radius - asteroids[j].radius);
        asteroids[i].centerX -= overlap * dx;
        asteroids[i].centerY -= overlap * dy;
        asteroids[j].centerX += overlap * dx;
        asteroids[j].centerY += overlap * dy;
      }
    }
  }

  // Update the bullets
  for (int i = 0; i < player->bullets.size(); i++) {
    if (player->bullets[i]->isAlive) {
      player->bullets[i]->Update(deltaTime);
    } else {
      delete player->bullets[i];
      player->bullets.erase(player->bullets.begin() + i);
    }
  }

  // Calculate frame rate
  calculateFramerate(deltaTime);
}

void renderOutput(void) {
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
  SDL_RenderClear(renderer);

  // Draw background texture
  SDL_RenderCopy(renderer, backgroundTexture, NULL, NULL);

  // Draw the asteroids
  for (int i = 0; i < 10; i++) {
    asteroids[i].Render(renderer);
  }

  // Draw the projectiles
  for (int i = 0; i < player->bullets.size(); i++) {
    player->bullets[i]->Render(renderer);
    std::cout << "bullets: " << player->bullets.size() << std::endl;
  }

  // Draw the player
  player->Render(renderer);

  // Create surface to contain text
  SDL_Color color = {255, 255, 255};
  std::string score = "Score: " + std::to_string(player->score) +
                      "    Lives: " + std::to_string(player->lives) +
                      "    FPS: " + std::to_string((int)frameRate);

  // Check if font is valid
  if (font != nullptr) {
    SDL_Surface *text = TTF_RenderText_Solid(font, score.c_str(), color);
    if (!text) {
      std::cout << "Failed to render text: " << TTF_GetError() << std::endl;
    } else {
      // Create texture
      SDL_Texture *text_texture;

      text_texture = SDL_CreateTextureFromSurface(renderer, text);

      // Get the width and height of the texture
      int text_width = text->w;
      int text_height = text->h;

      // Setup the destination rectangle to be at the position we want
      SDL_Rect dest = {10, 5, text_width, text_height};

      SDL_RenderCopy(renderer, text_texture, NULL, &dest);

      // Free the surface and texture
      SDL_FreeSurface(text);
      SDL_DestroyTexture(text_texture);
    }
  }

  SDL_RenderPresent(renderer);
}

void destroyWindow(void) {
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
}

int limitFPS(void) {
  int timeSinceLastFrame = SDL_GetTicks() - lastFrameTime;
  if (timeSinceLastFrame < frameDelay) {
    return frameDelay - timeSinceLastFrame;
  }
  return 0;
}

int main(void) {
  gameIsRunning = initialiseWindow();

  setup();

  while (gameIsRunning) {
    processInput();
    updateGame();
    renderOutput();

    if (limitFPS() > 0) {
      SDL_Delay(limitFPS());
    }
  }

  destroyWindow();

  return 0;
}
