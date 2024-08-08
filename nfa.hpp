#ifndef nfa_hpp
#define nfa_hpp
#include <iostream>
#include <iomanip>
#include <cstring>
#include <vector>
#include "deque.hpp"
using namespace std;

static const int SCAN = -1;
static const char NSTATE = ' ';

struct NFA {
    int num_states;
    vector<char> ch;
    vector<int> next1;
    vector<int> next2;
    vector<bool> in_progress;
    NFA(int ns) {
        num_states = ns;
        ch = vector<char>(ns, NSTATE);
        next1 = vector<int>(ns, 0);
        next2 = vector<int>(ns, 0);
    }
    NFA() {
        num_states = 0;
    }
    bool match(string text);
    void makeInUse(int state) {
        if (state >= 0 && state < in_progress.size())
            in_progress[state] = true;
    }    
    void makeNotInUse(int state) {
        if (state >= 0 && state < in_progress.size())
            in_progress[state] = false;
    }
    bool stateInUse(int state) {
        return (state >= 0 && state < in_progress.size() && in_progress[state]); 
    }
};

bool NFA::match(string txt) {
    int n1, n2;
    deque<char> dq;
    in_progress = vector<bool>(txt.size()+1, false);
    int j = 0, N = txt.size(), state = next1[0];
    dq.put(SCAN);
    while (state) {
        cout<<((state == -1) ? "scan":to_string(state))<<" -> ";
        if (state == SCAN) {
            j++;
            dq.put(SCAN);
        } else if (ch[state] == txt[j] || ch[state] == '.') {
            cout<<"matched on "<<txt[j]<<endl;
            if (!stateInUse(next1[state])) {
                dq.put(next1[state]);
                makeInUse(next1[state]);
            }
        } else if (ch[state] == ' ') {
            n1 = next1[state];
            n2 = next2[state];
            if (!stateInUse(n1)) {
                dq.push(n1);
                 makeInUse(n1);
            }
            if (n1 != n2 && !stateInUse(n2)) {
                dq.push(n2);
                makeInUse(n2);
            }
        }
        if (dq.empty() || j == N+1) {
            return false;
        }
        state = dq.pop();
        makeNotInUse(state);
    }
    return state == 0;
}

bool matchAll(NFA nfa, string txt) {
    bool didmatch = false;
    for (int i = 0; i < txt.length(); i++) {
        if (nfa.match(txt.substr(i))) {
            cout<<"Match found at index: "<<i<<endl;
            didmatch = true;
        }
    }
    cout<<endl;
    return didmatch;
}


void printNFA(NFA nfa) {
    for (int i = 0; i < nfa.num_states; i++) {
        cout<<setw(3)<<i<<" ";
    }
    cout<<endl;
    for (int i = 0; i < nfa.num_states; i++) {
        cout<<setw(3)<<nfa.ch[i]<<" ";
    }
    cout<<endl;
    for (int i = 0; i < nfa.num_states; i++) {
        cout<<setw(3)<<nfa.next1[i]<<" ";
    }
    cout<<endl;
    for (int i = 0; i < nfa.num_states; i++) {
        cout<<setw(3)<<nfa.next2[i]<<" ";
    }
    cout<<endl;
}

#endif