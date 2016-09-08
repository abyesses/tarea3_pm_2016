//
//  main.c
//  act3pm_pthreads
//
//  Created by Abraham Esses on 9/7/16.
//  Copyright © 2016 Abraham Esses. All rights reserved.
//

#include <stdio.h>
#include <time.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <semaphore.h>
#include <ctype.h>
#define N 2
#define hilos 100
#define mil 1000

//Se generan arreglos
typedef struct {
    int inicio;
    int fin;
    int id;
}thread_content;
int ** matrix_a, ** matrix_b,** matrix_c;
sem_t matriz;
sem_t matriz_mul;
int num;
void * multiplyMatrix(void *);
void * fillMatrix(void *);
void * fillMatrix(void * hilo){
    srand(time(NULL));
    int r;
    for (int i = ((thread_content *) hilo)->inicio ; i < ((thread_content *) hilo)->fin; i++) {
        for (int j = ((thread_content *) hilo)->inicio ; j < ((thread_content *) hilo)->fin; j++) {
            r = (rand() % (1000 + 1 - 10)) + 10;
            sem_wait(&matriz);
            matrix_a[i][j] = matrix_b[i][j] = r;
            sem_post(&matriz);
        }
    }
    pthread_exit(NULL);
}
void * multiplyMatrix(void * hilo){
    for (int i = ((thread_content *) hilo)->inicio ; i < ((thread_content *) hilo)->fin; i++) {
        for (int j = ((thread_content *) hilo)->inicio; j < ((thread_content *) hilo)->fin; j++) {
            matrix_c[i][j]=0;
            for (int k =((thread_content *) hilo)->inicio ; k < ((thread_content *) hilo)->fin; k++) {
                sem_wait(&matriz_mul);
                matrix_c[i][j] += matrix_a[i][k] * matrix_b[k][j];
                sem_post(&matriz_mul);
                //printf("c[%d][%d]: a[%d][%d]:%d * b[%d][%d]%d = %d\n",k+1,i+1,i+1,j+1,a[i][j],j+1,i+1,b[j][i],c[i][k]);
            }
        }
    }
    pthread_exit(NULL);
}
int main(int argc, const char * argv[]) {
    clock_t alloc_memory_begin,alloc_memory_end,threads_creation_begin,threads_creation_end,free_memory_begin,free_memory_end;
    double total,free_memory,allocation,threads_creation;
    sem_init(&matriz, 1, 2);
    sem_init(&matriz_mul, 1, 2);
    alloc_memory_begin = clock();
    int cc,index;
    while ((cc = getopt (argc, argv, "n:")) != -1)
        switch (cc)
    {
        case 'n':
            num = atoi(optarg);
            break;
        case '?':
            if ( optopt == 'n')
                fprintf (stderr, "Opción -%c requiere un argumento.\n", optopt);
            else if (isprint (optopt))
                fprintf (stderr, "Opción desconocida '-%c'.\n", optopt);
            else
                fprintf (stderr,
                         "Opción desconocida '\\x%x'.\n",
                         optopt);
            return 1;
        default:
            abort ();
    }
    
    
    for (index = optind; index < argc; index++)
        printf ("El argumento no es una opción válida %s\n", argv[index]);
    

    //Alloc memory for arrays
    matrix_a = (int **)malloc(num * sizeof(int *));
    matrix_b = (int **)malloc(num * sizeof(int *));
    matrix_c = (int **)malloc(num * sizeof(int *));
    
    for (int i = 0 ; i < N * mil; i++) {
        matrix_a[i] = (int *) malloc(num * sizeof(int));
        matrix_b[i] = (int *) malloc(num * sizeof(int));
        matrix_c[i] = (int *) malloc(num * sizeof(int));
    }
    //Reservando espacio para los hilos
    thread_content * ad = (thread_content *) malloc(sizeof(thread_content)*hilos);
    ad->inicio=0;
    ad->fin = ((num)/hilos)-1;
    alloc_memory_end = clock();
    threads_creation_begin = clock();
    
    pthread_t * threads = (pthread_t *) malloc(sizeof(pthread_t)*hilos);
    
    for (int k = 1; k < hilos ; k++) {
        if(k == hilos-1){
            (ad + k)->inicio = (ad + (k-1))->fin + 1;
            (ad + k)->fin = (((num)/hilos) - 1 + (ad + k)->inicio);
            printf("[%d]El thread %d computa de %d a %d\n",k,(ad+k)->id,(ad+k)->inicio,(ad+k)->fin);
        }
        else{
            (ad + k)->inicio = (ad + (k-1))->fin + 1;
            (ad + k)->fin = ((num)/hilos -1) + (ad + k)->inicio;
            printf("[%d]El thread %d computa de %d a %d\n",k, (ad+k)->id,(ad+k)->inicio,(ad+k)->fin);
        }
    }
    for (int i = 0; i < hilos ; i++) {
        (ad+i)->id = i;
        pthread_create(&threads[i], NULL, multiplyMatrix, (thread_content *)(ad+i));
    }
    threads_creation_end = clock();
    for (int j = 0; j < hilos; j++) {
        pthread_join(threads[j], NULL);
    }
    
    free_memory_begin=clock();
    for (int i = 0; i < num ; i++) {
        free(matrix_a[i]);
        free(matrix_b[i]);
        free(matrix_c[i]);
    }
    free(matrix_a);
    free(matrix_b);
    free(matrix_c);

    sem_destroy(&matriz);
    sem_destroy(&matriz_mul);
    free(threads);
    free(ad);
    free_memory_end = clock();
    allocation = (double)(alloc_memory_end - alloc_memory_begin)/ CLOCKS_PER_SEC;
    printf("Tiempo de reserva de memoria: %f s\n",allocation);
    threads_creation = (double)(threads_creation_end - threads_creation_begin)/ CLOCKS_PER_SEC;
    printf("Tiempo de creacion y procesamiento de hilos: %f s\n",threads_creation);
    threads_creation = (double)(threads_creation_end - threads_creation_begin)/ CLOCKS_PER_SEC;
    printf("Tiempo de creacion y procesamiento de hilos: %f s\n",threads_creation);
    free_memory = (double)(free_memory_end - free_memory_begin)/ CLOCKS_PER_SEC;
    printf("Tiempo de liberacion de memoria: %f s\n",free_memory);
    total = (double)(free_memory_end - alloc_memory_begin)/ CLOCKS_PER_SEC;
    printf("Tiempo de total: %f s\n",free_memory);
    return 0;
}
