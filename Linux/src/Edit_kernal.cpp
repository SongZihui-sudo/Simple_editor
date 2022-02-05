#include "../include/EasyCodingEditor.h"
#include "../include/Code_highlighting.h"
#include "../include/Code_completion.h"
#include "../include/Markdown_parser.h"
#include <termio.h>
#include <linux/input.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdint.h>
#include "../include/key_code.h"

using namespace edt;
using namespace std;
using namespace cht;
using namespace cct;
using namespace mpt;

int _get_input(void);

cht::Code_highlighting C3;
cct::Code_completion cc2;
mpt::Markdown_parser mp2; 
 
int easyhtmleditor::Edit_kernal(){
    setvbuf(stdout,NULL,_IONBF,0); 
	if (!out_data.empty());
    else{
        page_arr = {{""}};
    }        
    string        str;
    int 		  ch1 = 0;
    char 		  ch2 ;
    SetPos(0,0);
    int 		  num = 0;  
    string        Parr_str;

    int           l_ret = -1;
	int           i = 0;
	
	int           key_fd  = 0;
	
	int           enter_num = 0;
		
	
	while(!kbhit()){
	    ch2 = _get_input();
	    //l_ret = lseek(key_fd, 0, SEEK_SET);
		//l_ret = read(key_fd, &key_event, sizeof(key_event));
		//if(l_ret)
		//{		            
			//if((key_event.type == EV_KEY&& (key_event.value == 0 || key_event.value == 1)) || ch2){	
			//printf("key code %d\n",key_event.code);
		printf("key asill %d\n", ch2);
			switch (ch2){		    
            case BACKSPACE:
                if (cc2.c_str.empty());
                else{
                        cc2.c_str.pop_back();
                    }
                if (last_str.empty());
                else{                                           
                    last_str.pop_back();
                    }
                if (pos_x-1){                        
                    num = pos_x-1;
                    pos_x--;   
                    last_x--;                      
                    SetPos(pos_x,pos_y -  (page_now-1)*page_y);
                    if (num<page_arr[page_now-1][pos_y -  (page_now-1)*page_y].size()&&pos_y- (page_now-1)*page_y<=page_arr[page_now-1].size()){
                        Parr_str =page_arr[page_now-1][pos_y -  (page_now-1)*page_y].substr(num);
                        page_arr[page_now-1][pos_y -  (page_now-1)*page_y].erase(pos_x);
                        page_arr[page_now-1][pos_y -  (page_now-1)*page_y] += Parr_str; 
                    }
                    else if(num==page_arr[page_now-1][pos_y -  (page_now-1)*page_y].size()&&pos_y- (page_now-1)*page_y<=page_arr[page_now-1].size()){
                        Parr_str = page_arr[page_now-1][pos_y-(page_now-1)*page_y][page_arr[page_now-1][pos_y-(page_now-1)*page_y].length()-1];
                        page_arr[page_now-1][pos_y -  (page_now-1)*page_y].pop_back();
                    }
                    else;
                    SetPos(0,pos_y -  (page_now-1)*page_y);
                    printf("\033[K");
                    SetPos(0,pos_y -  (page_now-1)*page_y);
                    cout<<page_arr[page_now-1][pos_y -  (page_now-1)*page_y];
                    //printf("%s",page_arr[page_now-1][pos_y -  (page_now-1)*page_y].c_str());
                    }
                else if(pos_x-1 == 0 && pos_y -  (page_now-1)*page_y == 0){
                    pos_x = 0;
                    last_x = 0;
                    pos_y = (page_now-1)*page_y;
                }
                else if(pos_x-1==0){
                    pos_x = page_arr[page_now-1][pos_y-1 -  (page_now-1)*page_y].size()+1;//  out_data[pos_y-1].size();                    
                    page_arr[page_now-1][pos_y-1 -  (page_now-1)*page_y] += page_arr[page_now-1][pos_y -  (page_now-1)*page_y]; //out_data[pos_y];
                    page_arr[page_now-1].erase(page_arr[page_now-1].begin()+pos_y -  (page_now-1)*page_y);
                    if (page_arr[page_now-1].size() < page_y&&exact_buffer.size()!=0){
                        page_arr[page_now-1].push_back(exact_buffer.top());
                        exact_buffer.pop();
                    }    
                    last_str = "";
                    CLEAR();
                    for (int i = 0; i < page_arr[page_now-1].size(); i++){
                        //printf("%s\n",page_arr[page_now-1][i].c_str());
                        cout<<page_arr[page_now-1][i]<<endl;
                    }
                    pos_y--;
                }      
                else;   
                C3.Lexical_analysis(page_arr[page_now-1]);                                
                    SetPos(pos_x,pos_y -  (page_now-1)*page_y);
                    break;
                	//回车
                    case ENTER:                   
                        last_str = "";                   
                	    num = int(pos_y) -  (page_now-1)*page_y;
                	    if (pos_x-1>=page_arr[page_now-1][pos_y -  (page_now-1)*page_y].size()){
                    	    page_arr[page_now-1].insert(page_arr[page_now-1].begin()+num+1,""); 
                	    }         
                	    else{
                    	    str = page_arr[page_now-1][pos_y -  (page_now-1)*page_y].substr(pos_x);
                    	    page_arr[page_now-1][pos_y -  (page_now-1)*page_y].erase(pos_x);
                    	    page_arr[page_now-1].insert(page_arr[page_now-1].begin()+num+1,str);
                        }
                	    if (page_arr[page_now-1].size() > page_y){
                    	    exact_buffer.push(page_arr[page_now-1][page_arr[page_now-1].size()-1]);
                    	    page_arr[page_now-1].pop_back();
                	    }       
                	    //CLEAR();
                	    //printf( "\33[2J" ); 
                        for (int i = 0; i < page_arr[page_now-1].size(); i++){
                            if (page_arr[page_now-1][i]!=""){
                                cout<<page_arr[page_now-1][i]<<endl;
                                //printf("%s\n",page_arr[page_now-1][i].c_str());
                            }
                            else{
                                printf("\n");
                            }
                        }                    
                	    //C3.Lexical_analysis(page_arr[page_now-1]);                                
                	    pos_x = 1;
                	    last_x = 0;                    
                	    pos_y++;
                	    if (pos_y>=page_y*(page_now)){
                    	    pos_y--;
                    	    break;
                	    }
                	    else{
                    	    SetPos(pos_x-1,pos_y-(page_now-1)*page_y);    
                        }
                	    break;
                    //TAB
                case TAB:
                    if (cc2.c_str.empty());
                    else{
                        cc2.c_str.clear();
                        cc2.c_str = "";
                        last_x = 0;
                        last_str.push_back(' ');
                        last_str.push_back(' ');
                        last_str.push_back(' ');
                        last_str.push_back(' ');
                        last_str.push_back(' ');
                        last_str.push_back(' ');
                        last_str.push_back(' ');
                        last_str.push_back(' ');
                    }
                    if (pos_x){                    
                        num = pos_x-1;                    
                    }
                    else{
                        num = 0;
                    }
                    pos_x +=8;
                    page_arr[page_now-1][pos_y -  (page_now-1)*page_y].insert(page_arr[page_now-1][pos_y -  (page_now-1)*page_y].begin()+num,' ');
                    page_arr[page_now-1][pos_y -  (page_now-1)*page_y].insert(page_arr[page_now-1][pos_y -  (page_now-1)*page_y].begin()+num+1,' ');
                    page_arr[page_now-1][pos_y -  (page_now-1)*page_y].insert(page_arr[page_now-1][pos_y -  (page_now-1)*page_y].begin()+num+2,' ');
                    page_arr[page_now-1][pos_y -  (page_now-1)*page_y].insert(page_arr[page_now-1][pos_y -  (page_now-1)*page_y].begin()+num+3,' ');
                    page_arr[page_now-1][pos_y -  (page_now-1)*page_y].insert(page_arr[page_now-1][pos_y -  (page_now-1)*page_y].begin()+num,' ');
                    page_arr[page_now-1][pos_y -  (page_now-1)*page_y].insert(page_arr[page_now-1][pos_y -  (page_now-1)*page_y].begin()+num+1,' ');
                    page_arr[page_now-1][pos_y -  (page_now-1)*page_y].insert(page_arr[page_now-1][pos_y -  (page_now-1)*page_y].begin()+num+2,' ');
                    page_arr[page_now-1][pos_y -  (page_now-1)*page_y].insert(page_arr[page_now-1][pos_y -  (page_now-1)*page_y].begin()+num+3,' ');
                    SetPos(0,pos_y -  (page_now-1)*page_y);
                    printf("\033[K");
                    SetPos(0,pos_y -  (page_now-1)*page_y);
                    cout<<page_arr[page_now-1][pos_y -  (page_now-1)*page_y];
                    //printf("%s",page_arr[page_now-1][pos_y -  (page_now-1)*page_y].c_str());
                    C3.Lexical_analysis(page_arr[page_now-1]);                                
                    SetPos(pos_x,pos_y -  (page_now-1)*page_y);                    
                    break;
                //空格
                case SPACE:
                    if (cc2.c_str.empty());
                    else{
                        cc2.c_str.clear();
                        cc2.c_str = "";
                        last_x = 0;
                        last_str.push_back(' ');
                    }
                    if (pos_x){                    
                        num = pos_x-1;                    
                    }
                    else{
                        num = 0;
                    }
                    pos_x++;
                    page_arr[page_now-1][pos_y -  (page_now-1)*page_y].insert(page_arr[page_now-1][pos_y -  (page_now-1)*page_y].begin()+num,' ');    
                    SetPos(0,0);
                    printf("\033[K");
                    SetPos(0,pos_y -  (page_now-1)*page_y);
                    cout<<page_arr[page_now-1][pos_y -  (page_now-1)*page_y];
                    //printf("%s",page_arr[page_now-1][pos_y -  (page_now-1)*page_y].c_str());
                    C3.Lexical_analysis(page_arr[page_now-1]);                                
                    SetPos(pos_x,pos_y -  (page_now-1)*page_y);
                    break;
                case PAGE_UP:
                        if (page_now==1){
                            page_now = page_now;
                        }
                        else{
                            page_now--;
                            pos_y= pos_y - page_y;
                        }
                        CLEAR();
                        for (int i = 0; i < page_arr[page_now-1].size(); i++){
                            cout<<page_arr[page_now-1][i]<<endl;
                        }
                        C3.Lexical_analysis(page_arr[page_now-1]);     
                        SetPos(page_x-20,page_y);
                        //cout<<"page_y"<<page_now;  
                        printf("Page:%d",page_now);
                        SetPos(0,0);
                        if (exact_buffer.size()){
                            for (int i = 0; i < exact_buffer.size(); i++){
                                exact_buffer.pop();
                            }
                        }
                        else;
                        break;
                    case PAGE_DOWN:
                        if (page_now<=page_y&&page_y){ 
                            page_now++;
                            pos_y+=page_y;           
                        }
                        else{
                            deque <string> space_line;
                            space_line.push_back("");
                            page_now++;
                            page_y++;
                            pos_y+=page_y;
                            page_arr.push_back(space_line);
                            pos_x = 0;
                        }
                        CLEAR();
                        for (int i = 0; i < page_arr[page_now-1].size(); i++){
                            cout<<page_arr[page_now-1][i]<<endl;
                        }
                        C3.Lexical_analysis(page_arr[page_now-1]);  
                        SetPos(page_x-20,page_y);
                        //cout<<"page_y"<<page_now;
                        printf("Page:%d",page_now);
                        SetPos(0,0);
                        if (exact_buffer.size()){
                            for (int i = 0; i < exact_buffer.size(); i++){
                                exact_buffer.pop();
                            }   
                        }
                        break;
                    //上
                    case UP:
                       //CLEAR();
                        if (pos_y <= (page_now-1)*page_y){
                            pos_y = pos_y;
                            break;
                        }
                        else{
                            if (pos_y){
                                pos_y--;
                            }
                            else;
                        }
                        SetPos(page_x-10,page_y);
                        //cout<<"Line:"<<pos_y;
                        printf("Lines:%d",pos_y);
                        pos_x = page_arr[page_now-1][pos_y-  (page_now-1)*page_y].size();
                        last_x = page_arr[page_now-1][pos_y-  (page_now-1)*page_y].size();
                        SetPos(0,0);
                        //printg(page_arr[page_now-1]);
                        SetPos(pos_x,pos_y -  (page_now-1)*page_y);
                        break;   
                    //下
                    case DOWN:
                    	//CLEAR();
                        if (pos_y-  (page_now-1)*page_y>=page_arr[page_now-1].size()-1);
                        else pos_y++;
                        SetPos(page_x-10,page_y);
                        //cout<<"Line:"<<pos_y;
                        printf("Lines:%d",pos_y);
                        pos_x = page_arr[page_now-1][pos_y-  (page_now-1)*page_y].size();
                        SetPos(0,0);
                        //printg(page_arr[page_now-1]);                      
                        SetPos(pos_x,pos_y -  (page_now-1)*page_y);
                        break; 
                    //左
                    case LEFT: 
                        //CLEAR();
                        if (pos_x){
                            pos_x--;
                            last_x--;
                        }
                        else; 
                        SetPos(page_x-30,page_y);
                        //cout<<pos_x<<"th";           
                        printf("%dth",pos_x);
                        SetPos(0,0);
                        //printg(page_arr[page_now-1]);                        
                        SetPos(pos_x,pos_y -  (page_now-1)*page_y);
                        break;
                    //右
                    case RIGHT:
                        //CLEAR();
                        if (pos_x>=page_arr[page_now-1][pos_y-  (page_now-1)*page_y].size()) pos_x = page_arr[page_now-1][pos_y-  (page_now-1)*page_y].size();
                        else{
                            pos_x++;
                            last_x++;                    
                        } //
                        SetPos(page_x-30,page_y);
                        //cout<<pos_x<<"th"; 
                        printf("%dth",pos_x);
                        SetPos(0,0);
                        //printg(page_arr[page_now-1]);                        
                        SetPos(pos_x,pos_y -  (page_now-1)*page_y);
                        break;     
                    //case ESC:
                        //return true;
                    default:     
                        int bit_c = 0;
                        string input_str;
                        int mid = 0;
                        int left = word[0];
                        int right = word[word.size()-1];                            
                        for (int i = 0; i < word.size(); i++){
                            if (left==0&&right==0){
                                break;
                            }
                            mid = (left+right)/2;
                            if (ch2>mid){
                                left = mid + 1;                            
                            }
                            else if(ch2<mid){
                                right = mid -1;
                            }
                            else{
                                bit_c =1; 
                                SetPos(pos_x,pos_y -  (page_now-1)*page_y);  
                                printf("%c",ch2);                            
                                pos_x++;
                                last_x++;
                                break;
                            }
                        }
                        if (bit_c){
                            if (pos_x==2&&pos_y-  (page_now-1)*page_y>page_arr[page_now-1].size()){
                                input_str.push_back(ch2);
                                page_arr[page_now-1] .push_back(input_str);
                                input_str.clear();
                            }
                            else if(pos_x==2&&pos_y -  (page_now-1)*page_y<=page_arr[page_now-1].size()){
                                page_arr[page_now-1][pos_y -  (page_now-1)*page_y].insert(page_arr[page_now-1][pos_y -  (page_now-1)*page_y].begin(),ch2);
                                SetPos(0,pos_y -  (page_now-1)*page_y);
                                cout<<page_arr[page_now-1][pos_y -  (page_now-1)*page_y];
                                //printf("%s",page_arr[page_now-1][pos_y - page_y*(page_now-1)].c_str());
                            }                 
                            else if(pos_y-  (page_now-1)*page_y<=page_arr[page_now-1].size()&&pos_x-1<=page_arr[page_now-1][pos_y -  (page_now-1)*page_y].size()){
                                num = int(pos_x)-1;
                                page_arr[page_now-1][pos_y -  (page_now-1)*page_y].insert(page_arr[page_now-1][pos_y -  (page_now-1)*page_y].begin()+num-1,ch2);
                                SetPos(0,pos_y -  (page_now-1)*page_y);
                                printf("\033[K");
                                SetPos(0,pos_y -  (page_now-1)*page_y);
                                cout<<page_arr[page_now-1][pos_y -  (page_now-1)*page_y];
                                //printf("%s",page_arr[page_now-1][pos_y -  (page_now-1)*page_y].c_str());
                            }
                            else if(pos_x-1 > page_arr[page_now-1][pos_y -  (page_now-1)*page_y].size()&&pos_y -  (page_now-1)*page_y<=page_arr[page_now-1].size()){
                                page_arr[page_now-1][pos_y -  (page_now-1)*page_y].push_back(ch2);
                            }
                            else;  
                            /*
                            string code_compl = cc2.Lexical_analysis(ch2,pos_y -  (page_now-1)*page_y,last_x);
                            if (last_x-1  <= cc2.c_str.size()){   
                                if (code_compl[code_compl.length()-1]!=ch2){
                                    SetPos(0,pos_y - (page_now-1)*page_y);   
                                    printf("\033[K");                                
                                    page_arr[page_now-1][pos_y - (page_now-1)*page_y].clear();
                                    last_str+=code_compl;                            
                                    page_arr[page_now-1][pos_y - (page_now-1)*page_y] = last_str ;
                                    SetPos(0 ,pos_y - (page_now-1)*page_y);    
                                    //cout<<page_arr[page_now-1][pos_y - (page_now-1)*page_y];
                                    printf("%s",page_arr[page_now-1][pos_y - (page_now-1)*page_y].c_str());
                                    C3.Lexical_analysis(page_arr[page_now-1]); 
                                    pos_x = page_arr[page_now-1][pos_y - (page_now-1)*page_y].size(); 
                                    SetPos(pos_x,pos_y -  (page_now-1)*page_y);                              
                                    break;
                                }
                                else;
                                
                            }       
                            else;  */                       
                            //C3.Lexical_analysis(page_arr[page_now-1]);                                
                            SetPos(pos_x,pos_y -  (page_now-1)*page_y);
                        }  
                        break;
			    }
            //}
        //}
    }
	return 0;
}

//获取键盘数据
char easyhtmleditor::_get_input(void){
    char c;
    //不回显
    system("stty -echo");  
    system("stty -icanon");//设置一次性读完操作，如使用getchar()读操作，不需要按enter
    c=getchar();
    system("stty icanon");//取消上面的设置
    system("stty echo");//回显
    return c;
}

int easyhtmleditor::kbhit(void){
    struct termios oldt, newt;
    int ch;
    int oldf;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);
    ch = getchar();
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    fcntl(STDIN_FILENO, F_SETFL, oldf);
    if(ch != EOF){
        ungetc(ch, stdin);
        return 1;
    }
    return 0;
}