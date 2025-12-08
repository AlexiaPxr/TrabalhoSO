#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <chrono>
#include <random>
#include <unordered_map>
#include <unordered_set>
#include <queue>
#include <list>
#include <climits> 

using namespace std;
using ll = long long;

struct SimulationResult {
    long long elapsed_seconds; // tempo arredondado
    size_t page_faults;
    vector<int> swap_pages; // páginas presentes no swap ao final (ordenadas)
};

using Clock = chrono::high_resolution_clock;

vector<int> make_swap_vector(const unordered_set<int>& s) {
    vector<int> v;
    v.reserve(s.size());
    for (int x : s) v.push_back(x);
    sort(v.begin(), v.end());
    return v;
}

SimulationResult simulate_FIFO(const vector<int>& seq, int nframes) {
    auto t0 = Clock::now();
    size_t faults = 0;
    vector<int> frames(nframes, -1);
    unordered_map<int,int> page_to_frame;
    queue<int> fifo_q;
    unordered_set<int> swap_set;

    for (int req : seq) {
        if (nframes == 0) { 
            faults++;
            swap_set.insert(req);
            continue;
        }
        if (page_to_frame.count(req)) continue;
        faults++;
        int free_idx = -1;
        for (int i = 0; i < nframes; ++i) if (frames[i] == -1) { free_idx = i; break; }
        if (free_idx != -1) {
            frames[free_idx] = req;
            page_to_frame[req] = free_idx;
            fifo_q.push(req);
            swap_set.erase(req);
        } else {
            int victim_page = fifo_q.front(); fifo_q.pop();
            int victim_frame = page_to_frame[victim_page];
            swap_set.insert(victim_page);
            page_to_frame.erase(victim_page);
            frames[victim_frame] = req;
            page_to_frame[req] = victim_frame;
            fifo_q.push(req);
            swap_set.erase(req);
        }
    }

    auto t1 = Clock::now();
    long long elapsed = chrono::duration_cast<chrono::milliseconds>(t1 - t0).count();
    long long secs = (elapsed + 500) / 1000;
    return {secs, faults, make_swap_vector(swap_set)};
}

SimulationResult simulate_RAND(const vector<int>& seq, int nframes) {
    auto t0 = Clock::now();
    size_t faults = 0;
    vector<int> frames(nframes, -1);
    unordered_map<int,int> page_to_frame;
    unordered_set<int> swap_set;
    std::mt19937 rng(42);

    for (int req : seq) {
        if (nframes == 0) { faults++; swap_set.insert(req); continue; }
        if (page_to_frame.count(req)) continue;
        faults++;
        int free_idx = -1;
        for (int i = 0; i < nframes; ++i) if (frames[i] == -1) { free_idx = i; break; }
        if (free_idx != -1) {
            frames[free_idx] = req;
            page_to_frame[req] = free_idx;
            swap_set.erase(req);
        } else {
            uniform_int_distribution<int> dist(0, nframes-1);
            int victim_frame = dist(rng);
            int victim_page = frames[victim_frame];
            swap_set.insert(victim_page);
            page_to_frame.erase(victim_page);
            frames[victim_frame] = req;
            page_to_frame[req] = victim_frame;
            swap_set.erase(req);
        }
    }

    auto t1 = Clock::now();
    long long elapsed = chrono::duration_cast<chrono::milliseconds>(t1 - t0).count();
    long long secs = (elapsed + 500) / 1000;
    return {secs, faults, make_swap_vector(swap_set)};
}

SimulationResult simulate_LRU(const vector<int>& seq, int nframes) {
    auto t0 = Clock::now();
    size_t faults = 0;
    vector<int> frames(nframes, -1);
    unordered_map<int,int> page_to_frame;
    unordered_set<int> swap_set;
    list<int> recent;
    unordered_map<int, list<int>::iterator> iter_map;

    for (int req : seq) {
        if (nframes == 0) { faults++; swap_set.insert(req); continue; }
        if (page_to_frame.count(req)) {
            if (iter_map.count(req)) {
                recent.erase(iter_map[req]);
            }
            recent.push_front(req);
            iter_map[req] = recent.begin();
            continue;
        }
        faults++;
        int free_idx = -1;
        for (int i = 0; i < nframes; ++i) if (frames[i] == -1) { free_idx = i; break; }
        if (free_idx != -1) {
            frames[free_idx] = req;
            page_to_frame[req] = free_idx;
            recent.push_front(req);
            iter_map[req] = recent.begin();
            swap_set.erase(req);
        } else {
            int victim_page = recent.back();
            recent.pop_back();
            iter_map.erase(victim_page);
            int victim_frame = page_to_frame[victim_page];
            swap_set.insert(victim_page);
            page_to_frame.erase(victim_page);
            frames[victim_frame] = req;
            page_to_frame[req] = victim_frame;
            recent.push_front(req);
            iter_map[req] = recent.begin();
            swap_set.erase(req);
        }
    }

    auto t1 = Clock::now();
    long long elapsed = chrono::duration_cast<chrono::milliseconds>(t1 - t0).count();
    long long secs = (elapsed + 500) / 1000;
    return {secs, faults, make_swap_vector(swap_set)};
}

