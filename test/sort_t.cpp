#include <stdio.h>
#include <cstdint>
#include "../sort.h"

void print_array(int arr[], int size)
{
    for(int i=0; i<size; i++)
    {
        printf("%d ", arr[i]);
    }
    printf("\n");
}


int main()
{
    int arr[5] = {1,5,3,2,4};
    //int arr[]={5,-3,10,12,-2,-5};
    //int arr[]={3,3,3,3,3};
    //int arr[]={INT32_MIN,INT32_MIN,INT32_MIN,INT32_MIN,INT32_MIN};
    //int arr[]={INT32_MAX,INT32_MAX,INT32_MAX,INT32_MAX,INT32_MAX}; 
    //int arr[]={INT32_MAX,3,5,INT32_MIN,INT32_MIN,INT32_MAX};
    //int arr[]={INT32_MAX,3,5,INT32_MAX,INT32_MIN};
    //int arr[]={1};
    //int arr[]={1,2};
    int len = sizeof(arr)/sizeof(int);
    Sort::quickSort(arr,0, len-1);
    print_array(arr,len);

    return 0;
}
