#include <bits/stdc++.h>
using namespace std;

int main(){
	string input;
	int  target_number, min_number, max_number, guess_number, guess_times, play_times=0;
	bool valid, win=false, reset=true;
	srand(time(0));
	cout << "欢迎来到猜数游戏！" << endl << "按下 ENTER 开始游戏..." << endl;
    cin.get();
    system("cls");
    play_times+=1;
	cout << "第" << play_times << "局" << endl;
	while(true){
		if(reset){
			target_number=rand()%100+1, min_number=1, max_number=100, guess_times=0;
			reset=false;
		}
		valid=true;
		cout << endl << min_number << "到" << max_number << "之间：";
		cin >> input;
        for(int i=0; i<input.length(); i++){
            if(!isdigit(input[i])){
                valid=false;
                break;
            }
        }
        if(!valid){
        	cout << "请输入一个整数" << endl;
        	continue;
		}
        stringstream ss(input);
        ss >> guess_number;
        if(guess_number<min_number||guess_number>max_number){
        	cout << "请输入有效数字" << endl;
        	continue;
		}
		guess_times+=1;
		if(guess_number<target_number){
			min_number=guess_number+1;
		}else if(guess_number>target_number){
			max_number=guess_number-1;
		}else{
			cout << endl << "你猜对了！" << endl << "你猜了" << guess_times << "次" << endl;
			win=true;
		}
		if(win){
			cout << "继续？（输入1退出，否则继续）："; 
			cin >> input;
			system("cls");
			if(input.length()==1&&input[0]=='1'){
				break;
			}else{
				play_times+=1;
				win=false, reset=true;
				cout << "第" << play_times << "局" << endl;
			}
		}
	}
	cout << endl << "谢谢游玩！" << endl; 
	return 0;
}
