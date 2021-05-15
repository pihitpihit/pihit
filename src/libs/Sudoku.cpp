#include <iostream>
#include <fstream>
#include <string>
#include <vector>


using namespace std;

enum class SudokuCellType
{
	Initial,
	AutoRow,
	AutoCol,
	AutoLev,
	AutoSec,
	Manual,
	Working,
};

class SudokuCell
{
public:
	const string typeString()
	{
		switch(type)
		{
		case SudokuCellType::Initial: return "Initial";
		case SudokuCellType::AutoRow: return "AutoRow";
		case SudokuCellType::AutoCol: return "AutoCol";
		case SudokuCellType::AutoLev: return "AutoLev";
		case SudokuCellType::AutoSec: return "AutoSec";
		case SudokuCellType::Manual : return " Manual";
		case SudokuCellType::Working: return "Working";
		default: throw "Invalid Cell Type.";
		}
	}
public:
	int row;
	int col;
	int val;
	SudokuCellType type;
};

class Sudoku
{
public:
	Sudoku(const string& filePath);

public:
	void solve();
	void printAll();
	bool finished();
	bool put(int row, int col, int val);
	bool autoPut();
	bool validPut(int row, int col, int val);
	void pairing(bool verbose = false);

private:
	bool put(int row, int col, int val, SudokuCellType type);
	bool put(const SudokuCell& pos);

private:
	void printBoard();
	void printScore();
	void printMap();
	void printHistory();
	void showBorder(bool newLine = true);
	void print(int board[9][9], bool raw, bool toBool = false);
	void printLine(int board[9][9], int line, bool raw, bool toBool = false);
	int toDigit(const char ch);

private:
	bool valid();
	bool zeroScore();
	void updateScore();
	void updateScore(int i, int j);

private:
	bool forceEliminate();
	void eliminate(const SudokuCell& lastPos);
	void slice();
	void slice(int num);
	void pairingH(int sig[10], int level, bool verbose = false);
	void pairingV(int sig[10], int level, bool verbose = false);
	void pairingS(int sig[10], int level, bool verbose = false);


private:
	const int boardSize_;
	int board_[9][9];
	int score_[9][9];
	int map_[9+1][9][9];
	vector<SudokuCell> history_;
};


int main(int argc, char** argv)
{
	cout << "argc : " << argc << endl;
	for(int i = 0; i < argc; i++)
	{
		cout << "argv[" << i << "] = \"" << argv[i] << "\"" << endl;
	}

	Sudoku s(argv[1]);
	s.solve();
	s.printAll();
	return 0;
}


Sudoku::Sudoku(const string& filePath) :
	boardSize_(9)
{
	for(int i = 0; i < boardSize_; i++)
		for(int j = 0; j < boardSize_; j++)
			board_[i][j] = -1;

	for(int k = 1; k <= boardSize_; k++)
		for(int i = 0; i < boardSize_; i++)
			for(int j = 0; j < boardSize_; j++)
				map_[k][i][j] = 1;

	ifstream f;
	f.open(filePath);
	if(f.is_open())
	{
		int i = 0;
		while(i < boardSize_ && !f.eof())
		{
			char line[1024];
			f.getline(line, 1024);

			auto len = strlen(line);
			if(len != boardSize_)
			{
				throw "Invalid Line.";
			}
			for(int j = 0; j < boardSize_; j++)
			{
				if(!put(i, j, toDigit(line[j]), SudokuCellType::Initial))
				{
					throw "Can not Put.";
				}
			}
			i++;
		}
	}
}

void Sudoku::solve()
{
	struct SPos
	{
		int row;
		int col;
		int val;
	};

	vector<SPos> manualSequences = {
	};

	auto it = manualSequences.begin();

	while(!finished())
	{
		if(autoPut())
		{
			continue;
		}
		if(forceEliminate())
		{
			continue;
		}
		if(it == manualSequences.end())
		{
			break;
		}

		put(it->row, it->col, it->val);
		it++;
	}
}

