#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <limits.h>
#include <vector>
#include <stdlib.h>
#include <algorithm>
#include <memory.h>
#include <string.h>
#include <math.h>
#include <string>
#include <algorithm>
#include <functional>
#include <cassert>
#include <map>
#include <set>
#include <list>

using namespace std;
typedef long long lli;
typedef vector<int> vi;
typedef vector<lli> vli;
typedef pair<int, int> pii;
typedef vector<pii> vpii;
typedef long double ld;

const int INF = 0x3f3f3f3f;
const lli LINF = 0x3f3f3f3f3f3f3f3f;

//#define _LOCAL_DEBUG_
#ifdef _LOCAL_DEBUG_
#define eprintf(...) fprintf(stderr,__VA_ARGS__)
#else
#define eprintf(...) 
#endif

const int NR = 5, NC = 4;
const int NSTEPS = 4;
const int NANIMALS = 9;
const int MAXSEAT = 4;
int dx[NSTEPS] = { 1, -1, 0, 0 };
int dy[NSTEPS] = { 0, 0, 1, -1 };
struct Point { int x, y; };
int maxSteps, nAnimals;


int getHash(const set<int> &s) {
	int res = s.size();
	for (auto c : s) res = res * nAnimals + c;
	return res;
}
struct Car {
	set<int> s;
	Car(const Car &c) {
		s = c.s;
	}
	Car() { }

	bool operator <(const Car &c) {
		return getHash(s) < getHash(c.s);
	}
	void add(int animal) {
		assert(s.size() < MAXSEAT);
		s.insert(animal);
	}
	void remove(int animal) {
		assert(s.count(animal));
		s.erase(animal);
	}
	int hash() const { return getHash(s); }
	const bool operator ==(const Car &c) const {
		return hash() == c.hash();
	}
	bool test(int animal) { return s.count(animal); }
	bool hasSeats() {
		return s.size() < MAXSEAT;
	}
};
struct State {
	int x, y, m;
	Car car;
	const bool operator < (const State &s)  const {
		return x == s.x ? y == s.y ? m == s.m ? car.hash() < s.car.hash() : m < s.m : y < s.y : x < s.x;
	}

	const bool operator ==(const State &s) const {
		return x == s.x && y == s.y && m == s.m && car == s.car;
	}

	bool isDelivered(int animal) {
		return m >> animal & 1;
	}
	void deliver(int animal) {
		m |= 1 << animal;
	}
	bool completed() { return m == (1 << nAnimals) - 1; }
};

vector<Point> adj[5][4];

Point start[NANIMALS], dest[NANIMALS];
int mapStart[NR][NC] = { 0 }, mapDest[NR][NC] = { 0 };

Point carPos;

char m[2 * NR][2 * NC];

map<State, int> d;
map<State, State> from;
set<pair<int, State>> s;

void clear() {
	memset(mapStart, INF, sizeof(mapStart));
	memset(mapDest, INF, sizeof(mapDest));
}

void read() {
	for (int i = 0; i < 2 * NR - 1; i++) {
		char s[2 * NR];
		scanf("%s", s);
		for (int j = 0; j < 2 * NC - 1; j++)
			if (s[j] == '|')
				adj[i / 2][j / 2].push_back({ i / 2 + 1, j / 2 }),
				adj[i / 2 + 1][j / 2].push_back({ i / 2, j / 2 });
			else if (s[j] == '-')
				adj[i / 2][j / 2].push_back({ i / 2, j / 2 + 1}),
				adj[i / 2][j / 2 + 1].push_back({ i / 2, j / 2 });
	}

	scanf("%d", &nAnimals);
	scanf("%d", &maxSteps);
	scanf("%d%d", &carPos.x, &carPos.y);
	for (int i = 0; i < nAnimals; i++) {
		scanf("%d%d", &start[i].x, &start[i].y);
		scanf("%d%d", &dest[i].x, &dest[i].y);
		mapStart[start[i].x][start[i].y] = i, mapDest[dest[i].x][dest[i].y] = i;
	}
}

void updateState(int dist, State &state, State &fromState) {
	if (d.find(state) == d.end()) {
		d[state] = dist;
		from[state] = fromState;
		s.insert({ dist, state });
		return;
	}
	if (d[state] > dist) {
		s.erase(s.find({ d[state], state }));
		d[state] = dist;
		from[state] = fromState;
		s.insert({ dist, state });
	}
}

void solve() {
	Car car;
	State state = { carPos.x, carPos.y, 0, car };
	d[state] = 0;
	s.insert({ 0, state });
	while (s.size()) {
		State state = s.begin()->second;
		int dist = s.begin()->first;
		s.erase(s.begin());
		if (state.completed()) {
			while (1) {
				printf("%d %d ", state.x, state.y);
				if (from.count(state) == 0) break;
				auto pState = from[state];
				if (pState.m < state.m) printf("drop");
				if (pState.car.hash() < state.car.hash()) printf("pick");
				printf("\n");
				state = pState;
			}
			return;
		}
		if (dist == maxSteps) continue;

		int x = state.x, y = state.y, m = state.m;
		for (auto p : adj[x][y]) {
			State nState = { p.x, p.y, m, state.car };
			// move
			updateState(dist + 1, nState, state);
			// move & pick
			int animalAtP = mapStart[p.x][p.y];
			if (animalAtP != INF && !state.isDelivered(animalAtP) && !state.car.test(animalAtP) && state.car.hasSeats()) {
				State nState = { p.x, p.y, m, state.car };
				nState.car.add(animalAtP);
				updateState(dist + 1, nState, state);
			}
			// move & drop
			int houseAtP = mapDest[p.x][p.y];
			if (houseAtP != INF && !state.isDelivered(houseAtP) && state.car.test(houseAtP)) {
				State nState = { p.x, p.y, m, state.car };
				nState.car.remove(houseAtP);
				nState.deliver(houseAtP);
				updateState(dist + 1, nState, state);
			}
		}
	}
	printf("NO\n");
}

int main() {
#ifdef _LOCAL_VAN
	freopen("in.txt", "r", stdin);
#endif
	while (1) {
		clear();
		read();
		solve();
		break;
#ifndef _LOCAL_VAN
		return 0;
#endif
	}
	return 0;
}