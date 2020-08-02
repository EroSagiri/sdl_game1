#include <stdio.h>
#include <stdlib.h>
#include <SDL.h>
#include <time.h>
#include <SDL_ttf.h>

#define MAP_WIDTH 4
#define MAP_HEIGHT 4
#define L_WIDTH 150

#define WINDOW_WIDTH MAP_WIDTH*L_WIDTH
#define WINDOW_HEIGHT MAP_WIDTH*L_WIDTH
#define WINDOW_TITLE "2048"
#define FPS 60

SDL_Window *window;
SDL_Renderer *renderer;
SDL_Event event;
TTF_Font *font;
SDL_Thread *renderer_thread;
SDL_Texture *t2, *t4, *t8, *t16, *t32, *t64, *t128, *t256, *t512, *t1024, *t2048, *t_game_info;
SDL_Color texture_color = {255, 255, 255};
int map[MAP_WIDTH][MAP_HEIGHT] = { 0 };

int render_thread();
int load_texture();
int game_init();
int game_over();
int game_reste();
int game_test();
int game_quit();
int move_up();
int move_down();
int move_left();
int move_right();

int up_key_down = 0;
int down_key_down = 0;
int left_key_down = 0;
int right_key_down = 0;
int r_key_down = 0;
int is_game_over = 0;

int load_texture() {
    font = TTF_OpenFont("fonts/DejaVuSans-Bold.ttf", 20);
    if(font == NULL) {
        SDL_Log("字体丢失\n");
        return -1;
    }

    SDL_Surface *s;

    s = SDL_LoadBMP("images/2.bmp");
    if(s == NULL) {
        s = TTF_RenderText_Solid(font, "2", texture_color);
    }
    t2 = SDL_CreateTextureFromSurface(renderer, s);
    SDL_FreeSurface(s);

    s = SDL_LoadBMP("images/4.bmp");
    if(s == NULL) {
        s = TTF_RenderText_Solid(font, "4", texture_color);
    }
    t4 = SDL_CreateTextureFromSurface(renderer, s);
    SDL_FreeSurface(s);

    s = SDL_LoadBMP("images/8.bmp");
    if(s == NULL) {
        s = TTF_RenderText_Solid(font, "8", texture_color);
    }
    t8 = SDL_CreateTextureFromSurface(renderer, s);
    SDL_FreeSurface(s);

    s = SDL_LoadBMP("images/16.bmp");
    if(s == NULL) {
        s = TTF_RenderText_Solid(font, "16", texture_color);
    }
    t16 = SDL_CreateTextureFromSurface(renderer, s);
    SDL_FreeSurface(s);

    s = SDL_LoadBMP("images/32.bmp");
    if(s == NULL) {
        s = TTF_RenderText_Solid(font, "32", texture_color);
    }
    t32 = SDL_CreateTextureFromSurface(renderer, s);
    SDL_FreeSurface(s);

    s = SDL_LoadBMP("images/64.bmp");
    if(s == NULL) {
        s = TTF_RenderText_Solid(font, "64", texture_color);
    }
    t64 = SDL_CreateTextureFromSurface(renderer, s);
    SDL_FreeSurface(s);

    s = SDL_LoadBMP("images/128.bmp");
    if(s == NULL) {
        s = TTF_RenderText_Solid(font, "128", texture_color);
    }
    t128 = SDL_CreateTextureFromSurface(renderer, s);
    SDL_FreeSurface(s);

    s = SDL_LoadBMP("images/256.bmp");
    if(s == NULL) {
        s = TTF_RenderText_Solid(font, "256", texture_color);
    }
    t256 = SDL_CreateTextureFromSurface(renderer, s);
    SDL_FreeSurface(s);

    s = SDL_LoadBMP("images/512.bmp");
    if(s == NULL) {
        s = TTF_RenderText_Solid(font, "512", texture_color);
    }
    t512 = SDL_CreateTextureFromSurface(renderer, s);
    SDL_FreeSurface(s);

    s = SDL_LoadBMP("images/1024.bmp");
    if(s == NULL) {
        s = TTF_RenderText_Solid(font, "1024", texture_color);
    }
    t1024 = SDL_CreateTextureFromSurface(renderer, s);
    SDL_FreeSurface(s);

    s = SDL_LoadBMP("images/2048.bmp");
    if(s == NULL) {
        s = TTF_RenderText_Solid(font, "2048", texture_color);
    }
    t2048 = SDL_CreateTextureFromSurface(renderer, s);
    SDL_FreeSurface(s);

    return 0;
}

