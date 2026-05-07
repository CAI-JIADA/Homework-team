#include <iostream>
#include <vector>
#include <stack>
#include <algorithm>
using namespace std;
struct EdgeNode//邊節點
{
    int adjV;//鄰接頂點索引
    int weight;//邊的權重
    EdgeNode* next;//指向下一條邊
};
struct VertexNode//頂點節點
{
    int De;//多少邊到頂點(Degree)
    EdgeNode* fE;//第一條邊的指標
};
//AOV
bool TpSort(vector<VertexNode>& g, int n, vector<int>& sE) //拓撲排序 g是傳入資料 sE用來存節點
{
    stack<int> s;
    for (int i = 0; i < n; i++)//把所有邊數=0的頂點入堆疊
    {
        if (g[i].De == 0)s.push(i);
    }
    int count = 0;
    while (!s.empty()) 
    {
        int u = s.top();
        s.pop();
        sE.push_back(u); //記錄拓撲順序
        count++;
        for (EdgeNode* e = g[u].fE; e != nullptr; e = e->next)//走訪鄰居 Degree減1
        {
            int v = e->adjV;
            if (--g[v].De == 0) 
            {
                s.push(v);
            }
        }
    }
    //如果count< n表示圖中有循環會讓AOV無法排序
    return (count == n);
}
//AOE
void CPath(vector<VertexNode>& g, int n)//關鍵路徑 g=資料
{
    vector<int> sE;
    if (!TpSort(g, n, sE))
    {
        cout << "圖中有循環 無法計算" << endl;
        return;
    }
    vector<int> ee(n, 0);//最早發生時間
    for (int u : sE) //走訪sE的每一個元素 把當前走訪到的元素放在u
    {
        for (EdgeNode* e = g[u].fE; e != nullptr; e = e->next) 
        {
            int v = e->adjV;//v=鄰居節點
            if (ee[v] < ee[u] + e->weight) 
            {
                ee[v] = ee[u] + e->weight;//更新成更晚的時間點
            }
        }
    }//ee[]裡面存著每個節點的最早發生時間
    vector<int> le(n, ee[sE.back()]);//le=事件最晚發生時間
    for (int i = n - 1; i >= 0; i--)//從最後一個事件往回推算
    {
        int u = sE[i];//從拓撲排序容器的最後一個頂點開始拿
        for (EdgeNode* e = g[u].fE; e != nullptr; e = e->next) //檢查從u出發的所有邊 連向的鄰居是v
        {
            int v = e->adjV;
            if (le[u] > le[v] - e->weight) 
            {
                le[u] = le[v] - e->weight;
            }
        }
    }

    // 輸出關鍵活動 (當 E(e) == L(e))
    cout << "關鍵路徑上的活動:" << endl;
    for (int u = 0; u < n; u++) {
        for (EdgeNode* e = g[u].fE; e != nullptr; e = e->next) 
        {
            int v = e->adjV;
            int Ee = ee[u];//活動最早開始
            int Le = le[v] - e->weight;//活動最晚開始
            if (Ee == Le) {
                cout << "活動<" << u << ", " << v << ">是關鍵活動" << endl;
            }
        }
    }
}
void addEdge(vector<VertexNode>& g, int u, int v, int w)//新增一條有向邊
{
    EdgeNode* newNode = new EdgeNode;
    newNode->adjV = v;
    newNode->weight = w;
    newNode->next = g[u].fE; //將新邊插入在鏈結串列的開頭
    g[u].fE = newNode;
    g[v].De++; //更新終點節點的Degree
}
int main() {
    int n, e;
    cout << "頂點數量和邊的數量:";
    if (!(cin >> n >> e)) return 0;
    vector<VertexNode> g(n, { 0, nullptr });//初始化圖形 預設Degree=0，指標=nullptr
    cout << "每條邊的 起點 終點 權重:" << endl;
    for (int i = 0; i < e; i++) {
        int u, v, w;
        cin >> u >> v >> w;
        if (u >= 0 && u < n && v >= 0 && v < n) {
            addEdge(g, u, v, w);
        }
        else {
            cout << "頂點編號超出範圍，請重新輸入邊" << endl;
            i--; //重新讀取邊
        }
    }
    CPath(g, n);
    for (int i = 0; i < n; i++)//釋放動態記憶體
    {
        EdgeNode* curr = g[i].fE;
        while (curr != nullptr) 
        {
            EdgeNode* a = curr;
            curr = curr->next;
            delete a;
        }
    }
    return 0;
}
