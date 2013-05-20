//
// Defines the rand functions.
// RAM use: 4 bytes
//

#pragma once

//
// Prototypes
//

void rand_seed();
int rand_choice(int nchoice);
// Returns in this inclusive range!:
int rand_range(int min, int max);
void rand_angletodir(int angle, int *dx, int *dy);
void rand_dir(int *dx, int *dy);

#define FORALL_4DIR(dx, dy) \
    for (int lcl_angle = 0; rand_angletodir(lcl_angle, &dx, &dy), lcl_angle < 4; lcl_angle++)
