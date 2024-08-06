#ifndef nfa_hpp
#define nfa_hpp
#include <iostream>
#include <cstring>
#include <vector>
#include "deque.hpp"
using namespace std;

struct NFA {
    int num_states;
    vector<char> ch;
    vector<int> next1;
    vector<int> next2;
    NFA(int ns) {
        num_states = ns;
        ch = vector<char>(ns, ' ');
        next1 = vector<int>(ns, 0);
        next2 = vector<int>(ns, 0);
    }
    NFA() {
        num_states = 0;
    }
    bool match(string text);
};

const int scan = -1;
bool NFA::match(string txt) {
    int n1, n2;
    deque<char> dq;
    vector<bool> in_progress(txt.size()+1, false);
    int j = 0, N = txt.size(), state = next1[0];
    dq.put(scan);
    while (state) {
        cout<<state<<" -> ";
        if (state == scan) {
            j++;
            dq.put(scan);
        } else if (ch[state] == txt[j]) {
            if (!in_progress[next1[state]]) {
                cout<<"matched on "<<txt[j]<<endl;
                dq.put(next1[state]);
            }
        } else if (ch[state] == ' ') {
            n1 = next1[state];
            n2 = next2[state];
            if (!in_progress[n1]) {
                dq.push(n1);
            }
            if (n1 != n2 && !in_progress[n2])
                dq.push(n2);
        }
        if (dq.empty() || j == N+1) {
            return false;
        }
        state = dq.pop();
        if (state >= 0 && state < in_progress.size())
            in_progress[state] = false;
    }
    return state == 0;
}

bool contains(NFA nfa, string txt) {
    for (int i = 0; i < txt.length(); i++) {
        if (nfa.match(txt.substr(i))) {
            return true;
        }
    }
    return false;
}

void matchAll(NFA nfa, string txt) {
    for (int i = 0; i < txt.length(); i++) {
        if (nfa.match(txt.substr(i)))
            cout<<"Match found at index: "<<i<<endl;
    }
}


void printNFA(NFA nfa) {
    for (int i = 0; i < nfa.num_states; i++) {
        cout<<i<<" ";
    }
    cout<<endl;
    for (int i = 0; i < nfa.num_states; i++) {
        cout<<nfa.ch[i]<<" ";
    }
    cout<<endl;
    for (int i = 0; i < nfa.num_states; i++) {
        cout<<nfa.next1[i]<<" ";
    }
    cout<<endl;
    for (int i = 0; i < nfa.num_states; i++) {
        cout<<nfa.next2[i]<<" ";
    }
    cout<<endl;
}

#endif