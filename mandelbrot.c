#include <SDL2/SDL.h>
#include <complex.h>
#include <stdio.h>
#include <stdint.h>

#define ITERATIONS 255

#define XSCALE 640/4
#define YSCALE 640/4

#define SCALE 160

double ZOOM = 2.0f;

#define WINDOW_WIDTH 640
#define WINDOW_HEIGHT 640


typedef struct {
  long double x;
  long double y;
} vec2;

/* cool zoom: 
vec2 start_pos = { 
  .x = -0.02608450943428429618666485045874736670157290063798f,
  .y = 0.64422504136634055061901363292697908491390990093350f
};
*/

vec2 start_pos = {
  .x = 0.0f,
  .y = 0.0f
};

int mandelbrot(long double complex z) {

  long double complex z2 = z;

    for (int i = 0; i<ITERATIONS; i++) {
      z2 = cpow(z2, 2) + z;
      if (sqrt(pow(cimag(z2), 2) + pow(creal(z2), 2)) > 2) {
        return ITERATIONS - i;
      }
    }

  return 0;
}

void render(SDL_Renderer *rend) {

  uint8_t image[WINDOW_WIDTH][WINDOW_HEIGHT] = {{0}};

  long double xdist = (start_pos.x + 4/ZOOM) - (start_pos.x - 4/ZOOM);
  long double ydist = (start_pos.y + 4/ZOOM) - (start_pos.y - 4/ZOOM);

  int ycount = 0, xcount = 0;


  for (long double x = start_pos.x - 4/ZOOM; x<start_pos.x + 4/ZOOM; x += xdist/WINDOW_WIDTH) {
    for (long double y = start_pos.y - 4/ZOOM; y<start_pos.y + 4/ZOOM; y += ydist/WINDOW_HEIGHT) {
      long double complex z = x + y * I;
      int result = mandelbrot(z);
      image[xcount][WINDOW_HEIGHT - ycount] = result;
      ycount++;
    }
    ycount = 0;
    xcount++;
    if (xcount > 639) {
      break;
    }
  }

  for (int i = 0; i<WINDOW_WIDTH; i++) {
    for (int j = 0; j<WINDOW_HEIGHT; j++) {
      int col = image[i][j];

      SDL_SetRenderDrawColor(rend, col, col, col, col);
      SDL_RenderDrawPoint(rend, i, j);
    }
  }


  SDL_RenderPresent(rend);
  
}



int main() {
  
  SDL_Init(SDL_INIT_VIDEO);

  SDL_Renderer *rend;
  SDL_Window *win;
  SDL_Event ev;

  SDL_CreateWindowAndRenderer(WINDOW_WIDTH, WINDOW_HEIGHT, 0, &win, &rend);
  SDL_SetRenderDrawColor(rend, 255, 255, 255, 255);
  SDL_RenderClear(rend);
  SDL_RenderPresent(rend);

  SDL_SetRenderDrawColor(rend, 0, 0, 0, 0);

  render(rend);

  for (;;) {

    if (SDL_PollEvent(&ev)) {
      switch (ev.type) {
        case SDL_QUIT:
          goto end;
          break;
        case SDL_MOUSEBUTTONDOWN:
          int x, y;
          SDL_GetMouseState(&x, &y);

          vec2 origin = {
            .x = start_pos.x - 4/ZOOM,
            .y = start_pos.y + 4/ZOOM
          };

          start_pos.x = origin.x + (x * ((8/ZOOM)/640));
          start_pos.y = origin.y - (y * ((8/ZOOM)/640));

          ZOOM *= (int64_t)(1 << 1);
          printf("mouse button pressed at %d, %d, with zoom = %f\n", x, y, ZOOM);
          printf("centre:\n x = %0.50LF,\n y = %0.50LF\n", start_pos.x, start_pos.y);

          render(rend);

          break;
      }
    } 
  }

end:

  SDL_DestroyWindow(win);
  SDL_DestroyRenderer(rend);
  SDL_Quit();

  return 0;
}
