#include <stdlib.h>
#include <unistd.h>
#include <algorithm>
#include <locale.h>
#include <ncurses.h>
#include <string.h>
#include <vector>
#include <iostream>
#include <chrono>
#include <thread>
#include <time.h>
#include <math.h>
#define INVENTORY_SIZE 9
#define RESTAURANT 495
#define BANK 505
#define STORE 515
#define PHARM 485

int max_capacity = 5;
int capacity = max_capacity;
int cols = 800;
int rows = 800;
int posX = 500;
int posY = 0;
int price_meal = 10;
int max_x;
int max_y;
int surface = 50;
int health = 100;
int max_energy = 25;
int energy = max_energy;
long unsigned int total_income;
unsigned int money = 10; 
int fovX;
char tool[7] = "⛏️";
char person[8] = "🧍 ";//🯆

int fovY;

using namespace std;
vector<vector<int>> map(cols);
int lvl_pic = 0;
int lvl_prot = 0;
int inventory[INVENTORY_SIZE] = {0};
char info[100] = "";
void upgrades(){
	attron(COLOR_PAIR(70));
	clear();
	bool shop_wait = true;
	while(shop_wait){
		mvprintw(0,0, "Your account: %d$       ", money);
		mvprintw(1,0, "Upgrade:");
		mvprintw(2,0, "1. ⛏️ picaxe, level: %d -> %d$", lvl_pic, 1500+((lvl_pic+1)*(lvl_pic+1)*500));
		mvprintw(3,0, "2. 🛡️ protection, level: %d -> %d$", lvl_prot, 300+(lvl_prot*100));
		mvprintw(4,0, "3. 📦️ storage, level: %d -> %d$", max_capacity, 100*max_capacity);
		mvprintw(5,0, "4. ⚡ energy, level: %d ->%d$", max_energy, 500+max_energy*10);
		mvprintw(7,0, "Buy:");
		mvprintw(8,0, "5. ☕ coffe, owned: %d -> %d$", inventory[0], price_meal*50);
		mvprintw(9,0, "6. ✴️ warp star, owned: %d -> %d$", inventory[1], 200000);
		
		
		
		
		int g = getch();
		int price = 0;
		switch(g){
			case 49://1
				price = 1500+((lvl_pic+1)*(lvl_pic+1)*500);
				if(money>=price){
					money -= price;
					lvl_pic++;
				}
				break;
			case 50://2
				price = 300+(lvl_prot*100);
				if(money>price){
					money -= price;
					lvl_prot++;
				}
				break;
			case 51://3
				price = 100*max_capacity;
				if(money>=price){
					money -= price;
					max_capacity+= 5;
					capacity=max_capacity;
				}
				break;
			case 52://4
				price = 500+max_energy*10;
				if(money>=price){
					money -= price;
					max_energy+=15;
					price_meal += 5;
				}
				break;
			
			case 53://5
				price = price_meal*50;
				if(money>=price){
					money -= price;
					inventory[0]++;
				}
				break;
			case 54://6
				price = 200000;
				if(money>=price){
					money -= price;
					inventory[1]++;
				}
				break;
			case 113:
			case 32:
				
				shop_wait = false;
				break;
			default:
				mvprintw(10, 0, "%d", g);
				break;
		}
	}
}
void interact(){
	strcpy(person, "🧍");
	if(posY==0){
		if(posX>RESTAURANT-2&&posX<RESTAURANT+1){
			strcpy(info, "You couldn't afford food");
			while(energy<max_energy &&money>=price_meal){
				energy++;
				money-=price_meal;
				strcpy(info, "You ate a good meal. You feel full of energy");
			}
			
			
		}else if(posX>BANK-2&&posX<BANK+1){
			for(int i = 2; i < INVENTORY_SIZE; i++){
				money += inventory[i]*i*100;
				total_income += inventory[i]*i*100;
				inventory[i] = 0;
				strcpy(info, "You sold all your minerals to the bank");
			}
			capacity = max_capacity;
		}else if(posX>PHARM-2&&posX<PHARM+1){
			if(health==100)strcpy(info, "You don't need medical attention");
			else{
				health = 100;
				energy = max(energy, max_energy/5);
				strcpy(info, "You were treated at the hospital");
			}
		}else if(posX>STORE-2&&posX<STORE+1){
			upgrades();
		}
	}
}

