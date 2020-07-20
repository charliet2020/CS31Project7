// dinos.cpp

// Portions you are to complete are marked with a TODO: comment.
// We've provided some incorrect return statements (so indicated) just
// to allow this skeleton program to compile and run, albeit incorrectly.
// The first thing you probably want to do is implement the trivial
// functions (marked TRIVIAL).  Then get Valley::display going.  That gives
// you more flexibility in the order you tackle the rest of the functionality.
// As you finish implementing each TODO: item, remove its TODO: comment.

#include <iostream>
#include <string>
#include <random>
#include <utility>
#include <cstdlib>
#include <cassert>
#define CHECKTYPE(f, t) { (void)static_cast<t>(f); }
using namespace std;

///////////////////////////////////////////////////////////////////////////
// Manifest constants
///////////////////////////////////////////////////////////////////////////

const int MAXROWS = 20;        // max number of rows in a valley
const int MAXCOLS = 40;        // max number of columns in a valley
const int MAXDINOSAURS = 170;  // max number of dinosaurs allowed

const int UP = 0;
const int DOWN = 1;
const int LEFT = 2;
const int RIGHT = 3;
const int NUMDIRS = 4;

///////////////////////////////////////////////////////////////////////////
// Type definitions
///////////////////////////////////////////////////////////////////////////

class Valley;  // This is needed to let the compiler know that Valley is a
			   // type name, since it's mentioned in the Dinosaur declaration.

class Dinosaur
{
public:
	// Constructor
	Dinosaur(Valley* vp, int r, int c);

	// Accessors
	int  row() const;
	int  col() const;

	// Mutators
	void move();

private:
	Valley* m_valley;
	int     m_row;
	int     m_col;
};

class Player
{
public:
	// Constructor
	Player(Valley *vp, int r, int c);

	// Accessors
	int  row() const;
	int  col() const;
	int  age() const;
	bool isDead() const;

	// Mutators
	void   stand();
	void   move(int dir);
	bool   shoot(int dir);
	void   setDead();

private:
	Valley* m_valley;
	int     m_row;
	int     m_col;
	int     m_age;
	bool    m_dead;
};

class Valley
{
public:
	// Constructor/destructor
	Valley(int nRows, int nCols);
	~Valley();

	// Accessors
	int     rows() const;
	int     cols() const;
	Player* player() const;
	int     dinosaurCount() const;
	int     numDinosaursAt(int r, int c) const;
	void    display(string msg) const;

	// Mutators
	bool    addDinosaur(int r, int c);
	bool    addPlayer(int r, int c);
	bool    destroyDinosaur(int r, int c);
	bool    moveDinosaurs();

private:
	int        m_rows;
	int        m_cols;
	Player*    m_player;
	Dinosaur*  m_dinos[MAXDINOSAURS];
	int        m_nDinos;
};

class Game
{
public:
	// Constructor/destructor
	Game(int rows, int cols, int nDinos);
	~Game();

	// Mutators
	void play();

private:
	Valley* m_valley;
};

///////////////////////////////////////////////////////////////////////////
//  Auxiliary function declarations
///////////////////////////////////////////////////////////////////////////

int decodeDirection(char dir);
int randInt(int min, int max);
void clearScreen();

///////////////////////////////////////////////////////////////////////////
//  Dinosaur implementation
///////////////////////////////////////////////////////////////////////////

Dinosaur::Dinosaur(Valley* vp, int r, int c)
{
	if (vp == nullptr)
	{
		cout << "***** A dinosaur must be created in some Valley!" << endl;
		exit(1);
	}
	if (r < 1 || r > vp->rows() || c < 1 || c > vp->cols())
	{
		cout << "***** Dinosaur created with invalid coordinates (" << r << ","
			<< c << ")!" << endl;
		exit(1);
	}
	m_valley = vp;
	m_row = r;
	m_col = c;
}

int Dinosaur::row() const
{
	return m_row;
}

int Dinosaur::col() const
{
	// Return the number of the column the Dinosaur is at. 
	return m_col;  
}

void Dinosaur::move()
{
	// Attempt to move in a random direction; if we can't move, don't move
	switch (randInt(0, NUMDIRS - 1))
	{
	case UP:
		if (m_row > 1)
			m_row--;
		//   Move the dinosaur up one row if possible.
		break;
	case DOWN:
		if (m_row < m_valley->rows())
			m_row++;
		break;
	case LEFT:
		if (m_col > 1)
			m_col--;
		break;
	case RIGHT:
		if (m_col < m_valley->cols())
			m_col++;
		//  Implement the other movements.
		break;
	}
}

///////////////////////////////////////////////////////////////////////////
//  Player implementations
///////////////////////////////////////////////////////////////////////////

Player::Player(Valley* vp, int r, int c)
{
	if (vp == nullptr)
	{
		cout << "***** The player must be created in some Valley!" << endl;
		exit(1);
	}
	if (r < 1 || r > vp->rows() || c < 1 || c > vp->cols())
	{
		cout << "**** Player created with invalid coordinates (" << r
			<< "," << c << ")!" << endl;
		exit(1);
	}
	m_valley = vp;
	m_row = r;
	m_col = c;
	m_age = 0;
	m_dead = false;
}

