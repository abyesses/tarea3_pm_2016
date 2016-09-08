//
//  main.c
//  act3pm
//
//  Created by Abraham Esses on 9/3/16.
//  Copyright © 2016 Abraham Esses. All rights reserved.
//

#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <ctype.h>
#include <stdlib.h>
int num_filas_columnas;
int **matrix_a,**matrix_b,**matrix_c;
void llenaMatrices(int ** a,int **b){
    srand(time(NULL));
    int r;
    for (int i = 0; i < num_filas_columnas; i++) {
        for (int j = 0; j < num_filas_columnas; j++) {
            r = (rand() % (1000 + 1 - 10)) + 10;
            a[i][j] = b[i][j] = r;
        }
    }
    
}
void multiplicaMatrices(int ** a,int **b,int **c){
    for (int i=0; i < num_filas_columnas ; i++) {
        for (int j = 0; j < num_filas_columnas; j++) {
            c[i][j]=0;
            for (int k = 0; k < num_filas_columnas; k++) {
                c[i][j] += a[i][k] * b[k][j];
                //printf("c[%d][%d]: a[%d][%d]:%d * b[%d][%d]%d = %d\n",k+1,i+1,i+1,j+1,a[i][j],j+1,i+1,b[j][i],c[i][k]);
            }
        }
    }
}
void imprimeMatriz(int ** a){
    for (int i = 0; i < num_filas_columnas; i++) {
        for (int j = 0; j < num_filas_columnas; j++) {
            printf("|%d|",a[i][j]);
            if (j == num_filas_columnas-1) {
                printf("\n");
            }
        }
    }
}
void imprimeMatrizC( int **c){
    for (int i = 0; i < num_filas_columnas; i++) {
        for (int j = 0; j < num_filas_columnas; j++) {
            printf("|%d|",c[i][j]);
            if (j == num_filas_columnas-1) {
                printf("\n");
            }
        }
    }
}
int main(int argc, const char * argv[]) {
    clock_t a1,b1,b2,b3;
    double c_time,c_time2;
    int cc,index;
    while ((cc = getopt (argc, argv, "n:")) != -1)
        switch (cc)
    {
        case 'n':
            num_filas_columnas = atoi(optarg);
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
    
    printf ("numero = %d\n",num_filas_columnas);
    //num_filas_columnas = 5;
    a1 = clock();
    //Reservando memoria para apuntadores
    matrix_a = (int **) malloc(sizeof(int *) * num_filas_columnas);
    matrix_b = (int **) malloc(sizeof(int *) * num_filas_columnas);
    matrix_c = (int **) malloc(sizeof(int *) * num_filas_columnas);
    //Reservando memoria para filas
    for (int i = 0; i < num_filas_columnas ; i++) {
        matrix_a[i] = (int *) malloc(num_filas_columnas * sizeof(int));
        matrix_b[i] = (int *) malloc(num_filas_columnas * sizeof(int));
        matrix_c[i] = (int *) malloc(num_filas_columnas * sizeof(int));
    }
    b1 = clock();
   
    llenaMatrices(matrix_a,matrix_b);
    multiplicaMatrices(matrix_a,matrix_b,matrix_c);
    //imprimeMatrizC(matrix_c);
    b2 = clock();
    
    //Liberando memoria
    for (int i = 0; i < num_filas_columnas ; i++) {
        free(matrix_a[i]);
        free(matrix_b[i]);
        free(matrix_c[i]);
    }
    free(matrix_a);
    free(matrix_b);
    free(matrix_c);
    b3 = clock();
    c_time = (double)(b1-a1)/ CLOCKS_PER_SEC;
    printf("Tiempo de reserva de memoria: %f s\n",c_time);
    c_time2 = (double)(b2-a1)/ CLOCKS_PER_SEC;
    printf("Tiempo de procesamiento: %f s\n",c_time2);
    c_time2 = (double)(b3-b2)/ CLOCKS_PER_SEC;
    printf("Tiempo de liberacion: %f s\n",c_time2);
    c_time2 = (double)(b3-a1)/ CLOCKS_PER_SEC;
    printf("Tiempo de total: %f s\n",c_time2);
    //imprimeMatrizC(c);
    return 0;
}