void title(){
	mvprintw(1, 0, "\
████    ██   ████   ████   ██████  █████ \n\
██  ██      ██     ██      ██      ██  ██\n\
██  ██  ██  ██ ███ ██ ███  ████    █████ \n\
██  ██  ██  ██  ██ ██  ██  ██      ██  ██\n\
████    ██   ████   ████   ██████  ██  ██\n\
");
}

void hud(){
	attron(COLOR_PAIR(70));
	mvprintw(max_y-2, 0, ">%s", info);
	attron(COLOR_PAIR(10));
	if(health>0)mvprintw(0, max_x-2-((int)log10(health)+1), "❤️ %d", health);
	else mvprintw(0, max_x-3, "❤️ %d", health);
	attron(COLOR_PAIR(30));
	if(energy>0)mvprintw(1, max_x-2-((int)log10(energy)+1), "⚡️%d", energy);				
	else mvprintw(1, max_x-3, "⚡️%d", energy);
	attron(COLOR_PAIR(20));
	mvprintw(1, 0, "$️ %d", money);				
	attron(COLOR_PAIR(50));
	mvprintw(0, 0, "⛳️️%dm", posY);
	attron(COLOR_PAIR(70));
	mvprintw(max_y-1, 0, "Inv ");
	for(int i = 0; i<INVENTORY_SIZE;i++){
		if(inventory[i]){
			switch(i){
				case 0:
					attron(COLOR_PAIR(70));
					
					printw("☕");
					
					break;
				case 1:
					attron(COLOR_PAIR(50));
					
					printw("✴️");
					
					break;
                case 3:
					attron(COLOR_PAIR(30));
					
					printw("🌟 ");
					
					break;
				case 4:
					attron(COLOR_PAIR(60));
					
					printw("💎 ");
					break;
				case 5:
					attron(COLOR_PAIR(10));
					
					printw("🏮 ");
					break;
				case 6:
					attron(COLOR_PAIR(20));
					
					printw("🔲");
					break;
				case 8:
					attron(COLOR_PAIR(40));
					
					printw("✨");
					break;
				case 7:
					attron(COLOR_PAIR(30));
					
					printw(" ⚱️");
					break;
			}
            if(i!=2)printw(":%d", inventory[i]);
		}
	}
	attron(COLOR_PAIR(70));
	printw("/%d", capacity);
}
void death(){
	strcpy(info, "You passed out of exhaustion");
	hud();
	posX = 500;
	posY = 0;
	fill_n(inventory, INVENTORY_SIZE, 0);
	capacity = max_capacity;
	health = 100;
	
	
}
bool check(int x, int y){
	strcpy(info, "");
	if(y==0){
		//strcpy(tool, " ");
		return true;
		}
	if(energy > 0){
		if(map[x][y]==0){
			//strcpy(tool, " ️");
			return true;
		}
		else if (map[x][y]==1){
			//strcpy(tool, "⛏️");
			energy--;
			this_thread::sleep_for(chrono::milliseconds(200/(lvl_pic+1)));
			map[x][y]--;
			return true;
		}
		else if (map[x][y]==2){
			//strcpy(tool, "⛏️");
			
			
			if(lvl_pic){
				this_thread::sleep_for(chrono::milliseconds(200/(lvl_pic+1)));
				energy--;
				if (lvl_pic==1)map[x][y]=1;
				else {map[x][y]=0;return true;}
			}else
				strcpy(info, "You picaxe is too weak");
			return false;
			}
		else if (map[x][y]==-1){
			//strcpy(tool, "⛏️");
			if(lvl_pic>1){
				this_thread::sleep_for(chrono::milliseconds(200/(lvl_pic+1)));
				energy--;
				if(lvl_pic==2)map[x][y]=2;
				else if(lvl_pic==3)map[x][y]=1;
				else {map[x][y]=0;return true;}
			}else
				strcpy(info, "You picaxe is too weak");
			return false;
			}
		else if(capacity>0){
			if (map[x][y]==3){
					//strcpy(tool, "⛏️");
					map[x][y] = 1;
					energy--;
					inventory[3]++;
					capacity--;
					return false;
			}else if (map[x][y]==4){
					//strcpy(tool, "⛏️");
					map[x][y] = 2;
					energy--;
					inventory[4]++;
					capacity--;
				return false;
			}	
			else if (map[x][y]==5){
					//strcpy(tool, "⛏️");
					map[x][y] = -1;
					if(lvl_pic){
					energy--;
					inventory[5]++;
					capacity--;
				}else strcpy(info, "You picaxe is too weak");
			}	
			else if (map[x][y]==6){
					//strcpy(tool, "⛏️");
					
					if(lvl_pic>1){
						map[x][y] = -1;
						energy--;
						inventory[6]++;
						capacity--;
					}else strcpy(info, "You picaxe is too weak");
			}	
			else if (map[x][y]==8){
					//strcpy(tool, "⛏️");
					
					if(lvl_pic>3){
						map[x][y] = -1;
						energy--;
						inventory[8]++;
						capacity--;
					}else strcpy(info, "You picaxe is too weak");
			}	
			else if (map[x][y]==7){
					//strcpy(tool, "⛏️");
					map[x][y] = 1;
					
					energy--;
					inventory[2]+=posY;
					inventory[7]++;
					capacity--;
					strcpy(info, "You found an artifact!");
			}	
			
			return false;
		}strcpy(info, "You no longer have space in your inventory");
		return false;
	}else{
		if(map[x][y]==0){
			//strcpy(tool, "");
			if(rand()%(1+lvl_prot)==0){
				health-=(rand()%4+1);
				if(health<1){
					death();
					return false;
				}
			}
			return true;
		}else{
			strcpy(info, "You don't have the energy to do that");
			return false;
			}
	}
}