int Player::row() const
{
	//  Return the number of the row the player is at.
	return m_row;  
}

int Player::col() const
{
	// Return the number of the column the player is at.
	return m_col;  
}

int Player::age() const
{
	//   Return the player's age.
	return m_age;  
}

void Player::stand()
{
	m_age++;
}

void Player::move(int dir)
{
	m_age++;

	// Attempt to move in the indicated direction; if we can't move, don't move
	switch (dir)
	{
	case UP:
		if (m_row > 1)
			m_row--;
		//   Move the player up one row if possible.
		break;
	case DOWN:
		if (m_row < m_valley->rows())
			m_row++;
		break;
	case LEFT:
		if (m_col > 1)
			m_col--;
		break;
	case RIGHT:
		if (m_col < m_valley->cols())
			m_col++;
		//   Implement the other movements.
		break;
	}
}

bool Player::shoot(int dir)
{
	m_age++;
	int distanceRow = 20;
	int distanceCol = 40;
	if (randInt(1, 3) > 1)  // miss with 2/3 probability
		return false;
	if (dir != UP && dir != DOWN && dir != LEFT && dir != RIGHT)
		return false;
	if (dir == UP)
	{
		for (int w = m_row - 1; w > 0; w--)
		{
			if (m_valley->destroyDinosaur(w, m_col))
				return true;
		}
	}
	if (dir == DOWN)
	{
		for (int s = m_row + 1; s <= MAXROWS; s++)
		{
			if (m_valley->destroyDinosaur(s, m_col))
				return true;
		}
	}
	if (dir == LEFT)
	{
		for (int a = m_col-1 ; a > 0 ; a--)
		{
			if (m_valley->destroyDinosaur(m_row, a))
				return true;
		}
	}
	if (dir == RIGHT)
	{
		for (int d = m_col + 1; d <= MAXCOLS; d++)
		{
			if (m_valley->destroyDinosaur(m_row, d))
				return true;
		}
	}
	//   Destroy the nearest dinosaur in direction dir
	return false;  // Replace this line with the correct code.
}

bool Player::isDead() const
{
	// Return whether the player is dead.
	return m_dead;  
}

void Player::setDead()
{
	m_dead = true;
}

///////////////////////////////////////////////////////////////////////////
//  Valley implementations
///////////////////////////////////////////////////////////////////////////

Valley::Valley(int nRows, int nCols)
{
	if (nRows <= 0 || nCols <= 0 || nRows > MAXROWS || nCols > MAXCOLS)
	{
		cout << "***** Valley created with invalid size " << nRows << " by "
			<< nCols << "!" << endl;
		exit(1);
	}
	m_rows = nRows;
	m_cols = nCols;
	m_player = nullptr;
	m_nDinos = 0;
}

Valley::~Valley()
{
	delete m_player;
	for (int i = 0; i < m_nDinos; i++)
	{
		delete m_dinos[i];
	}
	// TODO:  Delete the player and all remaining dynamically allocated dinosaurs.
}

int Valley::rows() const
{
	// Return the number of rows in the valley.
	return m_rows;  
}

int Valley::cols() const
{
	// Return the number of columns in the valley.
	return m_cols;  
}

Player* Valley::player() const
{
	return m_player;
}

int Valley::dinosaurCount() const
{
	return m_nDinos;
}

int Valley::numDinosaursAt(int r, int c) const
{
	int count = 0;
	for (int i = 0; i < m_nDinos; i++)
	{
		if (r == m_dinos[i]->row() && c == m_dinos[i]->col())
			count++;
	}
	//  Return the number of dinosaurs at row r, column c.
	return count;  // Replace this incorrect line with the correct code.
}

void Valley::display(string msg) const
{
	// Position (row,col) in the valley coordinate system is represented in
	// the array element grid[row-1][col-1]
	char grid[MAXROWS][MAXCOLS];
	int r, c;

	// Fill the grid with dots
	for (r = 0; r < rows(); r++)
		for (c = 0; c < cols(); c++)
			grid[r][c] = '.';

	if (m_nDinos != 0)
	{
		cerr << "check" << endl;
		for (int m = 0; m < m_nDinos; m++)
		{
			char& gridCharDino = grid[m_dinos[m]->row()-1][m_dinos[m]->col()-1];
			if (gridCharDino == '.')
				gridCharDino = 'Z';
			else if (gridCharDino == 'Z')
				gridCharDino = '2';
			else if (gridCharDino == '2')
				gridCharDino = '3';
			else if (gridCharDino == '3')
				gridCharDino = '4';
			else if (gridCharDino == '4')
				gridCharDino = '5';
			else if (gridCharDino == '5')
				gridCharDino = '6';
			else if (gridCharDino == '6')
				gridCharDino = '7';
			else if (gridCharDino == '7')
				gridCharDino = '8';
			else
				gridCharDino = '9';
		}
	}
	// Indicate each dinosaur's position
	// If one dinosaur is at some position, set the char to 'Z'.
	//        If it's 2 through 8, set it to '2' through '8'.
	//        For 9 or more, set it to '9'.

	// Indicate player's position
	if (m_player != nullptr)
	{
		// Set the char to '@', unless there's also a dinosaur there,
		// in which case set it to '*'.
		char& gridChar = grid[m_player->row() - 1][m_player->col() - 1];
		if (gridChar == '.')
			gridChar = '@';
		else
			gridChar = '*';
	}

	// Draw the grid
	clearScreen();
	for (r = 0; r < rows(); r++)
	{
		for (c = 0; c < cols(); c++)
			cout << grid[r][c];
		cout << endl;
	}
	cout << endl;

	// Write message, dinosaur, and player info
	cout << endl;
	if (msg != "")
		cout << msg << endl;
	cout << "There are " << dinosaurCount() << " dinosaurs remaining." << endl;
	if (m_player == nullptr)
		cout << "There is no player." << endl;
	else
	{
		if (m_player->age() > 0)
			cout << "The player has lasted " << m_player->age() << " steps." << endl;
		if (m_player->isDead())
			cout << "The player is dead." << endl;
	}
}

