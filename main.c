#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct{
    int type;
    int gen;
    int food;
} Object;


int GEN_PROC_RABBITS, GEN_PROC_FOXES, GEN_FOOD_FOXES, N_GEN, R, C, N;
int objectCount = 0;
int current_gen = 0;

void print_gen(int g, Object **matrix){
    //Generation 0
    printf("Generation %d",g);
    printf("\n");
    printf("-------   ------- -------");
    printf("\n");

    for (int i =0; i<R; i++){
        printf("|");
        for (int j =0; j<C; j++){
            char symbol;
            switch (matrix[i][j].type)
            {
            case 0:
                symbol = ' ';
                break;
            case 1:
                symbol = 'R';
                break;
            case 2:
                symbol = 'F';
                break;
            case 3:
                symbol = '*';
                break;
            
            default:
                symbol = ' ';
                break;
            }
            printf("%c",symbol);
        }
        printf("|   |");
        for (int j =0; j<R; j++){
            char symbol;
            switch (matrix[i][j].type)
            {
            case 0:
                symbol = ' ';
                break;
            case 1:
                symbol = matrix[i][j].gen+'0';
                break;
            case 2:
                symbol = matrix[i][j].gen+'0';
                break;
            case 3:
                symbol = '*';
                break;
            
            default:
                symbol = ' ';
                break;
            }
            printf("%c",symbol);
        }
        printf("|   |");
        for (int j =0; j<R; j++){
            char symbol;
            switch (matrix[i][j].type)
            {
            case 0:
                symbol = ' ';
                break;
            case 1:
                symbol = 'R';
                break;
            case 2:
                symbol = (GEN_FOOD_FOXES-matrix[i][j].food) +'0';
                break;
            case 3:
                symbol = '*';
                break;
            
            default:
                symbol = ' ';
                break;
            }
            printf("%c",symbol);
        }
        printf("|\n");
    }

    printf("-------   ------- -------");
    printf("\n");

}

void store_output(const char *filename, Object **matrix){
    FILE *file;
    char buffer[256];
    file = fopen(filename, "w");

    fprintf(file, "%d %d %d %d %d %d %d\n", GEN_PROC_RABBITS, GEN_PROC_FOXES, GEN_FOOD_FOXES, N_GEN, R, C, N);

    for (int i=0; i<R; i++){
        for (int j=0; j<C ;j++){
            switch (matrix[i][j].type)
            {
            case 0:
                break;
            case 1:
                fprintf(file, "RABBIT %d %d\n", i, j);
                break;
            case 2:
                fprintf(file, "FOX %d %d\n", i, j);
                break;
            case 3:
                fprintf(file, "ROCK %d %d\n", i, j);
                break;
            
            default:
                break;
            }
        }
    }
    fclose(file);
}

void copy_matrix(Object **new_matrix, Object **original_matrix){
    for (int i =0; i<R; i++){
        for (int j=0; j<C; j++){
            new_matrix[i][j] = original_matrix[i][j];
        }
    }
}

