#include <iostream>
#include <raylib.h>
#include <memory>
#include <algorithm>

#include "witnessclone.h"
#include "raylibutils.h"

double THICKNESS = 0.02;
double gapprop = 0.5;
double START_RAD = 2.5;
double protrusion = 0.25;
std::pair<double, double> CE;
std::pair<double, double> TL;
std::pair<double, double> BR;
double SPACING;
int RANKS = 4;
int FILES = 4;

void render(Grid g, const int width, const int height, double marginprop = 0.1, bool buffer = true) { 
    if (buffer) BeginDrawing();
    ClearBackground(BLACK);
    int n = std::min(width, height);

    // First coordinate (Y coordinate or RANK coordinate) is first index
    // Second coordinate (X coordinate or FILE coordinate) is second index

    THICKNESS = n * 0.02;
    FILES = g.m / 2; 
    RANKS = g.n / 2;
    int nrows = std::max(RANKS, FILES);



    Color BG = BLACK;
    Color PATH = GRAY;
    Color LINE = WHITE;
    Color DOT = DARKGRAY;


    CE = {width * 0.5, height * 0.5};
    TL = {CE.first - 0.5 * n + n * marginprop, CE.second - 0.5 * n + n * marginprop};
    BR = {CE.first + 0.5 * n - n * marginprop, CE.second + 0.5 * n - n * marginprop};

    SPACING = (BR.first - TL.first) / nrows;
    double halfspacing = SPACING * 0.5;

    std::pair<double, double> GRIDTL = {CE.first - 0.5 * SPACING * FILES, CE.second - 0.5 * SPACING * RANKS}; 
    std::pair<double, double> GRIDBR = {CE.first + 0.5 * SPACING * FILES, CE.second + 0.5 * SPACING * RANKS}; 
    disp(CE);
    disp(TL);
    disp(BR);
    disp(GRIDTL);
    disp(GRIDBR);
    disp(std::make_pair(RANKS, FILES));
    // cout << "\n" << THICKNESS << " " << SPACING << "\n";

    // Draw the grid and the endpoints    

    // Vertical lines
    for (int i = 0; i <= FILES; i++) {
        double offset = SPACING * i;
        Rect2Points(GRIDTL.first - THICKNESS + offset, GRIDTL.second, GRIDTL.first + THICKNESS + offset, GRIDBR.second, PATH);
    }

    // Horizontal lines
    for (int i = 0; i <= RANKS; i++) {
        double offset = SPACING * i;
        Rect2Points(GRIDTL.first, GRIDTL.second - THICKNESS + offset, GRIDBR.first, GRIDTL.second + THICKNESS + offset, PATH);
    }

    for (int i = 0; i <= RANKS; i++) {
        for (int j = 0; j <= FILES; j++) {
            DrawCircle(GRIDTL.first + SPACING * j, GRIDTL.second + SPACING * i, THICKNESS, PATH);
        }
    }

    for (int y = 0; y < g.n; y++) {
        for (int x = 0; x < g.m; x++) {
            pair<double, double> POS = {GRIDTL.first + halfspacing * x, GRIDTL.second + halfspacing * y};
            if (x % 2 != 0 && y % 2 != 0) continue; 
            std::shared_ptr<Entity> entity = (g.board)[y][x];
            if (!(entity->isPath)) DrawRectangle(POS.first - gapprop * halfspacing, POS.second - gapprop * halfspacing, gapprop * SPACING, gapprop * SPACING, BG);
            if (isStartingPoint(entity)) DrawCircle(POS.first, POS.second, THICKNESS * START_RAD, PATH);
            else if (isEndingPoint(entity)) {
                cout << "ENDPOINT FOUND" << x << " " << y << "\n";
                double prodist = protrusion * SPACING;
                if (x == 0) {
                    if (y == 0) DrawSlot(POS.first, POS.second, POS.first - prodist, POS.second - prodist, THICKNESS, PATH);
                    else if (y == g.n - 1) DrawSlot(POS.first, POS.second, POS.first - prodist, POS.second + prodist, THICKNESS, PATH);
                    else DrawSlot(POS.first, POS.second, POS.first - prodist, POS.second, THICKNESS, PATH);
                }
                else if (x == g.m - 1) {
                    if (y == 0) DrawSlot(POS.first, POS.second, POS.first + prodist, POS.second - prodist, THICKNESS, PATH);
                    else if (y == g.n - 1) DrawSlot(POS.first, POS.second, POS.first + prodist, POS.second + prodist, THICKNESS, PATH);
                    else DrawSlot(POS.first, POS.second, POS.first - prodist, POS.second, THICKNESS, PATH);
                }
                else if (y == 0) DrawSlot(POS.first, POS.second, POS.first, POS.second - prodist, THICKNESS, PATH);
                else if (y == g.n - 1) DrawSlot(POS.first, POS.second, POS.first, POS.second - prodist, THICKNESS, PATH);
                else DrawSlot(POS.first, POS.second, POS.first + prodist, POS.second + prodist, THICKNESS, PATH);
            }
        }
    }

    // Draw Symbols

    for (int y = 0; y < g.n; y++) {
        for (int x = 0; x < g.m; x++) {
            pair<double, double> POS = {GRIDTL.first + halfspacing * x, GRIDTL.second + halfspacing * y};
            std::shared_ptr<Entity> e = g.board[y][x];
            if (instanceof<Dot>(e)) DrawPoly(vec2pd(POS), 6, THICKNESS * 0.8, 0, DOT);
            if (instanceof<Blob>(e)) {
                const double OBJ_SIZE = 0.8;
                double obj_width = OBJ_SIZE * halfspacing * gapprop;
                Rectangle rect = {POS.first - obj_width, POS.second - obj_width, 2 * obj_width, 2 * obj_width};
                DrawRectangleRounded(rect, 0.5, 8, getColor(e->color));
            }
            if (instanceof<Star>(e)) {
                const double OBJ_SIZE = 00.8;
                double obj_width = OBJ_SIZE * halfspacing * gapprop;
                DrawPoly(vec2pd(POS), 4, obj_width, 0, getColor(e->color));
                DrawPoly(vec2pd(POS), 4, obj_width, 45, getColor(e->color));
            }
            if (instanceof<Triangle>(e)) {
                int count = (std::dynamic_pointer_cast<Triangle>(e))->x;
                const double OBJ_SIZE = 0.2;
                const double OBJ_SPAC = 0.2;

                double leftmost = POS.first - SPACING * OBJ_SPAC * 0.5 * (count - 1);

                for (int i = 0; i < count; i++) DrawPoly(vec2pd({leftmost + SPACING * OBJ_SPAC * i, POS.second}), 3, OBJ_SIZE * SPACING * 0.5, -90, getColor(e->color));
            }
            if (instanceof<Cancel>(e)) {
                const double OBJ_SIZE = 0.2;
                const double OBJ_THICK = 0.05;

                DrawRotatedRect(POS.first, POS.second, POS.first, POS.second - OBJ_SIZE * SPACING, OBJ_THICK * SPACING, WHITE);
                DrawRotatedRect(POS.first, POS.second, POS.first - sqrt(0.75) * OBJ_SIZE * SPACING, POS.second + 0.5 * OBJ_SIZE * SPACING, OBJ_THICK * SPACING, WHITE);
                DrawRotatedRect(POS.first, POS.second, POS.first + sqrt(0.75) * OBJ_SIZE * SPACING, POS.second + 0.5 * OBJ_SIZE * SPACING, OBJ_THICK * SPACING, WHITE);
            }
            if (instanceof<BlockGroup>(e)) {
                // std::cout << "BLOCK GROUP LOCATED AT POSITION " << POS.first << " " << POS.second << std::endl;
                std::shared_ptr<BlockGroup> bg = std::dynamic_pointer_cast<BlockGroup>(e);
                // std::cout << "BLOCK GROUP AT INDEX " << x << " " << y << " ORIENTED? " << bg->oriented << "\n";
                bg->normalize();
                bg->updateBounds();
                std::pair<int, int> bb = bg->boundingbox;
                const double OBJ_SIZE = std::min(0.2, 0.9 / std::max(FILES, RANKS));
                const double OBJ_SPAC = OBJ_SIZE * 1.1;
                const double ROTATION = -1 * acos(-1) / 8;

                double leftmost = POS.first - SPACING * OBJ_SPAC * 0.5 * (bb.first - 1);
                double bottommost = POS.second + SPACING * OBJ_SPAC * 0.5 * (bb.second - 1);
                for (auto i : bg->pairs) {
                    double xp = leftmost + SPACING * OBJ_SPAC * i.first;
                    double yp = bottommost - SPACING * OBJ_SPAC * i.second;
                    std::pair<double, double> thepos = {xp, yp};
                    if (bg->oriented) DrawPoly(vec2pd(thepos), 4, OBJ_SIZE * SPACING * 0.5, 45, getColor(e->color));
                    else {
                        thepos = rotpd(thepos, POS, ROTATION);
                        DrawPoly(vec2pd(thepos), 4, OBJ_SIZE * SPACING * 0.5, 45 + ROTATION * RAD2DEG, getColor(e->color));
                    } 
                }
            }
        }
    }

    // Draw the solution path is there is one.

    for (int y = 0; y < g.n; y++) {
        for (int x = 0; x < g.m; x++) {
            pair<double, double> POS = {GRIDTL.first + halfspacing * x, GRIDTL.second + halfspacing * y};
            std::shared_ptr<Entity> e = g.board[y][x];

            if (e->isPathOccupied) {
                if (x % 2 == 0 && y % 2 == 0) DrawCircle(POS.first, POS.second, THICKNESS, LINE);
                else if (x % 2 == 0) DrawRectangle(POS.first - THICKNESS, POS.second - halfspacing, THICKNESS * 2, SPACING, LINE);
                else if (y % 2 == 0) DrawRectangle(POS.first - halfspacing, POS.second - THICKNESS, SPACING, THICKNESS * 2, LINE);

                if (isStartingPoint(e)) DrawCircle(POS.first, POS.second, THICKNESS * START_RAD, LINE);
                else if (isEndingPoint(e)) {
                
                double prodist = protrusion * SPACING;
                if (x == 0) {
                    if (y == 0) DrawSlot(POS.first, POS.second, POS.first - prodist, POS.second - prodist, THICKNESS, LINE);
                    else if (y == g.n - 1) DrawSlot(POS.first, POS.second, POS.first - prodist, POS.second + prodist, THICKNESS, LINE);
                    else DrawSlot(POS.first, POS.second, POS.first - prodist, POS.second, THICKNESS, LINE);
                }
                else if (x == g.m - 1) {
                    if (y == 0) DrawSlot(POS.first, POS.second, POS.first + prodist, POS.second - prodist, THICKNESS, LINE);
                    else if (y == g.n - 1) DrawSlot(POS.first, POS.second, POS.first + prodist, POS.second + prodist, THICKNESS, LINE);
                    else DrawSlot(POS.first, POS.second, POS.first - prodist, POS.second, THICKNESS, LINE);
                }
                else if (y == 0) DrawSlot(POS.first, POS.second, POS.first, POS.second - prodist, THICKNESS, LINE);
                else if (y == g.n - 1) DrawSlot(POS.first, POS.second, POS.first, POS.second - prodist, THICKNESS, LINE);
                else DrawSlot(POS.first, POS.second, POS.first + prodist, POS.second + prodist, THICKNESS, LINE);
                }
            }
        }
    }


    if (buffer) EndDrawing();
}

