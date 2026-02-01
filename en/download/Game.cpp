#include <fstream>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <windows.h>
#include <conio.h>
using namespace std;

const short LEN=21, HEI=11;//地图大小 
short pos_x, pos_y, display, attacked, teleport_cool, ammo, gun_cool;//玩家位置&显示时间&技能冷却&弹药数 
char direction;//玩家朝向 
string information;//消息 
unsigned long long health, max_health, score, max_score, upgrade;//玩家血量&分数&升级所需 
float enemy_move, enemy_upgrade, k;//敌人生成参数 
bool enemies_pos[LEN+1][HEI+1], bullets_pos[LEN+1][HEI+1];//标记敌人&子弹位置 
bool teleport, gun, load_gun;//技能参数 
bool run=true;//程序运行参数 

struct bullet{
	short x, y;
	char direction;//方向 
};

struct enemy{
	short x, y, attack, health;//位置&攻击间隔 
};

vector<bullet> bullets;
vector<enemy> enemies;

inline void add_bullet(){
	bullets.push_back(bullet{pos_x, pos_y, direction});
	bullets_pos[pos_x][pos_y]=true;
}

inline void add_enemy(){
	short enemy_x, enemy_y;
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
	enemies.push_back(enemy{enemy_x, enemy_y, 0, 5});
	enemies_pos[enemy_x][enemy_y]=true;
}