void move_rabbits(Object **matrix){
    Object *new_rows = malloc(R * C *sizeof(Object));
    Object **new_matrix = malloc(R * sizeof(Object*));
    for (int i=0; i<R; i++){
        new_matrix[i] =&new_rows[i*C];
    } 
    copy_matrix(new_matrix, matrix);
    
    for (int i=0; i<R; i++){
        for (int j=0; j<C; j++){
            if (matrix[i][j].type == 1){
                int moves[4];
                int move_count =0;
                
                if (i != 0 && j !=0 && i != R-1 && j != C-1){
                    if (matrix[i-1][j].type == 0){
                        move_count ++;
                        moves[0] = 1;
                    } else moves[0] = 0;

                    if (matrix[i][j+1].type == 0){
                        move_count ++;
                        moves[1] = 1;
                    } else moves[1] = 0;

                    if (matrix[i+1][j].type == 0){
                        move_count ++;
                        moves[2] = 1;
                    } else moves[2] = 0;

                    if (matrix[i][j-1].type == 0){
                        move_count ++;
                        moves[3] = 1;
                    } else moves[3] = 0;
                }else if (i == 0 && j == 0){
                    moves[0] = 0;

                    if (matrix[i][j+1].type == 0){
                        move_count ++;
                        moves[1] = 1;
                    } else moves[1] = 0;

                    if (matrix[i+1][j].type == 0){
                        move_count ++;
                        moves[2] = 1;
                    } else moves[2] = 0;

                    moves[3] = 0;
                }else if (i == R-1 && j == C-1){
                    if (matrix[i-1][j].type == 0){
                        move_count ++;
                        moves[0] = 1;
                    } else moves[0] = 0;

                    moves[1] = 0;

                    moves[2] = 0;

                    if (matrix[i][j-1].type == 0){
                        move_count ++;
                        moves[3] = 1;
                    } else moves[3] = 0;
                }else if (i == 0 && j == C-1){
                    moves[0] = 0;

                    moves[1] = 0;

                    if (matrix[i+1][j].type == 0){
                        move_count ++;
                        moves[2] = 1;
                    } else moves[2] = 0;

                    if (matrix[i][j-1].type == 0){
                        move_count ++;
                        moves[3] = 1;
                    } else moves[3] = 0;
                }else if(i == R-1 && j == 0){
                    if (matrix[i-1][j].type == 0){
                        move_count ++;
                        moves[0] = 1;
                    } else moves[0] = 0;

                    if (matrix[i][j+1].type == 0){
                        move_count ++;
                        moves[1] = 1;
                    } else moves[1] = 0;

                    moves[2] = 0;

                    moves[3] = 0;
                }else if (i == 0 && j != 0 && j != C-1){
                    moves[0] = 0;

                    if (matrix[i][j+1].type == 0){
                        move_count ++;
                        moves[1] = 1;
                    } else moves[1] = 0;

                    if (matrix[i+1][j].type == 0){
                        move_count ++;
                        moves[2] = 1;
                    } else moves[2] = 0;

                    if (matrix[i][j-1].type == 0){
                        move_count ++;
                        moves[3] = 1;
                    } else moves[3] = 0;
                }else if (i == R-1 && j != 0 && j != C-1){
                    if (matrix[i-1][j].type == 0){
                        move_count ++;
                        moves[0] = 1;
                    } else moves[0] = 0;

                    if (matrix[i][j+1].type == 0){
                        move_count ++;
                        moves[1] = 1;
                    } else moves[1] = 0;

                    moves[2] = 0;

                    if (matrix[i][j-1].type == 0){
                        move_count ++;
                        moves[3] = 1;
                    } else moves[3] = 0;
                }else if (j == 0 && i != 0 && i != R-1){
                    if (matrix[i-1][j].type == 0){
                        move_count ++;
                        moves[0] = 1;
                    } else moves[0] = 0;

                    if (matrix[i][j+1].type == 0){
                        move_count ++;
                        moves[1] = 1;
                    } else moves[1] = 0;

                    if (matrix[i+1][j].type == 0){
                        move_count ++;
                        moves[2] = 1;
                    } else moves[2] = 0;

                    moves[3] = 0;
                } else if (j == C-1 && i != 0 && i != R-1){
                    if (matrix[i-1][j].type == 0){
                        move_count ++;
                        moves[0] = 1;
                    } else moves[0] = 0;

                    moves[1] = 0;

                    if (matrix[i+1][j].type == 0){
                        move_count ++;
                        moves[2] = 1;
                    } else moves[2] = 0;

                    if (matrix[i][j-1].type == 0){
                        move_count ++;
                        moves[3] = 1;
                    } else moves[3] = 0;
                }
                
                
                if (move_count != 0){
                    int move = (i+j+current_gen) % move_count;
                    int count = 0;
                    int new_x = 0;
                    int new_y = 0;

                    for (int k=0; k<4; k++){
                        if (moves[k] == 1){
                            if (move == count){
                                switch (k)
                                {
                                case 0:
                                    new_x = i-1; new_y = j;
                                    break;
                                case 1:
                                    new_x = i; new_y = j+1;
                                    break;
                                case 2:
                                    new_x = i+1; new_y = j;
                                    break;
                                case 3:
                                    new_x = i; new_y = j-1;
                                    break;
                                default:
                                    break;
                                }
                            }else count ++;
                        }
                    }
                    if (new_matrix[new_x][new_y].type == 1){
                        if (new_matrix[new_x][new_y].gen < matrix[i][j].gen) new_matrix[new_x][new_y] = matrix[i][j];
                    }else{
                        new_matrix[new_x][new_y] = matrix[i][j];
                        new_matrix[i][j].type = 0;
                    }
                }
                
            }
        }
    }

    print_gen(current_gen, new_matrix);

    free(new_rows);
    free(new_matrix);
}

int main(){
    const char *filename = "ecosystem_examples/input5x5";
    Object *rows;
    Object **matrix;

    // read file
    FILE *file;
    char buffer[256];
    file = fopen(filename, "r");

    if (fgets(buffer, sizeof(buffer), file) != NULL) {
        sscanf(buffer, "%d %d %d %d %d %d %d", 
            &GEN_PROC_RABBITS, 
            &GEN_PROC_FOXES, 
            &GEN_FOOD_FOXES, 
            &N_GEN, 
            &R, 
            &C, 
            &N
        );
       
    }


    rows = malloc(R * C *sizeof(Object));
    matrix = malloc(R * sizeof(Object*));
    for (int i=0; i<R; i++){
        matrix[i] =&rows[i*C]; 
        for (int j=0; j<C; j++){
            matrix[i][j].type = 0;
        }
    }


    while (fgets(buffer, sizeof(buffer), file) != NULL) {
        char type[10];
        int x, y;

        // Parse the object type and its coordinates
        if (sscanf(buffer, "%s %d %d", type, &x, &y) == 3) {
            if (strcmp(type, "RABBIT") == 0) {
                matrix[x][y].type = 1;
                matrix[x][y].gen = 0;
            } else if (strcmp(type, "FOX") == 0) {
                matrix[x][y].type = 2;
                matrix[x][y].gen = 0;
                matrix[x][y].food = GEN_FOOD_FOXES;
            } else if (strcmp(type, "ROCK") == 0) {
                matrix[x][y].type = 3;
            }
        }
    }
    fclose(file);
    print_gen(0, matrix);

    move_rabbits(matrix);
    
    // for (int i =0; i<N_GEN; i++){
    //     print_gen(i+1, matrix);
    // }

    store_output("output", matrix);
    free(matrix);
    free(rows);

}