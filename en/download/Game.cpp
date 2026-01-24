#include <bits/stdc++.h>
#include <windows.h>
#include <conio.h>
using namespace std;

const int LEN=25, HEI=15;
int pos_x=LEN/2, pos_y=HEI/2, enemy_x=2, enemy_y=2, time_1=0, time_2=0, k=1;
bool is_enemy=false;

struct enemy{
	int x, y;
};

vector<enemy> enemies;

void add_enemy(){
	enemies.push_back(enemy{enemy_x, enemy_y});
}

void reset(){
	pos_x=LEN/2, pos_y=HEI/2, time_1=0, time_2=0;
    enemies.clear();
    add_enemy();
}

void flip(){
	int x=0, y=0;
	system("cls");
	for(enemy e : enemies){
		if(e.x==pos_x&&e.y==pos_y){
			cout << "GameOver!";
			Sleep(3000);
			cout << "  (Enter)";
			if(getchar()=='\n'){
				reset();
				flip();
			}
			break;
		}
	}
	while(y<HEI){
		y++;
		while(x<LEN){
			x++;
			if(x==1||x==LEN||y==1||y==HEI){
				cout << '#';
			}else if(x==pos_x&&y==pos_y){
				cout << 'I';
			}else{
				is_enemy=false;
				for(enemy e : enemies){
					if(x==e.x&&y==e.y){
						cout << 'E';
						is_enemy=true;
						break;
					}
				}
				if(!is_enemy)cout << ' ';
			}
		}
		x=0;
		cout << endl;
	}
	Sleep(150);
}

int main(){
	reset();
	while(true){
		time_1+=k;
		time_2+=k;
		if(_kbhit()){
			char a=_getch();
	        if(a=='a'&&pos_x>2){
	            pos_x-=1;
	        }else if(a=='d'&&pos_x<LEN-1){
	            pos_x+=1;
	        }else if(a=='w'&&pos_y>2){
	            pos_y-=1;
	        }else if(a=='s'&&pos_y<HEI-1){
	            pos_y+=1;
	        }
		}
		if(time_1>=5){
			time_1-=5;
			for(enemy& e : enemies){
				if(e.x>pos_x){
		            e.x-=1;
		        }else if(e.x<pos_x){
		            e.x+=1;
		        }
				if(e.y>pos_y){
		            e.y-=1;
		        }else if(e.y<pos_y){
		            e.y+=1;
		        }
			}
		}
		if(time_2>=100){
			time_2-=100;
			k*=1.05;
			add_enemy();
		}
        flip();
    }
	return 0;
}
