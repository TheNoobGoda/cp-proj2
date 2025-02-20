#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>

typedef struct
{
    int type;
    int gen;
    int food;
} Object;

#ifdef _OPENMP
omp_lock_t locks[100];
#endif

int GEN_PROC_RABBITS, GEN_PROC_FOXES, GEN_FOOD_FOXES, N_GEN, R, C, N;
int objectCount = 0;
int current_gen = 0;

int NTHREADS = 4;

void print_gen(int g, Object **matrix)
{
    // Generation 0
    printf("Generation %d", g);
    printf("\n");
    printf("-------   ------- -------");
    printf("\n");

    for (int i = 0; i < R; i++)
    {
        printf("|");
        for (int j = 0; j < C; j++)
        {
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
            printf("%c", symbol);
        }
        printf("|   |");
        for (int j = 0; j < R; j++)
        {
            char symbol;
            switch (matrix[i][j].type)
            {
            case 0:
                symbol = ' ';
                break;
            case 1:
                symbol = matrix[i][j].gen + '0';
                break;
            case 2:
                symbol = matrix[i][j].gen + '0';
                break;
            case 3:
                symbol = '*';
                break;

            default:
                symbol = ' ';
                break;
            }
            printf("%c", symbol);
        }
        printf("|   |");
        for (int j = 0; j < R; j++)
        {
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
                symbol = (GEN_FOOD_FOXES - matrix[i][j].food) + '0';
                break;
            case 3:
                symbol = '*';
                break;

            default:
                symbol = ' ';
                break;
            }
            printf("%c", symbol);
        }
        printf("|\n");
    }

    printf("-------   ------- -------");
    printf("\n");
}

