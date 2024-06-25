
//func.cpp

#include "head.h"

int nodeNum = 0;

/*������ת��ΪDFA����Ҫ����*/

// ���� NFA ״̬�Ħűհ�
DFAState eClosure(const set<string>& nfaStates, elem nfa) {
	DFAState eClosureState;
	eClosureState.nfaStates = nfaStates;

	stack<string> stateStack;

	// ��ʼ��ջ������ʼ״̬����ջ���ʼnfaState��ֻ��NFA_Elem.startName
	for (const string& nfaState_name : nfaStates) {
		stateStack.push(nfaState_name);
	}

	while (!stateStack.empty()) {
		string currentState = stateStack.top();
		stateStack.pop();

		// ���� NFA �ı�
		for (int i = 0; i < nfa.edgeCount; i++) {
			edge currentEdge = nfa.edgeSet[i];

			// ����ߵ���ʼ״̬�ǵ�ǰ״̬�����ұߵ�ת��������#����ô��Ŀ��״̬����űհ�
			if (currentEdge.startName.nodeName == currentState && currentEdge.tranSymbol == '#') {
				// ���Ŀ��״̬�Ƿ��Ѿ��ڦűհ��У������ظ����
				if (eClosureState.nfaStates.find(currentEdge.endName.nodeName) == eClosureState.nfaStates.end()) {
					eClosureState.nfaStates.insert(currentEdge.endName.nodeName);
					// ��Ŀ��״̬����ջ�Ա��һ������
					stateStack.push(currentEdge.endName.nodeName);
				}
			}
		}
	}

	// Ϊ�űհ�����һ��Ψһ������
	for (const string& nfaState_name : eClosureState.nfaStates) {
		eClosureState.stateName += nfaState_name;
	}

	return eClosureState;
}

//move����
DFAState move(const DFAState& dfaState, char transitionSymbol, elem nfa) {
	DFAState nextState;

	// ���� DFAState �е�ÿ�� NFA ״̬
	for (const string& nfaState_name : dfaState.nfaStates) {
		// ������������� NFA ״̬�ı�
		for (int i = 0; i < nfa.edgeCount; i++) {
			edge currentEdge = nfa.edgeSet[i];

			// ����ߵ���ʼ״̬�ǵ�ǰ״̬���ұߵ�ת�����ŵ���������ţ���Ŀ��״̬���� nextState
			if (currentEdge.startName.nodeName == nfaState_name && currentEdge.tranSymbol == transitionSymbol && currentEdge.tranSymbol != '#') {
				nextState.nfaStates.insert(currentEdge.endName.nodeName);
			}
		}
	}

	// Ϊ nextState ����һ��Ψһ������
	for (const string& nfaState_name : nextState.nfaStates) {
		nextState.stateName += nfaState_name;
	}

	return nextState;
}

// ��� DFA ״̬�Ƿ���״̬������,��dfaStates����û���ҵ�targetState
bool isDFAStateInVector(const vector<DFAState>& dfaStates, const DFAState& targetState) {
	for (const DFAState& state : dfaStates) {
		if (state.stateName == targetState.stateName) {
			return true; // �ҵ�ƥ���״̬
		}
	}
	return false; // û���ҵ�ƥ���״̬
}

//���ת�����Ƿ��ڱ߼����У�����a->b�Ƿ��Ѿ��ڼ�����
bool isTransitionInVector(DFAState dfaState, DFAState dfaNextState, char symbol, vector<DFATransition> dfaTransitions)
{
	for (const DFATransition& transition : dfaTransitions) {
		if (transition.fromState.stateName == dfaState.stateName && dfaNextState.stateName == dfaNextState.stateName && symbol == transition.transitionSymbol) {
			return true;	//�ҵ�ƥ���״̬
		}
	}
	return false;
}