int render_thread()
{
    int ww, wh;
    int t_x = 0;
    int t_y = 0;

    while(event.type != SDL_QUIT) {
        SDL_RenderClear(renderer);
        SDL_GetWindowSize(window, &ww, &wh);
        if(ww > MAP_WIDTH * L_WIDTH) {
            t_x = (ww-L_WIDTH*MAP_WIDTH)/2;
        }
        if(wh > MAP_HEIGHT * L_WIDTH) {
            t_y = (wh-L_WIDTH*MAP_HEIGHT)/2;
        }

        for(int x = 0; x <= MAP_WIDTH; x ++) {
            SDL_SetRenderDrawColor(renderer, 0xff, 0xb6, 0xc1, 0);
            SDL_RenderDrawLine(renderer, x*L_WIDTH + t_x, t_y, x*L_WIDTH+t_x, MAP_HEIGHT*L_WIDTH+t_y);
        }
        for(int y = 0; y <= MAP_HEIGHT; y ++) {
            SDL_SetRenderDrawColor(renderer, 0xff, 0xb6, 0xc1, 0);
            SDL_RenderDrawLine(renderer, t_x, y*L_WIDTH+t_y, MAP_WIDTH*L_WIDTH+t_x, y*L_WIDTH+t_y);
        }

        for(int x = 0; x < MAP_WIDTH; x++) {
            for(int y = 0; y < MAP_HEIGHT; y++) {
                if(map[x][y] == 2) {
                    SDL_Rect r = {(x*L_WIDTH)+t_x, (y*L_WIDTH)+t_y, L_WIDTH, L_WIDTH};
                    SDL_RenderCopy(renderer, t2, NULL, &r);
                }
                if(map[x][y] == 4) {
                    SDL_Rect r = {(x*L_WIDTH)+t_x, (y*L_WIDTH)+t_y, L_WIDTH, L_WIDTH};
                    SDL_RenderCopy(renderer, t4, NULL, &r);
                }
                if(map[x][y] == 8) {
                    SDL_Rect r = {(x*L_WIDTH)+t_x, (y*L_WIDTH)+t_y, L_WIDTH, L_WIDTH};
                    SDL_RenderCopy(renderer, t8, NULL, &r);
                }
                if(map[x][y] == 16) {
                    SDL_Rect r = {(x*L_WIDTH)+t_x, (y*L_WIDTH)+t_y, L_WIDTH, L_WIDTH};
                    SDL_RenderCopy(renderer, t16, NULL, &r);
                }
                if(map[x][y] == 32) {
                    SDL_Rect r = {(x*L_WIDTH)+t_x, (y*L_WIDTH)+t_y, L_WIDTH, L_WIDTH};
                    SDL_RenderCopy(renderer, t32, NULL, &r);
                }
                if(map[x][y] == 64) {
                    SDL_Rect r = {(x*L_WIDTH)+t_x, (y*L_WIDTH)+t_y, L_WIDTH, L_WIDTH};
                    SDL_RenderCopy(renderer, t64, NULL, &r);
                }
                if(map[x][y] == 128) {
                    SDL_Rect r = {(x*L_WIDTH)+t_x, (y*L_WIDTH)+t_y, L_WIDTH, L_WIDTH};
                    SDL_RenderCopy(renderer, t128, NULL, &r);
                }
                if(map[x][y] == 256) {
                    SDL_Rect r = {(x*L_WIDTH)+t_x, (y*L_WIDTH)+t_y, L_WIDTH, L_WIDTH};
                    SDL_RenderCopy(renderer, t256, NULL, &r);
                }
                if(map[x][y] == 512) {
                    SDL_Rect r = {(x*L_WIDTH)+t_x, (y*L_WIDTH)+t_y, L_WIDTH, L_WIDTH};
                    SDL_RenderCopy(renderer, t512, NULL, &r);
                }
                if(map[x][y] == 1024) {
                    SDL_Rect r = {(x*L_WIDTH)+t_x, (y*L_WIDTH)+t_y, L_WIDTH, L_WIDTH};
                    SDL_RenderCopy(renderer, t1024, NULL, &r);
                }
                if(map[x][y] == 2048) {
                    SDL_Rect r = {(x*L_WIDTH)+t_x, (y*L_WIDTH)+t_y, L_WIDTH, L_WIDTH};
                    SDL_RenderCopy(renderer, t2048, NULL, &r);
                }
            }
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
        SDL_RenderPresent(renderer);

        SDL_Delay(1000/FPS);
    }

    return 0;
}


int game_init() {
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
    TTF_Init();
    SDL_CreateWindowAndRenderer(WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_RESIZABLE, &window,& renderer);
    SDL_SetWindowTitle(window, WINDOW_TITLE);
    if(load_texture() == -1) {
        SDL_Log("加载失败...");
        return -1;
    }
    map[MAP_WIDTH/2][MAP_HEIGHT/2] = 2;
    /*
    map[0][0] = 2;
    map[0][1] = 4;
    map[0][2] = 8;
    map[0][3] = 16;
    map[0][4] = 32;
    map[1][0] = 64;
    map[1][1] = 128;
    map[1][2] = 256;
    map[1][3] = 512;
    map[1][4] = 1024;
    map[2][0] = 2048;
    map[3][0] = 2;
    map[3][2] = 2;
    map[3][1] = 2;
    map[3][4] = 2;
    map[4][4] = 2048;
    */
    return 0;
}

int game_quit()
{
    SDL_DestroyTexture(t2);
    SDL_DestroyTexture(t4);
    SDL_DestroyTexture(t8);
    SDL_DestroyTexture(t16);
    SDL_DestroyTexture(t32);
    SDL_DestroyTexture(t64);
    SDL_DestroyTexture(t128);
    SDL_DestroyTexture(t256);
    SDL_DestroyTexture(t512);
    SDL_DestroyTexture(t1024);
    SDL_DestroyTexture(t2048);
    TTF_CloseFont(font);
    TTF_Quit();
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}

int move_up()
{
    for(int y = 1; y < MAP_HEIGHT; y++) {
        for(int x = 0; x < MAP_WIDTH; x++) {
            if(map[x][y] != 0) {
                int t_y = y;
                while(t_y > 0) {
                    if(map[x][t_y] == map[x][t_y-1]) {
                        map[x][t_y-1] += map[x][t_y];
                        if(map[x][t_y-1] >= 2048) {
                            game_over(0);
                        }
                        map[x][t_y] = 0;
                    } else if(map[x][t_y-1] == 0) {
                        map[x][t_y-1] = map[x][t_y];
                        map[x][t_y] = 0;
                    } else {
                        break;
                    }
                    t_y --;
                }
            }
        }
    }

    int free_x[MAP_WIDTH];
    int free_index = 0;
    for(int x = 0; x < MAP_WIDTH; x++) {
        if(map[x][MAP_HEIGHT-1] == 0) {
            free_x[free_index] = x;
            free_index ++;
        }
    }
    if(free_index > 0) {
        srand(time(NULL));
        map[free_x[rand()%free_index]][MAP_HEIGHT-1] = 2;
        if(game_test()!=0) {
            game_over(1);
        }
    } else {
        if(game_test()!=0) {
            game_over(1);
        }
    }

    return 0;
}

int move_down()
{
    for(int y = MAP_HEIGHT-2; y >= 0; y --) {
        for(int x = 0; x < MAP_WIDTH; x ++) {
            if(map[x][y] != 0) {
                int t_y = y;
                while(t_y < MAP_HEIGHT-1) {
                    if(map[x][t_y] == map[x][t_y+1]) {
                        map[x][t_y+1] += map[x][t_y];
                        if(map[x][t_y-1] >= 2048) {
                            game_over(0);
                        }
                        map[x][t_y] = 0;
                    } else if(map[x][t_y+1] == 0) {
                        map[x][t_y+1] = map[x][t_y];
                        map[x][t_y] = 0;
                    } else {
                        break;
                    }
                    t_y ++;
                }
            }
        }
    }

    int free_x[MAP_WIDTH];
    int free_index = 0;
    for(int x = 0; x < MAP_WIDTH; x++) {
        if(map[x][0] == 0) {
            free_x[free_index] = x;
            free_index ++;
        }
    }
    if(free_index > 0) {
        srand(time(NULL));
        map[free_x[rand()%free_index]][0] = 2;
        if(game_test()!=0) {
            game_over(1);
        }
    } else {
        if(game_test()!=0) {
            game_over(1);
        }
    }

    return 0;
}
int move_left()
{
    for(int x = 1; x < MAP_WIDTH; x ++) {
        for(int y = 0; y < MAP_HEIGHT; y ++) {
            if(map[x][y] != 0) {
                int t_x = x;
                while(t_x > 0) {
                    if(map[t_x][y] == map[t_x-1][y]) {
                        map[t_x-1][y] += map[t_x][y];
                        if(map[x][t_x-1] >= 2048) {
                            game_over(0);
                        }
                        map[t_x][y] = 0;
                    } else if(map[t_x-1][y] == 0) {
                        map[t_x-1][y] = map[t_x][y];
                        map[t_x][y] = 0;
                    } else {
                        break;
                    }
                    t_x --;
                }
            }
        }
    }

    int free_y[MAP_HEIGHT];
    int free_index = 0;
    for(int y = 0; y < MAP_HEIGHT; y++) {
        if(map[MAP_WIDTH-1][y] == 0) {
            free_y[free_index] = y;
            free_index ++;
        }
    }
    if(free_index > 0) {
        srand(time(NULL));
        map[MAP_WIDTH-1][free_y[rand()%free_index]] = 2;
        if(game_test()!=0) {
            game_over(1);
        }
    } else {
        if(game_test()!=0) {
            game_over(0);
        }
    }

    return 0;
}

int move_right()
{
    for(int x = MAP_WIDTH-2; x >= 0; x--) {
        for(int y = 0; y < MAP_HEIGHT; y++) {
            if(map[x][y] != 0) {
                int t_x = x;
                while(t_x < MAP_WIDTH-1) {
                    if(map[t_x][y] == map[t_x+1][y]) {
                        map[t_x+1][y] += map[t_x][y];
                        if(map[t_x+1][y] == 2048) {
                            game_over(0);
                        }
                        map[t_x][y] = 0;
                    } else if(map[t_x+1][y] == 0) {
                        map[t_x+1][y] = map[t_x][y];
                        map[t_x][y] = 0;
                    }
                    t_x ++;
                }
            }
        }
    }

    int free_y[MAP_HEIGHT];
    int free_index = 0;
    for(int y = 0; y < MAP_HEIGHT; y++) {
        if(map[0][y] == 0) {
            free_y[free_index] = y;
            free_index ++;
        }
    }
    if(free_index > 0) {
        srand(time(NULL));
        map[0][free_y[rand()%free_index]] = 2;
        if(game_test()!=0) {
            game_over(1);
        }
    } else {
        if(game_test()!=0) {
            game_over(1);
        }
    }

    return 0;
}

int game_test()
{
    int t_x, t_y;
    for(int x = 0; x < MAP_WIDTH; x ++) {
        for(int y = 0; y < MAP_HEIGHT; y ++) {
            if(map[x][y] == 0) {
                return 0;
            }
            t_x = x;
            t_y = y+1;
            if(t_x >= 0 && t_x < MAP_WIDTH && t_y >= 0 && t_y < MAP_HEIGHT) {
                if(map[x][y] == map[t_x][t_y]) {
                    return 0;
                }
            }
            t_x = x;
            t_y = y-1;
            if(t_x >= 0 && t_x < MAP_WIDTH && t_y >= 0 && t_y < MAP_HEIGHT) {
                if(map[x][y] == map[t_x][t_y]) {
                    return 0;
                }
            }
            t_x = x-1;
            t_y = y;
            if(t_x >= 0 && t_x < MAP_WIDTH && t_y >= 0 && t_y < MAP_HEIGHT) {
                if(map[x][y] == map[t_x][t_y]) {
                    return 0;
                }
            }
            t_x = x+1;
            t_y = y;
            if(t_x >= 0 && t_x < MAP_WIDTH && t_y >= 0 && t_y < MAP_HEIGHT) {
                if(map[x][y] == map[t_x][t_y]) {
                    return 0;
                }
            }
        }
    }

    return 1;
}

int game_over(int s)
{
    is_game_over = 1;
    if(s == 0) {
        SDL_Log("你成功合成2048\n");
        SDL_ShowSimpleMessageBox(SDL_LOG_CATEGORY_ERROR, "游戏结束", "2048", NULL);
    } else if(s == 1){
        SDL_Log("完全不能走了\n");
        SDL_ShowSimpleMessageBox(SDL_LOG_CATEGORY_ERROR, "游戏结束", "Can't go at all", NULL);
    }

    game_reste();
    return 0;
}

int game_reste()
{
    for(int x = 0; x < MAP_WIDTH; x++) {
        for(int y = 0; y < MAP_HEIGHT; y ++) {
            map[x][y] = 0;
        }
    }
    map[MAP_WIDTH/2][MAP_HEIGHT/2] = 2;
    is_game_over = 0;
    return 0;
}

int event_thread() {
    int is_down = 0;
    SDL_FPoint p1;
    SDL_FPoint p2;
    float dx, dy;
    while(event.type != SDL_QUIT) {
        SDL_PollEvent(&event);

        if(event.type == SDL_KEYDOWN) {
            if(event.key.keysym.sym == SDLK_UP && up_key_down == 0 && is_game_over != 1) {
                SDL_Log("up\n");
                move_up();
                up_key_down = 1;
            }
            if(event.key.keysym.sym == SDLK_DOWN && down_key_down == 0 && is_game_over != 1) {
                SDL_Log("down\n");
                move_down();
                down_key_down = 1;
            }
            if(event.key.keysym.sym == SDLK_LEFT && left_key_down == 0 && is_game_over != 1) {
                SDL_Log("left\n");
                move_left();
                left_key_down = 1;
            }
            if(event.key.keysym.sym == SDLK_RIGHT && right_key_down == 0 && is_game_over != 1) {
                SDL_Log("right\n");
                move_right();
                right_key_down = 1;
            }
            if(event.key.keysym.sym == SDLK_f && r_key_down == 0) {
                // SDL_SetWindowSize(window, 1920, 1080);
                // SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN);
                r_key_down = 1;
            }
        }

        if(event.type == SDL_KEYUP) {
            if(event.key.keysym.sym == SDLK_UP) {
                up_key_down = 0;
            }
            if(event.key.keysym.sym == SDLK_DOWN) {
                down_key_down = 0;
            }
            if(event.key.keysym.sym == SDLK_LEFT) {
                left_key_down = 0;
            }
            if(event.key.keysym.sym == SDLK_RIGHT) {
                right_key_down = 0;
            }
            if(event.key.keysym.sym == SDLK_r) {
                r_key_down = 0;
            }
        }

        if(event.type == SDL_DOLLARGESTURE) {
            SDL_Log("手势\n");
        }
        if(event.type == SDL_FINGERDOWN) {
            //SDL_Log("触摸按下事件: x:%f y:%f dx:%f xy:%f\n", event.tfinger.x, event.tfinger.y, event.tfinger.dx, event.tfinger.dy);
            if(is_down == 0) {
                is_down = 1;
                p1.x = event.tfinger.x;
                p1.y  = event.tfinger.y;
            }
        }
        if(event.type == SDL_FINGERUP) {
            //SDL_Log("触摸抬起事件: x:%f y:%f dx:%f xy:%f\n", event.tfinger.x, event.tfinger.y, event.tfinger.dx, event.tfinger.dy);
            if(is_down == 1) {
                is_down = 0;
                p2.x = event.tfinger.x;
                p2.y = event.tfinger.y;
                SDL_Log("p1.x: %f p1.y %f   p2.x %f p2.y %f\n", p1.x, p1.y, p2.x, p2.y);
                dx = p1.x - p2.x;
                dy = p1.y - p2.y;

                if(dx > 0.2) {
                    move_left();
                } else if(dx < -0.2) {
                    move_right();
                } else if(dy > 0.14) {
                    move_up();
                } else if(dy < -0.14) {
                    move_down();
                }
            }
        }
        if(event.type == SDL_FINGERMOTION) {
            //SDL_Log("触摸移动事件: x:%f y:%f dx:%f xy:%f\n", event.tfinger.x, event.tfinger.y, event.tfinger.dx, event.tfinger.dy);
        }

    }
    
    return 0;
}


int main(int argc, char **argv) {
    game_init();

    SDL_CreateThread(event_thread, "renderer", NULL);
    render_thread();

    game_quit();
    return 0;
}