bool Valley::addDinosaur(int r, int c)
{
	if (m_nDinos >= MAXDINOSAURS)
		return false;
	m_dinos[m_nDinos] = new Dinosaur(this, r, c);
	m_nDinos++;

	// If MAXDINOSAURS have already been added, return false.  Otherwise,
	// dynamically allocate a new dinosaur at coordinates (r,c).  Save the
	// pointer to the newly allocated dinosaur and return true.

	
	return true;  
}

bool Valley::addPlayer(int r, int c)
{
	// Don't add a player if one already exists
	if (m_player != nullptr)
		return false;

	// Dynamically allocate a new Player and add it to the valley
	m_player = new Player(this, r, c);
	return true;
}

bool Valley::destroyDinosaur(int r, int c)
{
	for (int i = 0; i < m_nDinos; i++)
	{
		
		if (r == m_dinos[i]->row() && c == m_dinos[i]->col())
		{
			delete m_dinos[i];
			for (int j = i; j < m_nDinos - 1; j++)
			{
				m_dinos[j] = m_dinos[j + 1];
			}
			m_dinos[m_nDinos] = nullptr;
			m_nDinos--;
			
			return true;
		}
	}
		//  Destroy one dinosaur at row r, column c, and return true.
		//       Return false is there is no dinosaur there to destroy.
	return false;  // Replace this incorrect line with the correct code.
}

bool Valley::moveDinosaurs()
{
	for (int k = 0; k < m_nDinos; k++)
	{
		m_dinos[k]->move();
		if ((m_dinos[k]->col()==m_player->col()) && (m_dinos[k]->row()==m_player->row()))
		{
			m_player->setDead();
		}
		//  Have the k-th dinosaur in the valley make one move.
		//        If that move results in the dinosaur being at the same
		//        position as the player, the player dies.
	}

	// return true if the player is still alive, false otherwise
	return !m_player->isDead();
}

///////////////////////////////////////////////////////////////////////////
//  Game implementations
///////////////////////////////////////////////////////////////////////////

Game::Game(int rows, int cols, int nDinos)
{
	if (nDinos < 0)
	{
		cout << "***** Cannot create Game with negative number of Dinosaurs!" << endl;
		exit(1);
	}
	if (nDinos > MAXDINOSAURS)
	{
		cout << "***** Trying to create Game with " << nDinos
			<< " dinosaurs; only " << MAXDINOSAURS << " are allowed!" << endl;
		exit(1);
	}
	if (rows == 1 && cols == 1 && nDinos > 0)
	{
		cout << "***** Cannot create Game with nowhere to place the Dinosaurs!" << endl;
		exit(1);
	}

	// Create valley
	m_valley = new Valley(rows, cols);

	// Add player
	int rPlayer = randInt(1, rows);
	int cPlayer = randInt(1, cols);
	m_valley->addPlayer(rPlayer, cPlayer);

	// Populate with dinosaurs
	while (nDinos > 0)
	{
		int r = randInt(1, rows);
		int c = randInt(1, cols);
		// Don't put a dinosaur where the player is
		if (r == rPlayer  &&  c == cPlayer)
			continue;
		m_valley->addDinosaur(r, c);
		nDinos--;
	}
}

Game::~Game()
{
	delete m_valley;
}

void Game::play()
{
	string msg = "";
	m_valley->display(msg);
	Player* player = m_valley->player();
	if (player == nullptr)
		return;

	while (!m_valley->player()->isDead() && m_valley->dinosaurCount() > 0)
	{
		cout << "Move (u/d/l/r/su/sd/sl/sr/q or nothing): ";
		string action;
		getline(cin, action);
		if (action.size() == 0)
			player->stand();
		else
		{
			switch (action[0])
			{
			default:   // if bad move, nobody moves
				cout << '\a' << endl;  // beep
				continue;
			case 'q':
				return;
			case 'u':
			case 'd':
			case 'l':
			case 'r':
				player->move(decodeDirection(action[0]));
				break;
			case 's':
				if (action.size() < 2)  // if no direction, nobody moves
				{
					cout << '\a' << endl;  // beep
					continue;
				}
				switch (action[1])
				{
				default:   // if bad direction, nobody moves
					cout << '\a' << endl;  // beep
					continue;
				case 'u':
				case 'd':
				case 'l':
				case 'r':
					if (player->shoot(decodeDirection(action[1])))
						msg = "Hit!";
					else
						msg = "Missed!";
					break;
				}
				break;
			}
		}
		if (!player->isDead())
			m_valley->moveDinosaurs();
		m_valley->display(msg);
		msg = "";
	}
	if (m_valley->player()->isDead())
		cout << "You lose." << endl;
	else
		cout << "You win." << endl;
}

