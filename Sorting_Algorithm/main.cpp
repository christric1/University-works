#include <iostream>
#include <cstdlib> /* 亂數相關函數 */
#include <ctime>   /* 時間相關函數 */
#include <vector>
#include <string>
#include "sorting.h"

using namespace std;

int rand_c() {
    // RAND31_MAX = 2147483647
    return ((rand() << 16) + (rand() << 1) + (rand() & 1));
}

int main() {

    srand(time_t(NULL));  // 亂數種子

    clock_t start, end;
    double cpu_time_used;

    int arr_size[] = { 10000, 50000, 100000, 200000, 300000, 400000, 500000, 600000, 700000, 800000, 900000, 1000000 };

    vector<int> ivec[12];   // 宣告12個vector，每一個都可以存int
    
    for (int i = 0; i < 12; i++) {   // 用隨機亂數填充陣列
        for (int j = 0; j < arr_size[i]; j++) {    
            ivec[i].push_back(rand_c());
        }
    }

    string peko[] = {"一", "五", "十", "二十", "三十", "四十", "五十", "六十", "七十", "八十", "九十", "一百"};
    string gura[] = { "InsertSort", "ShellSort", "QuickSort", "MergeSort", "HeapSort" };

    for (int i = 0; i < 12; i++) {
        cout << peko[i] << "萬筆" << endl;

        for (int j = 0; j < 5; j++) {
            start = clock();    // 計算開始時間

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
            end = clock();      // 計算結束時間
            cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;
            cout << gura[j] << ": " << cpu_time_used << " secs" << endl;
        }
        cout << endl;

    }
    return 0;
    
}
