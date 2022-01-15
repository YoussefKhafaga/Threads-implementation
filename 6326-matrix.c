#include <stdio.h>
#include <stdlib.h>
#include<string.h>
#include<pthread.h>
#include<time.h>

int **arr1, **arr2, **arr, count = 0, sc = 0;

struct arraysdata {
    int i;
    int j;
    int col1;
    int col2;
};
typedef struct arraysdata Node;

void *computebyelement(void *Node);

void *computebyrow(void *Node);

void multiplicationbyelement(int row1, int col1, int col2);

void multiplicationbyrow(int row1, int col1, int col2);

void *computebyelement(void *node) {

    Node *node1 = (Node *) node;
    int i = node1->i;
    int j = node1->j;
    int col1 = node1->col1;
    for (int k = 0; k < col1; k++) {
        arr[i][j] += arr1[i][k] * arr2[k][j];
    }
}

void *computebyrow(void *node) {
    Node *node1 = (Node *) node;
    int i = node1->i;
    int col1 = node1->col1;
    int col2 = node1->col2;

    for (int j = 0; j < col2; j++) {
        arr[i][j] = 0;
        for (int k = 0; k <col1; k++) {
            arr[i][j] += arr1[i][k] * arr2[k][j];
        }

    }
}

void multiplicationbyrow(int row1, int col1, int col2) {
    count = 0,sc =0;
    Node *nodes = malloc(sizeof(Node) * row1);
    pthread_t *thread = malloc(sizeof(pthread_t) * row1);

    for (int i = 0; i < row1; i++) {
        nodes[sc].i = i;
        nodes[sc].col1 =col1;
        nodes[sc].col2 = col2;
        pthread_create(&thread[count++], NULL, &computebyrow, (void *) &nodes[sc]);
        sc++;
    }

    for (int i = 0; i < count; i++) {
        pthread_join(thread[i], NULL);
    }
}

void multiplicationbyelement(int row1, int col1, int col2) {

    Node *nodes = malloc(sizeof(Node) * row1 * col2);
    pthread_t *thread = malloc(sizeof(pthread_t) * row1 * col2);
    for (int i = 0; i < row1; i++) {
        for (int j = 0; j < col2; j++) {
            arr[i][j] = 0;
            nodes[sc].col1 = col1;
            nodes[sc].i = i;
            nodes[sc].j = j;
            pthread_create(&thread[count++], NULL, &computebyelement, (void *) &nodes[sc]);
            sc++;
        }
    }
    for (int i = 0; i < count; i++) {
        pthread_join(thread[i], NULL);
    }
}

int main() {
    char *file = malloc(20);
    printf("please enter file name : ");
    scanf("%s", file);
    FILE *f = fopen(file, "r");
    int i, j, row1, col1, row2, col2;
    fscanf(f, "%d", &row1);
    fscanf(f, "%d", &col1);
    arr1 = (int **) malloc(sizeof(int *) * row1);
    for (i = 0; i < row1; i++)
        arr1[i] = (int *) malloc(sizeof(int) * col1);
    for (i = 0; i < row1; i++) {
        for (j = 0; j < col1; j++) {
            fscanf(f, "%d", &arr1[i][j]);
        }
    }
    fscanf(f, "%d", &row2);
    fscanf(f, "%d", &col2);
    if (col1 != row2)
        printf("cannot perform multiplication column of first array not equal row of second array\n");
    arr2 = malloc(sizeof(int *) * row2);
    for (i = 0; i < row2; i++)
        arr2[i] = malloc(sizeof(int) * col2);
    for (i = 0; i < row2; i++) {
        for (j = 0; j < col2; j++) {
            fscanf(f, "%d", &arr2[i][j]);
        }
    }
    arr = (int **) malloc(sizeof(int *) * row1);
    for (i = 0; i < row1; i++)
        arr[i] = (int *) malloc(sizeof(int) * col2);

    clock_t start1 = clock();
    multiplicationbyelement(row1, col1, col2);
    clock_t end1 = clock();

    printf("By Element\n");
	for (int i = 0; i < row1; i++) {
        for (int j = 0; j < col2; j++) {
            printf("%d ", arr[i][j]);
        }
        printf("\n");
    }
    printf("time taken %f\n", (double) (end1 - start1)/CLOCKS_PER_SEC);

    clock_t start2 = clock();
    multiplicationbyrow(row1, col1, col2);
    clock_t end2 = clock();
    printf("By Row:\n");
    for (i = 0; i < row1; i++) {
        for (j = 0; j < col2; j++) {
            printf("%d ", arr[i][j]);
        }
        printf("\n");
    }
    printf("time taken %f\n", (double) (end2 - start2)/CLOCKS_PER_SEC);
    return 0;
}