///////////////////////////////////////////////////////////////////////////
//  Auxiliary function implementations
///////////////////////////////////////////////////////////////////////////

int decodeDirection(char dir)
{
	switch (dir)
	{
	case 'u':  return UP;
	case 'd':  return DOWN;
	case 'l':  return LEFT;
	case 'r':  return RIGHT;
	}
	return -1;  // bad argument passed in!
}

// Return a random int from min to max, inclusive
int randInt(int min, int max)
{
	if (max < min)
		swap(max, min);
	static random_device rd;
	static default_random_engine generator(rd());
	uniform_int_distribution<> distro(min, max);
	return distro(generator);
}

///////////////////////////////////////////////////////////////////////////
//  main()
///////////////////////////////////////////////////////////////////////////
void doBasicTests();

void thisFunctionWillNeverBeCalled()
{
	// If the student deleted or changed the interfaces to the public
	// functions, this won't compile.  (This uses magic beyond the scope
	// of CS 31.)

	Dinosaur(static_cast<Valley*>(0), 1, 1);
	CHECKTYPE(&Dinosaur::row, int  (Dinosaur::*)() const);
	CHECKTYPE(&Dinosaur::col, int  (Dinosaur::*)() const);
	CHECKTYPE(&Dinosaur::move, void (Dinosaur::*)());

	Player(static_cast<Valley*>(0), 1, 1);
	CHECKTYPE(&Player::row, int  (Player::*)() const);
	CHECKTYPE(&Player::col, int  (Player::*)() const);
	CHECKTYPE(&Player::age, int  (Player::*)() const);
	CHECKTYPE(&Player::isDead, bool (Player::*)() const);
	CHECKTYPE(&Player::stand, void (Player::*)());
	CHECKTYPE(&Player::move, void (Player::*)(int));
	CHECKTYPE(&Player::shoot, bool (Player::*)(int));
	CHECKTYPE(&Player::setDead, void (Player::*)());

	Valley(1, 1);
	CHECKTYPE(&Valley::rows, int     (Valley::*)() const);
	CHECKTYPE(&Valley::cols, int     (Valley::*)() const);
	CHECKTYPE(&Valley::player, Player* (Valley::*)() const);
	CHECKTYPE(&Valley::dinosaurCount, int     (Valley::*)() const);
	CHECKTYPE(&Valley::numDinosaursAt, int     (Valley::*)(int, int) const);
	CHECKTYPE(&Valley::display, void    (Valley::*)(string) const);
	CHECKTYPE(&Valley::addDinosaur, bool    (Valley::*)(int, int));
	CHECKTYPE(&Valley::addPlayer, bool    (Valley::*)(int, int));
	CHECKTYPE(&Valley::destroyDinosaur, bool    (Valley::*)(int, int));
	CHECKTYPE(&Valley::moveDinosaurs, bool    (Valley::*)());

	Game(1, 1, 1);
	CHECKTYPE(&Game::play, void (Game::*)());
}

void doBasicTests()
{
	{
		Valley v(10, 20);
		assert(v.addPlayer(2, 6));
		Player* pp = v.player();
		assert(pp->row() == 2 && pp->col() == 6 && !pp->isDead());
		pp->move(UP);
		assert(pp->row() == 1 && pp->col() == 6 && !pp->isDead());
		pp->move(UP);
		assert(pp->row() == 1 && pp->col() == 6 && !pp->isDead());
		pp->setDead();
		assert(pp->row() == 1 && pp->col() == 6 && pp->isDead());
	}
	{
		Valley v(2, 2);
		assert(v.addPlayer(1, 1));
		assert(v.addDinosaur(2, 2));
		Player* pp = v.player();
		assert(v.moveDinosaurs());
		assert(!pp->isDead());
		for (int k = 0; k < 1000 && v.moveDinosaurs(); k++)
			;
		assert(pp->isDead());
	}
	{
		Valley v(1, 40);
		assert(v.addPlayer(1, 1));
		assert(v.addDinosaur(1, 40));
		assert(v.addDinosaur(1, 40));
		assert(v.addDinosaur(1, 39));
		assert(v.dinosaurCount() == 3 && v.numDinosaursAt(1, 40) == 2);
		Player* pp = v.player();
		for (int k = 0; k < 1000 && v.dinosaurCount() != 0; k++)
			pp->shoot(RIGHT);
		assert(v.dinosaurCount() == 0);
		assert(v.addDinosaur(1, 40));
		for (int k = 0; k < 38; k++)
		{
			v.moveDinosaurs();
			pp->stand();
		}
		assert(v.dinosaurCount() == 1);
	}
	cout << "Passed all basic tests" << endl;
}

