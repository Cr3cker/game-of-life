#include <stdlib.h>
#include <stdio.h>
#include <raylib.h>
#include <string.h>
#include <unistd.h>
#include <math.h>

#define RAYGUI_IMPLEMENTATION
#include "raygui.h"

void draw_grid(int gr[30][30], int x, int y, int w, int h) {
    int init_x = x;
    for (int i = 0; i < 30; i++) {
        for (int j = 0; j < 30; j++) {
            if (gr[i][j]) DrawRectangle(x, y, w, h, BLACK);
            else DrawRectangleLines(x, y, w, h, BLACK);
            x += w;
        }
        y += h;
        x = init_x;
    }
}

struct pair {
    int x;
    int y;
};

int count_nbors_alive(int x, int y, int gr[30][30]) {
    struct pair pairs[8] = {
        {(x + 1) % 30, y},
        {(x - 1 + 30) % 30, y},
        {x, (y + 1) % 30},
        {x, (y - 1 + 30) % 30},
        {(x - 1 + 30) % 30, (y - 1 + 30) % 30},
        {(x + 1) % 30, (y + 1) % 30},
        {(x - 1 + 30) % 30, (y + 1) % 30},
        {(x + 1) % 30, (y - 1 + 30) % 30}
    };

    int count = 0;
    for (int i = 0; i < 8; i++) {
        if (gr[pairs[i].x][pairs[i].y] == 1) {
            count++;
        }
    }

    return count;
}

void follow_rules(int gr[30][30]) {
    int gr_copy[30][30];
    memcpy(gr_copy, gr, sizeof(int) * 30 * 30); 

    for (int i = 0; i < 30; i++) {
        for (int j = 0; j < 30; j++) {
            int nbors = count_nbors_alive(i, j, gr_copy);
            if (nbors == 3 && !gr_copy[i][j]) gr[i][j] = 1;
            else if ((nbors < 2 || nbors > 3) && gr_copy[i][j]) gr[i][j] = 0;
            else if ((nbors == 3 || nbors == 2) && gr_copy[i][j]) gr[i][j] = 1;
        }
    }
}

void setup_glider(int grid[30][30], int x, int y) {
    int glider[3][3] = {
        {0, 1, 0},
        {0, 0, 1},
        {1, 1, 1}
    };

    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            grid[x+i][y+j] = glider[i][j];
        }
    }
}

int main() {
    InitWindow(600, 650, "Game of Life");

    float speed = 200000.0;
    int grid[30][30] = { 0 };

    setup_glider(grid, 20, 20);        
    bool fl = false; 
    bool showBtn = false;   

    double lastUpdateTime = 0.0;                   

    while (!WindowShouldClose()) {
        ClearBackground(WHITE);
        BeginDrawing();

        draw_grid(grid, 0, 0, 20, 20);
        if (IsKeyPressed(KEY_A)) showBtn = !showBtn;
        if (IsKeyPressed(KEY_SPACE)) fl = !fl;

        if (showBtn) {
            int sliderWidth = 250;
            int sliderX = GetScreenWidth() / 2 - sliderWidth / 2;
            GuiSliderBar((Rectangle){ sliderX, GetScreenHeight() - 50, sliderWidth, 50 }, "Min", "Max", &speed, 200000, 1000000);
        }

        double currentTime = GetTime();
        if (currentTime - lastUpdateTime > (1000000 - speed) / 1000000.0) {
            if (fl) follow_rules(grid);
            lastUpdateTime = currentTime;
        }

        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            Vector2 mousepos = GetMousePosition();
            mousepos.x = floor(mousepos.x / 20.0); 
            mousepos.y = floor(mousepos.y / 20.0);
            if (grid[(int)mousepos.y][(int)mousepos.x]) grid[(int)mousepos.y][(int)mousepos.x] = 0;
            else grid[(int)mousepos.y][(int)mousepos.x] = 1;
        }

        EndDrawing();
    }

    CloseWindow();
    return 0;
}