void buildDFAFromNFA(const elem& NFA_Elem, vector<DFAState>& dfaStates, vector<DFATransition>& dfaTransitions) {
	// ��ʼ�� DFA ״̬���Ϻ�ת����ϵ
	set<string> nfaInitialStateSet;
	nfaInitialStateSet.insert(NFA_Elem.startName.nodeName);
	DFAState dfaInitialState = eClosure(nfaInitialStateSet, NFA_Elem); // ���� NFA ��ʼ״̬�� �űհ�
	dfaStates.push_back(dfaInitialState);

	// ��ʼ���� DFA
	for (int i = 0; i < dfaStates.size(); i++) {
		DFAState dfaState = dfaStates[i];
		for (int j = 0; j < NFA_Elem.edgeCount; j++) {
			char symbol = NFA_Elem.edgeSet[j].tranSymbol;
			DFAState nextState = move(dfaState, symbol, NFA_Elem);
			DFAState dfaNextState = eClosure(nextState.nfaStates, NFA_Elem);

			if (!nextState.nfaStates.empty()) {
				// �����һ��״̬��Ϊ�գ����� DFA ״̬�����л�δ��ӣ������ DFA ״̬����
				if (!isDFAStateInVector(dfaStates, dfaNextState)) {
					dfaStates.push_back(dfaNextState);
				}
				// ���ڱ�ҲҪȥ�أ���Ϊ����a�ı߿��ܻ����������
				// �����ǰ���� DFA ת����ϵ�л�δ��ӣ������ DFA ת����ϵ
				if (!isTransitionInVector(dfaState, dfaNextState, symbol, dfaTransitions)) {
					dfaTransitions.push_back({ dfaState, dfaNextState, symbol });
				}
			}
		}
	}
}

// ��ʾ DFA ״̬��ת�ƹ�ϵ��������ʼ�ͽ���״̬
void displayDFA(const vector<DFAState>& dfaStates, const vector<DFATransition>& dfaTransitions) {
	cout << "DFA States:" << endl;

	for (const DFAState& state : dfaStates) {
		cout << "State " << state.stateName << " (NFA States: ";
		for (const string& nfaState_name : state.nfaStates) {
			cout << nfaState_name << " ";
		}
		cout << ")";
		if (state.stateName == dfaStates.front().stateName) {
			cout << " (Initial State)";
		}
		if (state.stateName == dfaStates.back().stateName) {
			cout << " (Final State)";
		}
		cout << endl;
	}

	cout << "DFA Transitions:" << endl;
	for (const DFATransition& transition : dfaTransitions) {
		cout << "State " << transition.fromState.stateName << " --(" << transition.transitionSymbol << ")--> State " << transition.toState.stateName << endl;
	}
}

//����DFA��dot�ļ�
void generateDotFile_DFA(vector<DFAState>& dfaStates, vector<DFATransition>& dfaTransitions) {
	std::ofstream dotFile("dfa_graph.dot");

	if (dotFile.is_open()) {
		dotFile << "digraph DFA {\n";
		dotFile << "  rankdir=LR;  // ���򲼾�\n\n";
		dotFile << " node [shape = circle];   // ��ʼ״̬\n\n";
		dotFile << dfaStates.back().stateName << "[shape = doublecircle];\n";
		// ���DFA״̬
		for (const auto& state : dfaStates) {
			dotFile << "  " << state.stateName;
			dotFile << " [label=\"State " << state.stateName;
			if (state.stateName == dfaStates.front().stateName) dotFile << "\\n(startState)";
			if (state.stateName == dfaStates.back().stateName) {
				dotFile << "\\n(endState)";
			}
			dotFile << "\"];\n";
		}
		dotFile << "\n";

		// ���DFAת��
		for (const auto& transition : dfaTransitions) {
			dotFile << "  " << transition.fromState.stateName << " -> " << transition.toState.stateName << " [label=\"" << transition.transitionSymbol << "\"];\n";
		}

		dotFile << "}\n";

		dotFile.close();
		std::cout << "DFA DOT file generated successfully.\n";
	}
	else {
		std::cerr << "Unable to open DOT file.\n";
	}
}

/*�����ǹ���NFA����Ҫ����*/

//�����½ڵ�
node new_node()
{
	node newNode;
	newNode.nodeName = nodeNum + 65;//�������ô�д��ĸ��ʾ
	nodeNum++;
	return newNode;
}

//��������������ʽ
void input(string& RE)
{
	cout << "������������ʽ��  ����������() * |;�ַ�����a~z A~Z��" << endl;
	cin >> RE;
}