/*
///////////////////////////////////////////////////////////////////
// Test Case main fuction
///////////////////////////////////////////////////////////////////

#include <iostream>
#include <sstream>
#include <streambuf>
#include <string>
#include <map>
#include <cstdlib>
#include <cassert>
using namespace std;

class streambuf_switcher
{
public:
	streambuf_switcher(ios& dest, ios& src)
		: dest_stream(dest), saved_streambuf(dest.rdbuf(src.rdbuf()))
	{}
	~streambuf_switcher()
	{
		dest_stream.rdbuf(saved_streambuf);
	}
private:
	ios& dest_stream;
	streambuf* saved_streambuf;
};

map<void*, size_t> allocMap;
bool recordaddrs = false;

inline bool isRecordedSize(size_t n)
{
	return n == sizeof(Flatulan) || n == sizeof(Player);
}

void* operator new(size_t n)
{
	void* p = malloc(n);
	fill_n(static_cast<char*>(p), n, 0xca);
	if (recordaddrs  &&  isRecordedSize(n))
	{
		recordaddrs = false;
		allocMap.insert(make_pair(p, n));
		recordaddrs = true;
	}
	return p;
}

void unrecordaddr(void* p)
{
	recordaddrs = false;
	auto it = allocMap.find(p);
	if (it != allocMap.end())
	{
		fill_n(static_cast<char*>(p), it->second, 0xcb);
		allocMap.erase(it);
	}
	recordaddrs = true;
}

#if __cplusplus >= 201402L  &&  ! defined(__clang__)
// Unless clang is supplied the -fsized-deallocation flag, it will
// not call the C++14 sized operator delete.

void operator delete(void* p) noexcept
{
	free(p);
}

void operator delete(void* p, size_t n) noexcept
{
	if (recordaddrs  &&  isRecordedSize(n))
		unrecordaddr(p);
	operator delete(p);
}
#else
void operator delete(void* p) noexcept
{
	if (recordaddrs)
		unrecordaddr(p);
	free(p);
}
#endif

void testone(int n)
{
	streambuf_switcher sso(cout, cerr);

	switch (n)
	{
	default: {
		cout << "Bad argument" << endl;
	} break; case  1: {
		Valley c(10, 18);
		c.addPlayer(2, 2);
		Dinosaur f(&c, 5, 17);
		assert(f.row() == 5 && f.col() == 17);
	} break; case  2: {
		bool movedr = false;
		bool movedl = false;
		bool stayed = false;
		for (int k = 0; k < 150; k++)
		{
			City c(1, 30);
			c.addPlayer(1, 1);
			Flatulan f(&c, 1, 29);
			assert(f.row() == 1 && f.col() == 29);
			for (int n = 0; n < 2; n++)
			{
				int oldc = f.col();
				f.move();
				assert(f.row() == 1 && f.col() <= 30);
				int d = f.col() - oldc;
				switch (d)
				{
				case  0:  stayed = true; break;
				case  1:  movedr = true; break;
				case -1:  movedl = true; break;
				default:  assert(false);
				}
			}
		}
		assert(movedr  &&  movedl  &&  stayed);
	} break; case  3: {
		bool movedd = false;
		bool movedu = false;
		bool stayed = false;
		for (int k = 0; k < 150; k++)
		{
			City c(20, 1);
			c.addPlayer(1, 1);
			Flatulan f(&c, 19, 1);
			assert(f.row() == 19 && f.col() == 1);
			for (int n = 0; n < 2; n++)
			{
				int oldr = f.row();
				f.move();
				assert(f.col() == 1 && f.row() <= 20);
				int d = f.row() - oldr;
				switch (d)
				{
				case  0:  stayed = true; break;
				case  1:  movedd = true; break;
				case -1:  movedu = true; break;
				default:  assert(false);
				}
			}
		}
		assert(movedd  &&  movedu  &&  stayed);
	} break; case  4: {
		City c(20, 20);
		c.addPlayer(1, 1);
		Flatulan f(&c, 12, 12);
		for (int k = 0; k < 8; k++)
		{
			int oldr = f.row();
			int oldc = f.col();
			f.move();
			assert((f.row() == oldr  &&  abs(f.col() - oldc) == 1) ||
				(f.col() == oldc  &&  abs(f.row() - oldr) == 1));
		}
	} break; case  5: {
		const int dr[NUMDIRS + 1] = { -1,  0,  1,  0,  0 };
		const int dc[NUMDIRS + 1] = { 0,  1,  0, -1,  0 };
		for (int k = 0; k < NUMDIRS; k++)
		{
			int nvisits[NUMDIRS + 1] = { 0 };
			for (int n = 0; n < 100; n++)
			{
				City c(20, 20);
				c.addPlayer(10, 10);
				int oldr = 10 - dr[k];
				int oldc = 10 - dc[k];
				Flatulan f(&c, oldr, oldc);
				f.move();
				int rdiff = f.row() - oldr;
				int cdiff = f.col() - oldc;
				int i;
				for (i = 0; i < NUMDIRS + 1; i++)
				{
					if (rdiff == dr[i] && cdiff == dc[i])
					{
						nvisits[i]++;
						break;
					}
				}
				assert(i < NUMDIRS + 1);
			}
			for (int i = 0; i < NUMDIRS + 1; i++)
				assert(k == i ? nvisits[i] == 0 : nvisits[i] > 1);
		}
	} break; case  6: {
		int n = 0;
		for (int k = 0; k < 600; k++)
		{
			City c(10, 20);
			c.addPlayer(1, 1);
			Flatulan f(&c, 1, 2);
			if (f.possiblyGetConverted())
				n++;
		}
		assert(n >= 360 && n <= 440);
	} break; case  7: {
		City c(10, 20);
		Player p(&c, 2, 3);
		assert(p.row() == 2 && p.col() == 3 && !p.isPassedOut());
	} break; case  8: {
		City c(10, 20);
		Player p(&c, 2, 3);
		for (int k = 0; k < 10; k++)
		{
			assert(!p.isPassedOut());
			p.getGassed();
		}
		assert(p.isPassedOut());
	} break; case  9: {
		City c(10, 20);
		Player p(&c, 2, 3);
		for (int k = 0; k < 10; k++)
		{
			assert(p.health() == 10 - k);
			p.getGassed();
		}
		assert(p.isPassedOut());
	} break; case 10: {
		City c(10, 18);
		c.addPlayer(10, 17);
		Player* pp = c.player();
		pp->move(RIGHT);
		assert(pp->row() == 10 && pp->col() == 18 && !pp->isPassedOut());
		pp->move(DOWN);
		assert(pp->row() == 10 && pp->col() == 18 && !pp->isPassedOut());
		pp->move(RIGHT);
		assert(pp->row() == 10 && pp->col() == 18 && !pp->isPassedOut());
	} break; case 11: {
		City c(10, 18);
		c.addPlayer(2, 1);
		Player* pp = c.player();
		pp->move(UP);
		assert(pp->row() == 1 && pp->col() == 1 && !pp->isPassedOut());
		pp->move(LEFT);
		assert(pp->row() == 1 && pp->col() == 1 && !pp->isPassedOut());
		pp->move(UP);
		assert(pp->row() == 1 && pp->col() == 1 && !pp->isPassedOut());
	} break; case 12: {
		City c(10, 20);
		c.addPlayer(8, 12);
		Player* pp = c.player();
		pp->move(UP);
		assert(pp->row() == 7 && pp->col() == 12);
		pp->move(DOWN);
		assert(pp->row() == 8 && pp->col() == 12);
		pp->move(LEFT);
		assert(pp->row() == 8 && pp->col() == 11);
		pp->move(RIGHT);
		assert(pp->row() == 8 && pp->col() == 12);
	} break; case 13: {
		City c(10, 18);
		c.addPlayer(5, 7);
		c.addFlatulan(3, 7);
		c.addFlatulan(4, 9);
		c.addFlatulan(6, 8);
		c.addFlatulan(5, 6);
		Player* pp = c.player();
		pp->move(UP);
		assert(pp->row() == 4 && pp->col() == 7 && !pp->isPassedOut());
		pp->move(UP);
		assert(pp->row() == 4 && pp->col() == 7 && !pp->isPassedOut());
		pp->move(RIGHT);
		assert(pp->row() == 4 && pp->col() == 8 && !pp->isPassedOut());
		pp->move(RIGHT);
		assert(pp->row() == 4 && pp->col() == 8 && !pp->isPassedOut());
		pp->move(DOWN);
		assert(pp->row() == 5 && pp->col() == 8 && !pp->isPassedOut());
		pp->move(DOWN);
		assert(pp->row() == 5 && pp->col() == 8 && !pp->isPassedOut());
		pp->move(LEFT);
		assert(pp->row() == 5 && pp->col() == 7 && !pp->isPassedOut());
		pp->move(LEFT);
		assert(pp->row() == 5 && pp->col() == 7 && !pp->isPassedOut());
	} break; case 14: {
		City c(10, 18);
		c.addPlayer(3, 9);
		assert(c.flatulanCount() == 0);
		c.addFlatulan(4, 9);
		for (int k = 0; k < 100 && c.flatulanCount() == 1; k++)
			c.player()->preach();
		assert(c.flatulanCount() == 0);
	} break; case 15: {
		City c(10, 18);
		c.addPlayer(3, 9);
		assert(c.flatulanCount() == 0);
		for (int k = 1; k <= 20; k++)
		{
			c.addFlatulan(3, 8);
			c.addFlatulan(3, 10);
			assert(c.flatulanCount() == 2 * k);
		}
		int prevct = c.flatulanCount();
		for (int k = 0; k < 100 && prevct > 0; k++)
		{
			c.player()->preach();
			int ct = c.flatulanCount();
			assert(ct <= prevct);
			prevct = ct;
		}
		assert(prevct == 0);
	} break; case 16: {
		City c(10, 18);
		c.addPlayer(3, 9);
		assert(c.nFlatulansAt(2, 9) == 0);
		c.addFlatulan(2, 9);
		for (int k = 0; k < 100 && c.nFlatulansAt(2, 9) == 1; k++)
			c.player()->preach();
		assert(c.nFlatulansAt(2, 9) == 0);
	} break; case 17: {
		City c(2, 3);
		c.addPlayer(1, 2);
		c.addFlatulan(1, 3);
		c.addFlatulan(2, 2);
		ostringstream oss;
		streambuf *sb = cout.rdbuf(oss.rdbuf());
		c.display();
		cout.rdbuf(sb);
		string s = oss.str();
		assert(s.find(".@F") != string::npos  &&  s.find(".F.") != string::npos);
	} break; case 18: {
		City c(2, 3);
		c.addPlayer(1, 2);
		for (int k = 1; k <= 8; k++)
			c.addFlatulan(1, 3);
		c.addFlatulan(2, 2);
		ostringstream oss;
		streambuf *sb = cout.rdbuf(oss.rdbuf());
		c.display();
		cout.rdbuf(sb);
		string s = oss.str();
		assert(s.find(".@8") != string::npos);
	} break; case 19: {
		City c(2, 3);
		c.addPlayer(1, 2);
		for (int k = 1; k <= 10; k++)
			c.addFlatulan(1, 3);
		c.addFlatulan(2, 2);
		ostringstream oss;
		streambuf *sb = cout.rdbuf(oss.rdbuf());
		c.display();
		cout.rdbuf(sb);
		string s = oss.str();
		assert(s.find(".@9") != string::npos);
	} break; case 20: {
		City c(10, 18);
		c.addPlayer(3, 9);
		int prevct[3];
		for (int dr = -1; dr <= 1; dr++)
		{
			if (dr == 0)
				continue;
			for (int k = 0; k < MAXFLATULANS / 2; k++)
				assert(c.addFlatulan(3 + dr, 9));
			prevct[1 + dr] = c.nFlatulansAt(3 + dr, 9);
			assert(prevct[1 + dr] == MAXFLATULANS / 2);
		}
		bool anyleft = true;
		for (int k = 0; k < 100 && anyleft; k++)
		{
			anyleft = false;
			c.player()->preach();
			for (int dr = -1; dr <= 1; dr++)
			{
				if (dr == 0)
					continue;
				int ct = c.nFlatulansAt(3 + dr, 9);
				assert(ct <= prevct[1 + dr]);
				prevct[1 + dr] = ct;
				if (ct > 0)
					anyleft = true;
			}
		}
		assert(!anyleft);
	} break; case 21: {
		City c(10, 18);
		c.addPlayer(3, 9);
		int prevct[3][3];
		for (int dr = -1; dr <= 1; dr++)
			for (int dc = -1; dc <= 1; dc++)
			{
				if (dr == 0 && dc == 0)
					continue;
				for (int k = 0; k < MAXFLATULANS / 8; k++)
					assert(c.addFlatulan(3 + dr, 9 + dc));
				prevct[1 + dr][1 + dc] = c.nFlatulansAt(3 + dr, 9 + dc);
				assert(prevct[1 + dr][1 + dc] == MAXFLATULANS / 8);
			}
		bool allgone = true;
		for (int k = 0; k < 100 && !allgone; k++)
		{
			allgone = true;
			c.player()->preach();
			for (int dr = -1; dr <= 1; dr++)
				for (int dc = -1; dc <= 1; dc++)
				{
					if (dr == 0 && dc == 0)
						continue;
					int ct = c.nFlatulansAt(3 + dr, 9 + dc);
					assert(ct <= prevct[1 + dr][1 + dc]);
					prevct[1 + dr][1 + dc] = ct;
					if (ct != 0)
						allgone = false;
				}
		}
		assert(allgone);
	} break; case 22: {
		int n = 0;
		for (int k = 0; k < 600; k++)
		{
			City c(10, 18);
			c.addPlayer(3, 9);
			assert(c.flatulanCount() == 0);
			c.addFlatulan(3, 10);
			assert(c.flatulanCount() == 1);
			c.player()->preach();
			if (c.flatulanCount() == 0)
				n++;
		}
		assert(n >= 360 && n <= 440);
	} break; case 23: {
		City c(10, 18);
		c.addPlayer(3, 9);
		for (int k = 0; k < MAXFLATULANS; k++)
			assert(c.addFlatulan(2, 9));
		assert(!c.addFlatulan(2, 9));
		assert(c.flatulanCount() == MAXFLATULANS);
		for (int n = 64; n > 0; n /= 2)
		{
			for (int k = 0; k < 100 && c.flatulanCount() >= n; k++)
				c.preachToFlatulansAroundPlayer();
			assert(c.flatulanCount() < n);
			for (int k = 0; k < MAXFLATULANS && c.flatulanCount() < MAXFLATULANS; k++)
				assert(c.addFlatulan(2, 9));
			assert(c.flatulanCount() == MAXFLATULANS);
		}
	} break; case 24: {
		const int dr[NUMDIRS] = { -1,  0,  1,  0 };
		const int dc[NUMDIRS] = { 0,  1,  0, -1 };
		City c(12, 18);
		c.addPlayer(1, 1);
		const int r[3] = { 3, 6, 9 };
		const int v[3] = { 4, 7, 10 };
		for (int k = 0; k < 3; k++)
			c.addFlatulan(r[k], v[k]);
		c.moveFlatulans();
		for (int k = 0; k < 3; k++)
		{
			int ct[2] = { 0 };
			for (int n = 0; n < NUMDIRS; n++)
			{
				int m = c.nFlatulansAt(r[k] + dr[n], v[k] + dc[n]);
				assert(m == 0 || m == 1);
				ct[m]++;
			}
			assert(ct[0] == 3 && ct[1] == 1);
		}
	} break; case 25: {
		bool gassed = false;
		for (int n = 0; n < 20; n++)
		{
			City c(10, 20);
			c.addPlayer(5, 9);
			for (int k = 0; k < 9; k++)
				c.addFlatulan(5, 11);
			assert(c.player()->health() == 10);
			c.moveFlatulans();
			int h = c.player()->health();
			if (h < 10)
				gassed = true;
			assert(c.nFlatulansAt(5, 10) == 10 - h);
		}
		assert(gassed);
	} break; case 26: {
		bool gassed = false;
		for (int n = 0; n < 20; n++)
		{
			City c(10, 20);
			c.addPlayer(5, 9);
			for (int k = 0; k < 9; k++)
				c.addFlatulan(5, 10);
			assert(c.player()->health() == 10);
			c.moveFlatulans();
			int h = c.player()->health();
			if (h < 10)
				gassed = true;
			assert(c.nFlatulansAt(5, 10) == 10 - h);
		}
		assert(gassed);
	} break; case 27: {
		for (int n = 0; n < 20; n++)
		{
			City c(1, 20);
			c.addPlayer(1, 2);
			for (int k = 0; k < 9; k++)
				c.addFlatulan(1, 1);
			assert(c.player()->health() == 10);
			c.moveFlatulans();
			assert(c.player()->health() == 10 - 9);
			assert(c.nFlatulansAt(1, 1) == 9);
		}
	} break; case 28: {
		int n;
		for (n = 0; n < 2; n++)
		{
			City c(10, 20);
			c.addPlayer(5, 9);
			for (int k = 0; k < MAXFLATULANS; k++)
				c.addFlatulan(5, 11);
			assert(c.player()->health() == 10 && !c.player()->isPassedOut());
			c.moveFlatulans();
			if (c.player()->health() <= 0 && c.player()->isPassedOut())
				break;
		}
		assert(n != 2);
	} break; case 29: {
		recordaddrs = true;
		int oldn = allocMap.size();
		{
			City c(10, 20);
			int oldn2 = allocMap.size();
			assert(oldn2 >= oldn);
			c.addPlayer(4, 3);
			c.addFlatulan(5, 6);
			c.addFlatulan(5, 7);
			c.addFlatulan(5, 8);
			assert(allocMap.size() == oldn2 + 4);
		}
		assert(allocMap.size() == oldn);
		recordaddrs = false;
	} break; case 30: {
		recordaddrs = true;
		int oldn = allocMap.size();
		{
			City c(10, 20);
			int oldn2 = allocMap.size();
			assert(oldn2 >= oldn);
			c.addPlayer(4, 3);
			assert(allocMap.size() == oldn2 + 1);
			for (int k = 0; k < MAXFLATULANS; k++)
				c.addFlatulan(4, 4);
			assert(allocMap.size() == oldn2 + 1 + MAXFLATULANS);
			for (int k = 0; k < 100 && c.nFlatulansAt(4, 4) > 0; k++)
			{
				c.preachToFlatulansAroundPlayer();
				assert(allocMap.size() == oldn2 + 1 + c.nFlatulansAt(4, 4));
			}
			assert(c.nFlatulansAt(4, 4) == 0);
		}
		assert(allocMap.size() == oldn);
		recordaddrs = false;
	} break;
	}
}

int main()
{
	cout << "Enter test number (1-30): ";
	int n;
	cin >> n;
	testone(n);
	cout << "Passed!" << endl;
}
*/

