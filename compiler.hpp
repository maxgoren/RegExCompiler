#ifndef compiler_hpp
#define compiler_hpp
#include <iostream>
#include "nfa.hpp"
using namespace std;

class Compiler {
    private:
        int d;
        bool trace;
        void s(string str) {
            if (trace) {
                for (int i = 0; i < d; i++) 
                    cout<<" ";
                cout<<str<<endl;
            }
        }
        void e(string str) { ++d; s(str); }
        void l(string str) { s(str); --d; }
        void l() { --d; }
    private:
        int j;
        int state;
        string p;
        NFA nfa;
        bool match(char c) {
            if (p[j] == c)
                return true;
            return false;
        }
        void nextChar() {
            j++;
        }
        bool letter(char c) {
            return c != '?' && c != '.' && c != '+' && c != '*' && c != '|' && c != '(' && c != ')' && c != '\0';
        }
        void setstate(int idx, char c, int n1, int n2) {
            cout<<"Set state: "<<idx<<" for "<<c<<" with "<<n1<<", "<<n2<<endl;
            nfa.ch[idx] = c;
            nfa.next1[idx] = n1;
            nfa.next2[idx] = n2;
        }
        void error(string str) {
            cout<<"Error: "<<str<<endl;
            cout<<"state: "<<state<<endl;
            cout<<"p[j]:  "<<p[j]<<endl;
            exit(0);
        }
        int closures(int enter_state, int prev_symbol_state) {
            int result;
            if (match('*')) {
                s("adding kleene closure");
                setstate(state, ' ', state+1, prev_symbol_state);
                result = state;
                nfa.next1[enter_state-1] = state++;
                nextChar(); 
            } else if (match('+')) {
                s("adding 1 or more closure");
                setstate(state, ' ', state+1, prev_symbol_state);
                result = state++;
                nextChar();
            } else if (match('?')) {
                s("0 or 1 closure");
                setstate(state, ' ', state+1, state+1);
                result = state;
                nfa.next1[enter_state-1] = state++;
                nextChar();
            } else {
                 result = prev_symbol_state;
            }
            l("returning " + to_string(result) + "from factor");
            return result;
        }
        int factor() {
            e("factor()");
            int enter_state, prev_symbol_state;
            enter_state = state;
            if (match('(')) {   // (expr)
                nextChar();
                prev_symbol_state = expression();
                if (match(')')) nextChar(); 
                else error("Missing close paren");
            } else if (letter(p[j])) { // e, r -> er
                string m = "adding state for ";
                m.push_back(p[j]);
                s(m);
                setstate(state,p[j], state+1,state+1);
                prev_symbol_state = state; 
                nextChar(); 
                state++;
            } else if (match('.')) {
                s("adding state for wildcard");
                setstate(state, p[j], state+1, state+1);
                prev_symbol_state = state;
                nextChar();
                state++;
            } else {
                error("Invalid symbol detected");
                return -9;
            }
            return closures(enter_state, prev_symbol_state);
        }
        int term() {
            int t, r;
            e("term()");
            r = factor();
            if (match('(') || letter(p[j]) || match('.')) t = term();
            l("returning " + to_string(r) + " from term");
            return r;
        }
        int expression() {
            e("expression()");
            int t1, t2, r;
            t1 = term();
            r = t1;
            if (match('|')) {
                s("adding or state");
                nextChar();
                state++;
                t2 = state; r = t2; state++;
                setstate(t2, ' ', expression(), t1);
                setstate(t2-1, ' ', state, state);
            }
            l("returning " + to_string(r) + " from expression ");
            return r;
        }
        void init(string pat) {
            d = 0;
            p = pat;
            j = 0;
            state = 0;
            nfa = NFA(p.size()+1);
            cout<<"Initializing NFA for "<<p<<endl;
        }
    public:
        Compiler(bool tracemsg = false) {
            trace = tracemsg;
        }
        NFA compile(string pat) {
            init(pat);
            j = 0; state = 1;
            int t = expression();
            setstate(0, ' ', t, t);
            setstate(state, ' ', 0, 0);
            printNFA(nfa);
            return nfa;
        }

};

#endif