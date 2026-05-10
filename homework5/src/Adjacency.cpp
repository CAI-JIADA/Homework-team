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

/*
# 41343124
# 41343145

DS_2 分組作業二

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

*/