int display(int x, int y){
	for (int i = 0; i<fovX; i++){
		for (int j = 0; j < max_y; j++){
			//mvprintw(0,0, "%d, %d, %d : %d; %d : %d", x,y, max_y, fovY+y, fovY, max_y+fovY+y);
			if(i-fovX/2+x<0||i-fovX/2+x+1>map.size()||j-max_y-((max_y+1)%2)+fovY+y<0||j-max_y+fovY+fovY%2+y>map[0].size()){
				attron(COLOR_PAIR(66));
				mvprintw(j, i*2, "  ️");
			}else{
				if(j-fovY+y==0){
					attron(COLOR_PAIR(66));
				}
				if(j-fovY-fovY%2+y<4){
					attron(COLOR_PAIR(20));
				}else{
					attron(COLOR_PAIR(70));
				}
				switch(map[i-fovX/2+x][j-fovY+y]){
				case -1:
					mvprintw(j, i*2, "░░");
					break;
				case 0:
					mvprintw(j, i*2, "  ");
					break;
				case 1:
					mvprintw(j, i*2, "▓▓");
					break;
				case 2:
					mvprintw(j, i*2, "▒▒");
					break;

				case 3:
					attron(COLOR_PAIR(37));
					mvprintw(j, i*2, "🌟 ");
					break;

				case 4:
					attron(COLOR_PAIR(67));
					mvprintw(j, i*2, "💎 ");
					break;

				case 5:
					attron(COLOR_PAIR(17));
					mvprintw(j, i*2, "🏮 ");
					break;

				case 6:
					attron(COLOR_PAIR(27));
					mvprintw(j, i*2, "🔲 ");
					break;
				case 8:
					attron(COLOR_PAIR(47));
					mvprintw(j, i*2, "✨ ");
					break;
				case 7:
					attron(COLOR_PAIR(37));
					if (y<50) mvprintw(j, i*2, "⚰️️ ");
					else if (y<100) mvprintw(j, i*2, "⚓️ ️");
					else if (y<150) mvprintw(j, i*2, " ☦️");
					else if (y<200) mvprintw(j, i*2, "⚱️ ");
					else if (y<250) mvprintw(j, i*2, "⚱ ");
					else if (y<300) mvprintw(j, i*2, " ☥");
					else if(y<350)mvprintw(j, i*2, "🗃 ");
					else mvprintw(j, i*2, "🛸 ");
					break;
				//case 2:
					//mvprintw(j, i, "░");
					//break;
				default:
					//mvprintw(j, i, " ");
					break;
				}
				
					
			}
				if(j-fovY+y-1==0){
					
					if(i==RESTAURANT-x+fovX/2){
						attron(COLOR_PAIR(57));
						mvprintw(j-2, i*2-2, " 🍽  ");
						mvprintw(j-1, i*2-2, "    ");
					}
					else if(i==BANK-x+fovX/2){
						attron(COLOR_PAIR(37));
						mvprintw(j-2, i*2-2, " 💰  ");
						mvprintw(j-1, i*2-2, "    ");
					}
					else if(i==PHARM-x+fovX/2){
						attron(COLOR_PAIR(17));
						mvprintw(j-2, i*2-2, " ⚕️️  ");
						mvprintw(j-1, i*2-2, "    ");
					}
					else if(i==STORE-x+fovX/2){
						attron(COLOR_PAIR(7));
						mvprintw(j-2, i*2-2, " ⚒️  ");
						mvprintw(j-1, i*2-2, "    ");
					}
					//if(y<fovY&&x>520-max_x&&x<520+max_x)mvprintw(fovY-y, fovX-fovX%2-x+520, " 🏛");
				}
			if(y==0){ 
				attron(COLOR_PAIR(16));
				if(x == RESTAURANT||x == RESTAURANT-1||x == BANK||x == BANK-1||x == PHARM||x == PHARM-1||x == STORE||x == STORE-1)attron(COLOR_PAIR(17));
				}
			else attron(COLOR_PAIR(10));
			mvprintw(fovY, fovX-fovX%2, "%s️", person);	
			hud();
		}
	}
	return 0;
}

