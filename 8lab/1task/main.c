#include <math.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

const int M = 255;
typedef struct timeval timeval;

int thread_number;
char * distribution_type;
char * image_input;
char * output_file;

int **picture;
int height, width;
int **result;

struct thread_info {
    int begin;
    int additional_value;
    int id;
};

struct thread_info **threads_info;

void read_image(FILE* image)
{
    char str[10];
    fscanf(image, "%s", str);
    fscanf(image, "%d %d", &width, &height);
    fscanf(image, "%s", str);
    picture = (int**) calloc((size_t) height, sizeof(int *));
    for (int i = 0; i < height; i++) {
        picture[i] = (int*) calloc((size_t) width, sizeof(int));
    }
    for(int i = 0; i < height; i++)
        for(int j = 0; j < width; j++)
            picture[i][j] = 0;
    for(int i = 0; i < height; i++)
        for(int j = 0; j < width; j++)
            fscanf(image, "%d", &picture[i][j]);
}

void free_2D_array(int **array, int size)
{
    for(int i = 0; i < size; i++)
        free(array[i]);
    free(array);
}

void set_up_distribution()
{
    if(strcmp(distribution_type, "sign") == 0)
    {
        int section_size = (M + 1)/thread_number;
        int modulo = (M + 1)%thread_number;

        threads_info[0] = malloc(sizeof(struct thread_info));
        threads_info[0]->begin = 0;
        threads_info[0]->additional_value = modulo > 0 ? section_size + 1 : section_size;
        threads_info[0]->id = 0;
        for (int i = 1; i < thread_number; i++) {
            threads_info[i] = malloc(sizeof(struct thread_info));
            threads_info[i]->begin = threads_info[i - 1]->begin + threads_info[i - 1]->additional_value;
            threads_info[i]->additional_value = modulo > i ? section_size + 1 : section_size;
            threads_info[i]->id = i;
        }
    }
    else if(strcmp(distribution_type, "block") == 0)
    {
        for(int i = 0; i < thread_number; i++)
        {
            threads_info[i] = malloc(sizeof(struct thread_info));
            threads_info[i]->begin = (i) * (int)(ceil((width + 0.0) / thread_number));
            threads_info[i]->additional_value = (i+1) * (int)(ceil((width + 0.0) / thread_number));
            threads_info[i]->id = i;
        }

    } else if(strcmp(distribution_type, "interleaved") == 0)
    {
        for(int i = 0; i < thread_number; i++)
        {
            threads_info[i] = malloc(sizeof(struct thread_info));
            threads_info[i]->begin = i;
            threads_info[i]->additional_value = thread_number;
            threads_info[i]->id = i;
        }

    }else {
        printf("Sposoby podzialu: sign / block / interleaved\n");
        free_2D_array(picture, height);
        exit(-1);
    }
}


void* sign(void* info)
{
    clock_t t;
    t = clock();
    struct thread_info *thread_info = (struct thread_info *) info;

    for(int i = 0; i < width; i++)
    {
        for(int j = 0; j < height; j++)
        {
            if(picture[j][i] >= thread_info->begin && picture[j][i] < thread_info->additional_value + thread_info->begin)
                result[thread_info->id][picture[j][i]]++;
        }
    }
    double* time = malloc(sizeof(double));
    (*time) = (((double)clock() - t)/CLOCKS_PER_SEC);
    return time;
}

void* block(void* info)
{
    clock_t t;
    t = clock();
    struct thread_info *thread_info = (struct thread_info *) info;

    for(int i = thread_info->begin; i < thread_info->additional_value; i++)
    {
        for(int j = 0; j < height; j++)
        {
            result[thread_info->id][picture[j][i]]++;
        }
    }
    double* time = malloc(sizeof(double));
    (*time) = (((double)clock() - t)/CLOCKS_PER_SEC);
    return time;
}

void* interleaved(void* info)
{
    clock_t t;
    t = clock();
    struct thread_info *thread_info = (struct thread_info *) info;
    for(int i = thread_info->begin; i < width; i += thread_info->additional_value)
    {
        for(int j = 0; j < height; j++)
        {
            result[thread_info->id][picture[j][i]]++;
        }
    }
    double* time = malloc(sizeof(double));
    (*time) = (((double)clock() - t)/CLOCKS_PER_SEC);
    return time;
}

int main(int argc, char **argv) {
    if (argc != 5)
    {
        printf("Poprawna skladnia: <liczba watkow> <sposob podzialu> <plik_in> <plik_out>\n");
        return -1;
    }

    thread_number = atoi(argv[1]);
    distribution_type  = argv[2];
    image_input  = argv[3];
    output_file = argv[4];
    FILE* image = fopen(image_input, "r");
    FILE* output = fopen(output_file, "w");
    if(image == NULL)
    {
        printf("Nie udalo sie otworzyc zdjecia!\n");
        return -2;
    }
    if(output == NULL)
    {
        printf("Nie udalo sie otworzyc pliku wyjsciowego!\n");
        return -2;
    }

    read_image(image);
    clock_t t;
    t = clock();
    threads_info = malloc(thread_number * sizeof( struct thread_info *));
    set_up_distribution();
    pthread_t *thread = calloc((size_t) thread_number, sizeof(pthread_t));

    result = (int **)malloc(thread_number * sizeof(int *));
    for (int i = 0; i < thread_number; i++){
        result[i] = (int *)malloc(256 * sizeof(int));
        for(int j = 0; j < 256; j++)
            (result[i][j]) = 0;
    }

    void* function = NULL;
    if(strcmp(distribution_type, "sign") == 0)
    {
        function = sign;
    }
    else if(strcmp(distribution_type, "block") == 0)
    {
        function = block;
    } else if(strcmp(distribution_type, "interleaved") == 0)
    {
        function = interleaved;
    }

    for (int i = 0; i < thread_number; i++) {
        pthread_create(&thread[i], NULL, function, (void *) threads_info[i]);
    }

    for (int i = 0; i < thread_number; i++) {
        void* time = NULL;
        pthread_join(thread[i], &time);
        printf("WATEK ID: %lu CZAS: %f\n", thread[i], *(double *)time);
        free(time);
        free(threads_info[i]);
    }


    int final_result[256];
    for(int i = 0; i < 256; i++)
        final_result[i] = 0;
    for(int i = 0; i < thread_number; i++)
    {
        for(int j = 0; j < 256; j++)
        {
            final_result[j] += result[i][j];
        }
    }

    printf("CAŁKOWITY CZAS: %f\n", ((double)clock() - t)/CLOCKS_PER_SEC);
    for(int i = 0; i < 256; i++)
    {
        fprintf(output,"kolor nr %d, wystapil %d razy\n", i, final_result[i]);
    }

    fclose(image);
    fclose(output);
    free_2D_array(picture, height);
    free_2D_array(result, thread_number);

    return 0;
}