inline void exit(){
	char a;
	system("cls");
	printf("Exit and save data?  (Enter 'Y')");
	scanf("%c", &a);
	if(a=='Y'){
		ofstream data("data.txt");
		data << max(score, max_score);
		data.close();
		run=false;
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
	pos_x=LEN/2, pos_y=HEI/2, direction='w', enemy_move=0.0, enemy_upgrade=0.0, k=1.0, max_health=100, health=100, score=0, upgrade=1000, display=0, attacked=0, teleport=false, gun=false, teleport_cool=0, ammo=6, gun_cool=0, load_gun=false;
	for(bullet& b : bullets){
		bullets_pos[b.x][b.y]=false;
	}
	for(enemy& e : enemies){
		enemies_pos[e.x][e.y]=false;
	}
	bullets.clear();
    enemies.clear();
    add_enemy();
    Sleep(200);
}

inline void flip(){//刷新屏幕 
	short x=0, y=0;
	system("cls");
	while(y<=HEI){
		while(x<=LEN){
			if(x==0||x==LEN||y==0||y==HEI){
				printf("#");//墙 
			}else if(x==pos_x&&y==pos_y){
				printf("I");//玩家 
			}else if(enemies_pos[x][y]){
				printf("E");//敌人 
			}else if(bullets_pos[x][y]){
				printf("'");//子弹 
			}else{
				printf(" ");//无 
			}
			if(x==LEN){
				if(y==1){
					printf("     Direction: %c", direction);
				}else if(y==2){
					printf("     Health: %d/%d", health, max_health);
				}else if(y==3){
					printf("     Score: %d", score);
				}else if(y==4){
					printf("     MaxScore: %d", max_score);
				}else if(teleport&&y==5){
					printf("     Teleport: %d%%", teleport_cool);
				}else if(gun&&y==6){
					if(load_gun){
						printf("     Load: %d%%", gun_cool);
					}else{
						printf("     Ammo: %d", ammo);
					}
				}else if(display>0&&y==8){
					display--;
					printf("     %s", information.c_str());
				}
			}
			++x;
		}
		++y;
		x=0;
		printf("\n");
	}
	if(health==0){
		ofstream data("data.txt");
		data << max(score, max_score);
		data.close();
		printf("\nGameOver!");
		Sleep(2000);
		printf("  (Press)");
		while(_kbhit())_getch();
		_getch();
		reset();
		return;
	}
	Sleep(150);//控制刷新频率 
}

inline void player_operate(){//玩家操作 
	char a=_getch();
	if(a==27){//退出（Esc键）
		exit();
	}else if(a==direction){
		if(a=='a'&&pos_x>1){//移动 
		    pos_x--;
		}else if(a=='d'&&pos_x<LEN-1){
		    pos_x++;
		}else if(a=='w'&&pos_y>1){
		    pos_y--;
		}else if(a=='s'&&pos_y<HEI-1){
		    pos_y++;
		}
	}else if(a=='w'||a=='a'||a=='s'||a=='d'){
		direction=a;
	}else if(teleport&&teleport_cool>=50&&a=='j'){//瞬移 
	    teleport_cool-=50;
		if(direction=='a'){
		    pos_x=max(pos_x-6, 1);
		}else if(direction=='d'){
		    pos_x=min(pos_x+6, LEN-1);
		}else if(direction=='w'){
		    pos_y=max(pos_y-4, 1);
		}else if(direction=='s'){
		    pos_y=min(pos_y+4, HEI-1);
		}
	}else if(gun&&ammo>0&&a=='k'){//攻击 
		ammo--;
		add_bullet();
		if(ammo==0){
			load_gun=true;
			gun_cool=0;
		}
	}
}

inline void player_update(){
	attacked=max(attacked-1, 0);
	if(attacked==0){
		health=min(health+1, max_health);
	}
	score+=k*10;
	max_score=max(max_score, score);
	teleport_cool=min(teleport_cool+4, 100);
	if(load_gun){
		gun_cool+=4;
		if(gun_cool>=100){
			load_gun=false;
			ammo+=6;
		}
	}
	if(!teleport&&score>1000){
		teleport=true;
		display=15;
		information="New skill!(Teleport[j])";
	}else if(!gun&&score>2000){
		gun=true;
		display=15;
		information="New skill!(Gun[k])";
	}
	if(score>=upgrade){//玩家升级 
		upgrade*=2.1;
		display=10;
		information="Upgraded!";
		max_health+=20;
		health=max_health;
	}
}

inline void enemy_update(){
	vector<enemy> new_enemies;
	enemy_move+=k;
	enemy_upgrade+=k;
	for(enemy& e : enemies){
		if(e.health!=0){
			if(e.x==pos_x&&e.y==pos_y&&e.attack==0){
				e.attack=10;
				attacked=15;
				health-=20;
				if(health<=0){
					health=0;
				}
			}
			e.attack=max(e.attack-1, 0);
			new_enemies.push_back(e);
		}else{
			score+=k*250.0;
			enemies_pos[e.x][e.y]=false;
		}
	}
	enemies=new_enemies;
	if(enemy_move>=5.0){//敌人移动 
		enemy_move-=5.0;
		for(enemy& e : enemies){
			enemies_pos[e.x][e.y]=false;
		}
		for(enemy& e : enemies){
			if(e.x>pos_x){
		        e.x--;
		    }else if(e.x<pos_x){
		        e.x++;
		    }
			if(e.y>pos_y){
		        e.y--;
		    }else if(e.y<pos_y){
		        e.y++;
		    }
		}
		for(enemy& e : enemies){
			enemies_pos[e.x][e.y]=true;
		}
	}
	if(enemy_upgrade>=30.0){//敌人刷新&升级 
		enemy_upgrade-=30.0;
		k*=1.02;
		add_enemy();
	}
}

inline void bullet_update(){
	vector<bullet> new_bullets;
	vector<enemy> new_enemies;
	for(bullet& b : bullets){
		bullets_pos[b.x][b.y]=false;
	}
	for(bullet& b : bullets){
		if(b.direction=='a'){
			b.x--;
		}else if(b.direction=='d'){
			b.x++;
		}else if(b.direction=='w'){
			b.y--;
		}else if(b.direction=='s'){
			b.y++;
		}
	}
	for(bullet& b : bullets){
		bool hit=false;
		for(enemy& e : enemies){
			if(b.x==e.x&&b.y==e.y){
				e.health=max(e.health-1, 0);
				hit=true;
				break;
			}
		}
		if(!hit&&0<b.x&&b.x<LEN&&0<b.y&&b.y<HEI){
			new_bullets.push_back(b);
			bullets_pos[b.x][b.y]=true;
		}
	}
	bullets=new_bullets;
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
    		bullets_pos[i][j]=false;
		}
	}
	reset();
	while(run){
		player_update();
		enemy_update();
		bullet_update();
		if(_kbhit()){
		    player_operate();
		}
        flip();
    }
    system("cls");
    printf("Thank you for playing!\n");
    Sleep(3000);
	return 0;
}
