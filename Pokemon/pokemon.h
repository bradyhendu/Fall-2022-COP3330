//Programming Assignment 7, Brady Henderson, Section 4
#include<iostream>
#include <cstring>
using namespace std;
class Type{
	public:
		virtual int SpecialAttack(char* target) = 0;
};
class Fire : public Type{
	public:
		int SpecialAttack(char* target);
};
class Grass : public Type{
	public:
		int SpecialAttack(char* target);
};
class Water : public Type{
	public:
		int SpecialAttack(char* target);
};
class Size{
	public:
		virtual int PhysicalAttack() = 0;
		virtual int SetHealth() = 0;
};
class Big : public Size{
	public:
		int PhysicalAttack();
		int SetHealth(); //to set the health of the monster
};
class Small : public Size{
	public:
		int PhysicalAttack();
		int SetHealth(); //to set the health of the monster
};
class Monster{
	public:
		Monster();
		Monster(char* _s, char* _t, const char* _name);
		~Monster();
		void Hit(int d);
		int GetHealth() const;
		char* GetType() const;
		char* GetSize() const;
		char* GetName() const;
		void Battle(Monster*& m);
		int GetX() const;
		int GetY() const;
		void SetX(int x);
		void SetY(int y);
		Monster& operator=(const Monster& m);	
	private:
		char* name;
		int health;
		int physical; //to denote how much physical damage a monster will do
		int x_coord;
		int y_coord;
		Size* s;
		Type* t;
		//For the random name + other functionality
		char* type; 
		char* size;
		//Functions
		int RandomMove();
		int RandomSpecial(char* t);
};
class Player{
	
	public:
		Player();
		void SetX(int x);
		int GetX() const;
		void SetY(int y);
		int GetY() const;
		int GetCoins() const;
		void SetCoins(int c);
		int GetPSize() const;
		void SelectMonster(Monster*& m);
	private:
		int x_coord;
		int y_coord;
		int coins;
		int partySize;
		Monster playerMonsters[3];
};
class Map{
	friend ostream& operator<<(ostream& s, const Map& db);
	
	public:
		Map(int _size);
		~Map();
		void Move();
		bool CheckGameState();
		int PlayerCoinCount();
		bool CheckWin();
	private:
		//data
		Player p;
		const int maxItems;
		const int maxMonsters;
		int numItems;
		int numMonsters;
		int size;
		char** mapState;
		int** items;
		Monster** monsters;
		bool gameWin;
		
		//functions
		void setupItem();
		void setupPlayer();
		void setupMonsters();
		void checkCollision();
		void StartBattle(Monster*& m);
};
