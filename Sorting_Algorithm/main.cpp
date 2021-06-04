#include <iostream>
#include <cstdlib> /* �üƬ������ */
#include <ctime>   /* �ɶ�������� */
#include <vector>
#include <string>
#include "sorting.h"

using namespace std;

int rand_c() {
    // RAND31_MAX = 2147483647
    return ((rand() << 16) + (rand() << 1) + (rand() & 1));
}

int main() {

    srand(time_t(NULL));  // �üƺؤl

    clock_t start, end;
    double cpu_time_used;

    int arr_size[] = { 10000, 50000, 100000, 200000, 300000, 400000, 500000, 600000, 700000, 800000, 900000, 1000000 };

    vector<int> ivec[12];   // �ŧi12��vector�A�C�@�ӳ��i�H�sint
    
    for (int i = 0; i < 12; i++) {   // ���H���üƶ�R�}�C
        for (int j = 0; j < arr_size[i]; j++) {    
            ivec[i].push_back(rand_c());
        }
    }

    string peko[] = {"�@", "��", "�Q", "�G�Q", "�T�Q", "�|�Q", "���Q", "���Q", "�C�Q", "�K�Q", "�E�Q", "�@��"};
    string gura[] = { "InsertSort", "ShellSort", "QuickSort", "MergeSort", "HeapSort" };

    for (int i = 0; i < 12; i++) {
        cout << peko[i] << "�U��" << endl;

        for (int j = 0; j < 5; j++) {
            start = clock();    // �p��}�l�ɶ�

            switch (j)
            {
            case 0:
                InsertSort(ivec[i]);
            case 1:
                ShellSort(ivec[i]);
            case 2:
                QuickSort(ivec[i], 0, ivec[i].size() - 1);
            case 3:
                MergeSort(ivec[i], 0, ivec[i].size() - 1);
            case 4:
                HeapSort(ivec[i]);
            default:
                break;
            }
            end = clock();      // �p�⵲���ɶ�
            cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;
            cout << gura[j] << ": " << cpu_time_used << " secs" << endl;
        }
        cout << endl;

    }
    return 0;
    
}