bool Sudoku::finished()
{
	for(int i = 0; i < boardSize_; i++)
		for(int j = 0; j < boardSize_; j++)
			if(board_[i][j] == -1)
				return false;
	return true;
}

bool Sudoku::put(int row, int col, int val)
{
	return put(row, col, val, SudokuCellType::Manual);
}
bool Sudoku::put(int row, int col, int val, SudokuCellType type)
{
	SudokuCell pos = {.row = row, .col = col, .val = val, .type = type };
	return put(pos);
}

bool Sudoku::put(const SudokuCell& pos)
{
	int i = pos.row;
	int j = pos.col;
	int num = pos.val;

	if(num == 0 || 9 < num)
	{
		throw "Invalid Number.";
	}
	if(num == -1)
	{
		board_[i][j] = num;
		return true;
	}
	if(map_[num][i][j] == 0)
	{
		cout << "(" << i << ", " << j << ", " << num << ")" << endl;
		printAll();
		cout << "(" << i << ", " << j << ", " << num << ")" << endl;
		return false;
	}

	board_[i][j] = num;
	eliminate(pos);
	updateScore();
	history_.push_back(pos);
	return true;
}

bool Sudoku::autoPut()
{
	bool putFlag = false;

	/* auto put row unique and col unique */
	for(int l = 1; l <= boardSize_; l++)
	{
		for(int i = 0; i < boardSize_; i++)
		{
			int rTarget = -1;
			for(int j = 0; j < boardSize_; j++)
			{
				if(map_[l][i][j] == 0)
				{
					continue;
				}
				if(rTarget != -1)
				{
					rTarget = -1;
					break;
				}
				rTarget = j;
			}
			if(rTarget != -1)
			{
				put(i, rTarget, l, SudokuCellType::AutoRow);
				putFlag = true;
			}

			int cTarget = -1;
			for(int j = 0; j < boardSize_; j++)
			{
				if(map_[l][j][i] == 0)
				{
					continue;
				}
				if(cTarget != -1)
				{
					cTarget = -1;
					break;
				}
				cTarget = j;
			}
			if(cTarget != -1)
			{
				put(cTarget, i, l, SudokuCellType::AutoCol);
				putFlag = true;
			}
		}
	}

	/* auto put level unique */
	for(int i = 0; i < boardSize_; i++)
	{
		for(int j = 0; j < boardSize_; j++)
		{
			int lTarget = -1;
			for(int l = 1; l <= boardSize_; l++)
			{
				if(map_[l][i][j] == 0)
				{
					continue;
				}
				if(lTarget != -1)
				{
					lTarget = -1;
					break;
				}
				lTarget = l;
			}
			if(lTarget != -1)
			{
				put(i, j, lTarget, SudokuCellType::AutoLev);
				putFlag = true;
			}
		}
	}

	/* auto put sector unique */
	for(int l = 1; l <= boardSize_; l++)
	{
		for(int s = 0; s < boardSize_; s++)
		{
			int sTarget = -1;
			for(int i = 0; i < boardSize_; i++)
			{
				int row = s / 3 * 3 + i / 3;
				int col = s % 3 * 3 + i % 3;
				if(map_[l][row][col] == 0)
				{
					continue;
				}
				if(sTarget != -1)
				{
					sTarget = -1;
					break;
				}
				sTarget = i;
			}
			if(sTarget != -1)
			{
				int row = s / 3 * 3 + sTarget / 3;
				int col = s % 3 * 3 + sTarget % 3;
				put(row, col, l, SudokuCellType::AutoSec);
				putFlag = true;
			}
		}
	}

	return putFlag;
}

bool Sudoku::validPut(int row, int col, int val)
{
	Sudoku* dup = new Sudoku(*this);

	dup->put(row, col, val);
	dup->solve();

	bool result = dup->finished();
	delete dup;
	return result;
}

