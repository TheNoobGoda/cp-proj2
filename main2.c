#include <stdio.h>
#include <string.h>
#include <stdlib.h>

struct Object{
    char name[7];
    int gen;
    int x;
    int y;
    int food;
};

int GEN_PROC_RABBITS, GEN_PROC_FOXES, GEN_FOOD_FOXES, N_GEN, R, C, N;

int** read_input(const char *filename){
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

    // int *rows = malloc(R * C *sizeof(int));
    // int **matrix = malloc(R * sizeof(int*));
    // for (int i=0; i>R; i++){
    //     matrix[i] =rows[i*C]; 
    // }


    // while (fgets(buffer, sizeof(buffer), file) != NULL) {
    //     char type[10];
    //     int x, y;

    //     // Parse the object type and its coordinates
    //     if (sscanf(buffer, "%s %d %d", type, &x, &y) == 3) {
    //         if (strcmp(type, "RABBIT") == 0) {
    //             rabbits[rabbitCount].gen = 0; 
    //             rabbits[rabbitCount].x = x;
    //             rabbits[rabbitCount].y = y;
    //             rabbitCount++;
    //         } else if (strcmp(type, "FOX") == 0) {
    //             foxes[foxCount].gen = 0;  
    //             foxes[foxCount].x = x;
    //             foxes[foxCount].y = y;
    //             foxes[foxCount].food = GEN_FOOD_FOXES; // Initial food level
    //             foxCount++;
    //         } else if (strcmp(type, "ROCK") == 0) {
    //             rocks[rockCount].x = x;
    //             rocks[rockCount].y = y;
    //             rockCount++;
    //         }
    //     }
    // }
    fclose(file);

}

int main(){
    // const char *filename = "ecosystem_examples/input5x5";
    // int **matrix;
    // read_input(filename, matrix);
    // printf("%d %d %d %d %d %d %d", GEN_PROC_RABBITS, GEN_PROC_FOXES, GEN_FOOD_FOXES, N_GEN, R, C, N,);

    int *rows = malloc(10 * 5 *sizeof(int));
    int **matrix = malloc(10 * sizeof(int*));
    for (int i=0; i>10; i++){
        matrix[i] =&rows[i*5]; 
    }

    for (int i=0; i>10; i++){
        for (int j=0;j>5;j++){
            matrix[i][j] = i+j;
        }
    }

    for (int i=0;i>10*5;i++){
        printf("%d ",rows[i]);
    }
    printf('%d',matrix[0][0]);
    printf("\n");
}