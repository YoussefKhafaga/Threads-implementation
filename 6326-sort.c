#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<pthread.h>

int *arr;
typedef struct Node Node;

struct Node
{
    int left;
    int right;
};


void *mergesort(void * Node1);

void merge(Node *Node1);


void * mergesort(void * Node1)
{
    Node * Node2 = (Node * )Node1;
    Node Node3;
    Node Node4;
    int mid;
    if (Node2->left < Node2->right)
    {
        mid = (Node2->left+Node2->right)/2;
        pthread_t thread1;
        pthread_t thread2;
        Node3.left = Node2->left;
        Node3.right = mid;
        pthread_create(&thread1,NULL,&mergesort,(void *) &Node3);
        pthread_join(thread1, NULL);
        Node4.left = mid+1;
        Node4.right = Node2->right;
        pthread_create(&thread2,NULL,&mergesort, (void *)&Node4);
        pthread_join(thread2, NULL);
        merge(Node2);
    }
    return NULL;
}

void merge(Node *Node1)
{
    int left = Node1->left;
    int right = Node1->right;
    int mid = (Node1->right+Node1->left)/2;
    int size1 = mid-left+1;
    int size2 = right-mid;
    int *arr1, *arr2;
    arr1 = malloc(sizeof(int)*size1);
    arr2 = malloc(sizeof(int)*size2);
    int i,j;

    for (i=0; i<size1; i++)
        arr1[i] = arr[left + i];


    for (i=0; i<size2; i++)
        arr2[i] = arr[mid + i + 1];


    int k=left;
    i = 0;
    j = 0;
    while(i<size1 && j<size2)
    {
        if(arr1[i]<=arr2[j])
        {
            arr[k++] = arr1[i++];
        }
        else
        {
            arr[k++] = arr2[j++];
        }
    }
    while(i<size1)
    {
        arr[k++] = arr1[i++];
    }
    while (j<size2)
    {
        arr[k++] = arr2[j++];
    }
}

int main()
{
    printf("enter file name : ");
    char *filename = malloc(100*sizeof(char));
    scanf("%s", filename);
    FILE *f = fopen(filename,"r");
    //taking size of array from input file
    int size;
    fscanf(f, "%d", &size);

    arr = (int*)malloc(size*sizeof(int));
    Node Node1;

    //reading array from file
    if(f)
    {
        for(int i = 0; i < size; i++)
        {
            fscanf(f, "%d", &arr[i]);
        }



    Node1.left = 0;
    Node1.right = size-1;
    mergesort((void*)&Node1);
    printf("merged array for size %d is \n",size);
    for(int i = 0; i < size; i++)
        printf("%d ", arr[i]);
    printf("\n");
    }
    return 0;
}