SimulationResult simulate_MIN(const vector<int>& seq, int nframes) {
    auto t0 = Clock::now();
    size_t faults = 0;
    vector<int> frames(nframes, -1);
    unordered_map<int,int> page_to_frame;
    unordered_set<int> swap_set;

    int R = (int)seq.size();
    unordered_map<int, queue<int>> positions;
    for (int i = 0; i < R; ++i) positions[seq[i]].push(i);

    for (int i = 0; i < R; ++i) {
        int req = seq[i];
        if (!positions[req].empty() && positions[req].front() == i) {
            positions[req].pop();
        }
        if (nframes == 0) { faults++; swap_set.insert(req); continue; }
        if (page_to_frame.count(req)) continue;
        faults++;
        int free_idx = -1;
        for (int j = 0; j < nframes; ++j) if (frames[j] == -1) { free_idx = j; break; }
        if (free_idx != -1) {
            frames[free_idx] = req;
            page_to_frame[req] = free_idx;
            swap_set.erase(req);
        } else {
            int victim_frame = -1;
            int victim_page = -1;
            long long farthest = -1;
            for (int f = 0; f < nframes; ++f) {
                int p = frames[f];
                if (positions[p].empty()) {
                    victim_frame = f;
                    victim_page = p;
                    farthest = LLONG_MAX;
                    break;
                } else {
                    long long nextpos = positions[p].front();
                    if (nextpos > farthest) {
                        farthest = nextpos;
                        victim_frame = f;
                        victim_page = p;
                    }
                }
            }
            swap_set.insert(victim_page);
            page_to_frame.erase(victim_page);
            frames[victim_frame] = req;
            page_to_frame[req] = victim_frame;
            swap_set.erase(req);
        }
    }

    auto t1 = Clock::now();
    long long elapsed = chrono::duration_cast<chrono::milliseconds>(t1 - t0).count();
    long long secs = (elapsed + 500) / 1000;
    return {secs, faults, make_swap_vector(swap_set)};
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    ll M, V;
    string A;
    ll P;

    if (!(cin >> M >> V >> A >> P)) return 0;

    ll SP = V / P;
    int Nframes = (SP == 0) ? 0 : (int)(M / SP);
    ll swap_bytes = V - M; // explícito e equivalente ao cálculo por páginas

    int N;
    if (!(cin >> N)) return 0;

    vector<vector<int>> sequences;
    sequences.reserve(N);
    for (int i = 0; i < N; ++i) {
        int R;
        cin >> R;
        vector<int> seq(R);
        for (int j = 0; j < R; ++j) cin >> seq[j];
        sequences.push_back(move(seq));
    }

    cout << SP << "\n";
    cout << Nframes << "\n";
    cout << swap_bytes << "\n\n";
    cout << N << "\n\n";

    for (int i = 0; i < N; ++i) {
        const vector<int>& seq = sequences[i];

        // <-- CORREÇÃO IMPORTANTE: imprimir o tamanho R antes da sequência -->
        cout << seq.size() << "\n";

        // imprime a sequência
        for (size_t k = 0; k < seq.size(); ++k) {
            if (k) cout << " ";
            cout << seq[k];
        }
        cout << "\n";

        vector<string> policy_names = {"FIFO", "RAND", "LRU", "MIN"};
        for (const string& pol : policy_names) {
            SimulationResult r;
            if (pol == "FIFO") r = simulate_FIFO(seq, Nframes);
            else if (pol == "RAND") r = simulate_RAND(seq, Nframes);
            else if (pol == "LRU") r = simulate_LRU(seq, Nframes);
            else if (pol == "MIN") r = simulate_MIN(seq, Nframes);

            cout << pol << "\n";
            cout << r.elapsed_seconds << "\n";
            cout << r.page_faults << "\n";
            if (r.swap_pages.empty()) {
                cout << "\n";
            } else {
                for (size_t j = 0; j < r.swap_pages.size(); ++j) {
                    if (j) cout << " ";
                    cout << r.swap_pages[j];
                }
                cout << "\n";
            }
        }

        if (i < N - 1) cout << "\n";
    }

    return 0;
}
