/**
 * @author Pengyuting, student ID 2013631
 * This is the code to implement network interconnection at the min cost
 * the methods are DP and BFS
 */
#include<iostream>
#include<queue>
using namespace std;
int b;//mention:there are b+1 graphs,so the boundary might be i<=b
int n;//number of vertex
int s, t;//the focus points
int K;//change penalty
int*** G;
int** subG;//used to store G(i,j)
int** Lij;//used to store l(i,j)
int* Opt;//used to store Opt(i)
bool wrong = 0;//mark wether there are wrong inputs
void Test_Element() {
	//test if the input elements are wrong
	if (b < 0 || n < 1 || s == t || s < 0 || s >(n - 1) || t < 0 || t >(n - 1) || K < 0) {
		cout << "非法输入元素";
		wrong = 1;
	}
}
void Test_Graph() {
	//test if the input graphs are wrong
	//test if there is any isolated node
	for (int k1 = 0; k1 <= b; k1++) {
		for (int k2 = 0; k2 < n; k2++) {
			int flag = 0;
			for (int k3 = 0; k3 < n; k3++) {
				if (G[k1][k2][k3] != 0) {
					flag = 1;
					break;
				}
			}
			if (flag == 0) {
				cout << "非法输入图";
				wrong = 1;
				return;
			}
		}
	}
	//test if the graph is symmetric and if the diagonal is 1
	for (int k1 = 0; k1 <= b; k1++) {
		for (int k2 = 0; k2 < n; k2++) {
			//if the diagonal isn't 1
			if (G[k1][k2][k2] != 1) {
				cout << "非法输入图";
				wrong = 1;
				return;
			}
			for (int k3 = k2 + 1; k3 < n; k3++) {
				//if the graph isn't symmetric
				if (G[k1][k2][k3] != G[k1][k3][k2]) {
					cout << "非法输入图";
					wrong = 1;
					return;
				}
			}
		}
	}
}
void Init_G() {
	//used to init Gi
	//if an edge doesn't exist,it is set 0
	//G[i][j][k] represents the ith Graph,the jth line and the kth column
	cin >> b >> n;
	cin >> s >> t;
	cin >> K;
	Test_Element();
	if (wrong) return;
	G = new int** [b + 1];
	for (int i = 0;i <= b; i++) {
		G[i] = new int* [n];
		for (int j = 0; j < n; j++) {
			G[i][j] = new int[n];
			for (int k = 0; k < n; k++) {
				cin >> G[i][j][k];
			}
		}
	}
	Test_Graph();
}
void Init_subG() {
	//used to init G(i,j)
	//if an edge doesn't exist,it is set 1
	subG = new int* [n];
	for (int i = 0; i < n; i++) {
		subG[i] = new int[n];
		for (int j = 0; j < n; j++) {
			subG[i][j] = 1;
		}
	}
}
void Reinit_subG() {
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++)
			subG[i][j] = 1;
	}
}
void Init_Lij() {
	Lij = new int* [b + 1];
	for (int i = 0; i <= b; i++) {
		Lij[i] = new int[b + 1];
		for (int j = 0; j <= b; j++) {
			Lij[i][j] = -1;
		}
	}
}
void Update_subG(int j) {
	//update the G(i,j) by given j
	//we only traverse the jth graph
	for (int k1 = 0; k1 < n; k1++) {
		for (int k2 = 0; k2 < n; k2++) {
			if (G[j][k1][k2] == 0)
				subG[k1][k2] = 0;
		}
	}
}
void Find_L() {
	//this function is used to caculate all values in Lij
	//we first find G(i,j),then caculate the corresponding l(i,j)
	for (int i = 0; i <= b; i++) {
		for (int j = i; j <= b; j++) {
			//1.find G(i,j)
			Update_subG(j);
			//2.find the corresponding l(i,j)
			bool flag = 0;//there isn't a path between s and t if flag == 0
			int layer = 0;//the min s-t path length
			bool* visited = new bool[n];//array visited is used to record wether a node is visited or not
			for (int k = 0; k < n; k++) {
				visited[k] = 0;
			}
			queue<int> myq;
			myq.push(s);
			visited[s] = 1;
			myq.push(-1);//the layer flag
			while (myq.size() != 1) {
				int currparent = myq.front();
				myq.pop();
				layer++;
				//traverse all the nodes of this layer
				while (currparent != -1) {
					//traverse all the children of given node
					for (int k = 0; k < n; k++) {
						if (subG[currparent][k] == 1 && visited[k] == 0) {
							if (k == t) {
								flag = 1;
								break;
							}
							myq.push(k);
							visited[k] = 1;
						}
					}
					//found t already
					if (flag == 1) break;
					//the next node
					currparent = myq.front();
					myq.pop();
				}
				//found t already
				if (flag == 1) break;
				//if currparent == -1,then push -1 again
				myq.push(-1);
			}
			if (flag == 1) {
				Lij[i][j] = layer;
			}
			else Lij[i][j] = -1;
		}
		Reinit_subG();
	}	
}
int Find_Min(int i) {
	bool isFirst = 1;
	int val2;
	for (int j = 0; j < i; j++) {
		if (Lij[j + 1][i] == -1) {
			continue;
		}
		//if we find the val for the first time,then initialize val2
		if (isFirst) {
			val2 = Opt[j] + (i - j) * Lij[j + 1][i] + K;
			isFirst = 0;
			continue;
		}
		val2 = (Opt[j] + (i - j) * Lij[j + 1][i] + K) < val2 ? (Opt[j] + (i - j) * Lij[j + 1][i] + K) : val2;
	}
	if (Lij[0][i] == -1) {
		return val2;
	}
	else {
		return (i + 1) * Lij[0][i] < val2 ? (i + 1) * Lij[0][i] : val2;
	}
}
void DP() {
	Opt = new int[b + 1];
	Opt[0] = Lij[0][0];
	for (int i = 1; i <= b; i++) {
		Opt[i] = Find_Min(i);
	}
	/*for (int i = 0; i <= b; i++) {
		for (int j = 0; j <= b; j++) {
			cout << "Lij:" << Lij[i][j] << ' ';
		}
		cout << endl;
	}
	for (int i = 0; i <= b; i++) {
		cout << "Opt[i]:" << Opt[i] << endl;
	}*/
	cout << Opt[b];
}
int main() {
	//initialization
	Init_G();
	if (wrong) return 0;
	Init_subG();
	Init_Lij();
	//find all l(i,j)
	Find_L();
	DP();

	return 0;
}