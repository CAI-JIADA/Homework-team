# 41343124 
# 41343145

# DS_2 分組作業二

## 第一份

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
#include <iostream>
#include <vector>
#include <list>// 雙向 linked list 在 Adjacency List 會用到

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

using namespace std;

class Graph {// 新增父類別 Graph
protected:// 設為 protected 的用意讓子類別可以直接使用但外部不能直接存取
    int n;// 初始化節點
    int e;// 初始化邊

public:

    Graph(int cs = 0) {// 初始化
        n = cs;
        e = 0;
    }

    virtual ~Graph() {}// 釋放記憶體配置因為 *g 可能指向子類別且 virtual 非常重要若沒有 virtual 刪除時可能只刪父類別沒刪子類別

    bool IsEmpty() const {// 判斷圖是否為空
        return n == 0;
    }

    int NV() const {// 回傳節點數
        return n;
    }

    int NE() const {// 回傳邊數
        return e;
    }

    /*
      為何要使用純虛擬函式 virtual 因為 Graph 變成抽象類別
      ex.
      不能使用 Graph g;
      必須使用 MatrixGraph g;
    */

    virtual int DE(int u) const = 0;// Graph 不知道如何算 Degree 交給子類別完成

    virtual bool edge_check(int u, int v) const = 0;// 檢查 (u, v) 是否存在

    virtual void in_edge(int u, int v) = 0;// 加入新邊

    virtual void del_edge(int u, int v) = 0;// 刪除邊