//��ɵ�Ԫ��������
void elem_copy(elem& dest, elem source)
{
	for (int i = 0; i < source.edgeCount; i++) {
		dest.edgeSet[dest.edgeCount + i] = source.edgeSet[i];
	}
	dest.edgeCount += source.edgeCount;
}

//���� a
elem act_Elem(char c)
{
	//�½ڵ�
	node startNode = new_node();
	node endNode = new_node();

	//�±�
	edge newEdge;
	newEdge.startName = startNode;
	newEdge.endName = endNode;
	newEdge.tranSymbol = c;

	//��NFA���Ԫ�أ�С��NFAԪ��/��Ԫ)
	elem newElem;
	newElem.edgeCount = 0;	//��ʼ״̬
	newElem.edgeSet[newElem.edgeCount++] = newEdge;
	newElem.startName = newElem.edgeSet[0].startName;
	newElem.endName = newElem.edgeSet[0].endName;

	return newElem;
}

//����a|b
elem act_Unit(elem fir, elem sec)
{
	elem newElem;
	newElem.edgeCount = 0;
	edge edge1, edge2, edge3, edge4;

	//����µ�״̬�ڵ�
	node startNode = new_node();
	node endNode = new_node();

	//����e1����������AB����ʼ��A��
	edge1.startName = startNode;
	edge1.endName = fir.startName;
	edge1.tranSymbol = '#';

	//����e2����������CD����ʼ��C��
	edge2.startName = startNode;
	edge2.endName = sec.startName;
	edge2.tranSymbol = '#';

	//����e3������AB���յ���յ㣩
	edge3.startName = fir.endName;
	edge3.endName = endNode;
	edge3.tranSymbol = '#';

	//����e4������CD���յ���յ㣩
	edge4.startName = sec.endName;
	edge4.endName = endNode;
	edge4.tranSymbol = '#';

	//��fir��sec�ϲ�
	elem_copy(newElem, fir);
	elem_copy(newElem, sec);

	//�¹�����4����
	newElem.edgeSet[newElem.edgeCount++] = edge1;
	newElem.edgeSet[newElem.edgeCount++] = edge2;
	newElem.edgeSet[newElem.edgeCount++] = edge3;
	newElem.edgeSet[newElem.edgeCount++] = edge4;

	newElem.startName = startNode;
	newElem.endName = endNode;

	return newElem;
}

//���� N(s)N(t)
elem act_join(elem fir, elem sec)
{
	//��fir�Ľ���״̬��sec�Ŀ�ʼ״̬�ϲ�����sec�ı߸��Ƹ�fir����fir����
	//��sec��������StartState��ͷ�ı�ȫ���޸�
	for (int i = 0; i < sec.edgeCount; i++) {
		if (sec.edgeSet[i].startName.nodeName.compare(sec.startName.nodeName) == 0)
		{
			sec.edgeSet[i].startName = fir.endName; //�ñ�e1�Ŀ�ʼ״̬����N(t)����ʼ״̬
		}
		else if (sec.edgeSet[i].endName.nodeName.compare(sec.startName.nodeName) == 0) {
			sec.edgeSet[i].endName = fir.endName; //�ñ�e2�Ľ���״̬����N(t)����ʼ״̬
		}
	}
	sec.startName = fir.endName;

	elem_copy(fir, sec);

	//��fir�Ľ���״̬����Ϊsec�Ľ���״̬
	fir.endName = sec.endName;
	return fir;
}

//����a*
elem act_star(elem Elem)
{
	elem newElem;
	newElem.edgeCount = 0;
	edge edge1, edge2, edge3, edge4;

	//�����״̬�ڵ�
	node startNode = new_node();
	node endNode = new_node();

	//e1
	edge1.startName = startNode;
	edge1.endName = endNode;
	edge1.tranSymbol = '#';	//�հ�ȡ�մ�

	//e2
	edge2.startName = Elem.endName;
	edge2.endName = Elem.startName;
	edge2.tranSymbol = '#';

	//e3
	edge3.startName = startNode;
	edge3.endName = Elem.startName;
	edge3.tranSymbol = '#';

	//e4
	edge4.startName = Elem.endName;
	edge4.endName = endNode;
	edge4.tranSymbol = '#';

	//������Ԫ
	elem_copy(newElem, Elem);

	//���¹����������߼���EdgeSet
	newElem.edgeSet[newElem.edgeCount++] = edge1;
	newElem.edgeSet[newElem.edgeCount++] = edge2;
	newElem.edgeSet[newElem.edgeCount++] = edge3;
	newElem.edgeSet[newElem.edgeCount++] = edge4;

	//����NewElem����ʾ״̬�ͽ���״̬
	newElem.startName = startNode;
	newElem.endName = endNode;

	return newElem;
}

