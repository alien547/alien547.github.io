#include <iostream>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <windows.h>
#include <conio.h>
using namespace std;

const int LEN=21, HEI=11;//地图大小 
int pos_x, pos_y;//玩家位置 
double time_1, time_2, k;//敌人生成参数 
bool enemies_pos[LEN+1][HEI+1];//标记敌人位置 

struct enemy{
	int x, y;
};

vector<enemy> enemies;

void add_enemy(){
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
	enemies.push_back(enemy{enemy_x, enemy_y});
	enemies_pos[enemy_x][enemy_y]=true;
}

void reset(){
	pos_x=LEN/2, pos_y=HEI/2, time_1=0.0, time_2=0.0, k=1.0;
	for(enemy& e : enemies){
		enemies_pos[e.x][e.y]=false;
	}
    enemies.clear();
    add_enemy();
    Sleep(200);
}

void flip(){
	int x=0, y=0;
	system("cls");
	if(enemies_pos[pos_x][pos_y]){
		printf("GameOver!");
		Sleep(2000);
		printf("  (Press)");
		while(_kbhit())_getch();
		_getch();
		reset();
		return;
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
			++x;
		}
		++y;
		x=0;
		printf("\n");
	}
	Sleep(150);//控制刷新频率 
}

int main(){
	srand(time(nullptr));//随机数初始化 
	for(int i=0; i<=LEN; i++){
    	for(int j=0; j<=HEI; j++){
    		enemies_pos[i][j]=false;
		}
	}
	reset();
	while(true){
		time_1+=k;
		time_2+=k;
		if(_kbhit()){
			char a=_getch();//玩家移动 
	        if(a=='a'&&pos_x>1){
	            pos_x-=1;
	        }else if(a=='d'&&pos_x<LEN-1){
	            pos_x+=1;
	        }else if(a=='w'&&pos_y>1){
	            pos_y-=1;
	        }else if(a=='s'&&pos_y<HEI-1){
	            pos_y+=1;
	        }
		}
		if(time_1>=5.0){
			time_1-=5.0;//敌人移动 
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
		if(time_2>=100.0){
			time_2-=100.0;//敌人刷新&升级 
			k*=1.05;
			add_enemy();
		}
        flip();//刷新屏幕 
    }
	return 0;
}
