# 41343124 
# 41343145

# DS_2 分組作業三
 
## 解題說明

1. 本題要求實作排序法：
   ```
   1. Insertion Sort
   2. Quick Sort (Median Of Three)
   3. Iterative Merge Sort
   4. Heap Sort
   ```   

2. 產生 Worst Case 測試資料
    ```
    1. Insertion Sort 最差情況： n, n-1, n-2, ..., 1
       例如： 10 9 8 7 6 5 4 3 2 1

    2. Merge Sort 設計能讓 Merge 過程最辛苦的資料
       例如： 1 3 5 7 2 4 6 8
       或利用遞迴方式產生 Worst Case

    3. Heap Sort 題目沒有明確 Worst Case 題目要求使用 "隨機排列產生器" (Random Permutation Generator) 產生許多隨機排列
       執行 Heap Sort 取最大時間當作 Worst Case

    4. Quick Sort 同 Heap Sort 使用大量 Random Permutations
       取最大執行時間近似 Worst Case 測試資料
   ```
    
3. 測資筆數
   ```
   n = 500 →
   1000 →
   2000 →
   3000 →
   4000 →
   5000
   ```
          
4. 預期時間複雜度
   ```
   Insertion Sort → n^2
   Quick Sort Worst Case → n^2
   Merge Sort → nlogn
   Heap Sort → nlogn
   ```
    
5. 預期輸出結果
   
<img width="986" height="319" alt="image" src="https://github.com/user-attachments/assets/66b31c98-a3fa-48c1-8b90-ab3d1972adfa" />
   

### 解題策略

功能：
   自動產生測試資料
   測量執行時間
   輸出 CSV 檔供 Excel 畫圖

本題的目標是比較不同排序演算法在不同資料規模下的效能差異，並透過實際測試分析各排序法的特性，因此本程式採用模組化設計，將資料產生、排序演算法、時間量測及結果輸出分別實作

1. 實作四種排序演算法

   首先依照作業要求完成四種排序演算法：
   Insertion Sort: 利用逐步插入的方式完成排序並將目前元素插入已排序區間中的正確位置
  
   Quick Sort（Median-of-Three）採用選擇 point 分割資料 
  
   Median-of-Three 遞迴排序左右子區間避免 point 選擇不佳造成效能下降，從左端 → 中間 → 右端，三個元素中選擇中位數作為 Pivot
  
   Iterative Merge Sort 採用非遞迴方式實作 Merge Sort 解法：
   ```
   1個元素合併
   ↓
   2個元素合併
   ↓
   4個元素合併
   ↓
   8個元素合併
   ↓
   直到完成排序 
   ```
   Heap Sort 利用 Binary Heap 建立 Max Heap 並取出最大值後重新調整 Heap 重複執行直到完成排序 

2. 建立測試資料

   Insertion Sort Worst Case 使用反向排列： n, n-1, n-2, ..., 1
   此資料會造成最大搬移次數。
   
   Quick Sort、Merge Sort、Heap Sort 使用隨機排列： random_shuffle(...) 產生測試資料
   模擬一般實際使用情況

3. 執行時間量測
   ```
   使用： chrono::high_resolution_clock 量測排序執行時間

   流程：

   開始計時
   ↓
   執行排序
   ↓
   結束計時
   ↓
   計算時間差
   ```
   輸出單位：毫秒(ms)
   
4. 結果輸出

   將所有測試結果輸出至： sorting_result.csv
   方便利用 Excel 繪製效能比較圖。


## 程式實作