    virtual void DP() const = 0;// 顯示目前的圖示 
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class AM : public Graph {// 新增繼承 Graph 的物件 Adjacency Matrix
private:
    vector<vector<int>> mx;// 建立二維陣列

public:// override 是什麼？在程式裡 override 的意思是覆寫(改寫)父類別的 virtual function」override = 覆寫 / 改寫 讓子類別重新定義父類別的函式

    AM(int cs = 0) : Graph(cs) {// 繼承 Graph 的初始化

        mx.resize(n, vector<int>(n, 0));// 建立 n×n 矩陣並全部初始化為 0
    }

    int DE(int u) const override {

        int t = 0;// 臨時暫存器 t

        for (int i = 0; i < n; i++) {// 走訪整列或是走訪要求的整個節點

            if (mx[u][i] == 1)// 有邊時 t+1
                t++;
        }

        return t;
    }

    bool edge_check(int u, int v) const override {

        return mx[u][v] == 1;// 確認等於1回傳 ture 非1則傳 false
    }


    void in_edge(int u, int v) override {// 新增邊

        if (!mx[u][v]) {

            mx[u][v] = 1;
            mx[v][u] = 1;

            e++;
        }
    }

    void del_edge(int u, int v) override {// 刪除邊

        if (mx[u][v] == 1) {

            mx[u][v] = 0;
            mx[v][u] = 0;

            e--;
        }
    }

    void DP() const override {// 顯示目前每個節點狀態

        cout << "\nAdjacency Matrix:\n";

        for (int i = 0; i < n; i++) {

            for (int j = 0; j < n; j++) {

                cout << mx[i][j] << " ";
            }

            cout << endl;
        }
    }
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class AL : public Graph {// 新增繼承 Graph 的物件 Adjacency List
private:
    vector<list<int>> A_L;// 設每個 vertex 有自己的 linked list。

public:

    AL(int cs = 0) : Graph(cs) {// 繼承 Graph 的初始化

        A_L.resize(n);// 建立 n 個 linked list
    }

    int DE(int u) const override {

        return A_L[u].size();// 直接回傳 linked list 長度
    }

    bool edge_check(int u, int v) const override {

        for (int t : A_L[u]) {// 搜尋

            if (t == v)
                return true;
        }

        return false;
    }

    void in_edge(int u, int v) override {

        if (!edge_check(u, v)) {

            A_L[u].push_back(v);// 將 u 加入 v
            A_L[v].push_back(u);// 將 v 加入 u

            e++;
        }
    }

    void del_edge(int u, int v) override {

        if (edge_check(u, v)) {

            A_L[u].remove(v);// 將 u 移除 v
            A_L[v].remove(u);// 將 v 移除 u

            e--;
        }
    }

    void DP() const override {// 顯示目前每個 linked list 狀態

        cout << "\nAdjacency List:\n";

        for (int i = 0; i < n; i++) {

            cout << i << " -> ";

            for (int t : A_L[i]) {

                cout << t << " ";
            }

            cout << endl;
        }
    }
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

struct EdgeNode {// 表示為一條 edge

    int c0;// edge 編號 ex.N0,N1,N2.......
    int c1=0,c2=0;// edge(c1,c2)

    EdgeNode* c1_link;// 指向 c1 的下一條路徑
    EdgeNode* c2_link;// 指向 c2 的下一條路徑

    EdgeNode(int n, int i, int j) {

        c0 = n;// 建立 edge 的 node
        c1 = i;
        c2 = j;

        c1_link = nullptr;// 初始化
        c2_link = nullptr;// 初始化
    }
};

class AML : public Graph {// 新增繼承 Graph 的物件 Adjacency Multilist

private:

    vector<EdgeNode*> F_edge;// 指向第一條相關 edge

    vector<EdgeNode*> A_edges;// 所有 edge node

public:

    AML(int cs = 0) : Graph(cs) {// 繼承 Graph 的初始化

        F_edge.resize(n, nullptr);// 建立 n 個 vertex
    }

    int DE(int u) const override {// 計算 u 的 degree

        int t = 0;// // 臨時暫存器 t

        EdgeNode* p = F_edge[u];// 從第一個位置開始

        while (p != nullptr) {// 直到指向空集合停止

            t++;

            if (p->c1 == u)// 判斷下一條 edge 如果再 c1 否則走 c2
                p = p->c1_link;
            else
                p = p->c2_link;
        }

        return t;
    }

    bool edge_check(int u, int v) const override {// 檢查 edge(u, v) 是否存在

        EdgeNode* p = F_edge[u];

        while (p != nullptr) {

            if ((p->c1 == u && p->c2 == v) ||(p->c1 == v && p->c2 == u))// 找到 edge(u, v)
                return true;

            if (p->c1 == u)
                p = p->c1_link;
            else
                p = p->c2_link;
        }

        return false;
    }

    void in_edge(int u, int v) override {

        if (edge_check(u, v)) {

            cout << "Edge already exists.\n";
            return;
        }

        EdgeNode* edge = new EdgeNode(e, u, v);// 建立 edge node。

        edge->c1_link = F_edge[u];// 新 edge 接到 u 原本串列前面
        edge->c2_link = F_edge[v];// 新 edge 接到 v 原本串列前面

        F_edge[u] = edge;
        F_edge[v] = edge;

        A_edges.push_back(edge);// 保存 edge

        e++;
    }

    void del_edge(int u, int v) override {

        cout << "沒做出來 \n";
    }

    void DP() const override {

        cout << "\n---------- Edge Nodes ----------\n";

        for (auto edge : A_edges) {// 走訪所有 edge node

            cout << "N" << edge->c0 << "  ";// ex. edge->c0 = 3 輸出 N3

            cout << "[ ";

            cout << edge->c1 << " ";
            cout << edge->c2 << " ";

            // ilink
            if (edge->c1_link != nullptr)
                cout << "N" << edge->c1_link->c0 << " ";
            else
                cout << "0 ";

            // jlink
            if (edge->c2_link != nullptr)
                cout << "N" << edge->c2_link->c0 << " ";
            else
                cout << "0 ";

            cout << "]";

            cout << "   edge(" << edge->c1 << "," << edge->c2 << ")";

            cout << endl;
        }

        cout << "\n---------- Vertex Lists ----------\n";

        for (int i = 0; i < n; i++) {

            cout << "vertex "
                << i
                << " : ";

            EdgeNode* p = F_edge[i];

            while (p != nullptr) {

                cout << "N"
                    << p->c0;

                bool hasNext = false;

                if (p->c1 == i && p->c1_link != nullptr)
                    hasNext = true;

                if (p->c2 == i && p->c2_link != nullptr)
                    hasNext = true;

                if (hasNext)
                    cout << " -> ";

                if (p->c1 == i)
                    p = p->c1_link;
                else
                    p = p->c2_link;
            }

            cout << endl;
        }
    }

    ~AML() {
        for (auto edge : A_edges)
            delete edge;
    }
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int main() {

    int cp;
    int vr;

    cout << "請選擇實作方法\n";
    cout << "1. Adjacency Matrix\n";
    cout << "2. Adjacency List\n";
    cout << "3. Adjacency Multilist\n";
    cout << "A: ";

    cin >> cp;

    cout << "輸入空間大小 ";
    cin >> vr;

    Graph* g = nullptr;// g 可以指向任何 Graph 的子類別物件

    if (cp == 1)// 選擇方法一
        g = new AM(vr);// 鄰接矩陣

    else if (cp == 2)// 選擇方法二
        g = new AL(vr);// 鄰接表

    else if (cp == 3)// 選擇方法三
        g = new AML(vr);// 鄰接多重列表

    int op;// 選擇變數

    do {

        cout << "\n選則延伸方法\n";

        cout << "1. Insert Edge\n";// 插入邊
        cout << "2. Delete Edge\n";// 刪除邊
        cout << "3. Check Edge\n";// 檢查邊的狀態
        cout << "4. Degree\n";// 度數
        cout << "5. Display\n";// 顯示整體目前狀態
        cout << "6. Number Of Vertices\n";// 顯示節點數
        cout << "7. Number Of Edges\n";// 顯示節點總邊數
        cout << "0. Exit\n";// 退出

        cin >> op;// 輸入選擇的方法

        int u = 0, v = 0;// 預設節點和邊

        switch (op) {

        case 1:

            cout << "Enter u v: ";
            cin >> u >> v;

            g->in_edge(u, v);

            break;

        case 2:

            cout << "Enter u v: ";
            cin >> u >> v;

            g->del_edge(u, v);

            break;

        case 3:

            cout << "Enter u v: ";
            cin >> u >> v;

            if (g->edge_check(u, v))
                cout << "Edge exists.\n";
            else
                cout << "Edge does not exist.\n";

            break;

        case 4:

            cout << "Enter vertex: ";
            cin >> u;

            cout << "Degree = "
                << g->DE(u) << endl;

            break;

        case 5:

            g->DP();

            break;

        case 6:

            cout << "Vertices = "
                << g->NV()
                << endl;

            break;

        case 7:

            cout << "Edges = "
                << g->NE()
                << endl;

            break;
        }

    } while (op != 0);

    delete g;

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

-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
## 第二份
1. Depth First Search(DFS)
2. Breadth First Search(BFS)
3. Connected Components
4. Spanning Trees
5. Biconnected Components

## 解題說明
本題要求實作出:
1. 繼承 Graph
2. DFS
3. BFS
4. Connected Components
5. Spanning Tree
6. Biconnected Components

### 解題策略
本題主要是使用 Adjacency List 建立無向圖(Undirected Graph)再利用 DFS/BFS/Tarjan DFS 分析圖的結構來實現無向圖的多種表示方法:
1. 先建立 Graph 抽象資料型態作為(ADT)這代表 Graph 只規定功能但不規定實作方式
2. 利用 Adjacency List 建立無向圖,不使用 Adjacency Matrix 是因為會浪費空間且 Adjacency List 空間複雜度：O(V + E)較有效率
3. DFS/BFS & Spanning Tree 針對這三者之間的關係我們使用在輸出 DFS/BFS 的同時輸出生成數以節省程式過長的問題,作方法如下
   ```
   void DFS(){
       標記目前節點已走訪
       走訪所有鄰居
       if(遇到未走訪節點){
           輸出 DFS(v)
           建立 Tree Edge
       }
       Recursive DFS()
   }
   // DFS 利用紀錄 Stack
   // BFS 同理使用 Queue 紀錄且利用逐層(level-order)搜尋,所以離起點最近的節點會先被搜尋
   ``` 
5. Connected Components 找出圖中彼此連通的群組 策略：
  1. 從每個未拜訪節點開始 DFS
  2. DFS 能走到的全部節點：屬於同一個 Component
  3. DFS 結束：代表找到一個完整 Connected Component
6. 使用 Tarjan Algorithm 來針對 DFS 目的是為了同時找到 Articulation Point/Biconnected Component
   ```
   Tarjan 核心概念為對每個點的紀錄：
   1. t1/disc[u]: DFS 第一次拜訪時間
   2. t2/low[u]: u 能透過 back edge 回到的最早祖先
   3. 更新 low 的策略 :

     Tree Edge// low[u] = min(low[u], low[v]);
        表示：子節點能回去的祖先/父節點也能回去

      Back Edge// low[u] = min(low[u], disc[v]);
        表示：發現回到祖先的捷徑
   ```
7. Articulation Point 有兩種方法可以達成
   1. Root AP 條件式 (t4[u] == -1 && ch > 1) 表示： root 有超過一個子節點且移除 root 時 DFS Tree 會裂成多塊
   2. Non-root AP 判斷式 (t3[v] >= t2[u]) 表示： 子節點無法回到 u 的祖先所以移除 u 子節點會斷掉
9. Biconnected Components 為針對任意兩點間至少有兩條不同路徑也就是不存在 articulation point 的區塊做分群表示

## 程式實作

```
#include <iostream>
#include <vector>
#include <list>
#include <stack>
#include <queue>

using namespace std;

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class Graph {// 新增父類別 Graph
protected:// 設為 protected 的用意讓子類別可以直接使用但外部不能直接存取
    int n;// 初始化節點
    vector<list<int>> t;// Adjacency list（鄰接串列）存放相連的所有節點 ex. 0 -> 1 2,1 -> 0 3

public:
    Graph(int ver) {
        n = ver;// 將節點數量存入 n
        t.resize(n);// 建立 n 個鄰接串列
    }

    virtual void Add_Edge(int u, int v) = 0;
    virtual void DFS(int s) = 0;
    virtual void BFS(int s) = 0;
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class U_Graph : public Graph {// 建立無向圖類別繼承 Graph

private:

    vector<bool> t1;// 紀錄節點是否被拜訪

    vector<int> t2;// 紀錄DFS 第一次拜訪時間(disc)
    vector<int> t3;// 能回到最上層根節點的紀錄(low)
    vector<int> t4;// 紀錄DFS Tree 的父節點

    vector<bool> A_t;// 紀錄是否為 articulation point

    stack<pair<int, int>> st;// 儲存邊(edge)的 stack

    vector<vector<pair<int, int>>> bcc;// 儲存所有 Biconnected Components

    int tm;// DFS 的計數器

    void U_DFS(int u) {// DFS 遞迴函式

        t1[u] = true;// 標記已走訪;

        for (int v : t[u]) {// 走訪所有鄰居 v 

            if (!t1[v]) {// 如果 v 尚未走訪

                cout << "DFS(" << v << ")" << endl;// 印出 DFS(v)
              
                cout << u << " - " << v << endl;// 印出 DFS tree 的 edge

                U_DFS(v);// 遞回呼叫
            }
        }
    }

    void C_DFS(int u) {// 用於 Connected Components 的 DFS

        t1[u] = true;// 標記已走訪;

        cout << u << " ";

        for (int v : t[u]) {

            if (!t1[v]) {

                C_DFS(v);// 繼續搜尋相連節點
            }
        }
    }

    void T_DFS(int u) {// 基於 Tarjan Algorithm DFS

        t2[u] = t3[u] = ++tm;// low 初始等於 disc

        t1[u] = true;

        int ch = 0;// 紀錄 DFS tree 的子節點數

        for (int v : t[u]) {

            if (!t1[v]) {// 發現 Tree Edge

                ch++;

                t4[v] = u;// 設定父節點

                st.push({ u, v });// edge 放入 stack

                T_DFS(v);// DFS 遞迴

                t3[u] = min(t3[u], t3[v]);// 更新 low 值

                if (t3[v] >= t2[u]) {// 如果發現一個 BCC

                    vector<pair<int, int>> cm;// 就建立一個 Component

                    pair<int, int> e;

                    do {

                        e = st.top(); //從 stack 取 edge

                        st.pop();

                        cm.push_back(e);// 將 e 存入 cm 的尾端

                    } while (!(e.first == u &&
                        e.second == v));

                    bcc.push_back(cm);// 將 cm 存入 bcc 的尾端
                }

                if (t4[u] == -1 && ch > 1)//如果 root 有超過一個 child 則是 articulation point

                    A_t[u] = true;

                if (t4[u] != -1 && t3[v] >= t2[u])// 非 root articulation point 的條件

                    A_t[u] = true;
            }

            else if (v != t4[u]) {// 發現 back edge

                t3[u] = min(t3[u], t2[v]);// 更新 low

                if (t2[v] < t2[u]) {// 避免重複 push edge

                    st.push({ u, v });// back edge 加入 stack
                }
            }
        }
    }

public:
    U_Graph(int ver): Graph(ver) {// 呼叫父類別建構子

        t1.resize(n);// 建立 t1 陣列
    }

    void Add_Edge(int u, int v) override {// 加入邊

        t[u].push_back(v);

        t[v].push_back(u);
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void DFS(int s) override {

        t1.assign(n, false);// 把 t1 陣列重新變成 n 個 false

        cout << "\nDFS Spanning Tree\n";

        cout << "DFS(" << s << ")" << endl;

        U_DFS(s);
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void BFS(int s) override {

        t1.assign(n, false);// 把 t1 陣列重新變成 n 個 false

        queue<int> q;

        q.push(s);// 起點加入 queue

        t1[s] = true;

        cout << "\nBFS Spanning Tree\n";

        cout << "BFS(" << s << ")" << endl;

        while (!q.empty()) {// queue 不空則繼續

            int u = q.front();// 取出 front

            q.pop();

            for (int v : t[u]) {//

                if (!t1[v]) {// 若未走訪

                    t1[v] = true;

                    cout << "BFS(" << v << ")" << endl;

                    cout << u << " - " << v << endl;

                    q.push(v);
                }
            }
        }
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void C_C() {// Connected Components

        t1.assign(n, false);// 把 t1 陣列重新變成 n 個 false

        cout << "\nConnected Components\n";

        int ct = 0;// 分群

        for (int i = 0; i < n; i++) {

            if (!t1[i]) {// 發現新 Component

                cout << "Component " << ++ct << ": ";

                C_DFS(i);

                cout << endl;
            }
        }
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    
    void ZT_DFS() {// Zero Tarjan

        t2.assign(n, 0);// disc 初始化

        t3.assign(n, 0);// low 初始化

        t4.assign(n, -1);// 父節點初始化

        A_t.assign(n, false);// articulation 初始化

        t1.assign(n, false);

        tm = 0;// 時間歸零

        while (!st.empty()) {

            st.pop();// 清空 stack
        }

        bcc.clear();// 清空 BCC
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    
    void RT_DFS() {// Run Tarjan

        ZT_DFS();

        for (int i = 0; i < n; i++) {// 走訪所有節點

            if (!t1[i]) {// 若尚未 DFS

                T_DFS(i);// 執行 Tarjan DFS
            }
        }

        if (!st.empty()) {// 若 stack 尚有 edge

            vector<pair<int, int>> cm;

            while (!st.empty()) {

                cm.push_back(st.top());

                st.pop();
            }

            bcc.push_back(cm);// 作為最後一個 BCC
        }
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void A_P() {

        RT_DFS();

        cout << "\nArticulation Points: ";

        for (int i = 0; i < n; i++) {

            if (A_t[i])// 若為 articulation point

                cout << i << " ";// 印出節點
        }

        cout << endl;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void BC_C() {

        RT_DFS();

        cout << "\nBiconnected Components\n";

        for (int i = 0; i < bcc.size(); i++) {// 走訪所有 BCC

            cout << "BCC " << i + 1 << ": ";

            for (auto& e : bcc[i]) {// 走訪 BCC 中每條 edge

                cout << "(" << e.first << "," << e.second << ") ";// 印 edge
            }

            cout << endl;
        }
    }
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int main() {

    int n, m;

    cout << "請輸入測資數量: ";

    cin >> n;

    U_Graph g(n);

    cout << "請輸入邊的數量: ";

    cin >> m;

    cout << "請輸入邊:\n";

    for (int i = 0; i < m; i++) {

        int u, v;

        cin >> u >> v;

        g.Add_Edge(u, v);
    }

    int s;// 起始點暫存器

    cout << "請輸入起始點: ";

    cin >> s;

    while (true) {

        cout << "\n-----請選擇功能-----\n";

        cout << "1 DFS\n";
        cout << "2 BFS\n";
        cout << "3 Connected Components\n";
        cout << "4 Articulation Point \n";
        cout << "5 Biconnected Components\n";
        cout << "0 Exit\n";

        cout << "輸入: ";

        int c;// 功能選擇

        cin >> c;

        if (c == 0)
            break;

        if (c == 1)

            g.DFS(s);

        else if (c == 2)

            g.BFS(s);

        else if (c == 3)

            g.C_C();

        else if (c == 4)

            g.A_P();

        else if (c == 5)

            g.BC_C();
    }

    return 0;
}
```

## 效能分析
```
首先本程式使用 Adjacency List 作為圖的儲存方式表示
空間複雜度共有 V 個 Vertex / E 條 Edge 無向圖,每條 edge 會存兩次所以空間需求： O(V + E)

1. DFS & Spanning Tree
   DFS 中：每個 Vertex 最多拜訪一次/每條 Edge 最多檢查一次

   時間複雜度: O(V + E)
   空間複雜度: 使用 vector<bool> t1 & Recursive Call Stack 最壞情況：O(V)

   DFS 特性：先深入到底再回溯適合,適合解
     Spanning Tree
     Connected Components
     Tarjan Algorithm

2. BFS & Spanning Tree
   BFS 中：每個 Vertex 進 queue 一次/每條 Edge 最多檢查一次

   時間複雜度: O(V + E)
   空間複雜度: Queue 最多存: O(V)

   BFS 特性：先利用 FIFO 的方法在會逐層(level-order)搜尋,適合解
     最短路徑（無權重圖）
     Level Traversal
     BFS Spanning Tree

3. Connected Components
   函式：void C_C()/void C_DFS(int u)
   方法:對每個尚未走訪的 Vertex 執行一次 DFS 且每次 DFS 找出一整個 Connected Component

   時間複雜度: 所有 Vertex 與 Edge 只會被 DFS 一次因此為: O(V + E)
   空間複雜度: 使用 visited array/recursion stack 因此為 O(V)

4. Articulation Point
   函式：void A_P()
   方法:實際上直接使用 Tarjan DFS 因此
   
   時間複雜度: O(V + E)
   空間複雜度: O(V)

5. Biconnected Components
   函式：void BC_C()
   方法:利用 stack<pair<int,int>> 保存 edge
   
   時間複雜度: 每條 edge push/pop 一次因此為: O(V + E)
   空間複雜度: 最壞情況所有 edge 都在 stack 中因此為: O(E)

```     
## 測試與驗證

### 測試案例
# 測試案例

| 測試案例 | 輸入參數 | 預期輸出 | 實際輸出 |
|----------|----------|----------|----------|
| 測試一：DFS Spanning Tree | 9 10<br> 0 1<br>1 2<br>1 3<br>2 4<br>3 4<br>3 5<br>5 6<br>5 7<br>6 7<br>7 8<br>0<br>1 | DFS Spanning Tree<br>DFS(0)<br>DFS(1)<br>0 - 1<br>DFS(2)<br>1 - 2<br>DFS(4)<br>2 - 4<br>DFS(3)<br>4 - 3<br>DFS(5)<br>3 - 5<br>DFS(6)<br>5 - 6<br>DFS(7)<br>6 - 7<br>DFS(8)<br>7 - 8 | 與預期輸出相同 |
| 測試二：BFS Spanning Tree | 9 10<br>0 1<br>1 2<br>1 3<br>2 4<br>3 4<br>3 5<br>5 6<br>5 7<br>6 7<br>7 8<br>0<br>2 | BFS Spanning Tree<br>BFS(0)<br>BFS(1)<br>0 - 1<br>BFS(2)<br>1 - 2<br>BFS(3)<br>1 - 3<br>BFS(4)<br>2 - 4<br>BFS(5)<br>3 - 5<br>BFS(6)<br>5 - 6<br>BFS(7)<br>5 - 7<br>BFS(8)<br>7 - 8  | 與預期輸出相同 |
| 測試三：Articulation Point 與 Biconnected Components | 9 10<br>0 1<br>1 2<br>1 3<br>2 4<br>3 4<br>3 5<br>5 6<br>5 7<br>6 7<br>7 8<br>0<br>4 | Articulation Points: 1 3 5 7 | 與預期輸出相同 |
| 測試四：Connected Components | 7 4<br>0 1<br>1 2<br>3 4<br>5 6<br>0<br>3 | Connected Components<br>Component 1: 0 1 2<br>Component 2: 3 4<br>Component 3: 5 6 | 與預期輸出相同 |
| 測試五：Biconnected Components | 9 10<br>0 1<br>1 2<br>1 3<br>2 4<br>3 4<br>3 5<br>5 6<br>5 7<br>6 7<br>7 8<br>0<br>5 | Biconnected Components<br>BCC 1: (7,8)<br>BCC 2: (6,7) (5,6) (5,7)<br>BCC 3: (3,5)<br>BCC 4: (4,3) (2,4) (1,2) (1,3)<br>BCC 5: (0,1) | 與預期輸出相同 |
### 結論
```
Adjacency List 作為圖形資料結構，降低空間使用量並提升 "走訪" 效率，同時透過 Graph 抽象類別（ADT）與繼承（Inheritance）設計，實現物件導向程式設計（OOP）的概念，使程式具有良好的模組化與擴充性。

在功能方面完成了：

DFS Spanning Tree
BFS Spanning Tree
Connected Components
Articulation Points
Biconnected Components

其中 DFS 與 BFS 分別利用遞迴與 Queue(FIFO) 建立 Spanning Tree，能清楚展示圖形走訪的過程，Connected Components 則能有效分析圖中的連通群組，而 Tarjan Algorithm 更進一步完成 Articulation Point 與 Biconnected Components 的分析，展現圖論中 low-link value 的核心應用。

在效能分析上，由於所有演算法皆僅需拜訪每個 Vertex 與 Edge 一次，因此時間複雜度皆可達到 O(V + E) 這已是 Graph Traversal 的最佳效率，使用 Adjacency List 也使空間複雜度維持在O(V + E) 適合處理 Sparse Graph。

透過本次實作，不僅加深了對 DFS、BFS 與 Tarjan Algorithm 的理解，也學習到如何利用 OOP 與 ADT 提升程式的可讀性、維護性與擴充能力，對圖論演算法與資料結構的整合應用具有相當大的幫助。
```
## 申論及開發報告

程式目前已完成無向圖的 DFS、BFS、Connected Components、Articulation Points 與 Biconnected Components 等功能，但仍有許多可以進一步擴充與研究的方向。<br>

1. 最短路徑演算法<br>
     目前 BFS 僅能處理：Unweighted Graph<br>
     未來可加入：Dijkstra Algorithm<br>
     適用：Weighted Graph（非負權重）<br>

2. Minimum Spanning Tree（MST）<br>
     目前只建立：DFS/BFS Spanning Tree 但不是 Minimum Spanning Tree<br>
     未來可加入：Prim Algorithm<br>
     利用：Priority Queue/Greedy Strategy 建立 MST。<br>
     搭配：Disjoint Set Union (Union-Find) 實作最小生成樹。<br>

3. Graph 儲存方式最佳化<br>
     目前使用：vector<list<int>> 雖然插入方便，但 list 的記憶體不連續 CPU Cache 效率較差。<br>
     未來可改成：vector<vector<int>><br>
     優點：<br>
     Cache locality 較佳<br>
     Traversal 更快<br>
     STL vector 存取效率較高<br>

4. Iterative DFS（非遞迴 DFS）<br>
     目前 DFS 使用： Recursive DFS 若圖太大可能產生 Stack Overflow<br>
     未來可改用： stack<int><br>
     實作： Iterative DFS 降低 recursion 對 stack memory 的依賴。<br>
-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
## 第三份
# Kruskal and Prim 

## 解題說明

本題要求建立一個類別 `Graph` 的物件，並實作兩種最經典的最小生成樹演算法：

1. **Kruskal's Algorithm**
2. **Prim's Algorithm**

### 解題策略
整個程式的解題策略採用「物件導向的繼承機制」將圖片中的抽象基底類別 `Graph` 與自訂的圖形資料結構融合，並透過參數傳遞來執行獨立的 MST 演算法：

1. 先建立`Graph`然後使用`MyGraph`並把原本在`main`函式裡的`edgeList`和`adj`當他的成員變數。
2. 在`main`讓使用者輸入資料，建立`MyGraph`物件，最後分別丟給`Prim`和`Kruskal`跑出答案。
3. 在跑`Prim`和`Kruskal`時要確定不會形成循環。

## 程式實作

```
#include <iostream>
#include <vector>
#include <algorithm>
#include <queue>
using namespace std;
class Graph
{
public:
	virtual ~Graph() {}
	bool IsEmpty() const { return n == 0; };
	int NumberOfVertices() const { return n; };//傳回圖中的頂點數
	int NumberOfEdges() const { return e; };//傳回圖中的邊數
	virtual int Degree(int u) const = 0;//傳回與頂點u關聯的邊數量
	virtual bool ExistsEdge(int u, int v) const = 0;//當圖存在邊時傳回t
	virtual void InsertVertex(int v) = 0;//將頂點v插入圖中 v沒有關聯邊
	virtual void InsertEdge(int u, int v) = 0;//將邊(u,v)插入圖
	virtual void DeleteVertex(int v) = 0;//刪除v及其所有關聯邊
	virtual void DeleteEdge(int u, int v) = 0;//從圖中刪除邊(u,v)
private:
	int n;//頂點數量
	int e;//邊數
public:
	Graph(int numV = 0) 
	{
		n = numV;
		e = 0;//一開始的邊一定是0
	}
protected:
	void setN(int numV) { n = numV; }//設定點數量
	void setE(int numE) { e = numE; }//設定邊數量
	void addE() { e++; }//邊的數量+1
	void delE() { e--; }//邊的數量-1
};


struct Edge//定義邊
{
	int u, v, weight;
};
struct PrimEdge
{
	int now, weight;//now=目前頂點編號
	bool operator>(const PrimEdge& other) const {
		return weight > other.weight;//讓每次pop時都能推出權重最小的邊
	}
};
bool Edges(Edge a, Edge b)//邊由小到大
{
	return a.weight < b.weight;
}
class UF//union-find
{
public:
	vector<int> p;//p=根節點
	UF(int n)//初始化
	{
		p.resize(n);
		for (int i = 0; i < n; i++) p[i] = i;
	}
	int find(int i)//找根節點
	{
		if (p[i] == i)return i;
		else return p[i] = find(p[i]);//掛在根節點下
	}
	bool merge(int i, int j)//合併兩個集合
	{
		int I = find(i);
		int J = find(j);
		if (I != J) {
			p[I] = J;//對方為根節點
			return true;//成功合併
		}
		else return false;//本來就同屬一個集合，會造成循環
	}
};


class MyGraph : public Graph//繼承Graph的衍生類別
{
public:
	vector<Edge> edgeList;
	vector<vector<pair<int, int>>> adj;
	// 建構子：初始化頂點數量，並同步呼叫基底類別的建構子
	MyGraph(int numV) : Graph(numV) {
		adj.resize(numV);
	}
	void InsertEdgeWithWeight(int u, int v, int w)//新增一個有權重的插入邊函式
	{
		edgeList.push_back({ u, v, w });
		adj[u].push_back({ v, w });
		adj[v].push_back({ u, w });
		addE();
	}
	//Graph實作
	virtual int Degree(int u) const override 
	{
		return adj[u].size();
	}
	virtual bool ExistsEdge(int u, int v) const override 
	{
		for (auto& edge : adj[u]) 
		{
			if (edge.first == v) return true;
		}
		return false;
	}
	virtual void InsertVertex(int v) override 
	{
		if (v >= NumberOfVertices()) //若插入的點超出範圍則擴展
		{
			setN(v + 1);
			adj.resize(v + 1);
		}
	}
	virtual void InsertEdge(int u, int v) override 
	{
		InsertEdgeWithWeight(u, v, 1); //預設權重=1
	}
	virtual void DeleteVertex(int v) override 
	{
		setE(NumberOfEdges() - adj[v].size());
		adj[v].clear();
	}
	virtual void DeleteEdge(int u, int v) override 
	{
		delE();
	}
};
//Kruskal
void Kruskal(int n, vector<Edge>& edges)
{
	sort(edges.begin(), edges.end(), Edges);//按照邊權重排序
	UF u(n);
	vector<Edge> mst;//儲存最小生成樹
	int mCost = 0;//用來存總權重
	for (Edge& e : edges) {
		if (u.merge(e.u, e.v)) {//確認會不會形成循環 會的話執行 不會的話跳過這條邊
			mst.push_back(e);//將邊加入最小生成樹
			mCost += e.weight;//把這條邊的權重加進總權重裡
		}
	}
	cout << "\nKruskal結果" << endl;
	cout << "總最小權重:" << mCost << endl;
	for (Edge& e : mst)
	{
		cout << e.u << " -- " << e.v << " == " << e.weight << endl;
	}
}
//Prim
void Prim(int n, int sNode, const vector<vector<pair<int, int>>>& adj)//sNode=起始點 第一層vector=圖中的所有頂點 第二層vector=該頂點可以到達的所有鄰居 pair<int, int>=<鄰居編號, 邊的權重>
{
	vector<bool> Lmst(n, false);//記錄哪些頂點已經被納入最小生成樹
	priority_queue<pair<int, pair<int, int>>, vector<pair<int, pair<int, int>>>, greater<pair<int, pair<int, int>>>> pq;//優先佇列：存儲 (權重, 目標節點, 來源節點)
	int mCost = 0;
	vector<Edge> mst;
	pq.push({ 0, {sNode, -1} });//從起點開始
	while (!pq.empty()) {//還有邊就繼續
		int w = pq.top().first;//w=最低的權重
		int u = pq.top().second.first;//u=目標點
		int pre = pq.top().second.second;//pre=來源點
		pq.pop();
		if (Lmst[u]) continue;//如果u已經在樹裡跳過此次 確保不會造成循環
		Lmst[u] = true;//把點加入最小生成樹
		mCost += w;//加上這條邊的權重
		if (pre != -1) mst.push_back({ pre, u, w });//只要不是起點就記錄這條邊
		for (auto& edge : adj[u]) //走訪鄰居
		{
			int v = edge.first;//鄰居
			int weight = edge.second;//到鄰居的距離
			if (!Lmst[v])
			{
				pq.push({ weight, {v, u} });
			}
		}
	}
	cout << "\nPrim結果" << endl;
	cout << "總最小權重:" << mCost << endl;
	for (Edge& e : mst) cout << e.u << " -- " << e.v << " == " << e.weight << endl;
}
int main() {
	int n, e, s;
	cout << "請輸入頂點數量:";
	cin >> n;
	cout << "請輸入邊的數量:";
	cin >> e;
	cout << "請輸Prim的起始點:";
	cin >> s;
	MyGraph g(n);
	cout << "請輸入每條邊的資訊[起點 終點 權重]:" << endl;
	for (int i = 0; i < e; i++) {
		int u, v, w;
		cout << "第" << i + 1 << " 條邊:";
		cin >> u >> v >> w;
		g.InsertEdgeWithWeight(u, v, w);//呼叫內部的插入函式
	}
	Kruskal(n, g.edgeList);
	Prim(n, s, g.adj);
	return 0;
}
```
## 效能分析

1. **Kruskal 演算法**
時間複雜度： $O(e log e)$
空間複雜度： $O(e)$
2. **Prim 演算法**
時間複雜度： $O(elogv)$
空間複雜度： $O(v + e)$

### 測試案例

| 測試案例 | 輸入參數  | 實際輸出Prim |實際輸出Kruskal |
|----------|---------------|----------|----------|
| 測試一   | <img width="359" height="270" alt="image" src="https://github.com/user-attachments/assets/62ec40d3-839a-4170-84d4-5d280ddfcc2f" /> | <img width="134" height="141" alt="image" src="https://github.com/user-attachments/assets/23e2549a-8680-421d-9782-71bace556d08" />| <img width="157" height="132" alt="image" src="https://github.com/user-attachments/assets/d0a187bd-4e3f-4309-8e95-9af34406e13c" />|
| 測試二   | <img width="328" height="220" alt="image" src="https://github.com/user-attachments/assets/ad8d8c8c-312b-4fdc-91dc-373693411dd1" />| <img width="138" height="130" alt="image" src="https://github.com/user-attachments/assets/efc32582-87f9-4511-9db3-924f756be190" />| <img width="139" height="119" alt="image" src="https://github.com/user-attachments/assets/5ef26b40-d22e-4adc-b9ac-86b2429d0d49" /> |

### 結論

1. 程式能以環狀串列正確表示多項式，並完成加減乘與代入計算。
2. 透過 $AddTerm$ 統一處理插入與合併，避免出現重複次方或0係數項。  

## 申論及開發報告

透過本次開發實驗可以發現，沒有絕對完美的圖形表示法，只有最適合該演算法的資料結構。本程式利用繼承，將兩種演算法各自需要的資料封裝在同一個`MyGraph`類別節省空間效率。
