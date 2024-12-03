#include <stdio.h>
#include <string.h>
#include <stdlib.h>

struct Object{
    int n;
    int gen;
    int x;
    int y;
    int food;
};

struct Object *rabbits;
struct Object *foxes;
struct Object *rocks;


int GEN_PROC_RABBITS, GEN_PROC_FOXES, GEN_FOOD_FOXES, N_GEN, R, C, N;
int rabbitCount = 0, foxCount = 0, rockCount = 0;

void print_gen(int g){
    //Generation 0
    printf("Generation %d",g);
    printf("\n");
    printf("-------   ------- -------");
    printf("\n");

    for (int i =0; i<R; i++){
        printf("|");
        for (int j =0; j<C; j++){
            char symbol =' ';
            for (int k =0; k<rabbitCount;k++){
                if (rabbits[k].x == i && rabbits[k].y == j){
                    symbol = 'R';
                }
            }
            for (int k =0; k<foxCount;k++){
                if (foxes[k].x == i && foxes[k].y == j){
                    symbol = 'F';
                }
            }
            for (int k =0; k<rockCount;k++){
                if (rocks[k].x == i && rocks[k].y == j){
                    symbol = '*';
                }
            }
            printf("%c",symbol);
        }
        printf("|   |");
        for (int j =0; j<R; j++){
            char symbol =' ';
            for (int k =0; k<rabbitCount;k++){
                if (rabbits[k].x == i && rabbits[k].y == j){
                    symbol = rabbits[k].gen +'0';
                }
            }
            for (int k =0; k<foxCount;k++){
                if (foxes[k].x == i && foxes[k].y == j){
                    symbol = foxes[k].gen+'0';
                }
            }
            for (int k =0; k<rockCount;k++){
                if (rocks[k].x == i && rocks[k].y == j){
                    symbol = '*';
                }
            }
            printf("%c",symbol);
        }
        printf("|   |");
        for (int j =0; j<R; j++){
            char symbol =' ';
            for (int k =0; k<rabbitCount;k++){
                if (rabbits[k].x == i && rabbits[k].y == j){
                    symbol = 'R';
                }
            }
            for (int k =0; k<foxCount;k++){
                if (foxes[k].x == i && foxes[k].y == j){
                    int food = GEN_FOOD_FOXES-foxes[k].food;
                    symbol = food+'0';
                }
            }
            for (int k =0; k<rockCount;k++){
                if (rocks[k].x == i && rocks[k].y == j){
                    symbol = '*';
                }
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
    int *rows;
    int **matrix;

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

    rabbits = malloc(R * C * sizeof(struct Object));
    foxes = malloc(R * C * sizeof(struct Object));
    rocks = malloc(R * C * sizeof(struct Object));


    rows = malloc(R * C *sizeof(int));
    matrix = malloc(R * sizeof(int*));
    for (int i=0; i<R; i++){
        matrix[i] =&rows[i*C]; 
        for (int j=0; j<C; j++){
            matrix[i][j] = 0;
        }
    }


    while (fgets(buffer, sizeof(buffer), file) != NULL) {
        char type[10];
        int x, y;

        // Parse the object type and its coordinates
        if (sscanf(buffer, "%s %d %d", type, &x, &y) == 3) {
            if (strcmp(type, "RABBIT") == 0) {
                rabbits[rabbitCount].n = rabbitCount;
                rabbits[rabbitCount].gen = 0; 
                rabbits[rabbitCount].x = x;
                rabbits[rabbitCount].y = y;
                matrix[x][y] = 1;
                rabbitCount++;
            } else if (strcmp(type, "FOX") == 0) {
                foxes[foxCount].n = foxCount;
                foxes[foxCount].gen = 0;  
                foxes[foxCount].x = x;
                foxes[foxCount].y = y;
                foxes[foxCount].food = GEN_FOOD_FOXES; // Initial food level
                matrix[x][y] = 2;
                foxCount++;
            } else if (strcmp(type, "ROCK") == 0) {
                rocks[rockCount].n = rockCount;
                rocks[rockCount].x = x;
                rocks[rockCount].y = y;
                matrix[x][y] = 3;
                rockCount++;
            }
        }
    }
    fclose(file);
    printf("%d %d %d %d %d %d %d\n", GEN_PROC_RABBITS, GEN_PROC_FOXES, GEN_FOOD_FOXES, N_GEN, R, C, N);

    print_gen(0);

    free(matrix);
    free(rows);

}