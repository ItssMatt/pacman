#include <iostream>
#include <windows.h>
#include <stdlib.h>
#include <conio.h>

using namespace std;

#define REFRESH_TIME 2000
#define GHOSTS_MOVE_TIME 600
#define MAX_ENEMIES 3
#define MAX_POWERUPS 2
#define ROW 21
#define COL 51

int UPDATE_TIME = REFRESH_TIME;
int GHOSTS_MOVE = GHOSTS_MOVE_TIME;

int PlayerPos[2] = { 18, 10 };
int EnemyPos[MAX_ENEMIES][2] = { { 1, 20 }, { 6, 40 }, { 14, 32 } }; bool EnemyDown[MAX_ENEMIES] = { false, false }; int EnemyDirection[MAX_ENEMIES] = { 0, 0, 0 };
int PowerPos[MAX_POWERUPS][2] = { { 10, 1 }, { 10, 48 } }; bool PowerDown[MAX_POWERUPS] = { false, false }; int onPowerEffect = 0;
bool BusyBox[ROW][COL]; int busyboxes = 0;
bool PointBox[ROW][COL]; int points = -1;
bool game_over = false;

char harta[ROW][COL] = {
"##################################################", // 0
"#                                                #", // 1
"#     ############          ###############      #", // 2
"#     #          #          #             #      #", // 3
"#     #          #          #             #      #", // 4
"#     ############          ###############      #", // 5
"#                                                #", // 6
"#                   ######                       #", // 7
"#                   #    #           #####       #", // 8
"#                   #    #           #   #       #", // 9
"#                   #    #           #   #       #", // 10
"#                   #    #           #   #       #", // 11
"#                   ######           #   #       #", // 12
"#                                    #####       #", // 13
"#    #########                                   #", // 14
"#    #       #         ################          #", // 15
"#    #       #         #              #          #", // 16
"#    #########         ################          #", // 17
"#                                                #", // 18
"##################################################"
};

void RefreshTerrain()
{
    system("cls");
    UPDATE_TIME = REFRESH_TIME;
    for(int i = 0; i <= 19; i++) {
        for(int ii = 0; ii < 51; ii++) {
            for(int zz = 0; zz < MAX_ENEMIES; zz++) if(EnemyPos[zz][0] == PlayerPos[0] && EnemyPos[zz][1] == PlayerPos[1] && !EnemyDown[zz]) {
                if(onPowerEffect == 0) game_over = true;
                else {
                    points += 30;
                    EnemyDown[zz] = true;
                }
            }
            for(int zz = 0; zz < MAX_POWERUPS; zz++) if(PowerPos[zz][0] == PlayerPos[0] && PowerPos[zz][1] == PlayerPos[1] && !PowerDown[zz]) {
                onPowerEffect = 10000;
                PowerDown[zz] = true;
            }
            if(PlayerPos[0] == i && PlayerPos[1] == ii) {
                cout << "@";
                if(PointBox[PlayerPos[0]][PlayerPos[1]]) {
                    points++;
                    PointBox[PlayerPos[0]][PlayerPos[1]] = false;
                }
            }
            else {
                bool spawned = false;
                for(int zz = 0; zz < MAX_ENEMIES; zz++) if(EnemyPos[zz][0] == i && EnemyPos[zz][1] == ii && !EnemyDown[zz])
                {
                    if(onPowerEffect == 0) cout << "X";
                    else cout << "V";
                    spawned = true; break;
                }
                for(int zz = 0; zz < MAX_POWERUPS; zz++) if(PowerPos[zz][0] == i && PowerPos[zz][1] == ii && !PowerDown[zz]) { cout << "O"; spawned = true; break; }
                if(!spawned) {
                    if(harta[i][ii] != ' ' && PointBox[i][ii]) cout << "+";
                    else cout << harta[i][ii];
                }
            }
        }
        cout << endl;
    }
    cout << endl << "Your Score: " << points;
}

void MoveGhost(int ghost)
{
    switch(EnemyDirection[ghost])
    {
        case 0:
        {
            if(!BusyBox[EnemyPos[ghost][0]][EnemyPos[ghost][1]-1]) EnemyPos[ghost][1]--;
            else {
                EnemyDirection[ghost] = rand() % 3;
                MoveGhost(ghost);
            }
            break;
        }
        case 1:
        {
            if(!BusyBox[EnemyPos[ghost][0]][EnemyPos[ghost][1]+1]) EnemyPos[ghost][1]++;
            else {
                EnemyDirection[ghost] = rand() % 3;
                MoveGhost(ghost);
            }
            break;
        }
        case 2:
        {
            if(!BusyBox[EnemyPos[ghost][0]-1][EnemyPos[ghost][1]]) EnemyPos[ghost][0]--;
            else {
                EnemyDirection[ghost] = rand() % 3;
                MoveGhost(ghost);
            }
            break;
        }
        case 3:
        {
            if(!BusyBox[EnemyPos[ghost][0]+1][EnemyPos[ghost][1]]) EnemyPos[ghost][0]++;
            else {
                EnemyDirection[ghost] = rand() % 3;
                MoveGhost(ghost);
            }
            break;
        }
    }
}

int main()
{
    cout << "The game is preparing... Please wait.";
    for(int i = 0; i <= 19; i++) {
        for(int ii = 0; ii < 51; ii++) {
                if(harta[i][ii] == '#') { BusyBox[i][ii] = true; busyboxes++; }
                else if(harta[i][ii] == ' ') { PointBox[i][ii] = true; }
            }
    }
    for(int ghost = 0; ghost < MAX_ENEMIES; ghost++) EnemyDirection[ghost] = rand() % 3;

    while(true)
    {
        if(UPDATE_TIME > 0) {
            Sleep(200);
            UPDATE_TIME -= 200;
            GHOSTS_MOVE -= 200;
            if(onPowerEffect > 0) onPowerEffect -= 200;
            if(UPDATE_TIME == 0) RefreshTerrain();
            if(GHOSTS_MOVE == 0) {
                for(int i = 0; i < MAX_ENEMIES; i++) MoveGhost(i);
                RefreshTerrain();
                GHOSTS_MOVE = GHOSTS_MOVE_TIME;
            }
        }
        if(game_over) {
            cout << endl << "Game Over!";
            system("pause");
            return 65535;
        }
        char c = getch();
        if(c == 'a' || c == 'A') { if(!BusyBox[PlayerPos[0]][PlayerPos[1]-1]) PlayerPos[1]--; RefreshTerrain(); }
        else if(c == 'd' || c == 'D') { if(!BusyBox[PlayerPos[0]][PlayerPos[1]+1]) PlayerPos[1]++; RefreshTerrain(); }
        else if(c == 'W' || c == 'w') { if(!BusyBox[PlayerPos[0]-1][PlayerPos[1]]) PlayerPos[0]--; RefreshTerrain(); }
        else if(c == 'S' || c == 's') { if(!BusyBox[PlayerPos[0]+1][PlayerPos[1]]) PlayerPos[0]++; RefreshTerrain(); }
    }
    return 65535;
}
