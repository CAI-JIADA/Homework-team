#include <iostream>
#include <vector>
#include <algorithm>
#include <queue>
using namespace std;
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
bool Edges(Edge a,Edge b)//邊由小到大
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
	vector<Edge> edgeList;
	vector<vector<pair<int, int>>> adj(n); 
	cout << "請輸入每條邊的資訊[起點 終點 權重]:" << endl;
	for (int i = 0; i < e; i++) {
		int u, v, w;
		cout << "第" << i + 1 << " 條邊:";
		cin >> u >> v >> w;
		edgeList.push_back({ u, v, w });
		adj[u].push_back({ v, w });
		adj[v].push_back({ u, w });
	}
	Kruskal(n, edgeList);
	Prim(n, s, adj);
	return 0;
}