```

#include <iostream>  
#include <vector> 
#include <algorithm>  
#include <chrono>// 計時功能
#include <fstream>// 檔案輸出
#include <cstdlib>// rand
#include <ctime>        

using namespace std;
using namespace chrono;

void in_sort(vector<int>& a){
    int n = a.size();// 取得陣列大小
    for (int i = 1; i < n; i++){
        int key = a[i];// 目前欲插入元素
        int j = i - 1;// 往前比較位置
        while (j >= 0 && a[j] > key){
            a[j + 1] = a[j];// 元素右移
            j--;
        }
        a[j + 1] = key;// 插入正確位置
    }
}


int Median_Of_Three(vector<int>& a, int left, int right){
    int ct = (left + right) / 2;// 找中間位置。

    if (a[left] > a[ct])
        swap(a[left], a[ct]);

    if (a[left] > a[right])
        swap(a[left], a[right]);

    if (a[ct] > a[right])
        swap(a[ct], a[right]);

    swap(a[ct], a[right - 1]);

    return a[right - 1];
}


void q_sort(vector<int>& a, int left, int right) {
    
    if (left + 10 <= right) {// 區間夠大才用 Quick Sort 10個以下改用 Insertion Sort
        int ptr = Median_Of_Three(a, left, right);
        int i = left;
        int j = right - 1;

        while (true) {
            while (a[++i] < ptr) {}
            while (a[--j] > ptr) {}

            if (i < j)
                swap(a[i], a[j]);
            else
                break;
        }

        swap(a[i], a[right - 1]);

        q_sort(a, left, i - 1);
        q_sort(a, i + 1, right);
    }else{
        for (int p = left + 1; p <= right; p++) {
            int t = a[p];
            int j;
            for (j = p; j > left && a[j - 1] > t; j--) {
                a[j] = a[j - 1];
            }
            a[j] = t;
        }
    }
}

void m_sort(vector<int>& a, vector<int>& t, int left, int mid, int right) {
    int i = left;
    int j = mid + 1;
    int k = left;
    while (i <= mid && j <= right) {
        if (a[i] <= a[j])// 誰小先放
            t[k++] = a[i++];
        else
            t[k++] = a[j++];
    }
    while (i <= mid)
        t[k++] = a[i++];

    while (j <= right)
        t[k++] = a[j++];

    for (int x = left; x <= right; x++) {
        a[x] = t[x];// 複製回原陣列
    }
}


void in_m_sort(vector<int>& a){
    int n = a.size();
    vector<int> t(n);// 建立暫存空間
    for (int cs = 1; cs < n; cs *= 2) {// 合併到整個陣列完成排序 ex. cs=1 合併 1個 + 1個......
        for (int left = 0; left < n - 1; left += 2 * cs) {
            int mid = min(left + cs - 1, n - 1);

            int right = min(left + 2 * cs - 1, n - 1);

            if (mid < right) {
                m_sort(a, t, left, mid, right);
            }
        }
    }
}

void heap_sort(vector<int>& a, int n, int i) {
    int root = i;

    int l_child = 2 * i + 1;

    int r_child = 2 * i + 2;

    if (l_child < n && a[l_child] > a[root]) {// 更新最大值
        root = l_child;
    }

    if (r_child < n && a[r_child] > a[root]) {
        root = r_child;
    }

    if (root != i) {// 若發現更大則交換
        swap(a[i], a[root]);

        heap_sort(a, n, root);
    }
}

void Heap(vector<int>& a) {
    int n = a.size();

    for (int i = n / 2 - 1; i >= 0; i--) {
        heap_sort(a, n, i);
    }

    for (int i = n - 1; i > 0; i--) {
        swap(a[0], a[i]);

        heap_sort(a, i, 0);
    }
}

vector<int> in_worst(int n) {// 生成最壞的插入比數
    vector<int> a(n);
    for (int i = 0; i < n; i++) {
        a[i] = n - i;// ex.產生 5 4 3 2 1 這是 Insertion Sort 最差情況
    }
    return a;
}

vector<int> in_rand(int n) {
    vector<int> a(n);
    for (int i = 0; i < n; i++){
        a[i] = i + 1;
    }
    random_shuffle(a.begin(), a.end());// #include <cstdlib> 內建的 rand 打亂順序
    return a;
}

// 此樣板 test 為測量某個排序演算法執行了多久在網路上找到的方法並非完全自行選寫
// 聲明!!
template<typename test>// test 可以代表任何函式型態 ex. InsertionSort , HeapSort ......
double test_time(test sort_test, vector<int> data) {// soort_test 要測量的排序函式 , vector<int> data 要排序的資料
    auto start = high_resolution_clock::now();// 開始計時

    sort_test(data);

    auto end = high_resolution_clock::now();// 結束計時

    return duration<double, milli> (end - start) .count();// duration<double, milli> 轉換成毫秒 , end - start 計算時間差 , .count() 把 duration 轉成數字
}

int main() {
    srand(time(NULL));// 亂數初始化

    vector<int> s = { 500,1000,2000,3000,4000,5000 };

    ofstream fout("sorting_result.csv");

    fout << "N," << "Insertion," << "Quick," << "Merge," << "Heap\n";

    cout << "排序測試開始\n";

    for (int n : s){
        cout << "測資筆數 N = " << n << endl;

        vector<int> in_data = in_worst(n);// 給 Insertion

        vector<int> rand_data = in_rand(n);// 給其他排序

        double insertion_time = test_time(in_sort, in_data);

        double quick_time = test_time([](vector<int>& a) {q_sort(a, 0, a.size() - 1); }, rand_data);

        double merge_time = test_time(in_m_sort, rand_data);

        double heap_time = test_time(Heap, rand_data);

        fout << n << "," << insertion_time << "," << quick_time << "," << merge_time << "," << heap_time << "\n";
    }

    fout.close();// 關閉檔案

    cout<< "\nCSV File Created : "<< "sorting_result.csv"<< endl;

    return 0;
}
```