///////////////////////////////////////////////////////////////////
// Previous main function
///////////////////////////////////////////////////////////////////

int main()
{
	doBasicTests(); // Remove this line after completing test.
	return 0;       // Remove this line after completing test.
	// Create a game, real game: Game g(15, 18, 100);
	// Use this instead to create a mini-game:   Game g(3, 3, 2);
	Game g(15, 18, 100);

	// Play the game
	g.play();
}



///////////////////////////////////////////////////////////////////////////
//  clearScreen implementation
///////////////////////////////////////////////////////////////////////////

// DO NOT MODIFY OR REMOVE ANY CODE BETWEEN HERE AND THE END OF THE FILE!!!
// THE CODE IS SUITABLE FOR VISUAL C++, XCODE, AND g++ UNDER LINUX.

// Note to Xcode users:  clearScreen() will just write a newline instead
// of clearing the window if you launch your program from within Xcode.
// That's acceptable.  (The Xcode output window doesn't have the capability
// of being cleared.)

#ifdef _MSC_VER  //  Microsoft Visual C++

#include <windows.h>

void clearScreen()
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	GetConsoleScreenBufferInfo(hConsole, &csbi);
	DWORD dwConSize = csbi.dwSize.X * csbi.dwSize.Y;
	COORD upperLeft = { 0, 0 };
	DWORD dwCharsWritten;
	FillConsoleOutputCharacter(hConsole, TCHAR(' '), dwConSize, upperLeft,
		&dwCharsWritten);
	SetConsoleCursorPosition(hConsole, upperLeft);
}

#else  // not Microsoft Visual C++, so assume UNIX interface

#include <iostream>
#include <cstring>
#include <cstdlib>

void clearScreen()  // will just write a newline in an Xcode output window
{
	static const char* term = getenv("TERM");
	if (term == nullptr || strcmp(term, "dumb") == 0)
		cout << endl;
	else
	{
		static const char* ESC_SEQ = "\x1B[";  // ANSI Terminal esc seq:  ESC [
		cout << ESC_SEQ << "2J" << ESC_SEQ << "H" << flush;
	}
}

#endif