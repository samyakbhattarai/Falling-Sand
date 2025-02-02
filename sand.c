#include <stdio.h>
#include <SDL2/SDL.h>
#include <math.h>

#define WIDTH 600
#define HEIGHT 600
#define CELL_SIZE 1
#define COLS (WIDTH / CELL_SIZE)
#define ROWS (HEIGHT / CELL_SIZE)

typedef struct {
    int exists;
    float hue;  
} SandGrain;

SandGrain cells[COLS][ROWS] = {0};
float global_hue = 0.0;  // Hue value for new grains

// Convert HSL to RGB color format
Uint32 HSLtoRGB(float h) {
    float r, g, b;
    float s = 1.0, l = 0.5;
    float C = (1 - fabs(2 * l - 1)) * s;
    float X = C * (1 - fabs(fmod(h / 60.0, 2) - 1));
    float m = l - C / 2;

    if (h < 60) {
        r = C, g = X, b = 0;
    } else if (h < 120) {
        r = X, g = C, b = 0;
    } else if (h < 180) {
        r = 0, g = C, b = X;
    } else if (h < 240) {
        r = 0, g = X, b = C;
    } else if (h < 300) {
        r = X, g = 0, b = C;
    } else {
        r = C, g = 0, b = X;
    }

    Uint8 R = (Uint8)((r + m) * 255);
    Uint8 G = (Uint8)((g + m) * 255);
    Uint8 B = (Uint8)((b + m) * 255);

    return (R << 16) | (G << 8) | B;  // Convert to Uint32 color
}

// Draw a sand grain at (x, y)
void CreateGrain(SDL_Surface* surface, int x, int y, Uint32 color) {
    Uint32* pixels = (Uint32*)surface->pixels;
    pixels[y * COLS + x] = color;
}

// Handle mouse input to place sand grains
void HandleMouseClick(int mouseX, int mouseY, int size) {
    int gridX = mouseX / CELL_SIZE;
    int gridY = mouseY / CELL_SIZE;

    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            int nx = gridX + i, ny = gridY + j;
            if (nx >= 0 && nx < COLS && ny >= 0 && ny < ROWS && !cells[nx][ny].exists) {
                cells[nx][ny].exists = 1;
                cells[nx][ny].hue = global_hue;  // Store hue permanently
            }
        }
    }
}

int main(int argc, char* argv[]) {
    SDL_Init(SDL_INIT_EVERYTHING);
    SDL_Window* window = SDL_CreateWindow("Sand Simulation", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, 0);
    SDL_Surface* surface = SDL_GetWindowSurface(window);
    SDL_Event event;
    int size = 5;
    int dragging = 0;

    int simulation_running = 1;
    while (simulation_running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                simulation_running = 0;
            } else if (event.type == SDL_MOUSEBUTTONDOWN) {
                if (event.button.button == SDL_BUTTON_LEFT) {
                    dragging = 1;
                    HandleMouseClick(event.button.x, event.button.y, size);
                }
            } else if (event.type == SDL_MOUSEBUTTONUP) {
                if (event.button.button == SDL_BUTTON_LEFT) {
                    dragging = 0;
                }
            } else if (event.type == SDL_MOUSEMOTION) {
                if (dragging) {
                    HandleMouseClick(event.motion.x, event.motion.y, size);
                }
            } else if (event.type == SDL_KEYDOWN) {
                if (event.key.keysym.sym == SDLK_a) {
                    size++;
                } else if (event.key.keysym.sym == SDLK_d) {
                    size = (size > 1) ? size - 1 : 1;
                }
            }
        }

        SDL_FillRect(surface, NULL, 0x00000000);

        // Sand falling physics
        for (int x = 0; x < COLS; x++) {
            for (int y = ROWS - 1; y >= 0; y--) {
                if (cells[x][y].exists) {
                    if (y + 1 < ROWS && !cells[x][y + 1].exists) {
                        cells[x][y + 1] = cells[x][y];
                        cells[x][y].exists = 0;
                    } else if (y + 1 < ROWS && x + 1 < COLS && !cells[x + 1][y + 1].exists) {
                        cells[x + 1][y + 1] = cells[x][y];
                        cells[x][y].exists = 0;
                    } else if (y + 1 < ROWS && x - 1 >= 0 && !cells[x - 1][y + 1].exists) {
                        cells[x - 1][y + 1] = cells[x][y];
                        cells[x][y].exists = 0;
                    }
                }
            }
        }

        // Render the sand grains
        for (int x = 0; x < COLS; x++) {
            for (int y = 0; y < ROWS; y++) {
                if (cells[x][y].exists) {
                    Uint32 color = HSLtoRGB(cells[x][y].hue);
                    CreateGrain(surface, x, y, color);
                }
            }
        }

        SDL_UpdateWindowSurface(window);
        SDL_Delay(1);

        // Increase global hue for new grains
        global_hue = fmod(global_hue + 0.05, 360);
    }

    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