void store_output(const char *filename, Object **matrix)
{
    FILE *file;
    file = fopen(filename, "w");

    fprintf(file, "%d %d %d %d %d %d %d\n", GEN_PROC_RABBITS, GEN_PROC_FOXES, GEN_FOOD_FOXES, N_GEN, R, C, N);

    for (int i = 0; i < R; i++)
    {
        for (int j = 0; j < C; j++)
        {
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

void copy_matrix(Object **new_matrix, Object **original_matrix)
{
    for (int i = 0; i < R; i++)
    {
        for (int j = 0; j < C; j++)
        {
            new_matrix[i][j] = original_matrix[i][j];
        }
    }
}

void move_rabbits(Object **matrix)
{
    Object *new_rows = malloc(R * C * sizeof(Object));
    Object **new_matrix = malloc(R * sizeof(Object *));
    for (int i = 0; i < R; i++)
    {
        new_matrix[i] = &new_rows[i * C];
    }
    copy_matrix(new_matrix, matrix);

#pragma omp parallel for num_threads(NTHREADS)
    for (int i = 0; i < R; i++)
    {
        for (int j = 0; j < C; j++)
        {
            if (matrix[i][j].type == 1)
            {
                int moves[4];
                int move_count = 0;

                for (int k = 0; k < 4; k++)
                {
                    int x, y;
                    moves[k] = 0;
                    switch (k)
                    {
                    case 0:
                        x = i - 1;
                        y = j;
                        break;
                    case 1:
                        x = i;
                        y = j + 1;
                        break;
                    case 2:
                        x = i + 1;
                        y = j;
                        break;
                    case 3:
                        x = i;
                        y = j - 1;
                        break;

                    default:
                        x = i;
                        y = j;
                        break;
                    }
                    if (x >= 0 && x < R && y >= 0 && y < C)
                    {
                        if (matrix[x][y].type == 0)
                        {
                            move_count++;
                            moves[k] = 1;
                        }
                    }
                }

                if (move_count != 0)
                {
                    int move = (i + j + current_gen) % move_count;
                    int count = 0;
                    int new_x = 0;
                    int new_y = 0;

                    for (int k = 0; k < 4; k++)
                    {
                        if (moves[k] == 1)
                        {
                            if (move == count)
                            {
                                switch (k)
                                {
                                case 0:
                                    new_x = i - 1;
                                    new_y = j;
                                    break;
                                case 1:
                                    new_x = i;
                                    new_y = j + 1;
                                    break;
                                case 2:
                                    new_x = i + 1;
                                    new_y = j;
                                    break;
                                case 3:
                                    new_x = i;
                                    new_y = j - 1;
                                    break;
                                default:
                                    break;
                                }
                                break;
                            }
                            else
                                count++;
                        }
                    }

                    int index = (new_x + new_y) % 100;
                    int index2 = (i + j) % 100;

#pragma omp critical
                    {
#ifdef _OPENMP
                        omp_set_lock(&locks[index]);
                        omp_set_lock(&locks[index2]);
#endif
                    }

                    if (new_matrix[new_x][new_y].type == 1)
                    {

                        if (new_matrix[new_x][new_y].gen < matrix[i][j].gen)
                        {

                            if (new_matrix[new_x][new_y].gen < matrix[i][j].gen)
                            {
                                new_matrix[new_x][new_y] = matrix[i][j];
                                if (new_matrix[new_x][new_y].gen >= GEN_PROC_RABBITS + 1)
                                {
                                    new_matrix[i][j].type = 1;
                                    new_matrix[i][j].gen = 0;
                                    new_matrix[new_x][new_y].gen = 0;
                                }
                                else
                                    new_matrix[i][j].type = 0;
                            }
                        }
                        else if (matrix[i][j].gen >= GEN_PROC_RABBITS + 1)
                        {
                            new_matrix[i][j].gen = 0;
                        }
                        else
                            new_matrix[i][j].type = 0;
                    }
                    else
                    {

                        new_matrix[new_x][new_y] = matrix[i][j];
                        new_matrix[i][j].type = 0;

                        if (new_matrix[new_x][new_y].gen >= GEN_PROC_RABBITS + 1)
                        {
                            new_matrix[i][j].type = 1;
                            new_matrix[i][j].gen = 0;
                            new_matrix[new_x][new_y].gen = 0;
                        }
                    }
#ifdef _OPENMP
                    omp_unset_lock(&locks[index]);
                    omp_unset_lock(&locks[index2]);
#endif
                }
            }
        }
    }

    copy_matrix(matrix, new_matrix);

    free(new_rows);
    free(new_matrix);
}

void move_foxes(Object **matrix)
{
    Object *new_rows = malloc(R * C * sizeof(Object));
    Object **new_matrix = malloc(R * sizeof(Object *));
    for (int i = 0; i < R; i++)
    {
        new_matrix[i] = &new_rows[i * C];
    }
    copy_matrix(new_matrix, matrix);

#pragma omp parallel for num_threads(NTHREADS)
    for (int i = 0; i < R; i++)
    {
        for (int j = 0; j < C; j++)
        {
            if (matrix[i][j].type == 2)
            {
                int moves[4];
                int move_count = 0;
                int moves2[4];
                int move_count2 = 0;

                for (int k = 0; k < 4; k++)
                {
                    int x, y;
                    moves[k] = 0;
                    moves2[k] = 0;
                    switch (k)
                    {
                    case 0:
                        x = i - 1;
                        y = j;
                        break;
                    case 1:
                        x = i;
                        y = j + 1;
                        break;
                    case 2:
                        x = i + 1;
                        y = j;
                        break;
                    case 3:
                        x = i;
                        y = j - 1;
                        break;

                    default:
                        x = i;
                        y = j;
                        break;
                    }
                    if (x >= 0 && x < R && y >= 0 && y < C)
                    {
                        if (matrix[x][y].type == 1)
                        {
                            move_count++;
                            moves[k] = 1;
                        }
                        else if (matrix[x][y].type == 0)
                        {
                            move_count2++;
                            moves2[k] = 1;
                        }
                    }
                }

                if (move_count != 0)
                {

                    int move = (i + j + current_gen) % move_count;
                    int count = 0;
                    int new_x = 0;
                    int new_y = 0;

                    for (int k = 0; k < 4; k++)
                    {
                        if (moves[k] == 1)
                        {
                            if (move == count)
                            {
                                switch (k)
                                {
                                case 0:
                                    new_x = i - 1;
                                    new_y = j;
                                    break;
                                case 1:
                                    new_x = i;
                                    new_y = j + 1;
                                    break;
                                case 2:
                                    new_x = i + 1;
                                    new_y = j;
                                    break;
                                case 3:
                                    new_x = i;
                                    new_y = j - 1;
                                    break;
                                default:
                                    break;
                                }
                                break;
                            }
                            else
                                count++;
                        }
                    }
                    int index = (new_x + new_y) % 100;
                    int index2 = (i + j) % 100;
#pragma omp critical
                    {
#ifdef _OPENMP
                        omp_set_lock(&locks[index]);
                        omp_set_lock(&locks[index2]);
#endif

                        if (new_matrix[new_x][new_y].type == 2)
                        {
                            if (new_matrix[new_x][new_y].gen < matrix[i][j].gen || (new_matrix[new_x][new_y].gen == matrix[i][j].gen && new_matrix[new_x][new_y].food > matrix[i][j].food))
                            {

                                new_matrix[new_x][new_y] = matrix[i][j];
                                new_matrix[new_x][new_y].food = 0;
                                if (new_matrix[new_x][new_y].gen >= GEN_PROC_FOXES + 1)
                                {
                                    new_matrix[i][j].type = 2;
                                    new_matrix[i][j].gen = 0;
                                    new_matrix[i][j].food = 0;
                                    new_matrix[new_x][new_y].gen = 0;
                                }
                                else
                                    new_matrix[i][j].type = 0;
                            }
                            else if (matrix[i][j].gen >= GEN_PROC_FOXES + 1)
                            {
                                new_matrix[i][j].gen = 0;
                            }
                            else
                                new_matrix[i][j].type = 0;
                        }
                        else
                        {
                            new_matrix[new_x][new_y] = matrix[i][j];
                            new_matrix[i][j].type = 0;
                            new_matrix[new_x][new_y].food = 0;

                            if (new_matrix[new_x][new_y].gen >= GEN_PROC_FOXES + 1)
                            {

                                new_matrix[i][j].type = 2;
                                new_matrix[i][j].gen = 0;
                                new_matrix[i][j].food = 0;
                                new_matrix[new_x][new_y].gen = 0;
                            }
                        }
                    }

#ifdef _OPENMP
                    omp_unset_lock(&locks[index]);
                    omp_unset_lock(&locks[index2]);
#endif
                }
                else if (move_count2 != 0 && matrix[i][j].food != GEN_FOOD_FOXES)
                {
                    int move = (i + j + current_gen) % move_count2;
                    int count = 0;
                    int new_x = 0;
                    int new_y = 0;

                    for (int k = 0; k < 4; k++)
                    {
                        if (moves2[k] == 1)
                        {
                            if (move == count)
                            {
                                switch (k)
                                {
                                case 0:
                                    new_x = i - 1;
                                    new_y = j;
                                    break;
                                case 1:
                                    new_x = i;
                                    new_y = j + 1;
                                    break;
                                case 2:
                                    new_x = i + 1;
                                    new_y = j;
                                    break;
                                case 3:
                                    new_x = i;
                                    new_y = j - 1;
                                    break;
                                default:
                                    break;
                                }
                                break;
                            }
                            else
                                count++;
                        }
                    }
                    int index = (new_x + new_y) % 100;
                    int index2 = (i + j) % 100;

#pragma omp critical
                    {
#ifdef _OPENMP
                        omp_set_lock(&locks[index]);
                        omp_set_lock(&locks[index2]);
#endif

                        if (new_matrix[new_x][new_y].type == 2)
                        {
                            if (new_matrix[new_x][new_y].gen < matrix[i][j].gen || (new_matrix[new_x][new_y].gen == matrix[i][j].gen && new_matrix[new_x][new_y].food > matrix[i][j].food))
                            {

                                new_matrix[new_x][new_y] = matrix[i][j];
                                if (new_matrix[new_x][new_y].gen >= GEN_PROC_FOXES + 1)
                                {

                                    new_matrix[i][j].type = 2;
                                    new_matrix[i][j].gen = 0;
                                    new_matrix[i][j].food = 0;
                                    new_matrix[new_x][new_y].gen = 0;
                                }
                                else
                                    new_matrix[i][j].type = 0;
                            }
                            else if (matrix[i][j].gen >= GEN_PROC_FOXES + 1)
                            {
                                new_matrix[i][j].gen = 0;
                                new_matrix[i][j].food = 0;
                            }
                            else
                                new_matrix[i][j].type = 0;
                        }
                        else
                        {

                            new_matrix[new_x][new_y] = matrix[i][j];
                            new_matrix[i][j].type = 0;

                            if (new_matrix[new_x][new_y].gen >= GEN_PROC_FOXES + 1)
                            {

                                new_matrix[i][j].type = 2;
                                new_matrix[i][j].gen = 0;
                                new_matrix[i][j].food = 0;
                                new_matrix[new_x][new_y].gen = 0;
                            }
                        }
#ifdef _OPENMP
                        omp_unset_lock(&locks[index]);
                        omp_unset_lock(&locks[index2]);
#endif
                    }
                }
                else if (matrix[i][j].food == GEN_FOOD_FOXES)
                {
                    int index = (i + j) % 100;
#ifdef _OPENMP
                    omp_set_lock(&locks[index]);
#endif
                    new_matrix[i][j].type = 0;
#ifdef _OPENMP
                    omp_unset_lock(&locks[index]);
#endif
                }
            }
        }
    }

    copy_matrix(matrix, new_matrix);

    free(new_rows);
    free(new_matrix);
}

void add_gen(Object **matrix)
{
    #pragma omp parallel for
    for (int i = 0; i < R; i++)
    {
        for (int j = 0; j < C; j++)
        {
            if (matrix[i][j].type == 1)
            {
                matrix[i][j].gen++;
            }
            else if (matrix[i][j].type == 2)
            {
                matrix[i][j].gen++;
                matrix[i][j].food++;
            }
        }
    }
}

void count_objects(Object **matrix)
{
    N = 0;
    for (int i = 0; i < R; i++)
    {
        for (int j = 0; j < C; j++)
        {
            if (matrix[i][j].type != 0)
                N++;
        }
    }
}

void print_gen_to_file(int g, Object **matrix, FILE *f)
{
    fprintf(f, "Generation %d", g);
    fprintf(f, "\n");
    for (int j = 0; j < C + 2; j++)
    {
        fprintf(f, "-");
    }
    fprintf(f, "   ");
    for (int j = 0; j < C + 2; j++)
    {
        fprintf(f, "-");
    }
    fprintf(f, " ");
    for (int j = 0; j < C + 2; j++)
    {
        fprintf(f, "-");
    }
    fprintf(f, "\n");

    for (int i = 0; i < R; i++)
    {
        fprintf(f, "|");
        for (int j = 0; j < C; j++)
        {
            char symbol[5];
            switch (matrix[i][j].type)
            {
            case 0:
                sprintf(symbol, " ");
                break;
            case 1:
                sprintf(symbol, "R");
                break;
            case 2:
                sprintf(symbol, "F");
                break;
            case 3:
                sprintf(symbol, "*");
                break;

            default:
                sprintf(symbol, " ");
                break;
            }
            fprintf(f, "%s", symbol);
        }
        fprintf(f, "|   |");
        for (int j = 0; j < R; j++)
        {
            char symbol[5];
            switch (matrix[i][j].type)
            {
            case 0:
                sprintf(symbol, " ");
                break;
            case 1:
                sprintf(symbol, "%d", matrix[i][j].gen);
                break;
            case 2:
                sprintf(symbol, "%d", matrix[i][j].gen);
                break;
            case 3:
                sprintf(symbol, "*");
                break;

            default:
                sprintf(symbol, " ");
                break;
            }
            fprintf(f, "%s", symbol);
        }
        fprintf(f, "| |");
        for (int j = 0; j < R; j++)
        {
            char symbol[5];
            switch (matrix[i][j].type)
            {
            case 0:
                sprintf(symbol, " ");
                break;
            case 1:
                sprintf(symbol, "R");
                break;
            case 2:
                sprintf(symbol, "%d", matrix[i][j].food);
                break;
            case 3:
                sprintf(symbol, "*");
                break;

            default:
                sprintf(symbol, " ");
                break;
            }
            fprintf(f, "%s", symbol);
        }
        fprintf(f, "|\n");
    }

    for (int j = 0; j < C + 2; j++)
    {
        fprintf(f, "-");
    }
    fprintf(f, "   ");
    for (int j = 0; j < C + 2; j++)
    {
        fprintf(f, "-");
    }
    fprintf(f, " ");
    for (int j = 0; j < C + 2; j++)
    {
        fprintf(f, "-");
    }
    fprintf(f, "\n\n");
}

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("Usage: %s <filename>\n", argv[0]);
        return 1;
    }

    const char *filename = argv[1];
    Object *rows;
    Object **matrix;

    // read file
    FILE *file;
    char buffer[256];
    file = fopen(filename, "r");

    if (fgets(buffer, sizeof(buffer), file) != NULL)
    {
        sscanf(buffer, "%d %d %d %d %d %d %d",
               &GEN_PROC_RABBITS,
               &GEN_PROC_FOXES,
               &GEN_FOOD_FOXES,
               &N_GEN,
               &R,
               &C,
               &N);
    }

    rows = malloc(R * C * sizeof(Object));
    matrix = malloc(R * sizeof(Object *));
    for (int i = 0; i < R; i++)
    {
        matrix[i] = &rows[i * C];
        for (int j = 0; j < C; j++)
        {
            matrix[i][j].type = 0;
        }
    }

    while (fgets(buffer, sizeof(buffer), file) != NULL)
    {
        char type[10];
        int x, y;

        // Parse the object type and its coordinates
        if (sscanf(buffer, "%s %d %d", type, &x, &y) == 3)
        {
            if (strcmp(type, "RABBIT") == 0)
            {
                matrix[x][y].type = 1;
                matrix[x][y].gen = 0;
            }
            else if (strcmp(type, "FOX") == 0)
            {
                matrix[x][y].type = 2;
                matrix[x][y].gen = 0;
                matrix[x][y].food = 0;
            }
            else if (strcmp(type, "ROCK") == 0)
            {
                matrix[x][y].type = 3;
            }
        }
    }
    fclose(file);
#ifdef _OPENMP
    double start = omp_get_wtime();
    for (int i = 0; i < 100; i++)
    {
        omp_init_lock(&locks[i]);
    }
#endif

    // FILE *f;
    // f = fopen("gens", "w");
    // print_gen_to_file(current_gen, matrix, f);

    for (int i = 0; i < N_GEN; i++)
    {
        add_gen(matrix);
        move_rabbits(matrix);
        move_foxes(matrix);
        current_gen++;
        // print_gen_to_file(current_gen, matrix, f);
    }

    // fclose(f);
#ifdef _OPENMP
    for (int i = 0; i < 100; i++)
    {
        omp_destroy_lock(&locks[i]);
    }

    double end = omp_get_wtime();

    printf("time %f\n", end - start);
#endif

    count_objects(matrix);
    N_GEN = 0;
    store_output("output", matrix);
    free(matrix);
    free(rows);
}