int is_letter(char check) {
	if (check >= 'a' && check <= 'z' || check >= 'A' && check <= 'Z')
		return true;
	return false;
}
//
string add_join_symbol(string add_string)
{
	int length = add_string.size();
	int return_string_length = 0;
	char* return_string = new char[2 * length + 2];//���������
	char first, second;
	for (int i = 0; i < length - 1; i++)
	{
		first = add_string.at(i);
		second = add_string.at(i + 1);
		return_string[return_string_length++] = first;
		//Ҫ�ӵĿ�������ab �� *b �� a( �� )b �����
		//���ڶ�������ĸ����һ������'('��'|'��Ҫ���
		if (first != '(' && first != '|' && is_letter(second))
		{
			return_string[return_string_length++] = '+';
		}
		//���ڶ�����'(',��һ������'|'��'(',ҲҪ��
		else if (second == '(' && first != '|' && first != '(')
		{
			return_string[return_string_length++] = '+';
		}
	}
	//�����һ���ַ�д��second
	return_string[return_string_length++] = second;
	return_string[return_string_length] = '\0';
	string STRING(return_string);
	cout << "��'+'��ı��ʽ��" << STRING << endl;
	return STRING;
}

//����ĸ���Ԫ�ض���
infixToPostfix::infixToPostfix(const string& infix_expression) : infix(infix_expression), postfix("") {
	isp = { {'+', 3}, {'|', 5}, {'*', 7},  {'(', 1}, {')', 8}, {'#', 0} };
	icp = { {'+', 2}, {'|', 4}, {'*', 6}, {'(', 8}, {')', 1}, {'#', 0} };
}

int infixToPostfix::is_letter(char check) {
	if (check >= 'a' && check <= 'z' || check >= 'A' && check <= 'Z')
		return true;
	return false;
}

int infixToPostfix::ispFunc(char c) {
	int priority = isp.count(c) ? isp[c] : -1;
	if (priority == -1) {
		cerr << "error: ����δ֪���ţ�" << endl;
		exit(1);  // �쳣�˳�
	}
	return priority;
}

int infixToPostfix::icpFunc(char c) {
	int priority = icp.count(c) ? icp[c] : -1;
	if (priority == -1) {
		cerr << "error: ����δ֪���ţ�" << endl;
		exit(1);  // �쳣�˳�
	}
	return priority;
}

void infixToPostfix::infToPost() {
	string infixWithHash = infix + "#";
	stack<char> stack;
	int loc = 0;
	while (!stack.empty() || loc < infixWithHash.size()) {
		if (is_letter(infixWithHash[loc])) {
			postfix += infixWithHash[loc];
			loc++;
		}
		else {
			char c1 = (stack.empty()) ? '#' : stack.top();
			char c2 = infixWithHash[loc];
			if (ispFunc(c1) < icpFunc(c2)) {
				stack.push(c2);
				loc++;
			}
			else if (ispFunc(c1) > icpFunc(c2)) {
				postfix += c1;
				stack.pop();
			}
			else {
				if (c1 == '#' && c2 == '#') {
					break;
				}
				stack.pop();
				loc++;
			}
		}
	}
}

string infixToPostfix::getResult() {
	postfix = ""; // ��ս��
	infToPost();
	return postfix;
}

