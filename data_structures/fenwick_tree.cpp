#include <vector>
 
using namespace std;
 
class FenwickTree {
  public:
	FenwickTree(int n) : t(vector<int>(n)) {}
 
	void AddToElement(int i, int add) {
		while (i < t.size()) {
			t[i] += add;
			i = i | (i + 1);
		}
	}
 
	int GetSegmentSum(int l, int r) {
		return PrefixSum(r) - PrefixSum(l - 1);
	}
 
	void SetElement(int i, int val) {
		int curVal = PrefixSum(i) - PrefixSum(i - 1);
		AddToElement(i, val - curVal); // i.e. minus curVal and then plus val
	}
 
  private: 	
	vector<int> t;
 
	int PrefixSum(int k) {
		int ret = 0, i = k;
		while (i >= 0) {
			ret += t[i];
			i = i & (i + 1) - 1;
		}
		return ret;
	}
};
