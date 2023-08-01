#include <vector>
#include <list>
#include <set>
#include <unordered_set>
#include <iostream>
#include <map>
#include <unordered_map>
#include <algorithm> 

using namespace std;


class MyCalendar {
private:
	vector<vector<int>> booked;
	int dayBegin;
	int dayEnd;
public:
	MyCalendar(int dayBegin = 800, int dayEnd = 1700) {
		this->dayBegin = timeConvert(dayBegin);
		this->dayEnd = timeConvert(dayEnd);
	}

	int timeConvert(int n) {
		// 0 means 00:00, 2359 means 23:59
		return (n / 100) % 24 * 100 + (n % 100) % 60;
	}

	bool book(int start, int end) {
		int ts = timeConvert(start);
		int te = timeConvert(end);
		if (ts < dayBegin || ts >= dayEnd || te > dayEnd)
			return false;
		int i = 0;
		for (; i < booked.size(); ++i) {
			vector<int> schedule = booked[i];
			int currBegin = schedule[0];
			int currEnd = schedule[1];
			if (ts >= currEnd)
				continue;
			if (te >= currBegin)
				return false;
			else
				break;
		}
		vector<int> newSchedule = { ts, te };
		booked.insert(booked.begin() + i, newSchedule);
		return true;
	}
};


class GoldenCalendar
{
private:
	map<int, int> booked;
	int dayBegin;
	int dayEnd;
public:
	GoldenCalendar(int dayBegin = 800, int dayEnd = 1700) {
		this->dayBegin = timeConvert(dayBegin);
		this->dayEnd = timeConvert(dayEnd);
	}

	int timeConvert(int n) {
		// 0 means 00:00, 2359 means 23:59
		return (n / 100) % 24 * 100 + (n % 100) % 60;
	}

	bool book(int start, int end){
		start = timeConvert(start);
		end = timeConvert(end);
		if (start < dayBegin || start >= dayEnd || end > dayEnd)
			return false;
		auto it = booked.lower_bound(start);
		if (it != booked.end() && it->first < end)
			return false;
		if (it != booked.begin() && start < (--it)->second)
			return false;
		booked[start] = end;
		return true;
	}

};


class MyCalendarTwo {
private:
	map<int, int> calendar;
	int dayBegin;
	int dayEnd;
public:
	MyCalendarTwo(int dayBegin = 800, int dayEnd = 1700) {
		this->dayBegin = timeConvert(dayBegin);
		this->dayEnd = timeConvert(dayEnd);
	}

	int timeConvert(int n) {
		// 0 means 00:00, 2359 means 23:59
		return (n / 100) % 24 * 100 + (n % 100) % 60;
	}

	bool book(int start, int end) {
		start = timeConvert(start);
		end = timeConvert(end);
		if (start < dayBegin || start >= dayEnd || end > dayEnd)
			return false;
		++calendar[start];
		--calendar[end];

		int counter = 0;
		for (auto iter = calendar.begin(); iter != calendar.end(); ++iter) {
			counter += iter->second;

			if (counter > 2) {
				--calendar[start];
				++calendar[end];

				if (calendar[start] == 0) calendar.erase(start);
				if (calendar[end] == 0)calendar.erase(end);

				return false;
			}
		}

		return true;
		/*
		cur[start]++; cur[end]--;
		int sum = 0, maxc = 0;
		for (auto it = cur.begin(); it != cur.end() && it->first<end; it++){
			sum += it->second;
			maxc = max(maxc, sum);
		}
		if (maxc>2){
			cur[start]--;
			cur[end]++;
			return false;
		}
		return true;
		*/
	}
};


class OverlapIntervals {
public:
	int eraseOverlapIntervals(vector<vector<int>>& intervals) {
		int sz = intervals.size();
		if (sz < 2)
			return 0;

		sort(intervals.begin(), intervals.end(), [](vector<int>& i1, vector<int>& i2) {
			return i1[1] < i2[1];
		});
		int ans = 0;
		int i = 0, j = 1;
		while (j < sz) {
			if (intervals[i][1] <= intervals[j][0])
				i = j;
			else
				++ans;

			++j;

		}
		return ans;
	}
};

int main()
{
	MyCalendar calendar;
	cout << "calendar book [830 930]: " << calendar.book(830, 930) << endl;
	cout << "calendar book [900 1000]: " << calendar.book(900, 1000) << endl;
	cout << "calendar book [1030 1100]: " << calendar.book(1030, 1100) << endl;
	GoldenCalendar calendarGolden;
	cout << "calendarGolden book [830 930]: " << calendarGolden.book(830, 930) << endl;
	cout << "calendarGolden book [900 1000]: " << calendarGolden.book(900, 1000) << endl;
	cout << "calendarGolden book [1030 1100]: " << calendarGolden.book(1030, 1100) << endl;
	MyCalendarTwo calendarTwo;
	cout << "calendarTwo book [910 920]: " << calendarTwo.book(910, 920) << endl;
	cout << "calendarTwo book [950 1000]: " << calendarTwo.book(950, 1000) << endl;
	cout << "calendarTwo book [910 940]: " << calendarTwo.book(910, 940) << endl;
	cout << "calendarTwo book [905 915]: " << calendarTwo.book(905, 915) << endl;
	cout << "calendarTwo book [905 910]: " << calendarTwo.book(905, 910) << endl;
	cout << "calendarTwo book [925 955]: " << calendarTwo.book(925, 955) << endl;
	OverlapIntervals oi;
	vector<vector<int>> intervals = {
		{0, 2},
		{1, 3},
		{2, 4},
		{3, 5},
		{4, 6},
	};
	cout << "OverlapIntervals.eraseOverlapIntervals: " << oi.eraseOverlapIntervals(intervals) << endl;
	return 0;
}