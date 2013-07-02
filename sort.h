
class Sort
{
public:
    static void quickSort(int arr[], int begin, int end)
    {
        int p = partition(arr, begin, end);
        if(begin<p-1)
        {
            quickSort(arr, begin, p-1);
        }
        if(p+1<end)
        {
            quickSort(arr, p+1, end);
        }
        
    }
    static void swap(int &a, int &b)
    {
        int tmp = a;
        a = b;
        b = tmp;
    }

public:
    static int partition(int arr[], int begin, int end)
    {
        int m = arr[end];
        while(begin<end)
        {
            while(arr[begin]<=m)
            {
                if(begin>=end)
                {
                    break;
                }
                begin++;
            }
            if(begin<end)
            {
                arr[end] = arr[begin];
            }
            while(arr[end]>=m)
            {
                if(begin>=end)
                {
                    break;
                }
                end--;
            }
            if(begin<end)
            {
                arr[begin] = arr[end];
            }
        }
        arr[begin] = m;
        return begin;
    }
};
