#include <iostream>
#include "../include/EasyCodingEditor.h"
#include "../include/Code_highlighting.h"
#include "../include/Code_completion.h"

using namespace edt;
using namespace std;
using namespace cht;
using namespace cct;

int main(){
    edt::easyhtmleditor e;
    cct::Code_completion c2;
    e.print_size();
    //初始化字符集
    int k = 0;
    for (int i = 0; i < e.word.size()-1; i++){
        for (int j = 0; j < e.word.size()-i-1; j++){
            if (e.word[j] > e.word[j+1]){
                k = e.word[j];
                e.word[j] = e.word[j+1];
                e.word[j+1] = k;
            }   
        }
    }
    e.commander();
    return 0;
}    