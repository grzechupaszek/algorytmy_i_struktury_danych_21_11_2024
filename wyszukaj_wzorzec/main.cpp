#include <iostream>
#include <string>
using namespace std;

//wyszukiwanie wzorca - algorytm naiwny 
//Algorytm Rabina Karpa
// Automat skończony

void rk(string str, string pat){
    int a = str.length();
    int b = pat.length();
    int i,j;
    int s=0, p=0;
    const int characters_of_alphabet = 256;
    const int q=101;
    int h=1;

    bool flag = false;
    for (i=0;i<b-1;i++){
        h = (h*characters_of_alphabet)%q;
    }
    for (i=0;i<b;i++){
        s=(characters_of_alphabet*s +str[i])%q;
        p=(characters_of_alphabet*p +pat[i])%q;
    }
    
}
int main(){
    cout<<"Algorytm Rabina-Karpa\n";

    return 0;
}