/** 
 * backtracking looped garden path maker.
 *
 * Starting at (0,0) in a given grid, generates paths in the space that don't cross themselves.
 * Path ends when it fails by getting stuck or when it succeeds by finding its origin point.
 * So really, it generates loops in the space.
 * Will use this to generate garden paths that encompass raised bed spaces. If I get that far.
 *
 * pseudocode:
 * make grid of R rows and C columns. Initialise.
 * starting at (0,0), move in an available direction that isn't already occupied with a P. Mark it with a P. If there were no options, backtrack. (0,0) is an option.
 * When (0,0) is reached, display grid.
 *
 * later, when I use this, I need to evaluate a loop.
 * B squares must have two opposite and adjacent P squares. e.g.
 *          P         P
 * P B P or B but not B P.
 *          P
 * This will eliminate corner beds and beds on outside of the grid.
 * (Beds are 2 ft wide so you need to access both sides.)
 *
 * Then, find the grid with the most beds. If it works, I should start generating "spiral loops".
 * After that, I should be able to initialise a grid to include things like compost bins, existing no-go squares, and that way I can work around objects.
 *
 * main:
 *  make grid, initialise it, set P at (0,0).
 *  call move(grid, (0,0).
 *
 * move:
 *  if at origin and this is not first move, success(). use map and return.
 *  drop a P at current position (0,0).
 *  drop Bs around current position where possible.
 *  make list of possible moves.
 *  call move() for each one.
 *  return.
 *
 * success:
 *  map contains a looped path. need to check that all B cells have 2 adjacent and opposite P cells.
 *  otherwise, the map fails.
 *
 *  pppp
 *  p.Bp
 *  pbbp
 *  pppp
 *
 *
 * here's an idea.
 * a bed square has two orientations. : and ..
 * when you move and drop paths, you have a direction that you are facing.
 * if you are facing north or south, you are dropping .. on either side of you.
 * else you are dropping : on either side of you.
 * when loop is complete, eliminate : beds that don't have a p north and south of them, and eliminate .. beds that don't have p squares east and west of them.
 * if a bed already is reachable by two paths, don't change it.
 * seems to work. then do a pass afterwards to clean up beds that aren't connected.
 * sweet!
 *
 *  how to move and drop beds.
 *      start with where you are.
 *      make list of possible moves.
 *      say I choose to move down.
 *      now i have a direction, so I'm facing down.
 *      drop .. on either side of me. unless there's already a connected bed there.
 *      move to down square.
 *      make list of moves. say I move right. now I'm facing right.
 *      drop : on either side of me and move right unless there's already a connected bed there.
 *      and so on.
 *      
 * when loop completed, eliminate 'dead beds', then sum the beds. if better than last run, record and continue.
 *
 * I'm going to ignore the issue that I'll be using this with a 13x15 map, which is 195 squares, 
 * so there's an upper bound of a number with 94 decimals.
 *
 * I might get into a sitation where there's no way back to the origin but I keep trying billions of wrong paths.
 * my ideas is that if you keep backtracking to a particular node and failing to break through, then that node should be bypassed.
 * so each node should keep count of how many fails it has had. if it reaches a threshold, then reset its count and bypass it.
 * setting the threshold will be a better issue to handle, and this avoids a floodfill calculation, or A*.
 *
 * also, remember when moving, you have to move to a cell where beds could be dropped. e.g.
 *
 * P .
 * P P
 *
 * next move can't be where the . is. don't move to a cell that has an adjacent p excluding where you just came from and the origin.
 * it's implicit that a bed is actually a 2x2ft square. 1x1 ft square isn't included, which is fine because I would never use that.
 */


// problem: after it backtracks, it sometimes overwrites a P with a :. With current run, it happens at 3,5. Figure it out.
// problem: with 10x10 grid, I get a segfault. dafuk.
// with 8x8 the solutions generated are wrong. there's a blank at 1,0. why?
// if you're at 0,3, then all the space on the right of the wall you just made is useless and doesn't have a path back to the origin.
// so I need a node to burn out when it has failed a bunch of times in a row, because this is cheaper maybe than a flood fill to discover this.
// but finding the segfault is first I think.






