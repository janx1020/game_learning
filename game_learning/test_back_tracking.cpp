#include <vector>
#include <list>
#include <set>
#include <unordered_set>
#include <iostream>
#include <map>
#include <queue>
#include <unordered_map>
#include <algorithm> 

using namespace std;


class SolutionWordBreakII {
public:
	unordered_map<string, vector<string>> mp;
	void recursive(string& s, unordered_set<string>& dict) {
		if (mp.find(s) != mp.end()) return;

		mp[s] = vector<string>();
		for (int i = 1; i <= s.length(); ++i) {
			string ls = s.substr(0, i);
			if (dict.find(ls) == dict.end()) continue;
			string rs = s.substr(i);
			recursive(rs, dict);
			if (mp.find(rs) == mp.end()) continue;
			if (rs.length() != 0) {
				vector<string>& rightRes = mp[rs];
				for (auto& right : rightRes) {
					mp[s].push_back(ls + " " + right);
				}
			}
			else
				mp[s].push_back(ls);
		}
	}

	vector<string> wordBreak(string s, vector<string>& wordDict) {
		unordered_set<string> dict;
		for (auto& word : wordDict)
			dict.insert(word);
		int n = s.length();
		mp[""] = vector<string>();
		recursive(s, dict);
		return mp[s];
	}
};

class SolutionRegularExpressionMatching {
public:
    bool recursive(string& s, int idxS, string& p, int idxP) {
        if (idxS == -1 && idxP == -1) return true;
        else if (idxP <= -1 || idxS < -1) return false;
        char cp = p.at(idxP);
        char cs = 0;
        if (idxS >= 0) cs = s.at(idxS);
        if (cp == '.' || cp == cs) return recursive(s, idxS - 1, p, idxP - 1);
        else if (cp == '*') {
            if (idxP == 0) return false;
            cp = p.at(idxP - 1);
            for (int i = idxS; i >= 0 && (cp == '.' || cp == s.at(i)); --i)
                if (recursive(s, i - 1, p, idxP - 2)) return true;
            return recursive(s, idxS, p, idxP - 2);
        }
        return false;
    }

    bool isMatch(string s, string p) {
        return recursive(s, s.length() - 1, p, p.length() - 1);
    }
};


class SolutionCombinationSum {
public:
    int n;
    void dfs(vector<int>& candidates, int target, int idx, vector<int>& res, vector<vector<int>>& ans) {
        if (target == 0) {
            ans.push_back(res);
            return;
        }
        if (target < 0) {
            return;
        }
        for (; idx < n; ++idx) {
            res.push_back(candidates[idx]);
            dfs(candidates, target - candidates[idx], idx, res, ans);
            res.pop_back();
        }
        
    }
    vector<vector<int>> combinationSum(vector<int>& candidates, int target) {
        vector<int> res;
        vector<vector<int>> ans;
        n = candidates.size();
        dfs(candidates, target, 0, res, ans);
        return ans;
    }
};

class SolutionPermutationII {
public:
    unordered_map<int, int> counter;
    int n;
    void backtrack(vector<int>& res, vector<vector<int>>& ans) {
        if (res.size() == n) {
            ans.push_back(res);
            return;
        }

        for (auto it = counter.begin(); it != counter.end(); ++it) {
            if (it->second > 0) {
                --it->second;
                res.push_back(it->first);
                backtrack(res, ans);
                res.pop_back();
                ++it->second;
            }
        }
    }
    vector<vector<int>> permuteUnique(vector<int>& nums) {
        vector<vector<int>> ans;
        //sort(nums.begin(), nums.end());
        for (int num : nums)
            ++counter[num];
        n = nums.size();
        vector<int> res;
        backtrack(res, ans);
        return ans;
    }
};


void testWordBreakII() {
	string s("catsanddog");
	vector<string> wordDict = { "cat", "cats", "and", "sand", "dog" };
	vector<string> ans = SolutionWordBreakII().wordBreak(s, wordDict);
	cout << "SolutionWordBreakII: " << endl;
	for (auto& str : ans)
		cout << str.c_str() << endl;
}

void testRegularExpressionMatching() {
    string s = "aaaaaaaaaaaaab";
    //string p = "a*a*a*a*a*a*a*a*a*a*c";
    string p = "xa*b";
    cout << "RegularExpressionMatching: " << SolutionRegularExpressionMatching().isMatch(s, p) << endl;
}

void testCombinationSum() {
    vector<int> candidates = {2, 3, 6, 7 };
    int target = 7;
    vector<vector<int>> ans = SolutionCombinationSum().combinationSum(candidates, target);
    cout << "CombinationSum: " << endl;
    for (auto& r : ans) {
        for (int i : r)
            cout << i << " ";
        cout << endl;
    }

}

void testPermutationII() {
    vector<int> nums = { 2, 2, 1, 1};
    cout << "PermutationII: " << endl;
    vector<vector<int>> ans = SolutionPermutationII().permuteUnique(nums);
    for (auto& p : ans) {
        for (int i : p)
            cout << i << " ";
        cout << endl;
    }
}

/*
int main() {
	testWordBreakII();
    testRegularExpressionMatching();
    testCombinationSum();
    testPermutationII();
	return 0;
}
*/