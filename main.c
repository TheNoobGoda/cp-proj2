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
                
                for (int k = 0; k<4; k++){
                    int x, y;
                    moves[k] = 0;
                    switch (k)
                    {
                    case 0:
                        x = i-1;
                        y = j;
                        break;
                    case 1:
                        x = i;
                        y = j+1;
                        break;
                    case 2:
                        x = i+1;
                        y = j;
                        break;
                    case 3:
                        x = i;
                        y = j-1;
                        break;
                    
                    default:
                        x = i;
                        y = j;
                        break;
                    }
                    if (x >= 0 && x < R && y >= 0 && y < C){
                        if (matrix[x][y].type == 0){
                            move_count ++;
                            moves[k] = 1;
                        }
                    }
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
                    if (new_matrix[new_x][new_y].gen == GEN_PROC_RABBITS){
                        new_matrix[new_x][new_y].gen = 0;
                        new_matrix[i][j].type = 1;
                        new_matrix[i][j].gen = 0;
                    }

                    new_matrix[new_x][new_y].gen ++; // fix later
                }
                
            }
        }
    }

    copy_matrix(matrix, new_matrix);

    free(new_rows);
    free(new_matrix);
}

void move_foxes(Object **matrix){
    Object *new_rows = malloc(R * C *sizeof(Object));
    Object **new_matrix = malloc(R * sizeof(Object*));
    for (int i=0; i<R; i++){
        new_matrix[i] =&new_rows[i*C];
    } 
    copy_matrix(new_matrix, matrix);
    
    for (int i=0; i<R; i++){
        for (int j=0; j<C; j++){
            if (matrix[i][j].type == 2){
                int moves[4];
                int move_count =0;
                int moves2[4];
                int move_count2 = 0;
                
                for (int k = 0; k<4; k++){
                    int x, y;
                    moves[k] = 0;
                    moves2[k] = 0;
                    switch (k)
                    {
                    case 0:
                        x = i-1;
                        y = j;
                        break;
                    case 1:
                        x = i;
                        y = j+1;
                        break;
                    case 2:
                        x = i+1;
                        y = j;
                        break;
                    case 3:
                        x = i;
                        y = j-1;
                        break;
                    
                    default:
                        x = i;
                        y = j;
                        break;
                    }
                    if (x >= 0 && x < R && y >= 0 && y < C){
                        if (matrix[x][y].type == 1){
                            move_count ++;
                            moves[k] = 1;
                        }else if (matrix[x][y].type == 0){
                            move_count2 ++;
                            moves2[k] = 1;
                        }
                    }
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
                    if (new_matrix[new_x][new_y].type == 2){
                        if (new_matrix[new_x][new_y].gen < matrix[i][j].gen) new_matrix[new_x][new_y] = matrix[i][j];
                    }else{
                        new_matrix[new_x][new_y] = matrix[i][j];
                        new_matrix[i][j].type = 0;
                    }
                    if (new_matrix[new_x][new_y].gen == GEN_PROC_FOXES){
                        new_matrix[new_x][new_y].gen = 0;
                        new_matrix[i][j].type = 2;
                        new_matrix[i][j].gen = 0;
                        new_matrix[i][j].food = GEN_FOOD_FOXES;
                    }

                    new_matrix[new_x][new_y].gen ++; // fix later
                    new_matrix[new_x][new_y].food --; // fix later
                }else if (move_count2 !=0){
                    int move = (i+j+current_gen) % move_count2;
                    int count = 0;
                    int new_x = 0;
                    int new_y = 0;

                    for (int k=0; k<4; k++){
                        if (moves2[k] == 1){
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
                    if (new_matrix[new_x][new_y].type == 2){
                        if (new_matrix[new_x][new_y].gen < matrix[i][j].gen) new_matrix[new_x][new_y] = matrix[i][j];
                    }else{
                        new_matrix[new_x][new_y] = matrix[i][j];
                        new_matrix[i][j].type = 0;
                    }
                    if (new_matrix[new_x][new_y].gen == GEN_PROC_FOXES){
                        new_matrix[new_x][new_y].gen = 0;
                        new_matrix[i][j].type = 2;
                        new_matrix[i][j].gen = 0;
                        new_matrix[i][j].food = GEN_FOOD_FOXES;
                    }

                    new_matrix[new_x][new_y].gen ++; // fix later
                    new_matrix[new_x][new_y].food --; // fix later
                }
                
            }
        }
    }

    copy_matrix(matrix, new_matrix);

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
    
    for (int i =0; i<N_GEN; i++){
        move_rabbits(matrix);
        move_foxes(matrix);
        current_gen ++;
        print_gen(current_gen, matrix);
    }

    store_output("output", matrix);
    free(matrix);
    free(rows);

}