void Sudoku::showBorder(bool newLine)
{
	cout << "+---------+---------+---------+";
	if(newLine)
	{
		cout << endl;
	}
};

void Sudoku::print(int board[9][9], bool raw, bool toBool)
{
	auto showLine = [&](int i)
	{
		printLine(board, i, raw, toBool);
		cout << endl;
	};

	showBorder();
	showLine(0);
	showLine(1);
	showLine(2);
	showBorder();
	showLine(3);
	showLine(4);
	showLine(5);
	showBorder();
	showLine(6);
	showLine(7);
	showLine(8);
	showBorder();
}

void Sudoku::printLine(int board[9][9], int line, bool raw, bool toBool)
{
	auto toChar = [raw, toBool](int n)->char
	{
		if(toBool)
			return n == 0 ? ' ' : 'O';
		if(raw)
			return '0' + n;
		return n == -1 ? ' ' : ('0' + n);
	};
	auto showLineSection = [&](int i, int s)
	{
		cout << ' ' << toChar(board[i][s*3 + 0]) << ' ';
		cout << ' ' << toChar(board[i][s*3 + 1]) << ' ';
		cout << ' ' << toChar(board[i][s*3 + 2]) << ' ';
	};

	cout << "|";
	showLineSection(line, 0);
	cout << "|";
	showLineSection(line, 1);
	cout << "|";
	showLineSection(line, 2);
	cout << "|";
}

void Sudoku::printAll()
{
	printBoard();
	printScore();
	printMap();
	//printHistory();
}

void Sudoku::printBoard()
{
	cout << "+---------+---------+---------+" << endl;
	cout << "+ BOARD                       +" << endl;
	print(board_, false);
}

void Sudoku::printScore()
{
	cout << "+ SCORE                       +" << endl;
	print(score_, true );
}

void Sudoku::printMap()
{
	auto showBorder2 = [&]()
	{
		showBorder(false);
		showBorder(false);
		showBorder(true);
	};

	showBorder2();
	for(int i = 1; i <= 3; i++)
		cout << "+ MAP "<< i << "                       +";
	cout << endl;

	showBorder2();
	for(int l = 0; l < boardSize_; l++)
	{
		for(int i = 1; i <= 3; i++)
			printLine(map_[i], l, true, true );
		cout << endl;
		if(l % 3 == 2) showBorder2();
	}

	showBorder2();
	for(int i = 4; i <= 6; i++)
		cout << "+ MAP "<< i << "                       +";
	cout << endl;

	showBorder2();
	for(int l = 0; l < boardSize_; l++)
	{
		for(int i = 4; i <= 6; i++)
			printLine(map_[i], l, true, true );
		cout << endl;
		if(l % 3 == 2) showBorder2();
	}

	showBorder2();
	for(int i = 7; i <= 9; i++)
		cout << "+ MAP "<< i << "                       +";
	cout << endl;

	showBorder2();
	for(int l = 0; l < boardSize_; l++)
	{
		for(int i = 7; i <= 9; i++)
			printLine(map_[i], l, true, true );
		cout << endl;
		if(l % 3 == 2) showBorder2();
	}
}

void Sudoku::printHistory()
{
	int count = 0;
	for(auto pos : history_)
	{
		if(pos.type == SudokuCellType::Initial)
			continue;
		count++;

		printf("[PUT][%2d][%s] row=%d, col=%d, val=%d, cel=%d\n",
			   count,
			   pos.typeString().data(),
			   pos.row, pos.col,
			   pos.val,
			   pos.row/3*3 + pos.col/3);
	}
}

int Sudoku::toDigit(const char ch)
{
	switch(ch)
	{
	case '.':	return -1;
	case '1':	return  1;
	case '2':	return  2;
	case '3':	return  3;
	case '4':	return  4;
	case '5':	return  5;
	case '6':	return  6;
	case '7':	return  7;
	case '8':	return  8;
	case '9':	return  9;
	default :	throw "Invalid Character.";
	}
}

