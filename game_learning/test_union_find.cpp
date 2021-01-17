#include <vector>
#include <list>
#include <set>
#include <unordered_set>
#include <iostream>
#include <map>
#include <unordered_map>
#include <algorithm> 

using namespace std;

class SolutionCircleNum
{

public:
	int find(int p) {
		int root = p;
		while (root != uf[root]) {
			root = uf[root];
		}
		// path compression
		while (root != p) {
			int next = uf[p];
			uf[p] = root;
			p = next;
		}
		return root;
	}

	void unify(int p, int q) {
		p = find(p);
		q = find(q);
		if (p == q) return;

		if (sz[p] < sz[q]) swap(p, q);
		sz[p] += sz[q];
		uf[q] = p;
	}

	int findCircleNum(vector<vector<int>>& isConnected) {
		int nodeNum = isConnected.size();
		for (int i = 0; i < nodeNum; ++i) {
			uf.push_back(i);
			sz.push_back(1);
		}
		for (int i = 0; i < nodeNum; ++i)
			for (int j = 0; j < nodeNum; ++j) {
				if (isConnected[i][j] == 1)
					unify(i, j);
			}
		unordered_set<int> roots;
		for (int i = 0; i < nodeNum; ++i) {
			int root = find(i);
			if (roots.find(root) == roots.end())
				roots.insert(root);
		}
		return roots.size();
	}
private:
	vector<int> uf;
	vector<int> sz;

};

class SolutionRedundantConnection {
private:
	vector<int> uf, sz;

public:
	int find(int p) {
		int root = p;
		while (root != uf[root])
			root = uf[root];
		while (p != root) {
			int next = uf[p];
			uf[p] = root;
			p = next;
		}
		return root;
	}

	bool unify(int p, int q) {
		p = find(p);
		q = find(q);
		if (p == q) return false;
		if (sz[p] < sz[q]) swap(p, q);
		sz[p] += sz[q];
		uf[q] = p;
		return true;
	}

	vector<int> findRedundantConnection(vector<vector<int>>& edges) {
		unordered_map<int, int> mp;
		int i = 0;
		for (auto& edge : edges) {
			for (int j = 0; j < edge.size(); ++j) {
				if (mp.find(edge[j]) == mp.end()) {
					mp[edge[j]] = i;
					uf.push_back(i);
					sz.push_back(1);
					++i;
				}
			}
		}
		vector<int> ret;
		for (i = 0; i < edges.size(); ++i) {
			auto& edge = edges[i];
			if (!unify(mp[edge[0]], mp[edge[1]])) {
				ret.push_back(edge[0]);
				ret.push_back(edge[1]);
				break;
			}
		}
		return ret;
	}
};


class SolutionRedundantDirectedConnection {
private:
#define maxN  1000
	bool vis[maxN];
	bool vis2[maxN];
	int who[maxN], deg[maxN], uf[maxN];
	vector<int> adj[maxN], radj[maxN], st;
public:
	void dfs1(int u) {
		vis[u] = true;
		for (auto& v : adj[u]) {
			if (!vis[v])
				dfs1(v);
		}
		st.push_back(u);
	}
	void dfs2(int u, int root) {
		vis2[u] = true;
		who[u] = root;
		for (auto& v : radj[u])
			if (!vis2[v])
				dfs2(v, root);
	}

	vector<int> findRedundantDirectedConnection(vector<vector<int>>& edges) {
		// pre process edges
		unordered_map<int, int> mp;
		unordered_map<int, int> rmp;
		int n = 0;
		for (auto& edge : edges)
			for (auto& node : edge)
				if (mp.find(node) == mp.end()) {
					rmp[n] = node;
					mp[node] = n++;
				}

		// construct adj arrary and radj array
		memset(vis, 0, maxN);
		memset(vis2, 0, maxN);
		memset(deg, 0, sizeof(int)*maxN);
		for (auto& edge : edges) {
			adj[mp[edge[0]]].push_back(mp[edge[1]]);
			radj[mp[edge[1]]].push_back(mp[edge[0]]);
		}

		// push node to stack
		for (int i = 0; i < n; ++i) {
			if (!vis[i])
				dfs1(i);
		}
		// set root from stack
		while (st.size() > 0) {
			int u = st.back();
			st.pop_back();
			if (!vis2[u])
				dfs2(u, u);
		}
		// record the in degree
		for (int i = 0; i < n; ++i)
			for (int j : adj[i])
				++deg[j];
		vector<int> ret;
		// for the deg greater than 1 for other node, or root node greater than 0
		for (int i = 0; i < n; ++i)
			if (who[i] == i && deg[i] > 0 || deg[i] > 1) {
				ret.push_back(rmp[radj[i][0]]);
				ret.push_back(rmp[i]);
				break;
			}

		return ret;
	}

