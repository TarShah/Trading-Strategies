#include <iostream>
#include <pthread.h>
#include <format>
#include <cstdlib>
#include <fstream>
#include <cstdio>
using namespace std;
string start_date ="";
string symbol = "";
string end_date = "";


void* basic(void* arg) {
    string command = "g++ BASIC.cpp -o BASIC";
    string execution = "./BASIC "+ symbol+" 7 5 "+ start_date + " basic";
    system(&command[0]);
    system(&execution[0]);
    string executable = "BASIC";
    remove(&executable[0]);
    return NULL;
}

void* DMA(void* arg) {
    string command = "g++ DMA.cpp -o DMA";
     //   ./DMA $(symbol) $(n) $(x) $(p) "$(start_date)";
    string execution = "./DMA "+symbol+" 50 5 2 "+start_date+" dma";
    system(&command[0]);
    system(&execution[0]);
    string executable = "DMA";
    remove(&executable[0]);
    return NULL;
}

void* DMAplus(void* arg) {
    string command = "g++ DMA++.cpp -o DMA++";
   //     ./DMA++ $(symbol) $(n) $(x) $(p) $(max_hold_days) $(c1) $(c2) "$(start_date)";
    string execution = "./DMA++ "+symbol+" 14 5 5 28 2 0.2 "+start_date+" dma++";
    system(&command[0]);
    system(&execution[0]);
    string executable = "DMA++";
    remove(&executable[0]);
    return NULL;
}

void* MACD(void* arg) {
    string command = "g++ MACD.cpp -o MACD";
    //./MACD $(symbol) $(x) "$(start_date)";
    string execution = "./MACD "+symbol+" 5 "+start_date+" macd";
    system(&command[0]);
    system(&execution[0]);
    string executable = "MACD";
    remove(&executable[0]);
    return NULL;
}

void* RSI(void* arg) {
    string command = "g++ RSI.cpp -o RSI";
   // ./RSI $(symbol) $(n) $(x) $(oversold_threshold) $(overbought_threshold) "$(start_date)";
//    symbol=SBIN x=3 n=14 oversold_threshold=30 overbought_threshold=70
//    start_date="a"
    string execution = "./RSI "+symbol+" 14 5 30 70 "+start_date+" rsi";
    system(&command[0]);
    system(&execution[0]);
    string executable = "RSI";
    remove(&executable[0]);
    return NULL;
}

void* ADX(void* arg) {
    string command = "g++ ADX.cpp -o ADX";
//    make strategy="LINEAR_REGRESSION" symbol=SBIN x=3 p=2 train_start_date="a"
//    train_end_date="b" start_date="c" end_date="d"
    
    string execution = "./ADX "+symbol+" 5 14 25 "+start_date+" rsi";
    system(&command[0]);
    system(&execution[0]);
    string executable = "ADX";
    remove(&executable[0]);
    return NULL;
}

void* LINEAR_REGRESSION(void* arg) {
    string command = "g++ LINEAR_REGRESSION.cpp -o LINEAR_REGRESSION";
    
//    make strategy="LINEAR_REGRESSION" symbol=SBIN x=3 p=2 train_start_date="a"
//    train_end_date="b" start_date="c"
    string train_start_date = start_date.substr(0,6)+to_string(stoi(start_date.substr(6,4))-1);
    string train_end_date = start_date.substr(0,6)+to_string(stoi(end_date.substr(6,4))-1);
    string execution = "./LINEAR_REGRESSION "+symbol+" 5 p "+train_start_date+" "+train_end_date+" "+ start_date+" linear_regression";
    system(&command[0]);
    system(&execution[0]);
    string executable = "LINEAR_REGRESSION";
    remove(&executable[0]);
    return NULL;
}









int main(int argc, const char * argv[]) {
    
    // i am assuming that its argv is like ./a.out symbol start_date end_date
    // do not forget to create more data for this file i.e. data of last 1 more year
    vector<string> methods = {"basic","dma","dma++","macd","rsi","adx","linear_regression"};
    symbol = string(argv[1]);
    start_date = string(argv[2]);
    end_date = string(argv[3]);
    pthread_t thread1, thread2, thread3, thread4, thread5, thread6, thread7;

    pthread_create(&thread1, NULL, basic, NULL);
    pthread_create(&thread2, NULL, DMA, NULL);
    pthread_create(&thread3, NULL, DMAplus, NULL);
    pthread_create(&thread4, NULL, MACD, NULL);
    pthread_create(&thread5, NULL, RSI, NULL);
    pthread_create(&thread6, NULL, ADX, NULL);
    pthread_create(&thread7, NULL, LINEAR_REGRESSION, NULL);
    // Create threads for other functions as well

    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);
    pthread_join(thread3, NULL);
    pthread_join(thread4, NULL);
    // Join other threads as well
    
    
    vector<pair<long double,string>> net_gains;
    for(string &name: methods){
        ifstream cur(name+"final_pnl.txt");
        string intermediate = "";
        cur>>intermediate;
        net_gains.push_back({stold(intermediate),name});
    }
    sort(net_gains.begin(),net_gains.end());
    // deleting others
    for(int index = 0;index<(net_gains.size()-1);index++){
        string file1 =net_gains[index].second+"final_pnl.txt";
        string file2 =net_gains[index].second+"daily_cashflow.csv";
        string file3 =net_gains[index].second+"order_statistics.csv";
        remove(&file1[0]);
        remove(&file2[0]);
        remove(&file3[0]);
        

    }
    
    // renaming
    string name = net_gains.back().second;
    string old_file1 =name+"final_pnl.txt";
    string old_file2 =name+"daily_cashflow.csv";
    string old_file3 =name+"order_statistics.csv";
    string new_file1 ="final_pnl.txt";
    string new_file2 ="daily_cashflow.csv";
    string new_file3 ="order_statistics.csv";
    
    rename(&old_file1[0], &new_file1[0]);
    rename(&old_file2[0], &new_file2[0]);
    rename(&old_file3[0], &new_file3[0]);
    
    
    return 0;
    
    
    
    
}