#include <stdio.h>
#include <stdlib.h>

#define R 8
#define C 8

#define UP 1
#define DOWN 2
#define LEFT 3
#define RIGHT 4

typedef struct {
    int x, y;
    int direction;
}position;



// move count is used to flag if this is the first move.
void move(char array[R][C], position here, int first_move);
int connected(char array[R][C], unsigned x, unsigned y);        //tells me if a bed is connected. 0 = not connected, 1 = connected.
int bed_count(char array[R][C]); // count the number of beds in the array.)

unsigned most_beds = 0;
int main(void){
    char grid[R][C];
    position pos;               // This should be implicitly initialised as {0, 0}.

    // initialise grid using ' ' for empty position.
    unsigned int i, j;
    for(i = 0; i < R; i++){
        for(j = 0; j < C; j++){
            grid[i][j] = ' ';
        }
    }
    move(grid, pos, 1);
    printf("most beds found was %d\n", most_beds);
}



void move(char array[R][C], position here, int first_move){

    //                       U  D  L  R
    //                       0  1  2  3
    int possible_moves[4] = {1, 1, 1, 1};
    unsigned i, j;

//    printf("you're in move(). you are at %d, %d.\n", here.x, here.y);
//    puts("here is the map:");
//        for(i = 0; i < R; i++){
//            for(j = 0; j < C; j++){
//                if(i == here.x && j == here.y)
//                    putchar('.');
//                else
//                    putchar(array[i][j]);
//            }
//            putchar('\n');
//        }
//        putchar('\n');

        //puts("\npress enter");
        //getchar();
    // if this isn't your first move but you're standing on the origin, then you made a loop. success.
    if(!first_move && here.x == 0 && here.y == 0){
        //success. loop has been made. do something.
        // for now, just display grid.
      //  puts("**********************success. I made a loop.");

        // remove disconnected beds.
        for(i = 0; i < R; i++){
            for(j = 0; j < C; j++){
                if(array[i][j] == ':'){
                    if(i > 0 && i < (R - 1) && array[i - 1][j] == 'P' && array[i + 1][j] == 'P') {}
                    else array[i][j] = ' ';
                }
                else if(array[i][j] == '-'){
                    if(j > 0 && j < (C - 1) && array[i][j - 1] == 'P' && array[i][j + 1] == 'P') {}
                    else array[i][j] = ' ';
                }
            }
        }


        int number_of_beds = bed_count(array);
//        printf("There are %d beds\n", number_of_beds);
        if(number_of_beds > most_beds){
            most_beds = number_of_beds;
            printf("%d beds found:\n", number_of_beds);
                for(i = 0; i < R; i++){
                    for(j = 0; j < C; j++){
                        if(i == here.x && j == here.y)
                            putchar('.');
                        else
                            putchar(array[i][j]);
                    }
                    putchar('\n');
                }
                putchar('\n');

        puts("\npress enter");
        getchar();

        }


        return;
    }


    //# making a possible moves lives.

    // if you're on the left edge, you can't move left.
    if(here.y == 0)
        possible_moves[2] = 0;

    // if you're on the right edge, you can't move right.
    if(here.y == C - 1)
        possible_moves[3] = 0;

    // if you're on the top edge, you can't move up.
    if(here.x == 0)
        possible_moves[0] = 0;

    // if you're on the bottom edge, you can't move down.
    if(here.x == R - 1)
        possible_moves[1] = 0;

    // if the left move is a P and it's not the origin, you can't move left.
    if(here.y > 0 && array[here.x][here.y - 1] == 'P'){
        if(here.x == 0 && here.y == 1) {}
        else
            possible_moves[2] = 0;
    }

    // if the right move is a P, can't move right.
    if(here.y < (C - 1) && array[here.x][here.y + 1] == 'P'){
            possible_moves[3] = 0;
    }

    // if the up move is a P and it's not the origin, you can't move up.
    if(here.x > 0 && array[here.x - 1][here.y] == 'P'){
        if(here.x - 1 == 0 && here.y == 0) {}
        else
            possible_moves[0] = 0;
    }

    // if the down move is a P, can't move down.
    if(here.x < (R - 1) && array[here.x + 1][here.y] == 'P'){
            possible_moves[1] = 0;
    }

    // # you also can't move to a square that's adjacent to a P.
    // if 2 cells up is a P, and it's not the origiin, you can't move up.
    if(here.x >= 2 && array[here.x - 2][here.y] == 'P' && here.x != 2){
        possible_moves[0] = 0;
    }
    // if cell up and right is a P, you can't move up or right.
    if(here.x >= 1 && here.y < (C - 1) && array[here.x - 1][here.y + 1] == 'P'){
        possible_moves[0] = 0;
        possible_moves[3] = 0;
    }
    // if cell 2 to the right is a P, you can't move right.
    if(here.y < (C - 2) && array[here.x][here.y + 2] == 'P'){
        possible_moves[3] = 0;
    }
    // if cell down and right is a P, you can't move down or right.
    if(here.x < (R - 1) && here.y < (C - 1) && array[here.x + 1][here.y + 1] == 'P'){
        possible_moves[1] = 0;
        possible_moves[3] = 0;
    }
    // if cell 2 down is a P, you can't move down.
    if(here.x < (R - 2) && array[here.x + 2][here.y] == 'P'){
        possible_moves[1] = 0;
    }
    // if cell down and left is a P, you can't move down or left.
    // if left is origin however, you can move left.
    if(here.x < (R - 1) && here.y >= 1 && array[here.x + 1][here.y - 1] == 'P'){
        possible_moves[1] = 0;
        if(here.x == 0 && here.y == 1) {}
        else
            possible_moves[2] = 0;
    }
    // if cell 2 to the left is a P, and it's not the origin, you can't move left.
    if(here.y >= 2 && array[here.x][here.y - 2] == 'P'){
        if(here.x == 0 && here.y == 2) {}
        else
            possible_moves[2] = 0;
    }
    // if cell up and left is a P, and it's not the origin, you can't move up or left.
    if(here.x >= 1 && here.y >= 1 && array[here.x - 1][here.y - 1] == 'P'){
        if(here.x == 1 && here.y == 1){}
        else {
            possible_moves[0] = 0;
            possible_moves[2] = 0;
        }
    }

    if(possible_moves[0] + possible_moves[1] + possible_moves[2] + possible_moves[3] == 0){
//        puts("can't move. backtracking.");
        return;
    }
    else{
//        printf("I can move ");
//        if(possible_moves[0]) printf("up ");
//        if(possible_moves[1]) printf("down ");
//        if(possible_moves[2]) printf("left ");
//        if(possible_moves[3]) printf("right ");
    }


    // drop a P where I am.
    array[here.x][here.y] = 'P';

    // # now I know the possible moves, do each of them in turn.

    // if up is possible, do an up move: turning to face upward, dropping beds
    // left and right, then move. don't turn Ps into beds. Also, you can't
    // change a connected bed, so I need to check for that too.
    //TODO finish adding connected() to my lines.

    // if UP is possible, do that.
    if(possible_moves[0] == 1){

        // make copy of array.
        char array_copy[R][C];
        for(i = 0; i < R; i++){
            for(j = 0; j < C; j++){
                array_copy[i][j] = array[i][j];
            }
        }

        // make copy of position.
        position here_copy;
        here_copy = here;
        here_copy.direction = UP;

        // If cell to left exists and isn't a P and isn't a connected bed, put a - there.
        if(here_copy.y > 0 && array[here_copy.x][here_copy.y - 1] != 'P' && !connected(array, here_copy.x, here_copy.y - 1))
            array_copy[here_copy.x][here_copy.y - 1] = '-';

        // if cell to the right exists and isn't a P and isn't a connected bed, put a - there.
        if(here_copy.y < (C - 1) && array[here_copy.x][here_copy.y + 1] != 'P' && !connected(array, here_copy.x, here_copy.y + 1))
            array_copy[here_copy.x][here_copy.y + 1] = '-';

//        puts("I'm moving UP.");
        here_copy.x = here.x - 1;
        move(array_copy, here_copy, 0);
    }


    // if DOWN is possible, do that.
    if(possible_moves[1] == 1){

        // make copy of array.
        char array_copy[R][C];
        for(i = 0; i < R; i++){
            for(j = 0; j < C; j++){
                array_copy[i][j] = array[i][j];
            }
        }

        // make copy of position.
        position here_copy;
        here_copy = here;
        here_copy.direction = DOWN;

        // If cell to left exists and isn't a P and isn't a connected bed, put a - there.
        if(here_copy.y > 0 && array[here_copy.x][here_copy.y - 1] != 'P' && !connected(array, here_copy.x, here_copy.y - 1))
            array_copy[here_copy.x][here_copy.y - 1] = '-';

        // if cell to the right exists and isn't a P and isn't a connected bed, put a - there.
        if(here_copy.y < (C - 1) && array[here_copy.x][here_copy.y + 1] != 'P' && !connected(array, here_copy.x, here_copy.y + 1))
            array_copy[here_copy.x][here_copy.y + 1] = '-';

//        puts("I'm moving DOWN");
        here_copy.x = here.x + 1;
        move(array_copy, here_copy, 0);
    }

    // if LEFT is possible, do that.
    if(possible_moves[2] == 1){

        // make copy of array.
        char array_copy[R][C];
        for(i = 0; i < R; i++){
            for(j = 0; j < C; j++){
                array_copy[i][j] = array[i][j];
            }
        }

        // make copy of position.
        position here_copy;
        here_copy = here;
        here_copy.direction = LEFT;

        // If cell above exists and isn't a P and isn't a connected bed, put a : there.
        if(here_copy.x > 0 && array[here_copy.x - 1][here_copy.y] != 'P' && !connected(array, here_copy.x - 1, here_copy.y)){
            array_copy[here_copy.x - 1][here_copy.y] = ':';
        }


        // if cell below exists and isn't a P and isn't a connected bed, put a : there.
        if(here_copy.x < (R - 1) && array[here_copy.x + 1][here_copy.y] != 'P' && !connected(array, here_copy.x + 1, here_copy.y)){
            array_copy[here_copy.x + 1][here_copy.y] = ':';
        }
//        puts("I'm moving LEFT.");
        here_copy.y = here.y - 1;
        move(array_copy, here_copy, 0);
    }

    // if RIGHT is possible, do that.
    if(possible_moves[3] == 1){

        // make copy of array.
        char array_copy[R][C];
        for(i = 0; i < R; i++){
            for(j = 0; j < C; j++){
                array_copy[i][j] = array[i][j];
            }
        }

        // make copy of position.
        position here_copy;
        here_copy = here;
        here_copy.direction = RIGHT;

        // If cell above exists and isn't a P and isn't a connected bed, put a : there.
        if(here_copy.x > 0 && array[here_copy.x - 1][here_copy.y] != 'P' && !connected(array, here_copy.x - 1, here_copy.y)){
            array_copy[here_copy.x - 1][here_copy.y] = ':';
        }
        // if cell below exists and isn't a P and isn't a connected bed, put a : there.
        if(here_copy.x < (R - 1) && array[here_copy.x + 1][here_copy.y] != 'P' && !connected(array, here_copy.x + 1, here_copy.y)){
            array_copy[here_copy.x - 1][here_copy.y] = ':';
        }
//        puts("I'm moving RIGHT.");
        here_copy.y = here.y + 1;
        move(array_copy, here_copy, 0);
    }
}


//tells me if a bed is connected. 0 = no, 1 = yes. Check that you're actually sending a bed tile before using this.
int connected(char array[R][C], unsigned x, unsigned y){
    if(array[x][y] == ':'){
        if(x > 0 && x < (R - 1) && array[x - 1][y] == 'P' && array[x + 1][y] == 'P')
            return 1;
        else return 0;
    }
    else if(array[x][y] == '-'){
        if(y > 0 && y < (C - 1) && array[x][y - 1] == 'P' && array[x][y + 1] == 'P')
            return 1;
        else return 0;
    }
    else return 0;
}





int bed_count(char array[R][C]){
    int i, j, count = 0;
    for(i = 0; i < R; i++){
        for(j = 0; j < C; j++){
            if(array[i][j] == ':' || array[i][j] == '-')
                count++;
        }
    }
    return count;
}