int main(){
	srand(time(NULL));
	setlocale(LC_ALL, "");
	//initialize ncurses
	initscr();
	keypad(stdscr, TRUE);
	noecho();
	cbreak();
	curs_set(0);
	//deal with colorless displays
	if(!has_colors()){
		endwin();
		printf("NO COLORS IN YOUR TERMINAL");
		exit(1);
		return 0;
	}
	//define color pairs
	//think of the numbers as mixing the colors
	//black 0
	//red 1
	//green 2
	//yellow 3
	//blue 4
	//magenta 5
	//cyan 6
	//white 7
	start_color();
	
	init_pair(10, COLOR_RED, COLOR_BLACK);//red
	init_pair(20, COLOR_GREEN, COLOR_BLACK);//green
	init_pair(30, COLOR_YELLOW, COLOR_BLACK);//yellow
	init_pair(40, COLOR_BLUE, COLOR_BLACK);//blue
	init_pair(50, COLOR_MAGENTA, COLOR_BLACK);//blue
	init_pair(60, COLOR_CYAN, COLOR_BLACK);//green
	init_pair(70, COLOR_WHITE, COLOR_BLACK);//white
	
	init_pair(17, COLOR_RED, COLOR_WHITE);//red
	init_pair(27, COLOR_GREEN, COLOR_WHITE);//green
	init_pair(37, COLOR_YELLOW, COLOR_WHITE);//yellow
	init_pair(36, COLOR_YELLOW, COLOR_CYAN);//yellow
	init_pair(47, COLOR_BLUE, COLOR_WHITE);//blue
	init_pair(46, COLOR_BLUE, COLOR_CYAN);//blue
	init_pair(57, COLOR_MAGENTA, COLOR_WHITE);//blue
	init_pair(67, COLOR_CYAN, COLOR_WHITE);//green
	init_pair(77, COLOR_WHITE, COLOR_WHITE);//white
	init_pair(66, COLOR_CYAN, COLOR_CYAN);//green
	init_pair(16, COLOR_RED, COLOR_CYAN);//green
	
	
	init_pair(7, COLOR_BLACK, COLOR_WHITE);//white
	init_pair(6, COLOR_BLACK, COLOR_CYAN);//green
	
	
	//attron(A_REVERSE);
	
	printw("INITIALIZING");
	for (int i = 0; i<rows; i++){
		map[i] = vector<int>(cols);
		
		for(int j = 1; j<4;j++){
			
			map[i][j] = 1;
		}
		
		for(int j = 4; j<cols;j++){
			int r = rand()%1000;
			if (r==0)map[i][j] = 0;
			if (r==666&&j>100)map[i][j] = 7;
			
			else if (r<200)map[i][j] = 2;
			else if (r<200+min(j, 50)&&j<200)map[i][j] = 3;
			else if (r<300+min(j-100, 50)&&j<300) map[i][j] = 4;
			else if (r<400+min(j-200, 50)&&j>150&&j<500) map[i][j] = 5;
			else if (r<450+min(j-300, 50)&&j>350) map[i][j] = 6;
			else if (r<650+min(j-450, 50)&&j>450) map[i][j] = 8;
			else if (r>950-min(j-500, 75)) map[i][j] = -1;
			else if (j>500)map[i][j] = -1;
			else if (j>300)map[i][j] = 2;
			else map[i][j] = 1;
		}
	}


	mvprintw(0,0, "DONE!          ");

	title();
	getch();
	getmaxyx(stdscr, max_y, max_x);
	fovX = max_x/2;
	fovY = max_y/2;
	int mainloop = 1;
	while(mainloop){
		//clear();
		
		display(posX, posY);
		int c = getch();
		//🚶 🧍  
		strcpy(person, "🧍");
		switch(c){
			case KEY_DOWN:
				if(posY<map.size()-1&&check(posX, posY+1))
				posY++;
				break;
			case KEY_UP:
				if(posY>0&&check(posX, posY-1))
				posY--;
				break;
			case KEY_LEFT:
				if(posX>0&&check(posX-1, posY))posX--;
				break;
			case KEY_RIGHT:
				if(posX<map[0].size()-1&&check(posX+1, posY))posX++;
				break;
			
			case 410:
			case 112://p;
				getmaxyx(stdscr, max_y, max_x);
				fovX = max_x/2;
				fovY = max_y/2;
				break;
			case 49://h
				if(inventory[0]){
					inventory[0]--;
					energy+=50;
				}
				break;
			case 50://h
				if(inventory[1]){
					inventory[1]--;
					posX = 500;
					posY = 0;
				}
				break;
			case 104://h
				energy = max_energy;
				money += 1000;
				break;
			case 120://x
				death();
				break;
			case 119://w Restauran
				if(posY==0)posX=RESTAURANT;
				break;
			case 101://e BANK
				if(posY==0)posX=BANK;
				break;
			case 113://q PHARM
				if(posY==0)posX=PHARM;
				break;
			case 114: //r STORE
				if(posY==0)posX=STORE;
				break;
			case 32://space
				interact();
				break;
			default:
				mvprintw(0,0, "%d    ", c);
				break;
		}
	}
	display(posX+1, posY);
	getch();
	display(posX, posY);
	getch();
	display(posX, posY+1);
	getch();
	endwin();
	return 0;
	}
