#include <vector>
#include <list>
#include <set>
#include <unordered_set>
#include <iostream>
#include <map>
#include <unordered_map>
#include <algorithm> 

using namespace std;

/*
A generic bloom filter implementation that supports any hash function(s)
*/
class BloomFilter
{
	typedef unsigned long long  uint64;
	// The number of bitsets. This should be proportional
	// to the number of hash functions for this bloom filter
	int nSets;

	// A 2D array containing the bitsets
	vector<vector<uint64>> bitsets;

	// Tracks the size of the bitsets in this bloom filter
	vector<int> setSizes;

	// Doing 'n & 0x7F' is the same as modding by 64, but faster
	const uint64 mod64Mask = 0x7F;

	// Doing 'n >> 6' is the same as dividing by 64, but faster
	const uint64 div64Shift = 6;

public:
	BloomFilter(vector<int>& bitSetSizes) {
		nSets = bitSetSizes.size();
		setSizes = bitSetSizes;
		bitsets.resize(nSets);
		for (int i = 0; i < nSets; ++i)
			bitsets[i].resize(setSizes[i]);
	}

	// Add a hash value to one of the bitsets in the bloom filter
	void add(int setIndex, uint64 hash) {
		hash = hash % setSizes[setIndex];
		int block = (int)(hash >> div64Shift);
		bitsets[setIndex][block] |= (1L << (hash & mod64Mask));
	}

	// Adds a group of related hash values to the bloom filter.
	// These hash values should be the hash values that were applied
	// to all the various hash functions on the same key.
	void add(vector<uint64>& hashes) {
		for (int i = 0; i < nSets; ++i)
			add(i, hashes[i]);
	}

	bool contains(vector<uint64>& hashes) {
		for (int i = 0; i < hashes.size(); ++i) {
			int block = (int)(hashes[i] >> div64Shift);
			uint64 mask = 1L << (hashes[i] & mod64Mask);
			if (bitsets[i][block] & mask != mask)
				return false;
		}
		return true;
	}
};

/*
int main()
{
	return 0;
}
*/