	int find(int u) {
		int root = u;
		while (root != uf[root])
			root = uf[root];
		while (u != root) {
			int next = uf[u];
			uf[u] = root;
			u = next;
		}
		return root;
	}
	void unify(int u, int v) {
		u = find(u);
		v = find(v);
		if (u == v) return;
		uf[v] = u;
	}

	vector<int> findRedundantDirectedConnectionV2(vector<vector<int>>& edges) {
		// pre process edges
		unordered_map<int, int> mp;
		unordered_map<int, int> rmp;
		int n = 0;
		for (auto& edge : edges)
			for (auto& node : edge)
				if (mp.find(node) == mp.end()) {
					uf[n] = n;
					rmp[n] = node;
					mp[node] = n++;
				}
		memset(deg, 0, sizeof(int)*maxN);
		for (auto& edge : edges) {
			radj[mp[edge[1]]].push_back(mp[edge[0]]);
			unify(mp[edge[0]], mp[edge[1]]);
			++deg[mp[edge[1]]];
		}
		vector<int> ret;
		for (int i = 0; i < n; ++i)
			if (find(i) == i && deg[i] > 0 || deg[i] > 1) {
				ret.push_back(rmp[radj[i][0]]);
				ret.push_back(rmp[i]);
				break;
			}
		return ret;
	}
};



class SolutionAccountsMerge {

public:
#define maxNm 100000
	int uf[maxNm];

	int find(int p) {
		int root = p;
		while (root != uf[root]) {
			root = uf[root];
		}
		while (p != root) {
			int next = uf[p];
			uf[p] = root;
			p = next;
		}
		return root;
	}
	void unify(int p, int q) {
		p = find(p);
		q = find(q);
		if (p == q) return;
		uf[p] = q;
	}
	vector<vector<string>> accountsMerge(vector<vector<string>>& accounts) {

		for (int i = 0; i < maxNm; ++i) {
			uf[i] = i;
		}
		unordered_map<string, int> emailIdMp;
		unordered_map<string, string> emailNameMp;
		int id = 0;
		for (auto& account : accounts) {
			int i = 0;
			for (string& email : account) {
				if (i > 0 && emailIdMp.find(email) == emailIdMp.end())
					emailIdMp[email] = id++;
				if (i > 0 && emailNameMp.find(email) == emailNameMp.end())
					emailNameMp[email] = account[0];
				if (i > 1)
					unify(emailIdMp[account[1]], emailIdMp[email]);
				++i;
			}
		}

		unordered_map<int, vector<string>> groups;
		for (auto& it = emailNameMp.begin(); it != emailNameMp.end(); ++it) {
			int id = find(emailIdMp[it->first]);
			groups[id].push_back(it->first);
		}
		vector<vector<string>> res;
		for (auto& it = groups.begin(); it != groups.end(); ++it) {
			vector<string> group = it->second;
			std::sort(group.begin(), group.end());
			group.insert(group.begin(), emailNameMp[group[0]]);
			res.push_back(group);
		}
		return res;

	}
};


/*
int main()
{
	SolutionCircleNum s = SolutionCircleNum();
	vector<vector<int>> isConnected = { { 1, 1, 0 }, { 1, 1, 0 }, { 0, 0, 1 } };
	//vector<vector<int>> isConnected = { { 1, 0, 0 }, { 0, 1, 0 }, { 0, 0, 1 } };
	cout << "findCircleNum: " << s.findCircleNum(isConnected) << endl;
	vector<vector<int>> edges = { { 1, 2 }, { 1, 3 }, { 2, 3 } };
	//vector<vector<int>> edges = { { 1, 2 }, { 2, 3 }, { 3, 4 }, { 1, 4 }, { 1, 5 } };
	vector<int> ret = SolutionRedundantConnection().findRedundantConnection(edges);
	cout << "findRedundantConnection: " << ret[0] << ", " << ret[1] << endl;
	vector<vector<int>> edges2 = { { 1, 2 }, { 2, 3 }, { 3, 4 }, { 4, 1 }, { 1, 5 } };
	ret = SolutionRedundantDirectedConnection().findRedundantDirectedConnectionV2(edges2);
	cout << "findRedundantDirectedConnection: " << ret[0] << ", " << ret[1] << endl;
	vector<vector<string>> accs = { { "John", "johnsmith@mail.com", "john00@mail.com" },
	{ "John", "johnnybravo@mail.com" },
	{ "John", "johnsmith@mail.com", "john_newyork@mail.com" }, { "Mary", "mary@mail.com" } };
	vector<vector<string>> res = SolutionAccountsMerge().accountsMerge(accs);
	cout << "accountsMerge: " << endl;
	for (vector<string>& group : res) {
		for (string& email : group) 
			cout << email.c_str() << "     ";
		cout << endl;
	}
	return 0;
}
*/