## 效能分析
```
1. Insertion Sort
   理論分析: Insertion Sort 透過逐一插入元素完成排序
   時間複雜度： O(n^2)
   最差情況發生於資料完全反向排列時： 5000 4999 4998 ... 1 此時每個元素都必須與前面所有元素比較並搬移。

   實際分析:
   當資料量增加時 n	成長趨勢
   500 → 1000	約增加 4 倍
   1000 → 2000	約增加 4 倍
   2000 → 4000	約增加 4 倍
   因為 T(n)=n^2 資料量增加 2 倍時，執行時間理論上會增加約 4 倍

   優點:
     1. 程式簡單
     2. 幾乎不需要額外記憶體
     3. 小型資料表現良好
   缺點:
     1. 大型資料效率極差
     2. 不適合大量資料排序

2. Quick Sort（Median-of-Three）
   理論分析:
   平均時間複雜度： O(nlogn)
   最差情況： O(n^2)

   實際分析:
   在隨機資料下執行速度通常最快成長曲線較平緩遠優於 Insertion Sort
   原因：每次分割後可快速縮小問題規模

   優點:
     1. 平均效率最佳
     2. 記憶體需求低
     3. 適合大型資料
   缺點:
     1. 最差情況仍可能退化
     2. 遞迴深度過深可能增加負擔

3. Iterative Merge Sort
   理論分析:
   本作業使用非遞迴版本，不論最佳、平均或最差情況皆相同
   時間複雜度： O(nlogn)

   實際分析:
   執行時間非常穩定，即使資料型態改變差異都不大

   優點:
     1. 效能穩定
     2. Stable Sort
     3. 不容易出現最差情況
   缺點:
     → 需要額外空間 O(n) 用於暫存合併結果

4. Heap Sort
   理論分析:
   Heap Sort 利用建立 Max Heap 取出最大值並重新整理 Heap
   時間複雜度： O(nlogn)

   實際分析:
   效能通常介於 Quick Sort 與 Merge Sort 之間，雖然理論複雜度與 Merge Sort 相同，但 Heapify 的交換次數較多，因此實際速度通常略慢

   優點:
     1. 不需額外記憶體
     2. 最差情況仍維持 O(n log n)
   缺點:
     1. 常數因子較大
     2. 實際速度通常不如 Quick Sort
```
# 四種排序法比較

|  排序法  |  Best Case  |  Average Case  |  Worst Case  |  額外空間  |  是否穩定  |  主要特點  |
|----------|-------------|----------|----------|----------|----------|----------|
| Insertion Sort | O(n) | O(n²) | O(n²) | O(1) |  Stable | 適合小型資料，實作簡單 |
| Quick Sort (Median-of-Three) | O(nlogn) | O(n log n) | O(n²) | O(log n) |  Unstable | 平均速度最快，實務應用廣泛 |
| Merge Sort | O(nlogn) | O(n log n) | O(n log n) | O(n) |  Stable | 效能穩定，不受資料排列影響 |
| Heap Sort | O(nlogn) | O(n log n) | O(n log n) | O(1) |  Unstable | 不需額外記憶體，最差情況仍維持高效率 |

