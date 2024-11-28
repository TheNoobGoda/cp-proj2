#include <stdio.h>
#include <string.h>

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



void print_gen(struct Rabbit *rabbits, struct Fox *foxes, struct Rock *rocks){
    //Generation 0
    int g=0;
    printf("Generation 0");
    printf("\n");
    printf("-------   ------- -------");
    printf("\n");
    for (int r=0; r<R; r++){
        printf("|");
        // first grid
        for (int c=0; c<C; c++){
            char symbol = ' ';
            //Rabits
            for (int i=0; i<rabbitCount;i++ ){
                if(rabbits[i].x==r && rabbits[i].y==c && rabbits[i].gen==g){
                    symbol= 'R';
                    break;
                }
            }
            //Foxes
            for (int i=0; i<foxCount;i++ ){
                if(foxes[i].x==r && foxes[i].y==c && foxes[i].gen==g){
                    symbol= 'F';
                    break;
                }
            }
            //Rocks
            for (int i=0; i<rockCount;i++ ){
                if(rocks[i].x==r && rocks[i].y==c ){
                    symbol= '*';
                    break;
                }
            }
            printf("%c", symbol);

        }
        printf("|   |" );
        
        // second grid
        for (int c=C; c<C+C; c++){
            char symbol = ' ';
            //Rabits
            for (int i=0; i<rabbitCount;i++ ){
                if(rabbits[i].x==r && rabbits[i].y==c-C && rabbits[i].gen==g){
                    symbol = (char)(g + '0');
                    break;
                }
            }
            //Foxes
            for (int i=0; i<foxCount;i++ ){
                if(foxes[i].x==r && foxes[i].y==c-C && foxes[i].gen==g){
                    symbol = (char)(g + '0');
                    break;
                }
            }
            //Rocks
            for (int i=0; i<rockCount;i++ ){
                if(rocks[i].x==r && rocks[i].y==c-C ){
                    symbol= '*';
                    break;
                }
            }
            printf("%c", symbol);
        }
        // third grid
        printf("| |");
        for (int c=C+C; c<C+C+C; c++){
            char symbol = ' ';
            //Rabits
            for (int i=0; i<rabbitCount;i++ ){
                if(rabbits[i].x==r && rabbits[i].y==c-C-C && rabbits[i].gen==g){
                    symbol= 'R';
                    break;
                }
            }
            //Foxes---> Dúvidas!!!!!!!
            for (int i=0; i<foxCount;i++ ){
                if(foxes[i].x==r && foxes[i].y==c-C-C && foxes[i].gen==g){
                    symbol = (char)(g + '0');
                    break;
                }
            }
            //Rocks
            for (int i=0; i<rockCount;i++ ){
                if(rocks[i].x==r && rocks[i].y==c-C-C ){
                    symbol= '*';
                    break;
                }
            }
            printf("%c", symbol);
        }
        printf("|" );
        printf("\n");
        if (r == R - 1) {
            printf("-------   ------- -------\n");}

    }
}





//void print_output(){
    
//}


int main() {
    const char *filename = "ecosystem_examples/input5x5";
    read_input(filename);
    print_gen(rabbits, foxes, rocks);
}

//|* R F|   |* 0 0   | |* R 0    |