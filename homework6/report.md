# 41343124 
# 41343145

# DS_2 分組作業三

1. Adjacency Matrix（鄰接矩陣）
2. Adjacency List（鄰接串列）
3. Adjacency Multilist (鄰接多重串列）
 
## 解題說明

本題要求建立一個滿足 ADT 6.1 的 Graph 物件，並利用三種圖的表示方式實現

1. Adjacency Matrix（鄰接矩陣）
2. Adjacency List（鄰接串列）
3. Adjacency Multilist (鄰接多重串列）

<img width="676" height="713" alt="image" src="https://github.com/user-attachments/assets/ef856c24-181e-48c9-9298-aff1905852be" />


### 解題策略
整個程式的解題策略用「繼承 + 多型」實作三種 Graph 表示法並且使用同一套操作介面 ex. Insert Edge,Delete Edge,Degree.Display
1. 先建立：抽象父類別 Graph 因為三種圖形表示法都有共同功能,使所有 Graph都必須會插入 edge 但怎麼插入由子類別決定=polymorphism（多型）
2. 建立：三個子類別,且每個類別都使用 override 雖然功能相同,但資料結構不同,所以演算法也不同
    ```
   ex. int Degree(int u) const override 
   Matrix: 走訪整列
   List: linked list 長度。
   Multilist 走訪 edge chain。
    ```

3. 使用 Graph* g = nullptr; 用一個指標能操作所有 Graph 類型,讓使用者選擇不同表示方法,重點之後 g->in_edge() 不用管到底是哪種 graph 因為編譯器會透過 virtual function 自動找到真正函式
4. 最後個別實作三種表示方法
   
## 程式實作

```

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
