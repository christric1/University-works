#include <iostream>
#include <stdlib.h>
#include "Board.h"

using namespace std;

int main(void) {

	FILE* pFile;
    char buffer[10];   // 足夠容納一行字的 buffer
    int Trob[5];
	errno_t err;  
	err = fopen_s(&pFile, "board.txt", "r");   // 檔案打開成功返回 0 ，失敗返回 1

    if (err != 0)  
    { 
        perror("檔案不存在");
    }

    else
    {
        Grid gr[256]; // 建立 256 格子
        int i = 0;
        cout << "開檔成功" << endl;

        Board bod;  // 建立棋盤

        while (fgets(buffer, 10, pFile))  // 用 fgets 一次讀進一行
        {
            Change(buffer, Trob);

            gr[i].Row = i / 16;
            gr[i].Column = i % 16;

            if (Trob[1] == 1)     // 牆壁
                gr[i].barrier[0] = true;
            if (Trob[2] == 1)
                gr[i].barrier[1] = true;
            if (Trob[3] == 1)
                gr[i].barrier[2] = true;
            if (Trob[4] == 1)
                gr[i].barrier[3] = true;
            
            if (Trob[0] == 6 || Trob[0] == 7 || Trob[0] == 14 || Trob[0] == 21) {   // 起點
                gr[i].SetAttribute(Origin);
                gr[i].SetColor(DetermineColor(Trob[0]));

                bod.SetOriginPointer(gr[i]);  // 設定起點指標
            }

            else if (Trob[0] != 0) {   // 終點
                gr[i].SetAttribute(End);
                gr[i].SetColor(DetermineColor(Trob[0]));

                bod.SetEndPointer(DetermineColor(Trob[0]), gr[i]);  // 設定終點指標
            }

            bod.AddBoard(gr[i], i/16);  // 加入棋盤

            i++;
        }

        cout << "初始盤面已讀取完畢...請開始遊戲" << endl << endl;

        for (int j = 1; j <= 5; j++) {  // 五回合
            cout << "第" << j << "回合" << endl;
            cout << "請輸入終點 : ";
           
            int EndPoint;
            cin >> EndPoint;
            cout << endl;

            int arr[4] = { 119, 120, 135, 136 };
            for (int i = 0; i < 4; i++) {     // 設定中央終點
                gr[arr[i]].SetAttribute(End);
                gr[arr[i]].SetColor(DetermineColor(EndPoint));
                bod.Bo[arr[i] / 16].at(arr[i] % 16) = gr[arr[i]];
                bod.SetEndPointer(DetermineColor(EndPoint), gr[arr[i]]);  // 設定終點指標
            }

            int TotalStep = 0;
            cout << "Path : " << endl;

            for (int i = 0; i < bod.OriginPointer.size(); i++)     // 棋子位置指標設定
                bod.PresentPosition.push_back(bod.OriginPointer.at(i));

            for (int i = 0; i < bod.OriginPointer.size(); i++) {   // 開始起點巡迴

                Direction di;  // 方位
                Grid OriginGrid;  // 起點
                Grid PresentLocation;  // 目前位置

                OriginGrid = bod.OriginPointer.at(i);
                PresentLocation = OriginGrid;

                while (!bod.ArriveEndCheck(PresentLocation, OriginGrid.cr)) {
                    di = bod.GetDirection(PresentLocation);  // 得移動到方向
                    cout << enum_Color[OriginGrid.cr] << "  " << enum_Direction[di] << endl;

                    PresentLocation = bod.KeepGoingBro(di, PresentLocation, OriginGrid.cr);  //按照方位前進
                    bod.PresentPosition.at(i) = PresentLocation;
                    TotalStep++;
                }
            }
            cout << endl << "Total Step : " << TotalStep << endl << endl;
            cout << "------------------------------------------" << endl << endl;
        }

        bod.ClearAll();  // 初始化 &&　歸零
    }
       
    fclose(pFile);   // 關閉檔案
    return 0;
}