#include <fstream>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <windows.h>
#include <conio.h>
using namespace std;

const int LEN=21, HEI=11;//地图大小 
int pos_x, pos_y, display, attacked;//玩家位置&显示时间 
long long health, max_health, score, max_score, update;//玩家血量&分数&升级所需 
double enemy_move, enemy_update, k;//敌人生成参数 
bool run=true;//程序运行参数 
bool enemies_pos[LEN+1][HEI+1];//标记敌人位置 

struct enemy{
	int x, y, attack;//位置&攻击间隔 
};

vector<enemy> enemies;

inline void add_enemy(){
	int enemy_x, enemy_y;
	switch(rand()%4){//敌人生成位置随机 
		case 0:
			enemy_x=1;
			enemy_y=1;
			break;
		case 1:
			enemy_x=LEN-1;
			enemy_y=1;
			break;
		case 2:
			enemy_x=1;
			enemy_y=HEI-1;
			break;
		case 3:
			enemy_x=LEN-1;
			enemy_y=HEI-1;
			break;
	}
	enemies.push_back(enemy{enemy_x, enemy_y, 0});
	enemies_pos[enemy_x][enemy_y]=true;
}

inline void exit(){
	char a;
	system("cls");
	printf("Exit and save data?  (Enter 'Y')");
	scanf("%c", &a);
	if(a=='Y'){
		run=false;
		ofstream data("data.txt");
		data << max(score, max_score);
		data.close();
	}
}

inline void reset(){
	ifstream data("data.txt");
	if(data.is_open()){
		data >> max_score;
	}else{
		max_score=0;
	}
	data.close();
	pos_x=LEN/2, pos_y=HEI/2, enemy_move=0.0, enemy_update=0.0, k=1.0, max_health=100, health=100, score=0, update=1000, display=0, attacked=0;
	for(enemy& e : enemies){
		enemies_pos[e.x][e.y]=false;
	}
    enemies.clear();
    add_enemy();
    Sleep(200);
}

inline void flip(){
	int x=0, y=0;
	system("cls");
	for(enemy& e : enemies){
		if(e.x==pos_x&&e.y==pos_y&&e.attack==0){
			e.attack+=10;
			attacked=10;
			health-=20;
			if(health<=0){
				ofstream data("data.txt");
				data << max(score, max_score);
				data.close();
				printf("GameOver!");
				Sleep(2000);
				printf("  (Press)");
				while(_kbhit())_getch();
				_getch();
				reset();
				return;
			}
		}
	}
	while(y<=HEI){
		while(x<=LEN){
			if(x==0||x==LEN||y==0||y==HEI){
				printf("#");//墙 
			}else if(x==pos_x&&y==pos_y){
				printf("I");//玩家 
			}else if(enemies_pos[x][y]){
				printf("E");//敌人 
			}else{
				printf(" ");//无 
			}
			if(x==LEN){
				if(y==2){
					printf("     Health: %d/%d", health, max_health);
				}else if(y==4){
					printf("     Score: %d", score);
				}else if(y==5){
					printf("     MaxScore: %d", max_score);
				}else if(display>0&&y==7){
					display--;
					printf("     Updated!");
				}
			}
			++x;
		}
		++y;
		x=0;
		printf("\n");
	}
	Sleep(150);//控制刷新频率 
}

int main(){
	printf("Welcome to the game! Enjoy yourself!\nAnd welcome to visit my website alien547.github.io.\n\nKeys: W, A, S, D, Esc and Enter.");
	Sleep(1000);
	printf("  (Press)");
	while(_kbhit())_getch();
	_getch();
	srand(time(nullptr));//随机数初始化 
	for(int i=0; i<=LEN; i++){
    	for(int j=0; j<=HEI; j++){
    		enemies_pos[i][j]=false;
		}
	}
	reset();
	while(run){
		enemy_move+=k;
		enemy_update+=k;
		if(attacked>0){
			attacked--;
		}else{
			attacked=0;
			if(health<max_health){
				health++;
			}else{
				health=max_health;
			}
		}
		score+=k*10;
		if(score>max_score){
			max_score=score;
		}
		for(enemy& e : enemies){
			if(e.attack>0){
				e.attack-=1;
			}else{
				e.attack=0;
			}
		}
		if(_kbhit()){//玩家操作 
			char a=_getch();
			if(a==27){//退出（Esc键） 
				exit();
			}else if(a=='a'&&pos_x>1){//移动 
	            pos_x-=1;
	        }else if(a=='d'&&pos_x<LEN-1){
	            pos_x+=1;
	        }else if(a=='w'&&pos_y>1){
	            pos_y-=1;
	        }else if(a=='s'&&pos_y<HEI-1){
	            pos_y+=1;
	        }
		}
		if(enemy_move>=5.0){//敌人移动 
			enemy_move-=5.0;
			for(enemy& e : enemies){
				enemies_pos[e.x][e.y]=false;
			}
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
			for(enemy& e : enemies){
				enemies_pos[e.x][e.y]=true;
			}
		}
		if(enemy_update>=100.0){//敌人刷新&升级 
			enemy_update-=100.0;
			k*=1.05;
			add_enemy();
		}
		if(score>=update){//玩家升级 
			update*=2.1;
			display+=10;
			max_health+=20;
			health=max_health;
		}
        flip();//刷新屏幕 
    }
    system("cls");
    printf("Thank you for playing!\n");
	return 0;
}
