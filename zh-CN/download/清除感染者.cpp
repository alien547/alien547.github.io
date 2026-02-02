/*
我的网站：alien547.github.io 
我的QQ：3882104862 
*/
#include <fstream>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <windows.h>
#include <conio.h>
using namespace std;

const short LEN=21, HEI=11;//地图大小 
short pos_x, pos_y, display, attacke, attacked, teleport_cool, ammo, gun_cool, enemy_health;//玩家位置&显示时间&技能冷却&弹药数&攻击间隔&回血间隔 
char direction;//玩家朝向 
string information, talk_1, talk_2;//消息 
long long health, max_health, score, max_score, upgrade;//玩家血量&分数&升级所需 
float enemy_move, enemy_upgrade, k;//敌人生成参数 
bool enemies_pos[LEN+1][HEI+1], bullets_pos[LEN+1][HEI+1];//标记敌人&子弹位置 
bool teleport, gun, load_gun;//技能&武器参数 
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
	enemies.push_back(enemy{enemy_x, enemy_y, 0, 6});
	enemies_pos[enemy_x][enemy_y]=true;
}

inline void exit(){
	char a;
	system("cls");
	MessageBeep(MB_ICONWARNING);
	printf("退出并保存分数？  （输入‘Y’）");
	scanf("%c", &a);
	if(a=='Y'){
		ofstream data("data.txt");
		data << max(score, max_score);
		data.close();
		run=false;
		MessageBeep(MB_OK);
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
	pos_x=LEN/2, pos_y=HEI/2, direction='w', max_health=100, health=100, score=0, upgrade=1000;
	display=0, attacked=0, attacked=0, teleport=false, gun=false, teleport_cool=0, ammo=6, gun_cool=0, load_gun=false;
	enemy_move=0.0, enemy_upgrade=0.0, k=1.0, enemy_health=0;
	talk_1="我：已到达感染区，请求指令。\n指挥官：清除该区域的感染者。\n我：收到。";
	talk_2="我：..请...求....支......援.........[撕咬声]";
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

inline void talking(string talk){
	short times_1=3, times_2=3;
	printf("\n");
	while(times_1--){
		Beep(440, 1500);
		Sleep(1000);
	}
	printf("[通讯已连接]\n");
	for(int i=0; i<talk.length(); i++){
		Beep(350, 100);
		printf("%c", talk[i]);
	}
	Sleep(1500);
	printf("\n[通讯已中断]");
	while(times_2--){
		Beep(540, 1000);
		Sleep(250);
	}
}

inline void flip(){//刷新屏幕 
	short x=0, y=0;
	system("cls");
	while(y<=HEI){
		while(x<=LEN){
			if((x==0||x==LEN)&&(y==0||y==HEI)){
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
					printf("     方向：%c", direction);
				}else if(y==2){
					printf("     血量：%d/%d", health, max_health);
				}else if(y==3){
					printf("     分数：%d", score);
				}else if(y==4){
					printf("     最高分：%d", max_score);
				}else if(teleport&&y==5){
					printf("     瞬移：%d%%", teleport_cool);
				}else if(gun&&y==6){
					if(load_gun){
						printf("     装填：%d%%", gun_cool);
					}else{
						printf("     弹药：%d", ammo);
					}
				}else if(display>0&&y==8){
					display--;
					printf("     %s", information.c_str());
				}else if(enemy_health!=0&&y==9){
					printf("     敌人血量：%d", enemy_health);
				}
			}
			++x;
		}
		++y;
		x=0;
		printf("\n");
	}
	if(health<=0){
		ofstream data("data.txt");
		data << max(score, max_score);
		data.close();
		talking(talk_2);
		printf("\n\n游戏结束！");
		Sleep(2000);
		printf("  （按下任意键）");
		while(_kbhit())_getch();
		_getch();
		MessageBeep(MB_OK);
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
	}else if(attacke==0&&a=='j'){
		for(enemy& e : enemies){
		    if((e.x==pos_x-1&&direction=='a')||(e.x==pos_x+1&&direction=='d')||(e.y==pos_y-1&&direction=='w')||(e.y==pos_y+1&&direction=='s')){
		    	enemies_pos[e.x][e.y]=false;
		    	if(direction=='a'){
		    		e.x=max(e.x-1, 1);
				}else if(direction=='d'){
					e.x=min(e.x+1, LEN-1);
				}else if(direction=='w'){
					e.y=max(e.y-1, 1);
				}else if(direction=='s'){
					e.y=min(e.y+1, HEI-1);
				}
				enemies_pos[e.x][e.y]=true;
		    	attacke=4;
			    e.health=max(e.health-1, 0);
			    enemy_health=e.health;
			    break;
			} 
		}
	}else if(teleport&&teleport_cool>=50&&a=='l'){//瞬移 
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
	}else if(gun&&ammo>0&&attacke==0&&a=='k'){//攻击 
		ammo--;
		attacke=3;
		add_bullet();
		if(ammo==0){
			load_gun=true;
			gun_cool=0;
		}
	}
}

inline void player_update(){
	attacke=max(attacke-1, 0);
	attacked=max(attacked-1, 0);
	if(attacked==0&&health>0){
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
	if(!teleport&&score>2000){
		teleport=true;
		display=15;
		information="新技能！（瞬移[l]）";
		MessageBeep(MB_ICONINFORMATION);
	}else if(!gun&&score>3000){
		gun=true;
		display=15;
		information="新武器！（手枪[k]）";
		MessageBeep(MB_ICONINFORMATION);
	}
	if(score>=upgrade){//玩家升级 
		upgrade*=2.1;
		display=10;
		information="升级！";
		MessageBeep(MB_ICONINFORMATION);
		max_health+=20;
		health=max_health;
	}
}

inline void enemy_update(){
	vector<enemy> new_enemies;
	enemy_move+=k;
	enemy_upgrade+=k;
	for(enemy& e : enemies){
		if(e.health==0){
			score+=k*100.0;
			enemies_pos[e.x][e.y]=false;
		}else{
			if(e.x==pos_x&&e.y==pos_y&&e.attack==0){
				e.attack=10;
				attacked=15;
				health-=15;
			}
			e.attack=max(e.attack-1, 0);
			new_enemies.push_back(e);
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
	if(enemy_upgrade>=50.0){//敌人刷新&升级 
		enemy_upgrade-=50.0;
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
				enemy_health=e.health;
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
	printf("欢迎来到这个游戏！游戏愉快！\n也欢迎你来到我的网站 alien547.github.io。\n游戏有bug或有改进建议可联系我，QQ：3882104862\n\n按键: W，A，S，D，Esc，Enter。");
	Sleep(1000);
	printf("  （按下任意键）");
	while(_kbhit())_getch();
	_getch();
	MessageBeep(MB_OK);
	srand(time(nullptr));//随机数初始化 
	for(int i=0; i<=LEN; i++){
    	for(int j=0; j<=HEI; j++){
    		enemies_pos[i][j]=false;
    		bullets_pos[i][j]=false;
		}
	}
	reset();
	flip();
	talking(talk_1);
	display=15;
	information="新武器！（刀[j]）";
	MessageBeep(MB_ICONINFORMATION);
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
    printf("感谢你的游玩！\n");
    Sleep(3000);
	return 0;
}