bool Sudoku::valid()
{
	return zeroScore() == finished();
}

bool Sudoku::zeroScore()
{
	updateScore();
	for(int i = 0; i < boardSize_; i++)
		for(int j = 0; j < boardSize_; j++)
			if(score_[i] != 0)
				return false;
	return true;
}

void Sudoku::updateScore()
{
	for(int i = 0; i < boardSize_; i++)
		for(int j = 0; j < boardSize_; j++)
			updateScore(i, j);
}

void Sudoku::updateScore(int i, int j)
{
	int score = 0;
	for(int l = 1; l <= boardSize_; l++)
		score += map_[l][i][j];
	score_[i][j] = score;
}

bool Sudoku::forceEliminate()
{
	int eliminated = false;
	for(int l = 1; l <= boardSize_; l++)
	{
		for(int i = 0; i < boardSize_; i++)
		{
			for(int j = 0; j < boardSize_; j++)
			{
				if(map_[l][i][j] == 0)
					continue;
				if(!validPut(i, j, l))
				{
					map_[l][i][j] = 0;
					eliminated = true;
				}
			}
		}
	}
	return eliminated;
}

void Sudoku::eliminate(const SudokuCell& lastPos)
{
	int i = lastPos.row;
	int j = lastPos.col;
	int num = lastPos.val;

	/* simple row/col/level/sector eliminate*/
	for(int l = 0; l < boardSize_; l++)
		map_[l+1][i][j] =
		map_[num][l][j] =
		map_[num][i][l] =
		map_[num][i/3*3 + l/3][j/3*3 + l%3] = 0;

	slice();
	pairing();
}

void Sudoku::slice()
{
	for(int l = 1; l <= boardSize_; l++)
		slice(l);
}

void Sudoku::slice(int num)
{
	auto sliceableS = [&](int s) -> int
	{
		int r[3] = { 0, };
		int c[3] = { 0, };

		for(int l = 0; l < 3; l++)
		{
			for(int m = 0; m < 3; m++)
			{
				auto v = map_[num][s/3*3 + l][s%3*3 + m];
				r[l] += v;
				c[m] += v;
			}
		}

		if(r[0] >= 2 && r[1] == 0 && r[2] == 0) return 0;
		if(r[0] == 0 && r[1] >= 2 && r[2] == 0) return 1;
		if(r[0] == 0 && r[1] == 0 && r[2] >= 2) return 2;

		if(c[0] >= 2 && c[1] == 0 && c[2] == 0) return 3;
		if(c[0] == 0 && c[1] >= 2 && c[2] == 0) return 4;
		if(c[0] == 0 && c[1] == 0 && c[2] >= 2) return 5;

		return -1;
	};
	auto sliceableH = [&](int r) -> int
	{
		int matched = -1;
		int matchedCount = 0;

		for(int j = 0; j < boardSize_; j++)
		{
			if(map_[num][r][j] != 1)
			{
				continue;
			}
			if(matched == -1)
			{
				matched = j/3;
				matchedCount++;
				continue;
			}
			if(matched == j/3)
			{
				matchedCount++;
				continue;
			}

			matched = -1;
			matchedCount = 0;
			break;
		}
		if(matchedCount < 2)
		{
			matched = -1;
		}
		return matched;
	};
	auto sliceableV = [&](int c) -> int
	{
		int matched = -1;
		int matchedCount = 0;

		for(int j = 0; j < boardSize_; j++)
		{
			if(map_[num][j][c] != 1)
			{
				continue;
			}
			if(matched == -1)
			{
				matched = j/3;
				matchedCount++;
				continue;
			}
			if(matched == j/3)
			{
				matchedCount++;
				continue;
			}

			matched = -1;
			matchedCount = 0;
			break;
		}
		if(matchedCount < 2)
		{
			matched = -1;
		}
		return matched;
	};

	for(int i = 0; i < boardSize_; i++)
	{
		bool sliced = false;

		auto res = sliceableS(i);
		if(0 <= res && res < 3)
		{
			for(int j = 0; j < boardSize_; j++)
				if(j < i%3*3 || i%3*3 + 3 <= j)
				{
					if(map_[num][i/3*3+res][j])
					{
						map_[num][i/3*3+res][j] = 0;
						sliced = true;
					}
				}
		}
		else if(3 <= res && res < 6)
		{
			res -= 3;
			for(int j = 0; j < boardSize_; j++)
				if(j < i/3*3 || i/3*3 + 3 <= j)
				{
					if(map_[num][j][i%3*3+res])
					{
						map_[num][j][i%3*3+res] = 0;
						sliced = true;
					}
				}
		}

		res = sliceableH(i);
		if(res != -1)
		{
			for(int j = 0; j < boardSize_; j++)
				if(res*3 <= j && j < res*3 + 3)
				{
					if(map_[num][i/3*3 + (i%3+1)%3][j])
					{
						map_[num][i/3*3 + (i%3+1)%3][j] = 0;
						sliced = true;
					}
					if(map_[num][i/3*3 + (i%3+2)%3][j])
					{
						map_[num][i/3*3 + (i%3+2)%3][j] = 0;
						sliced = true;
					}
				}
		}

		res = sliceableV(i);
		if(res != -1)
		{
			for(int j = 0; j < boardSize_; j++)
				if(res*3 <= j && j < res*3 + 3)
				{
					if(map_[num][j][i/3*3 + (i%3+1)%3])
					{
						map_[num][j][i/3*3 + (i%3+1)%3] = 0;
						sliced = true;
					}
					if(map_[num][j][i/3*3 + (i%3+2)%3])
					{
						map_[num][j][i/3*3 + (i%3+2)%3] = 0;
						sliced = true;
					}
				}
		}

		if(sliced)
		{
			i = 0;
		}
	}
}

