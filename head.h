#pragma once
//head.h
#ifndef HEAD_H
#define HEAD_H

#include <iostream>
#include <stdio.h>
#include <cctype>
#include <stack>
#include <string>
#include <map>
#include <set>
#include <vector>
#include<iterator>
#include <fstream>

using namespace std;

/*����NFA��DFA����Ҫ�Ľṹ��*/
//NFA�Ľڵ�
struct node
{
	string nodeName;
};

//NFA�ı�
struct edge
{
	node startName;	//��ʼ��
	node endName;	//Ŀ���
	char tranSymbol;	//ת������
};


//NFA����ɵ�Ԫ��һ�����NFA��Ԫ�������ɺܶ�С��Ԫͨ������ƴ������
struct elem
{
	int edgeCount;	//����
	edge edgeSet[100];	//��NFAӵ�еı�
	node startName;	//��ʼ״̬
	node endName; //����״̬
};

// ���� DFA ��״̬
struct DFAState {
	set<string> nfaStates;	//һ������NFA״̬�ļ���
	string stateName;
};

// ���� DFA ��ת����ϵ
struct DFATransition {
	DFAState fromState;
	DFAState toState;
	char transitionSymbol;
};

/*������ת��ΪDFA����Ҫ����*/

// ���� NFA ״̬�Ħűհ�
DFAState eClosure(const set<string>& nfaStates, elem nfa);

// ���� DFA ��״̬ת��
DFAState move(const DFAState& dfaState, char transitionSymbol, elem nfa);

// ��� DFA ״̬�Ƿ���״̬������
bool isDFAStateInVector(const vector<DFAState>& dfaStates, const DFAState& targetState);

//���ת�����Ƿ��ڱ߼����У�����a->b�Ƿ��Ѿ��ڼ�����
bool isTransitionInVector(DFAState, DFAState, char, vector<DFATransition>);

//NFAת��ΪDFA
void buildDFAFromNFA(const elem& NFA_Elem, vector<DFAState>& dfaStates, vector<DFATransition>& dfaTransitions);

// ��ʾ DFA ״̬��ת�ƹ�ϵ
void displayDFA(const vector<DFAState>& dfaStates, const vector<DFATransition>& dfaTransitions);

//����dot�ļ�
void generateDotFile_DFA(vector<DFAState>& dfaStates, vector<DFATransition>& dfaTransitions);


/*�����ǹ���NFA����Ҫ����*/
//�����½ڵ�
node new_node();

//���� a
elem act_Elem(char);

//����a|b
elem act_Unit(elem, elem);

//��ɵ�Ԫ��������
void elem_copy(elem&, elem);

//����ab
elem act_join(elem, elem);

//���� a*
elem act_star(elem);

void input(string&);

string add_join_symbol(string);	//������Ԫƴ����һ���൱���м���һ��+����ab�൱��a+b

class infixToPostfix {
public:
	infixToPostfix(const string& infix_expression);

	int is_letter(char check);
	int ispFunc(char c);
	int icpFunc(char c);
	void infToPost();
	string getResult();

private:
	string infix;
	string postfix;
	map<char, int> isp;
	map<char, int> icp;
};

elem express_to_NFA(string);

void Display(elem);

int is_letter(char check);

void generateDotFile_NFA(const elem& nfa);
#endif