/**���ʽתNFA������,�������յ�NFA����
*/
elem express_to_NFA(string expression)
{
	int length = expression.size();
	char element;
	elem Elem, fir, sec;
	stack<elem> STACK;
	for (int i = 0; i < length; i++)
	{
		element = expression.at(i);
		switch (element)
		{
		case '|':
			sec = STACK.top();
			STACK.pop();
			fir = STACK.top();
			STACK.pop();
			Elem = act_Unit(fir, sec);
			STACK.push(Elem);
			break;
		case '*':
			fir = STACK.top();
			STACK.pop();
			Elem = act_star(fir);
			STACK.push(Elem);
			break;
		case '+':
			sec = STACK.top();
			STACK.pop();
			fir = STACK.top();
			STACK.pop();
			Elem = act_join(fir, sec);
			STACK.push(Elem);
			break;
		default:
			Elem = act_Elem(element);
			STACK.push(Elem);
		}
	}
	cout << "�ѽ�������ʽת��ΪNFA!" << endl;
	Elem = STACK.top();
	STACK.pop();

	return Elem;
}

//��ӡNFA
void Display(elem Elem) {
	cout << "NFA States:" << endl;
	cout << "Start State: " << Elem.startName.nodeName << endl;
	cout << "End State: " << Elem.endName.nodeName << endl;

	cout << "NFA Transitions:" << endl;
	for (int i = 0; i < Elem.edgeCount; i++) {
		cout << "Edge " << i + 1 << ": ";
		cout << Elem.edgeSet[i].startName.nodeName << " --(" << Elem.edgeSet[i].tranSymbol << ")--> ";
		cout << Elem.edgeSet[i].endName.nodeName << endl;
	}

	cout << "End" << endl;
}

//����NFAdot�ļ�
void generateDotFile_NFA(const elem& nfa) {
	std::ofstream dotFile("nfa_graph.dot");

	if (dotFile.is_open()) {
		dotFile << "digraph NFA {\n";
		dotFile << "  rankdir=LR;  // ���򲼾�\n\n";
		dotFile << " node [shape = circle];   // ״̬�ڵ�\n\n";

		dotFile << nfa.endName.nodeName << " [shape=doublecircle];\n";
		// ��� NFA ״̬
		dotFile << "  " << nfa.startName.nodeName << " [label=\"Start State: " << nfa.startName.nodeName << "\"];\n";
		dotFile << "  " << nfa.endName.nodeName << " [label=\"End State: " << nfa.endName.nodeName << "\"];\n";

		// ��� NFA ת��
		for (int i = 0; i < nfa.edgeCount; i++) {
			const edge& currentEdge = nfa.edgeSet[i];
			dotFile << "  " << currentEdge.startName.nodeName << " -> " << currentEdge.endName.nodeName << " [label=\"" << currentEdge.tranSymbol << "\"];\n";
		}

		dotFile << "}\n";

		dotFile.close();
		std::cout << "NFA DOT file generated successfully.\n";
	}
	else {
		std::cerr << "Unable to open NFA DOT file.\n";
	}
}
//main.cpp
#include "head.h" // �����ṩ��ͷ�ļ�

int main() {
	string Regular_Expression;
	elem NFA_Elem;
	input(Regular_Expression);
	if (Regular_Expression.length() > 1)    Regular_Expression = add_join_symbol(Regular_Expression);
	infixToPostfix Solution(Regular_Expression);
	//��׺ת��׺
	cout << "��׺���ʽΪ��";
	Regular_Expression = Solution.getResult();
	cout << Regular_Expression << endl;
	//���ʽתNFA
	NFA_Elem = express_to_NFA(Regular_Expression);
	//��ʾ
	Display(NFA_Elem);
	//����NFAdot�ļ�
	generateDotFile_NFA(NFA_Elem);

	// ��ʼ�� DFA ״̬���Ϻ�ת����ϵ
	vector<DFAState> dfaStates; //���ڴ洢���е�DFA״̬
	vector<DFATransition> dfaTransitions; //���ڴ洢DFA״̬֮���ת��
	set<string> nfaInitialStateSet;   //�洢NFA�ĳ�ʼ״̬
	buildDFAFromNFA(NFA_Elem, dfaStates, dfaTransitions);//��NFA����DFA
	// ��ʾ DFA
	displayDFA(dfaStates, dfaTransitions);

	//����DFAdot�ļ�
	generateDotFile_DFA(dfaStates, dfaTransitions);
	return 0;
}
