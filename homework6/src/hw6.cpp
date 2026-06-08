
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