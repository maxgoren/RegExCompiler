#ifndef compiler_hpp
#define compiler_hpp
#include <iostream>
#include "nfa.hpp"
using namespace std;


class Compiler {
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
            return c != '*' && c != '|' && c != '(' && c != ')' && c != '\0';
        }
        void setstate(int idx, char c, int n1, int n2) {
            cout<<"Set state: "<<idx<<" for "<<c<<" with "<<n1<<", "<<n2<<endl;
            nfa.ch[idx] = c;
            nfa.next1[idx] = n1;
            nfa.next2[idx] = n2;
        }
        void error() {
            cout<<"Wagwan Error!"<<endl;
            cout<<"state: "<<state<<endl;
            cout<<"j:     "<<j<<endl;
            exit(0);
        }
        int factor() {
            int t1, t2, r;
            t1 = state;
            if (match('(')) {   // (expr)
                nextChar();
                t2 = expression();
                if (match(')')) 
                    nextChar(); 
                else {
                    cout<<"Missing close paren"<<endl;
                    error();
                }
            } else if (letter(p[j])) { // e, r -> er
                setstate(state,p[j], state+1,state+1);
                t2 = state; 
                nextChar(); 
                state++;
            } else {
                cout<<"Invalid character: '"<<p[j]<<"'"<<endl;
                error();
            }
            if (match('*')) {
                setstate(state, ' ', state+1, t2);
                r = state;
                nfa.next1[t1-1] = state;
                nextChar(); 
                state++;
            } else {
                 r = t2;
            }
            return r;
        }
        int term() {
            int t, r;
            r = factor();
            if (match('(') || letter(p[j])) t = term();
            return r;
        }
        int expression() {
            int t1, t2, r;
            t1 = term();
            r = t1;
            if (match('|')) {
                nextChar();
                state++;
                t2 = state; r = t2; state++;
                setstate(t2, ' ', expression(), t1);
                setstate(t2-1, ' ', state, state);
            }
            return r;
        }
        void init(string pat) {
            p = pat;
            j = 0;
            state = 0;
            nfa = NFA(p.size()+1);
            cout<<"Ok, initialized for "<<p<<endl;
        }
    public:
        Compiler() {

        }
        NFA compile(string pat) {
            init(pat);
            j = 0; state = 1;
            int t = expression();
            setstate(0, ' ', t, t);
            setstate(state, ' ', 0, 0);
            return nfa;
        }

};

#endif