int main() {

    const int screenWidth = 800;
    const int screenHeight = 600;
    srand(time(0));

    Solver sx;
    RandGrid random;
    //vector<vector<Entity*>> v;/
    
    random.pathfind();

    cout << "Hello World" << endl;

    InitWindow(screenWidth, screenHeight, "");
    SetTargetFPS(60);

    while (WindowShouldClose() == false){
        Grid g;

        int randval = rand() % 4;
        if (randval == 0) g = random.randBlocks(6, 2);
        else if (randval == 1) g = random.randChallengeStars(2);
        else if (randval == 2) g = random.randBlobs(9, 3, 1);
        else if (randval == 3) g = random.randTriangles(10, 1);

        /*

        std::vector<std::vector<std::shared_ptr<Entity>>> v (7, std::vector<std::shared_ptr<Entity>>(5));
        for (int i = 0; i < 7; i++) {
            for (int j = 0; j < 5; j++) v[i][j] = std::shared_ptr<Entity>(new Entity());
        }
            v[1][3] = std::shared_ptr<Triangle>(new Triangle(1));
            v[6][0] = std::shared_ptr<Endpoint>(new Endpoint(false));
            v[0][4] = std::shared_ptr<Endpoint>(new Endpoint(true));
            g = Grid(v);
            g.disp();
            g.defaultGrid();

            g.disp();

        */

        render(g, screenWidth, screenHeight, 0.1);

        for (long long i = 0; i < 5000000000; i++);

        std::cout << "SETTING SOLVER AND SOLVING\n";

        sx.set(g);
        sx.solve();
        sx.activate();

        render(g, screenWidth,screenHeight, 0.1);
        sx.deactivate();
        for (long long i = 0; i < 5000000000; i++);
        continue;
    }

    CloseWindow();
    return 0;
}


/*


    int ball_x = 100;
    int ball_y = 100;
    int ball_speed_x = 5;
    int ball_speed_y = 5;
    int ball_radius = 15;

        BeginDrawing();
        ClearBackground(BLACK);
        ball_x += ball_speed_x;
        ball_y += ball_speed_y;

        if(ball_x + ball_radius >= screenWidth  || ball_x - ball_radius <= 0)
        {
            ball_speed_x *= -1;
        }

        if(ball_y + ball_radius >= screenHeight  || ball_y - ball_radius <= 0)
        {
            ball_speed_y *= -1;
        }

        DrawCircle(ball_x,ball_y,ball_radius, WHITE);
        EndDrawing();

*/
