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
    
5. 輸出結果
   
<img width="986" height="319" alt="image" src="https://github.com/user-attachments/assets/66b31c98-a3fa-48c1-8b90-ab3d1972adfa" />
   

### 解題策略

整個程式的解題策略用「繼承 + 多型」實作三種 Graph 表示法並且使用同一套操作介面 ex. Insert Edge,Delete Edge,Degree.Display
功能：
   自動產生測試資料
   測量執行時間
   輸出 CSV 檔供 Excel 畫圖
   
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
1. Adjacency Matrix 本身建立為 V×V 矩陣
   空間複雜度： 每個 vertex 都需要 V 個欄位所以總空間＝ O(V^2)
     優點：查詢超快
     缺點：浪費記憶體
   各操作效能：
     1. edge_check(u,v)　直接存取陣列
        時間複雜度：O(1)
     2. in_edge(u,v) 固定操作
        時間複雜度：O(1)
     3. del_edge(u,v)　固定操作
        時間複雜度：O(1)
     4. DE(u)　走訪整列
        時間複雜度：O(V)
     5. DP()雙層迴圈需要印 V^2 個元素
        時間複雜度：O(V^2)
        
2. Adjacency List 每個 vertex 擁有自己的 linked list 且每條 edge 需要存兩次因此
   總空間複雜度：O(V+E)
     優點：節省空間適合 Sparse Graph
     缺點: 查詢 edge 需要搜尋 linked list
   各操作效能：
     1. edge_check(u,v)　直接存取陣列
        時間複雜度：O(degree(u))
        最壞情況：O(V)
     2. in_edge(u,v) 先執行 edge_check() 再呼叫 push_back()
        時間複雜度：O(degree(u))
        最壞情況：O(V)
     3. del_edge(u,v)　使用 remove()需要走訪
        時間複雜度：O(degree(u)+degree(v))
        最壞情況：O(V)
     4. DE(u)　直接回傳 A_L[u].size()
        時間複雜度：O(1)
     5. DP()需要輸出所有 vertex/edge
        時間複雜度：O(V+E)
        
 3. Adjacency Multilist 每條 edge 建立一個 EdgeNode 並同時被兩個 vertex 共用且每條 edge 只存一次因此
   總空間複雜度：O(V+E)
     優點：最節省空間 edge 不重複存放
     缺點:結構複雜走訪與刪除較難實作
   各操作效能：
     1. edge_check(u,v)　需要沿著 edge chain 搜尋
        時間複雜度：O(degree(u))
        最壞情況：O(V)
     2. in_edge(u,v) 先執行 edge_check() 再建立新 edge
        時間複雜度：O(degree(u))
        最壞情況：O(V)
     3. del_edge(u,v)　目前程式尚未完成若完整實作需要：
        搜尋 edge
        修改 link
        更新 chain
        時間複雜度：O(degree(u)+degree(v))
     4. DE(u)　需要走訪
        時間複雜度：O(degree(u))
     5. DP()需要輸出所有 vertex/edge
        時間複雜度：O(V+E)
```     
## 測試與驗證

### 測試案例

| 測試案例 | 輸入參數 | 預期輸出 | 實際輸出 |
|----------|---------------|----------|----------|
| 測試一：AM 新增與顯示 | 選擇 `1`（AM）<br>Vertex=`4`<br>Insert Edge：`0 1`、`0 2`、`1 3`<br>Display | `Adjacency Matrix`<br>`0 1 1 0`<br>`1 0 0 1`<br>`1 0 0 0`<br>`0 1 0 0` | 與預期相同 |
| 測試二：AL Degree 與 Edge Check | 選擇 `2`（AL）<br>Vertex=`4`<br>Insert Edge：`0 1`、`0 2`、`2 3`<br>Degree(0)<br>Check Edge(0,2) | `Degree = 2`<br>`Edge exists.` | 與預期相同 |
| 測試三：AML 顯示 Multilist | 選擇 `3`（AML）<br>Vertex=`4`<br>Insert Edge：`0 1`、`0 2`、`1 2`<br>Display | `---------- Edge Nodes ----------`<br>`N0 [ 0 1 0 0 ] edge(0,1)`<br>`N1 [ 0 2 N0 0 ] edge(0,2)`<br>`N2 [ 1 2 N0 N1 ] edge(1,2)`<br><br>`---------- Vertex Lists ----------`<br>`vertex 0 : N1 -> N0`<br>`vertex 1 : N2 -> N0`<br>`vertex 2 : N2 -> N1` | 與預期相同 |

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
