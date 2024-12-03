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

    free(matrix);
    free(rows);

}