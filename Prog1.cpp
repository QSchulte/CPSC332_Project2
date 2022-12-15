#include <vector>
#include <string>
#include <iostream>

#define LOG(v)  \
std::cout << "Current stack: ";  \
for (auto& x : v) std::cout << x << " ";  \
std::cout << std::endl;

/*
Column 1: 'a' | Column 2: '+' | Column 3: '-' | Column 4: '*' | Column 5: '/' | 
Column 6 = '(' | Column 7 = ')' | Column 8 = '$' 

Row 1: 'E' | Row 2: 'Q' | Row 3: 'T' | Row 4: 'R' | Row 5: 'F' 
*/
class ParsingTable {
public:
	std::vector<std::vector<int> > table{ 7, std::vector<int>(9,0) };
	ParsingTable() {
		for (int i = 0; i < table.size(); ++i) {
			table[i][0] = 0;
		}
		for (int i = 0; i < table[0].size(); ++i) {
			table[0][i] = 0;
		}
        table[1][1] = 11; // 11 = TQ
        table[1][6] = 16; // 16 = TQ
        table[2][2] = 22; // 22 = +TQ
        table[2][3] = 23; // 23 = -TQ
        table[2][7] = 27; // 27 = epsilon
        table[2][8] = 28; // 28 = epsilon
        table[3][1] = 31; // 31 = FR
        table[3][6] = 36; // 36 = FR
        table[4][2] = 42; // 42 = epsilon
        table[4][3] = 43; // 43 = epsilon
        table[4][4] = 44; // 44 = *FR
        table[4][5] = 45; // 45 = /FR
        table[4][7] = 46; // 46 = epsilon
        table[4][8] = 47; // 47 = epsilon
        table[5][1] = 51; // 51 = a
        table[5][6] = 56; // 56 = (E)
	}

	// Given the row and column, return the state.
	int Get(int row, int col) {
		return table[row][col];
	}
};

int ConvertToRow(char c) {
	switch (c)
	{
	case 'E':
		return 1;
	case 'Q':
		return 2;
	case 'T':
		return 3;
	case 'R':
		return 4;
	case 'F':
		return 5;
	default:
		return 0;
		break;
	}
}

int ConvertToCol(char c) {
	switch (c)
	{
	case 'a':
		return 1;
	case '+':
		return 2;
	case '-':
		return 3;
	case '*':
		return 4;
	case '/':
		return 5;
	case '(':
		return 6;
	case ')':
		return 7;
	case '$':
		return 8;
	default:
		return 0;
		break;
	}
}

bool push_backToStack(std::vector<char>& stk, int x) {
	switch (x)
	{
	case 0:
		return false;
    case 11:
    case 16:
        std::cout << "Pusing TQ onto the stack." << std::endl;
        stk.push_back('Q');
        stk.push_back('T');
		LOG(stk);
        return true;
        break;
    case 22:
        std::cout << "Pushing +TQ onto the stack." << std::endl;
		stk.push_back('Q');
		stk.push_back('T');
		stk.push_back('+');
		LOG(stk);
		return true;
		break;
    case 23:
        std::cout << "Pushing -TQ onto the stack." << std::endl;
		stk.push_back('Q');
		stk.push_back('T');
		stk.push_back('-');
		LOG(stk);
		return true;
		break;
    case 27:
    case 28:
    case 42:
    case 43:
    case 47:
    case 48:
        std::cout << "Pushing ^ (epsilon) onto the stack." << std::endl;
		stk.push_back('^');	// ^ = epsilon
		LOG(stk);
		return true;
		break;
    case 31:
    case 36:
    	std::cout << "Pushing FR onto the stack." << std::endl;
		stk.push_back('R');
		stk.push_back('F');
		LOG(stk);
		return true;
		break;
    case 44:
        std::cout << "Pushing *FR onto the stack." << std::endl;
		stk.push_back('R');
		stk.push_back('F');
		stk.push_back('*');
		LOG(stk);
		return true;
		break;
    case 45:
        std::cout << "Pushing /FR onto the stack." << std::endl;
		stk.push_back('R');
		stk.push_back('F');
		stk.push_back('/');
		LOG(stk);
		return true;
		break;
    case 51:
        std::cout << "Pushing 'a' onto the stack." << std::endl;
		stk.push_back('a');
		LOG(stk);
		return true;
		break;
    case 56:
        std::cout << "Pushing (E) onto the stack." << std::endl;
		stk.push_back(')');
		stk.push_back('E');
		stk.push_back('(');
		LOG(stk);
		return true;
		break;
    default:
		return false;
		break;
	}
}

int main(){
	ParsingTable parse_table;

	std::string input_string;

	char state;

	bool accepted = true;

	std::vector<char> stk;

	std::cout << "Enter string" << std::endl;
	std::cin >> input_string;

	LOG(stk);
	do {
		state = stk.back();
		
		char input = input_string[0];

		// Look ahead to the next input symbol.
		char next = input_string[1];

		// If state is Epsilon, simply pop epsilon from the stack.
		if (state == '^') 
		{
			std::cout << "Popping ^ (Epsilon) from the stack." << std::endl;
			stk.pop_back();
			LOG(stk);
		}
		// If state is a terminal or $ and it is equal to input, pop the stack and read the string.
		// If state does not equal input, then we know that input string must be rejected.
		else if (state == '(' || state == ')' || state == '$' || state == 'a' || 
			state == '+' || state == '-' || state == '*' || state == '/'){
			if (state == input) {
					std::cout << "Popping from stack: " << stk.back() << std::endl;
					stk.pop_back();
					LOG(stk);
					input_string.erase(0, 1);
					std::cout << "Input: " << input_string << std::endl;
				}
			else {
				std::cout << "Rejected." << std::endl;
				accepted = false;
				break;
			}
		}
		// If state is non-terminal, then retrieve the states from the parsing table.
		// Then, pop the state off of the stack and push the new states onto the stack.
		// If there are no new states retrieved from the parsing table, then the input is rejected.
		else if (state == 'E' || state == 'T' || state == 'Q' || state == 'F' || state == 'R') 
		{
			int row = ConvertToRow(state);
			int col = ConvertToCol(input);
			if (parse_table.Get(row,col) != 0 || (!(row == 6 && col == 2)) || (!(row == 6 && col == 3))) {
				std::cout << "Popping from stack: " << stk.back() << std::endl;
				stk.pop_back();
				LOG(stk);
				if (!(push_backToStack(stk, parse_table.Get(row, col)))) {
					accepted = false;
					break;
				}
			}
			else {
				std::cout << "Rejected." << std::endl;
				accepted = false;
				break;
			}
		}

	} while (state != '$');	// Repeat until we reach $

	if (accepted) 
		std::cout << "Accepted.";
	else 
		std::cout << "Rejected.";

	std::cin.ignore();
	std::cin.get();

	return 0;
}