//Programming Assignment 7, Brady Henderson, Section 4
#include<iostream>
#include "pokemon.h"
using namespace std;

int main() {
	int size;
	cout << "~~~~~~Welcome to Brady's Fake-mon Game!~~~~~~";
	cout << endl << "Your goal is to get all the coins (I) and beat all the monsters!";
	cout << endl << "Note: you can spawn on a coin or monster and either immediately collect or battle it!";
	cout << endl << "Please enter a size for the board: ";
	cin >> size;
	//cin.get();
	while(size < 5 || size > 20){
		cout << endl << "Please pick a better size: ";
		cin >> size;
		//cin.get();
	}
	Map m(size);
	cout << m << endl;
	while (!m.CheckGameState()){
		m.Move();
		cout << m << endl;
	}
	if (m.CheckWin()){
		cout << "Congratulations! You won! You are so epic!" << endl;
		cout << "You finished with " << m.PlayerCoinCount() << " coins" << endl;
	}else{
		cout << "Sorry :( You lost... Maybe next time don't do that :/" << endl;
		cout << "You finished with " << m.PlayerCoinCount() << " coins" << endl;
	}
	return 0;
}