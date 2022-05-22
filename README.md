## CS250-ALCO-Project-2
題目位置：https://hackmd.io/@wycchen/1102ALCO_project2   
Demo 影片位置：https://yzu365-my.sharepoint.com/:f:/g/personal/s1051336_mail_yzu_edu_tw/EgYAMBLaYShHgjGLCm7KolcBzdHQSDeF4rS4Eq9RbVM_NQ

## 如何執行
請下載 1051336_proj_2.cpp (此為高分版) 檔案，並利用 C++ compiler 進行編譯，然後執行，並可以利用 test.txt 內的測資進行測試。

## 程式說明
實作模擬 3-bit history predictor，各個函數的詳細說明可以參考註解。

## 重點功能說明
1. 在程式碼中第 9 行 #define BIT_SIZE 可以指定此程式 n-bit 的 history predictor，此範例預設為 3-bit history predictor   
2. 可以指定自由指定 entries 數量，如 2, 4, 8, 16, ... 等   
3. 可進行解讀的 instructions 為 add, addi, beq, bne, li

## 執行結果
![](https://github.com/chengchiehhung/CS250-ALCO-Project-2/blob/master/outcome/outcome_1.png)
![](https://github.com/chengchiehhung/CS250-ALCO-Project-2/blob/master/outcome/outcome_2.png)
![](https://github.com/chengchiehhung/CS250-ALCO-Project-2/blob/master/outcome/outcome_3.png)
