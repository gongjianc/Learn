/*
** @file    QuickSort.cc
** @author  jaygong(gongjian0309@gmail.com)
** @date    2016-04-21 23:14:30
*/
 
#include <iostream>
#include <string.h>
 
using std::cout;
using std::endl;

int Partition(int A[], int low, int high)
{
    int pivot = A[low];
    while(low < high){
        while(low < high && A[high] >= pivot)
            --high;
        A[low] = A[high];
        while(low < high && A[low] <= pivot)
            ++low;
        A[high] = A[low];
    }
    A[low] = pivot;
    return low;
}

void QuickSort(int A[], int low, int high)
{
    if(low < high){
        int pivotpos = Partition(A, low, high);
        QuickSort(A, low, pivotpos - 1);
        QuickSort(A, pivotpos + 1, high);
    }
}

void printArrary(int A[], int length)
{
    for(int i = 0; i < length; ++i)
        cout << A[i] << " ";
    cout << endl;
}
int main(void)
{
    int A[8] = {3, 8, 7, 1, 2, 5, 6, 4};
    QuickSort(A, 0, 7);
    printArrary(A, 8);
    //int (*pf)(1024);
    return 0;
}
