# 41343145

作業一

## 解題說明

本題要求實現一個Ackermann函數，利用遞迴計算 $A(m,n)$ 的答案。

### 解題策略

1. 使用遞迴函式讓程式可讀性提高
2. 透過 $Ackermann(m, n)$ 呼叫遞迴函式，最後輸出結果。  
3. 每次遞迴根據條件分支決定往更深層呼叫或是回傳結果。
4. 終止條件為 $m == 0$ 。

## 程式實作

以下為主要程式碼：

```#include <iostream>
using namespace std;
int Ackermann(int m,int n){//遞迴函式
	if (m == 0) return n + 1;
	else if (n == 0)return Ackermann(m - 1, 1);
	else return Ackermann(m - 1, Ackermann(m, n - 1));
}
int main() {
	int m, n;
	cout << "輸入Ackermann的m和n:";
	cin >> m>> n;//輸入Ackermann的m和n
	cout << Ackermann(m, n) << endl;//呼叫遞迴函式
	return 0;
}
```

## 效能分析

1. 時間複雜度：程式的時間複雜度遠大於 $2^n$。
2. 空間複雜度：空間複雜度為 $O(n)$。

## 測試與驗證

### 測試案例

| 測試案例 | 輸入參數 $m$ $n$ | 預期輸出 | 實際輸出 |
|----------|---------------|----------|----------|
| 測試一   | $m = 1$ $n = 1$     | 3        | 3        |
| 測試二   | $m = 2$ $n = 3$     | 9        | 9        |
| 測試三   | $m = 4$ $n = 2$     | 異常拋出  | 異常拋出  |

### 結論

1. 程式能正確計算 $Ackermann(m, n)$ 的答案。
2. $Ackermann$ 函數成長數度極快以至於過大數字程式無法處理。  

## 申論及開發報告

1. **為何使用遞迴**
   
除了題目本身需求外 $Ackermann$ 函數的定義本身是遞迴形式，因此以遞迴程式最直觀。 

2. **函式特性**  
$Ackermann$ 函數的成長極快，超過一定值（如 m ≥ 4, n ≥ 2）會導致遞迴過深而異位。

-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
作業二

## 解題說明

本題要求使用遞迴方式列出所有可能的子集合。

### 解題策略
使用一個 char 陣列 S[] 儲存輸入的集合元素。
使用另一個 char 陣列 subset[] 暫存目前遞迴過程中產生的子集合。
為了讓重複的元素相鄰，先用```sort(S, S + n)```排序，若在遞迴中若發現連續相同元素，利用```if (i > index && S[i] == S[i - 1]) continue;```跳過重複組合。
## 程式實作

以下為主要程式碼：

```#include<iostream>
#include <algorithm>//排序用
using namespace std;
void powerset(char S[], int n, int index, char subset[], int subsetSize) {//index指目前遞迴到第幾個元素，subsetSize表示目前子集合中有多少元素
    // 輸出目前子集合
    cout << "(";
    for (int i = 0; i < subsetSize; i++) {
        cout << subset[i];
    }
    cout << ")" << endl;
    for (int i = index; i < n; i++) {//如果有重複元素跳過
        if (i > index && S[i] == S[i - 1])continue;
        subset[subsetSize] = S[i];
        powerset(S, n, i + 1, subset, subsetSize + 1);
    }
}
int main() {
	int n;
	cout << "請輸入n:";
	cin >> n;
	cout << endl<< "請輸入元素:";
	char* S = new char[n];//建立集合陣列
    char* subset = new char[n];//暫存目前正在生成的子集合
	for (int i = 0;i < n;i++) {
		cin >> S[i];
	}
    sort(S, S + n);//將重複元素排一起
	cout << endl<< "冪集合為：" ;
    powerset(S, n, 0, subset, 0);
    delete[] S;
    delete[] subset;
	return 0;
}
```

## 效能分析

1. 時間複雜度：程式的時間複雜度為 $O(n*2^n)$。
2. 空間複雜度：空間複雜度為 $O(n)$。

## 測試與驗證

### 測試案例

測試一: $n = 3$  $abc$
輸出:()(a)(ab)(abc)(ac)(b)(bc)(c)

測試二: $n = 3$  $abb$
輸出:()(a)(ab)(abb)(b)(bb)

### 結論

1. 程式能正確排列出元素的冪集合。  
2. 在有相同元素時不會出現重複的答案。 

## 申論及開發報告

1. **選擇遞迴的原因**

遞迴能以簡潔的方式展現所有組合，程式結構清楚，邏輯自然。

2. **動態記憶體配置**  

因為剛開始需要輸入元素的個數所以使用動態記憶體配置以節省空間