## 測試與驗證

### 測試案例

| 測試案例 | 輸入參數 | 預期輸出 | 實際輸出 |
|----------|---------------|----------|----------|
| 測試一：AM 新增與顯示 | 選擇 `1`（AM）<br>Vertex=`4`<br>Insert Edge：`0 1`、`0 2`、`1 3`<br>Display | `Adjacency Matrix`<br>`0 1 1 0`<br>`1 0 0 1`<br>`1 0 0 0`<br>`0 1 0 0` | 與預期相同 |
| 測試二：AL Degree 與 Edge Check | 選擇 `2`（AL）<br>Vertex=`4`<br>Insert Edge：`0 1`、`0 2`、`2 3`<br>Degree(0)<br>Check Edge(0,2) | `Degree = 2`<br>`Edge exists.` | 與預期相同 |
| 測試三：AML 顯示 Multilist | 選擇 `3`（AML）<br>Vertex=`4`<br>Insert Edge：`0 1`、`0 2`、`1 2`<br>Display | `---------- Edge Nodes ----------`<br>`N0 [ 0 1 0 0 ] edge(0,1)`<br>`N1 [ 0 2 N0 0 ] edge(0,2)`<br>`N2 [ 1 2 N0 N1 ] edge(1,2)`<br><br>`---------- Vertex Lists ----------`<br>`vertex 0 : N1 -> N0`<br>`vertex 1 : N2 -> N0`<br>`vertex 2 : N2 -> N1` | 與預期相同 |

### 輸出結果
<img width="354" height="180" alt="image" src="https://github.com/user-attachments/assets/7ac2ffe4-ca5c-466d-ad4d-eab53ab149f8" />

| cpu | 記憶體 | 輸出資料 | 
|----------|---------------|----------|
| i5-13420H | ddr5 5200MHz  |  <img width="361" height="156" alt="image" src="https://github.com/user-attachments/assets/e676afa1-060c-4fc4-aa7f-792eea3e8144" />|
| i7-12700 | ddr4 2666MHz | <img width="363" height="156" alt="image" src="https://github.com/user-attachments/assets/ded55819-8463-4dc3-8a39-0d610f9960f2" />|
| i7-12700 | ddr5 5600MHz | <img width="360" height="154" alt="image" src="https://github.com/user-attachments/assets/f7645a9b-15d7-42a2-a225-e644ee21e618" />|

### 結論
```
第一部分的作業成功利用 C++ 的：

Inheritance（繼承）
Polymorphism（多型）
Virtual Function（虛擬函式）
Override（覆寫）

實作三種 Graph 表示法：

Adjacency Matrix
Adjacency List
Adjacency Multilist

並透過抽象父類別 Graph 統一操作介面，使不同 Graph 表示法皆能使用相同功能

ex.
Insert Edge
Delete Edge
Degree
Edge Check
Display

加以提升程式的可讀性/可維護性/可擴充性,在效能方面:

AM 適合 Dense Graph，查詢速度最快，但空間需求較大。
AL 適合 Sparse Graph，空間效率較佳，但搜尋 edge 需要 traversal。
AML 進一步降低 edge 儲存空間，每條 edge 只存一次，但結構與 traversal 較複雜。

其中 Adjacency Multilist（AML）為本作業最核心與最困難的部分，因為一條 edge 同時被兩個 vertex 共用traversal 必須依照不同方向走訪結構較為複雜

因此能更深入理解：

Pointer 操作
Linked Structure
Dynamic Memory Allocation
Graph Traversal

整體而言，本程式完整展示了 Graph ADT 的設計概念,三種 Graph Representation 的差異,並以不同資料結構在時間與空間效率上的取捨達成以繼承與多型方式實作 Graph Representation 的目標。 
```
## 申論及開發報告

Weighted Graph（加權圖）目前 edge 只有：存在 / 不存在,可延伸： weight
```
struct EdgeNode{

    int weight;
};
```
此應用可支援：

1. 最短路徑
2. 最小生成樹
3. AOV/AOE2等演算法
