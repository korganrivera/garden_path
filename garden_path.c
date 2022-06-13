/**
# Garden Path

 * Finds the best way to place a looped garden path in a space to maximise the
 * number of accessible raised beds.
 * 
 * ### The problem
 * 
 * Say you have an area to place a garden in and the area can be viewed as a
 * grid. Say each tile is a 2x2 ft square and it can either be a path square or
 * a bed square. A path square is one which you can step on. A bed square is a
 * raised bed area.
 * 
 * Each raised bed square has an orientation. it's either north-south or
 * east-west. You can access a north-south bed square from the north side or
 * the south side but not from the east or west side. Similar rules for the
 * east-west bed.
 * 
 * You want a path that starts at one point on the grid, moves around the
 * garden space and ends up back at the beginning. You can place bed tiles to
 * the left and right of path tiles as you move.
 * 
 * But a bed tile has to have path tiles on both sides to be valid. Also, you
 * can't place a path tile next to another path tile (except the one you just
 * moved from, obviously.)
 * 
 * With these rules, what's the best path to maximise the number of valid beds?
 * 
 * ### Solution
 * 
 * This program starts at a given grid at (0,0). It evaluates the possible
 * moves from the current position and tries each of them. If it hits a dead
 * end, it backtracks. Along the way, it lays path tiles and adjacent bed tiles
 * where possible; it won't place a bed tile where there is an existing path
 * tile nor where there is a bed tile that's already connected to two path
 * tiles. When it finds its way back to the origin, it elimates the invalid
 * beds, counts the valid ones and shows the result if it's better than the
 * previous result.
 * 


 * I might get into a sitation where there's no way back to the origin but I
 * keep trying billions of wrong paths. my ideas is that if you keep
 * backtracking to a particular node and failing to break through, then that
 * node should be bypassed. so each node should keep count of how many fails it
 * has had. if it reaches a threshold, then reset its count and bypass it.
 * setting the threshold will be a better issue to handle, and this avoids a
 * floodfill calculation, or A*.
 *

 Speed up ideas:
 replace pass by value in move() with pass by reference: pass pointer to array, not the whole array.
 could I multithread the recursion? Like, for each possible move I can make at the current position, have a thread take care of each one. Would that work?
 * not certain about that rule to not move up in on rows 0 to 2. that assumes the first move from the origin was down, and it could be right. need to mess with that.
 * if first move was down, then use it, else write one for a right move too.
 * similar rules around the grid too, like if first move was down, then don't bother turning downward if you're on the 2nd last column because you can't come back up. and so on.
*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h> // for memcpy()

// grid dimensions
#define R 8
#define C 12

// toggle debugging stuff.
#define DEBUG 0

// directions used to set position.direction more easily.
#define UP 1
#define DOWN 2
#define LEFT 3
#define RIGHT 4

// current position and direction.
typedef struct {
    int x, y;
    int direction;
} position;



void move(char array[R][C], position here, int first_move);     // Recursive function responsible for moving and backtracking. If first use, first_move = 1, else 0;
void move2(char array[R][C], position here, int first_move);     // Recursive function responsible for moving and backtracking. If first use, first_move = 1, else 0;
void display_grid(char array[R][C]);
void press_to_continue(void);
int most_beds = 0;                                              // Holds the highest number of beds found so far.

int main(void){
    char grid[R][C];
    position pos;
    pos.x = pos.y = 0;

    // Fill grid with ' ' characters using memset.
    memset(grid, ' ', sizeof(grid));

    // add an object maker instead of using points.

    // Let's put in some objects: two compost bins.
    // two bins facing each other 4 sq ft in between.
   // grid[1][0] = 'C';
   // grid[2][0] = 'C';
//    grid[6][11] = 'C';
//    grid[6][12] = 'C';
//    grid[6][13] = 'C';
//    grid[6][14] = 'C';
//
//    grid[7][11] = 'C';
//    grid[7][12] = 'C';
//    grid[7][13] = 'C';
//    grid[7][14] = 'C';
//
//    grid[8][11] = 'C';
//    grid[8][12] = 'C';
//    grid[8][13] = 'C';
//    grid[8][14] = 'C';


    if(DEBUG){
        puts("I'm in main(). Here's my starting grid:");
        display_grid(grid);
        printf("my starting position is %d, %d.\n", pos.x, pos.y);
    }

    move(grid, pos, 1);

    printf("Most beds found: %d\n", most_beds);
}



void move(char array[R][C], position here, int first_move){

    int i, j;
    int number_of_beds;
    int up, down, left, right;      // flags for possible moves.
    up = down = left = right = 1;   // Assume possible until proven otherwise.
    position here_copy;
    char array_copy[R][C];

    if(DEBUG){
        puts("entering move(). Here's the array:");
        display_grid(array);
        press_to_continue();
    }

    // Drop a P where I am.
    array[here.x][here.y] = 'P';

    if(DEBUG){
        puts("Just dropped a P. Here's the array:");
        display_grid(array);
        press_to_continue();
    }

    // If this isn't your first move but you're standing on the origin, then you made a loop. Success.
    if(!first_move && here.x == 0 && here.y == 0){
        //  puts("**********************success. I made a loop.");

        if(DEBUG){
            puts("Made a loop. Here's the array:");
            system("clear");
            display_grid(array);
            press_to_continue();
            puts("Now I'll add beds.");
        }
        // add connected beds and count them as you go.
        number_of_beds = 0;
        for(i = 0; i < R; i++){
            for(j = 0; j < C; j++){

                if(array[i][j] != ' '){
                    continue;
                }

                else if(i > 0 && i < (R - 1) && array[i - 1][j] == 'P' && array[i + 1][j] == 'P'){
                    array[i][j] = ':';
                    number_of_beds++;
                }

                else if(j > 0 && j < (C - 1) && array[i][j - 1] == 'P' && array[i][j + 1] == 'P'){
                    array[i][j] = '-';
                    number_of_beds++;
                }
            }
        }
    

    if(DEBUG){
        puts("Added connected beds. Here's the array:");
        display_grid(array);
        //press_to_continue();
    }

        // Record number of beds if it's the highest found so far.
        if(number_of_beds > most_beds){
            printf("There are %d beds\n", number_of_beds);
            puts("This bed count was a high score.");
            most_beds = number_of_beds;
            display_grid(array);
            //press_to_continue();
        }
        return;
    }

    // # Make a possible moves list.

    if(DEBUG){
        puts("Making possible moves list.");
        press_to_continue();
    }

// EDGES

    // if you're on the left edge, you can't move left.
    if(here.y == 0){
        left = 0;
        if(DEBUG){
            puts("I'm on left edge so left move was eliminated.");
            press_to_continue();
        }
    }
    // if you're on the right edge, you can't move right.
    if(here.y == C - 1){
        right = 0;
        if(DEBUG){
            puts("I'm on right edge so right move was eliminated.");
            press_to_continue();
        }
    }
    // if you're on the top edge, you can't move up.
    if(here.x == 0){
        up = 0;
        if(DEBUG){
            puts("I'm on top edge so up move was eliminated.");
            press_to_continue();
        }
    }
    // if you're on the bottom edge, you can't move down.
    if(here.x == R - 1){
        if(DEBUG){
            puts("I'm on bottom edge so down move was eliminated.");
            press_to_continue();
        }
        down = 0;
    }

    if(DEBUG){
        puts("Now I'm checking to prevent moves to a P square.");
        press_to_continue();
    }
// PREVENT MOVING TO A P SQUARE (excluding the origin).

    // if the left move is a P and it's not the origin, you can't move left.
    if(here.y > 0 && array[here.x][here.y - 1] == 'P'){
        if(here.x == 0 && here.y == 1) {}
        else{
            left = 0;
            if(DEBUG){
                puts("Left move is a P so I can't move there.");
                press_to_continue();
            }
        }
    }

    // if the right move is a P, can't move right.
    if(here.y < (C - 1) && array[here.x][here.y + 1] == 'P'){
            right = 0;
            if(DEBUG){
                puts("Right move is a P so I can't move there.");
                press_to_continue();
            }
    }

    // if the up move is a P and it's not the origin, you can't move up.
    if(here.x > 0 && array[here.x - 1][here.y] == 'P'){
        if(here.x - 1 == 0 && here.y == 0) {}
        else{
            up = 0;
            if(DEBUG){
                puts("Up move is a P so I can't move there.");
                press_to_continue();
            }
        }
    }

    // if the down move is a P, can't move down.
    if(here.x < (R - 1) && array[here.x + 1][here.y] == 'P'){
            down = 0;
            if(DEBUG){
                puts("Down move is a P so I can't move there.");
                press_to_continue();
            }
    }


// PREVENT MOVING TO SQUARES ADJACENT TO A P SQUARE.

    if(DEBUG){
        puts("Now I'll check moves adjacent to P squares.");
        press_to_continue();
    }

    // if 2 cells up is a P, and it's not the origin, you can't move up.
    if(here.x >= 2 && array[here.x - 2][here.y] == 'P'){
        if(here.x == 2 && here.y == 0) {}
        else{
            up = 0;
            if(DEBUG){
                puts("2 cells up is a P, so I can't move up.");
                press_to_continue();
            }
        }
    }
    // if cell up and right is a P, you can't move up or right.
    if(here.x >= 1 && here.y < (C - 1) && array[here.x - 1][here.y + 1] == 'P'){
        up = 0;
        right = 0;
        if(DEBUG){
            puts("Cell up and right is a P, so I can't move up or right.");
            press_to_continue();
        }
    }
    // if cell 2 to the right is a P, you can't move right.
    if(here.y < (C - 2) && array[here.x][here.y + 2] == 'P'){
        right = 0;
        if(DEBUG){
            puts("2 cells to the right is a P, so I can't move right.");
            press_to_continue();
        }
    }
    // if cell down and right is a P, you can't move down or right.
    if(here.x < (R - 1) && here.y < (C - 1) && array[here.x + 1][here.y + 1] == 'P'){
        down = 0;
        right = 0;
        if(DEBUG){
            puts("Cell down and right is a P, so I can't move down or right.");
            press_to_continue();
        }
    }
    // if cell 2 down is a P, you can't move down.
    if(here.x < (R - 2) && array[here.x + 2][here.y] == 'P'){
        down = 0;
        if(DEBUG){
            puts("Cell 2 down is a P, so I can't move down.");
            press_to_continue();
        }
    }
    // if cell down and left is a P, you can't move down or left.
    // exception is if you're at 0,1.
    if(here.x < (R - 1) && here.y >= 1 && array[here.x + 1][here.y - 1] == 'P'){
        down = 0;

        if(here.x == 0 && here.y == 1) {}
        else left = 0;
        if(DEBUG){
            puts("Cell down and left is a P, so I can't move down or left. exception if left is origin.");
            press_to_continue();
        }
    }
    // if cell 2 to the left is a P, and it's not the origin, you can't move left.
    if(here.y >= 2 && array[here.x][here.y - 2] == 'P'){
        if(here.x == 0 && here.y == 2) {}
        else{
            left = 0;
            if(DEBUG){
                puts("Cell 2 to the left is a P, so I can't move left.");
                press_to_continue();
            }
        }
    }
    // if cell up and left is a P, and it's not the origin, you can't move up or left.
    if(here.x >= 1 && here.y >= 1 && array[here.x - 1][here.y - 1] == 'P'){
        if(here.x == 1 && here.y == 1){}
        else {
            up = 0;
            left = 0;
            if(DEBUG){
                puts("Cell up and left is a P, so I can't move up or left.");
                press_to_continue();
            }
        }
    }

// # TRIAL RULE: if x < 2, don't turn right. If you do, you'll create a path that will block a return to the origin.

    if(here.x < 2){
        right = 0;
    }

// # TRIAL RULE 2: You can't move onto a compost bin tile.
// Now I can add no-go tiles.
    if(here.x > 0 && array[here.x - 1][here.y] == 'C')
        up = 0;

    if(here.x < (R - 1) && array[here.x + 1][here.y] == 'C')
        down = 0;

    if(here.y > 0 && array[here.x][here.y - 1] == 'C')
        left = 0;

    if(here.y < (C - 1) && array[here.x][here.y + 1] == 'C')
        right = 0;

    if(up + down + left + right == 0){

        if(DEBUG){
            puts("I can't move anywhere. Backtracking...");
            press_to_continue();
        }
        return;
    }
    else{
        if(DEBUG){
            printf("I can move ");
            if(up) printf("up ");
            if(down) printf("down ");
            if(left) printf("left ");
            if(right) printf("right");
            press_to_continue();
        }
    }

    // # now I know the possible moves, do each of them in turn.

    // if up is possible, do an up move: turning to face upward.
    if(up == 1){
/*
        // make copy of array.
        for(i = 0; i < R; i++){
            for(j = 0; j < C; j++){
                array_copy[i][j] = array[i][j];
            }
        }
*/
        // Copying arrays using memcpy.
        memcpy(array_copy, array, sizeof(char) * R * C);

        if(DEBUG){
            puts("I'm going to try moving up.");
            display_grid(array_copy);
            press_to_continue();
        }
        // make copy of position and set direction.
        here_copy = here;
        here_copy.direction = UP;
        here_copy.x = here.x - 1;
        move(array_copy, here_copy, 0);
    }


    // if DOWN is possible, do that.
    if(down == 1){
/*
        // make copy of array.
        for(i = 0; i < R; i++){
            for(j = 0; j < C; j++){
                array_copy[i][j] = array[i][j];
            }
       }
*/
        // Copying arrays using memcpy.
        memcpy(array_copy, array, sizeof(char) * R * C);

        if(DEBUG){
            puts("I'm going to try moving down.");
            display_grid(array_copy);
            press_to_continue();
        }
        // make copy of position.
        here_copy = here;
        here_copy.direction = DOWN;
        here_copy.x = here.x + 1;
        move(array_copy, here_copy, 0);
    }

    // if LEFT is possible, do that.
    if(left == 1){
/*
        // make copy of array.
        for(i = 0; i < R; i++){
            for(j = 0; j < C; j++){
                array_copy[i][j] = array[i][j];
            }
        }
*/
        // Copying arrays using memcpy.
        memcpy(array_copy, array, sizeof(char) * R * C);

        if(DEBUG){
            puts("I'm going to try moving left.");
            press_to_continue();
        }

        // make copy of position.
        here_copy = here;
        here_copy.direction = LEFT;
        here_copy.y = here.y - 1;
        move(array_copy, here_copy, 0);
    }

    // if RIGHT is possible, do that.
    if(right == 1){
/*
        // make copy of array.
        for(i = 0; i < R; i++){
            for(j = 0; j < C; j++){
                array_copy[i][j] = array[i][j];
            }
        }
*/
        // Copying arrays using memcpy.
        memcpy(array_copy, array, sizeof(char) * R * C);

        if(DEBUG){
            puts("I'm going to try moving right.");
            press_to_continue();
        }

        // make copy of position.
        here_copy = here;
        here_copy.direction = RIGHT;
        here_copy.y = here.y + 1;
        move(array_copy, here_copy, 0);
    }
}

void display_grid(char array[R][C]){
    int i, j;

    // print blank space before top row of numbers.
    putchar(' ');

    // Print line along the top.
    // Trying a backwards for loop.
    for(i = C; i--; )
        putchar('-');

    putchar('\n');

    for(i = 0; i < R; i++){
        for(j = 0; j < C; j++){
            if(j == 0)
                putchar('|');
            putchar(array[i][j]);
        }
        putchar('\n');
    }
    putchar('\n');
}



void press_to_continue(void){
    puts("\n--pktc--");
    getchar();
}
