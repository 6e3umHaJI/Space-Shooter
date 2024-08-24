#pragma once
extern const float PI = 3.14159265f;
extern const unsigned char ENEMY_AMOUNT_MAX = 4;
extern const unsigned char ASTEROID_AMOUNT_MAX = 3;
extern const short TIME_CYCLE = 4000; // as Milliseconds
extern const unsigned char DISTANCE_BETWEEN = 5;
extern const unsigned char ENEMY_SCORE = 30;
extern const unsigned char ASTEROID_SCORE = 10;

struct Record {
    char name[15];
    int score;
};