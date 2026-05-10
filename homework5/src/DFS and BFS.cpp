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