void Sudoku::pairing(bool verbose)
{
	/* pairing eliminate*/
	for(int i = 1; i <= boardSize_; i++)
	{
		for(int j = i + 1; j <= boardSize_; j++)
		{
			int sig[10] = { 0, };
			sig[i] = 1;
			sig[j] = 1;
			pairingH(sig, 2);
			pairingV(sig, 2);
			pairingS(sig, 2);
		}
	}
	for(int i = 1; i <= boardSize_; i++)
	{
		for(int j = i + 1; j <= boardSize_; j++)
		{
			for(int k = j + 1; k <= boardSize_; k++)
			{
				int sig[10] = { 0, };
				sig[i] = 1;
				sig[j] = 1;
				sig[k] = 1;
				pairingH(sig, 3, verbose);
				pairingV(sig, 3, verbose);
				pairingS(sig, 3, verbose);
			}
		}
	}
}
void Sudoku::pairingH(int sig[10], int level, bool verbose)
{
	for(int i = 0; i < boardSize_; i++)
	{
		vector<SudokuCell> matched;
		for(int j = 0; j < boardSize_; j++)
		{
			int matchedCount = 0;
			for(int l = 1; l <= boardSize_; l++)
			{
				if(map_[l][i][j] == 1)
				{
					if(sig[l] == 1)
					{
						matchedCount++;
					}
					else
					{
						matchedCount = 0;
						break;
					}
				}
			}
			if(0 < matchedCount)
			{
				SudokuCell cell = { .row = i, .col = j };
				matched.push_back(cell);
				if(level < matched.size())
				{
					matched.clear();
					break;
				}
			}
		}
		if(matched.size() == level)
		{
			vector<int> rSig;
			for(int l = 1; l <= boardSize_; l++)
				if(sig[l] == 1) rSig.push_back(l);

			if(verbose)
			{
				cout << "[PAIRING-H DETECTED] level:" << level;
				cout << ", sig(";
				for(auto n : rSig) cout << n << ", ";
				cout << ")";
				for(auto pos : matched) cout << ", pos(" << pos.row << ", " << pos.col << ")";
				cout << endl;
			}

			for(int j = 0; j < boardSize_; j++)
			{
				bool pass = false;
				for(auto& pos : matched)
				{
					if(j == pos.col)
					{
						pass = true;
						break;
					}
				}
				if(pass == false)
				{
					for(int l : rSig)
					{
						map_[l][i][j] = 0;
					}
				}
			}
		}
	}
}

