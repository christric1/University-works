#include <iostream>
#include <vector>
#include <ctime>

using namespace std;

#define Max 2147483647

// �L�X�}�C
void PrintArr(vector<int> arr) {
    int size = arr.size();
    for (int i = 0; i < size; i++) {
        cout << arr[i] << " ";
    }
    cout << endl;
}

// �º��ƧǪk
void ShellSort(vector<int> arr){

    int size = arr.size();
    for (int gap = size / 2; gap > 0; gap /= 2)
    {
        for (int i = gap; i < size; i += 1)
        {
            int temp = arr[i];
            int j;
            for (j = i; j >= gap && arr[j - gap] > temp; j -= gap)
                arr[j] = arr[j - gap];
            arr[j] = temp;
        }
    }
}

// ���J�ƧǪk
void InsertSort(vector<int> arr) {

    int size = arr.size();
    for (int i = 1; i < size; i++) {
        int key = arr[i];
        int j = i - 1;
        while ( j >= 0 && key < arr[j] ) {
            arr[j + 1] = arr[j];
            j--;
        }
        arr[j + 1] = key;
    }
}

// ����
void swap(int* a, int* b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

// Partition
int Partition(vector<int> &arr, int front, int end) {
    int pivot = arr[end];
    int i = front - 1;
    for (int j = front; j < end; j++) {
        if (arr[j] < pivot) {
            i++;
            swap(&arr[i], &arr[j]);
        }
    }
    i++;
    swap(&arr[i], &arr[end]);
    return i;
}

// �ֳt�ƧǪk
void QuickSort_(vector<int>& arr, int front, int end) {
    if (front < end) {
        int pivot = Partition(arr, front, end);
        QuickSort_(arr, front, pivot - 1);
        QuickSort_(arr, pivot + 1, end);
    }
}

// �ֳt�ƧǪk �J�f
void QuickSort(vector<int> arr, int front, int end) {
    QuickSort_(arr, front, end);
}


// Merge
void Merge(vector<int> &arr, int front, int mid, int end) {

    vector<int> LeftSub(arr.begin() + front, arr.begin() + mid + 1),
        RightSub(arr.begin() + mid + 1, arr.begin() + end + 1);

    LeftSub.insert(LeftSub.end(), Max);      // �bLeftSub[]���ݥ[�J�Ȭ� Max ������
    RightSub.insert(RightSub.end(), Max);    // �bRightSub[]���ݥ[�J�Ȭ� Max ������

    int idxLeft = 0, idxRight = 0;

    for (int i = front; i <= end; i++) {

        if (LeftSub[idxLeft] <= RightSub[idxRight]) {
            arr[i] = LeftSub[idxLeft];
            idxLeft++;
        }
        else {
            arr[i] = RightSub[idxRight];
            idxRight++;
        }
    }
};

// �X�ֱƧǪk
void MergeSort_(vector<int>& arr, int front, int end) {

    if (front < end) {
        int mid = (front + end) / 2;
        MergeSort_(arr, front, mid);
        MergeSort_(arr, mid + 1, end);
        Merge(arr, front, mid, end);
    }
}

// �X�ֱƧǪk �J�f
void MergeSort(vector<int> arr, int front, int end) {
    MergeSort_(arr, front, end);
}

// Heap �վ�
void MaxHeapify(vector<int>& arr, int i, int size_)
{
    int largest, l = (2 * i) + 1, r = l + 1;

    if (l < size_ && arr[l] > arr[i])
        largest = l;
    else
        largest = i;

    if (r < size_ && arr[r] > arr[largest])
        largest = r;

    if (largest != i)
    {
        std::swap(arr[i], arr[largest]);
        MaxHeapify(arr, largest, size_);
    }
}

// �إ� Heap
void BuildMaxHeap(vector<int>& arr)
{
    for (int i = (arr.size() / 2); i >= 0; i--)
        MaxHeapify(arr, i, arr.size());
}

// ��n�ƧǪk
void HeapSort(vector<int> arr)
{
    BuildMaxHeap(arr);
    int sz = arr.size();
    for (int i = arr.size() - 1; i > 0; i--)
    {
        swap(arr[0], arr[i]);
        sz--;
        MaxHeapify(arr, 0, sz);
    }
}

