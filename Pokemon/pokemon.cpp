//Programming Assignment 7, Brady Henderson, Section 4
#include "pokemon.h"
#include <cctype>
#include <cstdlib>
//Map Class
Map::Map(int _size) : maxItems(_size/2),maxMonsters(_size/2) {
	srand(time(0));
	size = _size;
	//allocate a new board
	mapState = new char*[size];
	for(int i = 0; i < size; i++) {
		mapState[i] = new char[size];
		for(int j = 0; j < size; j++) {
			if(i == 0 || i == size-1) {
				mapState[i][j]= '-';
				continue;
			} else if (j == 0 || j == size-1) {
				mapState[i][j] = '|';
				continue;
			}
			mapState[i][j] = ' ';
		}
	}
	setupItem();
	setupMonsters();
	setupPlayer();
}
Map::~Map() {
	for(int i = 0; i < size; i++) {
		delete [] mapState[i];
	}
	delete [] mapState;
	delete [] monsters;
	for(int i = 0; i < numItems; i++) {
		delete [] items[i];
	}
	delete [] items; 
}
ostream& operator<<(ostream& s, const Map& m) {
	for(int i = 0; i < m.size; i++) {
		for(int j = 0; j < m.size; j++) {
			s << m.mapState[i][j] << "\t";
		}
		s << endl<<endl;
	}
	return s;
}
void Map::setupPlayer(){
	int row = rand()%(size-2) + 1;
	int column = rand()%(size-2) + 1;
	mapState[row][column] = 'P';
	
	p.SetY(row);
	p.SetX(column);
	checkCollision();
}
void Map::setupItem(){
	int count = 0;
	numItems = rand() % maxItems + 1;
	items = new int*[numItems];
	for(int i = 0; i < numItems; i++) {
		//get row
		int row = rand()%(size-2) + 1;
		//get column
		int column = rand()%(size-2) + 1;
		//place items
		items[i] = new int[2];
		count++;
		for(int i = 0; i < count; i++){
			if(row == items[i][0]){
				if(column == items[i][1]){
					row = rand()%(size-2) + 1;
					column = rand()%(size-2) + 1;
				}
			}
		}
		mapState[row][column] = 'I';
		items[i][0] = row;
		items[i][1] = column;
	}
}
void Map::setupMonsters(){
	int count = 0;
	numMonsters = rand() % maxMonsters + 1;
	monsters = new Monster*[numMonsters]; 
	for(int i = 0; i < numMonsters; i++) {
		monsters[i] = new Monster;
		count++;
		//get row
		int row = rand()%(size-2) + 1;
		//get column
		int column = rand()%(size-2) + 1;
		//place Monsters + check if the new placement equals an old monsters placement
		for(int i = 0; i < count; i++){
			if(row == monsters[i]->GetY()){
				if(column == monsters[i]->GetX()){
					row = rand()%(size-2) + 1;
					column = rand()%(size-2) + 1;
				}
			}
		}
		mapState[row][column] = 'M';
		monsters[i]->SetY(row);
		monsters[i]->SetX(column);
	}
}
void Map::Move(){
	char d;
	int change; //Changes the position
	cout << endl << "Please enter the direction you'd like to move (WASD keys): ";
	cin >> d;
	cin.get();
	d = toupper(d);
	
	while (d != 'W' && d != 'A' && d != 'S' && d != 'D'){
		cout << endl << "Please enter W (Up) or A (Left) or S (Down) or D (Right): ";  
		cin >> d;
		cin.get();
		d = toupper(d);
	}
	mapState[p.GetY()][p.GetX()] = ' ';
	if(d == 'W'){
		change = p.GetY() - 1;
		if(change == size - 1 || change == 0){
			cout << endl << "You cannot move this way, you will hit the wall." << endl;
		}else{
			p.SetY(change);
		}
	}else if( d == 'A'){
		change = p.GetX() - 1;
		if(change == size - 1 || change == 0){
			cout << endl << "You cannot move this way, you will hit the wall." << endl;
		}else{
			p.SetX(change);
		}
	}else if( d == 'S'){
		change = p.GetY() + 1;
		if(change == size - 1 || change == 0){
			cout << endl << "You cannot move this way, you will hit the wall." << endl;
		}else{
			p.SetY(change);
		}
	}else{
		change = p.GetX() + 1;
		if(change == size - 1 || change == 0){
			cout << endl << "You cannot move this way, you will hit the wall." << endl;
		}else{
			p.SetX(change);
		}
	}
	mapState[p.GetY()][p.GetX()] = 'P';
	checkCollision();
}
void Map::checkCollision(){
	for(int i = 0; i < numItems; i++){
		if(items[i][0] == p.GetY()){
			if(items[i][1] == p.GetX()){
				for (int j = i + 1; j < numItems; j++){
					items[j - 1] = items[j];
				}		
				numItems--;
				cout << "Item collected." << endl;
				int coins = p.GetCoins() + 1;
				p.SetCoins(coins);
				break;
			}
		}
	}
	for(int i = 0; i < numMonsters; i++){
		if(monsters[i]->GetY() == p.GetY()){
			if(monsters[i]->GetX() == p.GetX()){
				if(p.GetPSize() != 0){
					StartBattle(monsters[i]);
				}else{
					break;
				}
				if(monsters[i]->GetHealth() <= 0){
					cout << endl << monsters[i]->GetName() << " has been eliminated." << endl;
					for (int j = i + 1; j < numMonsters; j++){
						monsters[j - 1] = monsters[j];
					}		
					numMonsters--;
					break;
				}else{
					int change = p.GetX() - 1;
				if(change == size - 1 || change == 0){
				    change = p.GetX() + 1;	
				}
					p.SetX(change);
					mapState[p.GetY()][p.GetX()] = 'P';
					mapState[monsters[i]->GetY()][monsters[i]->GetX()] = 'M';
					checkCollision();
				break;
				}
			}
		}
	}
}
void Map::StartBattle(Monster*& m){
	cout << endl << "You encountered a wild " << m->GetName();
	p.SelectMonster(m);
}
int Map::PlayerCoinCount(){
	int coins = p.GetCoins();
	return coins;
}
bool Map::CheckGameState(){
	if(numItems == 0 && numMonsters == 0){
		gameWin = true;
		return true;
	}else if(p.GetPSize() == 0){
		gameWin = false;
		return true;
	}
	return false;
}
bool Map::CheckWin(){
	if(gameWin){
		return true;
	}
	return false;
}
//Player Class
Player::Player(){
	x_coord = 0;
	y_coord= 0;
	coins = 0;
	partySize = 3;
	char* _name = new char[80];
	char* _s = new char[6];
	char* _t = new char[6];
	strcpy (_name, "Guggimon");
	strcpy (_s, "Big");
	strcpy (_t, "Fire");
	Monster m1(_s, _t, _name);
	playerMonsters[0] = m1;
	char* _name1 = new char[80];
	char* _s1 = new char[6];
	char* _t1 = new char[6];
	strcpy (_name1, "JAG");
	strcpy (_s1, "Small");
	strcpy (_t1, "Grass");
	Monster m2(_s1, _t1, _name1);
	playerMonsters[1] = m2;
	char* _name2 = new char[80];
	char* _s2 = new char[6];
	char* _t2 = new char[6];
	strcpy (_name2, "Hibari");
	strcpy (_s2, "Big");
	strcpy (_t2, "Water");
	Monster m3(_s2, _t2, _name2);
	playerMonsters[2] = m3;
}
void Player::SetX(int x){
	x_coord = x;
}
void Player::SetY(int y){
	y_coord = y;
}
void Player::SetCoins(int c){
	coins = c;
}
int Player::GetX() const{
	return x_coord;
}
int Player::GetY() const{
	return y_coord;
}
int Player::GetCoins() const{
	return coins;
}
int Player::GetPSize() const{
	return partySize;
}
void Player::SelectMonster(Monster*& m){
	int selection;
	cout << endl << "Which of your monsters would you like to choose (type the number): ";
	for(int i = 0; i < partySize; i++) {
			cout << endl << "   " << i + 1 << ": " << playerMonsters[i].GetSize() << " " << playerMonsters[i].GetType() << " " << playerMonsters[i].GetName();
	}
	cout << endl << "Select: ";
	cin >> selection;
	cin.get();
	while(selection < 1 || selection > partySize){
		cout << endl << "Please pick a correct monster: ";
		cin >> selection;
		cin.get();
	}
	playerMonsters[selection - 1].Battle(m);
	if(playerMonsters[selection - 1].GetHealth() <= 0){
		cout << endl << playerMonsters[selection - 1].GetName() << " has been eliminated." << endl;
		for (int j = selection; j < partySize; j++){
				playerMonsters[j - 1] = playerMonsters[j];
		}		
		partySize--;
		cout << endl << "You have " << partySize << " monsters left";
		if(partySize == 0){
			cout << endl << "You are out of monsters to battle with :(" << endl;
		}else if(partySize != 0){
			SelectMonster(m);
		}
	}
} 
//Monster Class + Type and Size
int Big::SetHealth(){
	return 120;
}
int Small::SetHealth(){
	return 80;
}
int Big::PhysicalAttack(){
	return 20;
}
int Small::PhysicalAttack(){
	return 5;
}
int Fire::SpecialAttack(char* target){
	
	if(target[0] == 'G'){
		return 40;
	}
	return 20;
}
int Water::SpecialAttack(char* target){
	if(target[0] == 'F'){
		return 40;
	}
	return 20;
}
int Grass::SpecialAttack(char* target){
	if(target[0] == 'W'){
		return 40;
	}
	return 20;
}
Monster::Monster(){
	int randType = rand()%3;
	if(randType == 0){
		Fire f;
		t = new Fire;
		type = new char[5];
		strcpy (type, "Fire");
	}else if(randType == 1){
		t = new Grass;
		type = new char[6];
		strcpy (type, "Grass");
	}else{
		t = new Water;
		type = new char[6];
		strcpy (type, "Water");
	}
	int randSize = rand()%2;
	if(randSize == 0){
		s = new Big;
		size = new char[4];
		strcpy (size, "Big");
	}else{
		s = new Small;
		size = new char[6];
		strcpy (size, "Small");
	}
	health = s->SetHealth();
	physical = s->PhysicalAttack();
	name = new char[20]; //size of the biggest possible name combination (grass small monster)
	strcat(name, size);
	strcat(name, " ");
	strcat(name, type);
	strcat(name, " ");
	strcat(name, "Monster");
	x_coord = 0;
	y_coord = 0;
}
Monster::Monster(char* _s, char* _t, const char* _name){
	int _sizeSize = 0;
	while(_s[_sizeSize] != '\0'){
		_sizeSize++;
	}
	_sizeSize++;
	
	size = new char[_sizeSize];
	for(int i = 0; i < _sizeSize; i++) {
		size[i] = _s[i];
	}
	if(size[0] == 'B'){
		s = new Big;
	}else{
		s = new Small;
	}
	health = s->SetHealth();
	physical = s->PhysicalAttack();
	int _typeSize = 0;
	while(_t[_typeSize] != '\0'){
		_typeSize++;
	}
	_typeSize++;
	
	type = new char[_typeSize];
	for(int i = 0; i < _typeSize; i++) {
		type[i] = _t[i];
	}
	if(type[0] == 'F'){
		t = new Fire;
	}else if(type[0] == 'G'){
		t = new Grass;
	}else{
		t = new Water;
	}
	int _nameSize = 0;
	while(_name[_nameSize] != '\0'){
		_nameSize++;
	}
	_nameSize++;
	
	name = new char[_nameSize];
	for(int i = 0; i < _nameSize; i++) {
		name[i] = _name[i];
	}
	x_coord = 0;
	y_coord = 0;
}
char* Monster::GetName() const{
	return name;
}
char* Monster::GetSize() const{
	return size;
}
char* Monster::GetType() const{
	return type;
}
void Monster::Battle(Monster*& m){
	char selection;
	int damage;
	while(health > 0 && m->GetHealth() > 0){
		cout << endl << "Would you like to Run (R), Wait (W), Physically Attack (P) or use a Special Attack (S): ";
		cin >> selection;
		selection = toupper(selection);
		while (selection != 'R' && selection != 'W' && selection != 'P' && selection != 'S'){
			cout << endl << "Please pick Run (R), Wait (W), Physically Attack (P) or use a Special Attack (S): ";  
			cin >> selection;
			cin.get();
			selection = toupper(selection);
		}
		if(selection == 'R'){
			cout << endl << "You chose to run away!" << endl;
			break;
		}else if (selection == 'W'){
			cout << endl << "Okay... go ahead an waste a turn???";
		}else if (selection == 'P'){
			m->Hit(physical);
		}else{
			damage = t->SpecialAttack(m->GetType());
			m->Hit(damage);
		}
		int enemyMove = m->RandomMove();
		if(enemyMove == 0){
			cout << endl << "The enemy monster waited...";
		}else if(enemyMove == 1){
			cout << endl << "The enemy monster physically attacked!";
			Hit(m->physical);
		}else{
			cout << endl << "The enemy monster used a special attack";
			damage = m->RandomSpecial(GetType());
			Hit(damage);
		}
		cout << endl << m->GetName() << " is at " << m->GetHealth() << " health";
		cout << endl << GetName() << " is at " << GetHealth() << " health";
	}
}
int Monster::RandomMove(){
	int move = rand()%3;
	return move;
}
void Monster::Hit(int d){
	cout << endl << name << " was hit for " << d << " damage";
	health = health - d;
	if(health < 0){
		health = 0;
	}
}
int Monster::RandomSpecial(char* target){
	int damage = t->SpecialAttack(target);
	return damage;
}
void Monster::SetX(int x){
	x_coord = x;
}
void Monster::SetY(int y){
	y_coord = y;
}
int Monster::GetX() const{
	return x_coord;
}
int Monster::GetY() const{
	return y_coord;
}
int Monster::GetHealth() const{
	return health;
}
Monster& Monster::operator=(const Monster& m){
	if(this != &m){
		delete [] name;
		delete [] size;
		delete [] type;
		int nSize = 0;
		int sSize = 0;
		int tSize = 0;
		//name
		while(m.name[nSize] != 0){
			nSize++;
		}
		nSize++;
		name = new char[nSize];
		for(int i = 0; i < nSize; i++) {
			name[i] = m.name[i];
		}
		//size 
		while(m.size[sSize] != 0){
			sSize++;
		}
		sSize++;
		
		size = new char[sSize];
		for(int j = 0; j < sSize; j++) {
			size[j] = m.size[j];
		}
		//type
		while(m.type[tSize] != 0){
			tSize++;
		}
		tSize++;
		
		type = new char[tSize];
		for(int k = 0; k < tSize; k++) {
			type[k] = m.type[k];
		}
		//Variables
		health = m.GetHealth();
		physical = m.physical; //to denote how much physical damage a monster will do
		x_coord = m.GetX();
		y_coord = m.GetY();
		t = m.t;
		s = m.s;
	}
	return *this;
}
Monster::~Monster(){
	delete [] name;
	delete [] size;
	delete [] type;	
}