void Sudoku::pairingV(int sig[10], int level, bool verbose)
{
	for(int j = 0; j < boardSize_; j++)
	{
		vector<SudokuCell> matched;
		for(int i = 0; i < boardSize_; i++)
		{
			int matchedCount = 0;
			for(int l = 1; l <= boardSize_; l++)
			{
				if(map_[l][i][j] == 1)
				{
					if(sig[l] == 1)
					{
						matchedCount++;
					}
					else
					{
						matchedCount = 0;
						break;
					}
				}
			}
			if(0 < matchedCount)
			{
				SudokuCell cell = { .row = i, .col = j };
				matched.push_back(cell);
				if(level < matched.size())
				{
					matched.clear();
					break;
				}
			}
		}
		if(matched.size() == level)
		{
			vector<int> rSig;
			for(int l = 1; l <= boardSize_; l++)
				if(sig[l] == 1) rSig.push_back(l);

			if(verbose)
			{
				cout << "[PAIRING-V DETECTED] level:" << level;
				cout << ", sig(";
				for(auto n : rSig) cout << n << ", ";
				cout << ")";
				for(auto pos : matched) cout << ", pos(" << pos.row << ", " << pos.col << ")";
				cout << endl;
			}

			for(int i = 0; i < boardSize_; i++)
			{
				bool pass = false;
				for(auto& pos : matched)
				{
					if(i == pos.row)
					{
						pass = true;
						break;
					}
				}
				if(pass == false)
				{
					for(int l : rSig)
					{
						map_[l][i][j] = 0;
					}
				}
			}
		}
	}
}

void Sudoku::pairingS(int sig[10], int level, bool verbose)
{
	for(int i = 0; i < boardSize_; i++)
	{
		vector<SudokuCell> matched;
		for(int j = 0; j < boardSize_; j++)
		{
			int matchedCount = 0;
			int row = i/3*3 + j/3;
			int col = i%3*3 + j%3;

			for(int l = 1; l <= boardSize_; l++)
			{
				if(map_[l][row][col] == 1)
				{
					if(sig[l] == 1)
					{
						matchedCount++;
					}
					else
					{
						matchedCount = 0;
						break;
					}
				}
			}
			if(0 < matchedCount)
			{
				SudokuCell cell = { .row = row, .col = col };
				matched.push_back(cell);
				if(level < matched.size())
				{
					matched.clear();
					break;
				}
			}
		}
		if(matched.size() == level)
		{
			vector<int> rSig;
			for(int l = 1; l <= boardSize_; l++)
				if(sig[l] == 1) rSig.push_back(l);

			if(verbose)
			{
				cout << "[PAIRING-S DETECTED] level:" << level;
				cout << ", sig(";
				for(auto n : rSig) cout << n << ", ";
				cout << ")";
				for(auto pos : matched) cout << ", pos(" << pos.row << ", " << pos.col << ")";
				cout << endl;
			}

			for(int j = 0; j < boardSize_; j++)
			{
				bool pass = false;
				int row = i/3*3 + j/3;
				int col = i%3*3 + j%3;

				for(auto& pos : matched)
				{
					if(row == pos.row && col == pos.col)
					{
						pass = true;
						break;
					}
				}
				if(pass == false)
				{
					for(int l : rSig)
					{
						map_[l][row][col] = 0;
					}
				}
			}
		}
	}
}
