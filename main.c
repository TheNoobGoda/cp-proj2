#include <stdio.h>
#include <string.h>
#include <stdlib.h>

struct Rabbit
{
    int gen;
    int x;
    int y;
};

struct Fox
{
    int gen;
    int x;
    int y;
    int food;
};

struct Rock
{
    int x;
    int y;
};

int GEN_PROC_RABBITS, GEN_PROC_FOXES, GEN_FOOD_FOXES, N_GEN, R, C, N;

// Arrays to store objects
struct Rabbit rabbits[100];
struct Fox foxes[100];
struct Rock rocks[100];


// Counts for each object type
int rabbitCount = 0;
int foxCount = 0;
int rockCount = 0;



void read_input(const char *filename){
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

    while (fgets(buffer, sizeof(buffer), file) != NULL) {
        char type[10];
        int x, y;

        // Parse the object type and its coordinates
        if (sscanf(buffer, "%s %d %d", type, &x, &y) == 3) {
            if (strcmp(type, "RABBIT") == 0) {
                rabbits[rabbitCount].gen = 0; 
                rabbits[rabbitCount].x = x;
                rabbits[rabbitCount].y = y;
                rabbitCount++;
            } else if (strcmp(type, "FOX") == 0) {
                foxes[foxCount].gen = 0;  
                foxes[foxCount].x = x;
                foxes[foxCount].y = y;
                foxes[foxCount].food = GEN_FOOD_FOXES; // Initial food level
                foxCount++;
            } else if (strcmp(type, "ROCK") == 0) {
                rocks[rockCount].x = x;
                rocks[rockCount].y = y;
                rockCount++;
            }
        }
    }
    fclose(file);

}



void print_gen(struct Rabbit *rabbits, struct Fox *foxes, struct Rock *rocks, int size, int g){
    //Generation 0
    printf("Generation %d",g);
    printf("\n");
    printf("-------   ------- -------");
    printf("\n");

    for (int i =0; i<size; i++){
        printf("|");
        for (int j =0; j<size; j++){
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
        for (int j =0; j<size; j++){
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
        for (int j =0; j<size; j++){
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





//void print_output(){
    
//}


int main() {
    const char *filename = "ecosystem_examples/input5x5";
    read_input(filename);
    rabbits[0].gen = 1;
    foxes[0].food = 2;
    print_gen(rabbits, foxes, rocks, C, 0);
}

