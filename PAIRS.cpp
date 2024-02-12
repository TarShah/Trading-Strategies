#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <cmath>
#include <vector>
#include <algorithm>
using namespace std;

vector<string> date;
vector<long double> spread;
vector<long double> price_1;
vector<long double> price_2;

// to change the dd/mm/yyyy to yyyy/mm/dd format
string change_format_yyyymmdd(string start){
    string new_date = string(1,start[6])+string(1,start[7])+string(1,start[8])+string(1,start[9])+"-"+string(1,start[3])+string(1,start[4])+"-"+string(1,start[0])+string(1,start[1]);
    
    return new_date;
    
}
string change_format_ddmmyyyy(string start){
    string new_date = string(1,start[8])+string(1,start[9])+"/"+string(1,start[5])+string(1,start[6])+"/"+string(1,start[0])+string(1,start[1])+string(1,start[2])+string(1,start[3]);
    return new_date;
}

void extract_data(string symbol,string column_name,vector<string>& column){
    ifstream file(symbol + ".csv");
    string line;
    int targetColumnIndex = -1;
    string targetColumnName = column_name;
    while (std::getline(file, line)) {
        if (targetColumnIndex == -1) {
            stringstream ss(line);
            string columnName;
            int columnIndex = 0;
            while (std::getline(ss, columnName, ',')) {
                if (columnName == targetColumnName) {
                    targetColumnIndex = columnIndex;
                    break;
                }
                columnIndex++;
            }
            if (targetColumnIndex == -1) {
                cerr << "Target column not found!"<<endl;
                return;
            }
        } 
        else {
            stringstream ss(line);
            string cell;
            int currentColumn = 0;
            while (std::getline(ss, cell, ',')) {
                if (currentColumn == targetColumnIndex) {
                    column.push_back(cell);
                    break;
                }
                currentColumn++;
            }
        }
    }

    
    
}

void solve1(string start,string end,int period,long double threshold, string symbol_1, string symbol_2,long double x){
    
    long double sum = 0; // to store the sum of spreads of last n(period) days
    long double sum_power_2 = 0; // to store the sum of square of spreads of last n(period) days
    
    ofstream daily_cashflow("daily_cashflow.csv");
    ofstream order_statistics_1("order_statistics_1.csv");
    ofstream order_statistics_2("order_statistics_2.csv");
    long double net_change = 0;
    
    // below is to convert to yyyy/mm/dd format
    string start_date = change_format_yyyymmdd(start);
    string end_date = change_format_yyyymmdd(end);
    int pos_stock_1 = 0;
    int pos_stock_2 = 0;
    int index = 0; // this will get the end date of the period of n days to be analyzed at first
    
    for(;index<date.size();index++){
        if(date[index]>start_date){
            break;
        }
        else if(date[index]==start_date){
            break;
        }
        else{
            continue;
        }
    }
    
    int dummy = index;
    
    for(int iterator = 0;iterator<period;iterator++)
    {
        sum+=spread[index];
        sum_power_2+=(spread[index]*spread[index]);
        index--;
    }
    index = dummy;
    
    daily_cashflow<<"Date,Cashflow\n";
    order_statistics_2<<"Date,Order_dir,Quantity,Price\n";
    order_statistics_1<<"Date,Order_dir,Quantity,Price\n";
    
    
    
    int  i = 0;
    while(date[index]<=end_date and index<date.size()){
        long double mean  = sum/period;
        long double standard_deviation = sqrt((sum_power_2/period)-(mean*mean));
        long double z_score = (spread[index]-mean)/standard_deviation;
        long double cash_flow = 0;
        
        
        if(z_score>threshold){
            if(pos_stock_1>-x and pos_stock_2<x){
                order_statistics_1<<change_format_ddmmyyyy(date[index]);
                order_statistics_1<<",";
                order_statistics_1<<"SELL,1,";
                order_statistics_1<<price_1[index];
                order_statistics_1<<"\n";
                
                order_statistics_2<<change_format_ddmmyyyy(date[index]);
                order_statistics_2<<",";
                order_statistics_2<<"BUY,1,";
                order_statistics_2<<price_1[index];
                order_statistics_2<<"\n";
                
                cash_flow+=price_1[index];
                cash_flow-=price_2[index];
                pos_stock_1--;
                pos_stock_2++;
            }
        }
        else if(z_score<-threshold){
            if(pos_stock_1<x and pos_stock_2>-x){
                order_statistics_1<<change_format_ddmmyyyy(date[index]);
                order_statistics_1<<",";
                order_statistics_1<<"BUY,1,";
                order_statistics_1<<price_1[index];
                order_statistics_1<<"\n";
                
                order_statistics_2<<change_format_ddmmyyyy(date[index]);
                order_statistics_2<<",";
                order_statistics_2<<"SELL,1,";
                order_statistics_2<<price_1[index];
                order_statistics_2<<"\n";
                
                
                
                cash_flow-=price_1[index];
                cash_flow+=price_2[index];
                pos_stock_1++;
                pos_stock_2--;
            }
            
            
        }
        
        
        // updating daily_cash_flow
        daily_cashflow<<change_format_ddmmyyyy(date[index]);
        daily_cashflow<<",";
        daily_cashflow<<cash_flow;
        daily_cashflow<<"\n";
        
        net_change+=cash_flow;
        
        sum-= spread[index-period+1];
        sum_power_2-=(spread[index-period+1]*spread[index-period+1]);
        index++;
        sum+=spread[index];
        sum_power_2+=(spread[index]*spread[index]);
        
        
    }
    
    index--;
    net_change+=(price_1[index]*pos_stock_1);
    net_change+=(price_2[index]*pos_stock_2);
    
    
    ofstream final_pnl("final_pnl.txt");
    final_pnl<<net_change<<endl;
    
    
    
    daily_cashflow.close();
    order_statistics_1.close();
    order_statistics_2.close();
    final_pnl.close();
    
}

// this function is supposed to populate date, price_1, price_2, spread arrays

void fill_data(string symbol_1, string symbol_2)
{
    vector<string> dum_price_1;
    vector<string> dum_price_2;
    vector<string> dum_dates_1;
    vector<string> dum_dates_2;
    extract_data(symbol_1, "DATE", dum_dates_1);
    extract_data(symbol_1, "CLOSE", dum_price_1);
    extract_data(symbol_2, "DATE", dum_dates_2);
    extract_data(symbol_2, "CLOSE", dum_price_2);
    
    // now i will be applying filter on these dum vectors as there may be some date which are in dum_dates_1 but not in dum_dates_2
    int index_1 = 0;
    int index_2 = 0;
    while((index_1<dum_dates_1.size()) and (index_2<dum_dates_2.size())){
        if(dum_dates_1[index_1]==dum_dates_2[index_2]){
            date.push_back(dum_dates_1[index_1]);
            price_1.push_back(stold(dum_price_1[index_1]));
            price_2.push_back(stold(dum_price_2[index_2]));
            spread.push_back(stold(dum_price_1[index_1])-stold(dum_price_2[index_2]));
            index_1++;
            index_2++;
        }
        else if(dum_dates_1[index_1]<dum_dates_2[index_2]){
            index_1++;
        }
        else{
            //dum_dates_1[index_1]>dum_dates_2[index_2]
            index_2++;
        }
        
    }
    
    reverse(date.begin(),date.end());
    reverse(price_1.begin(),price_1.end());
    reverse(price_2.begin(),price_2.end());
    reverse(spread.begin(),spread.end());
}


void solve2(string start,string end,int period,long double threshold, string symbol_1, string symbol_2,long double x,long double stop_loss){
    
    long double sum = 0; // to store the sum of spreads of last n(period) days
    long double sum_power_2 = 0; // to store the sum of square of spreads of last n(period) days
    
    ofstream daily_cashflow("daily_cashflow.csv");
    ofstream order_statistics_1("order_statistics_1.csv");
    ofstream order_statistics_2("order_statistics_2.csv");
    long double net_change = 0;
    
    // below is to convert to yyyy/mm/dd format
    string start_date = change_format_yyyymmdd(start);
    string end_date = change_format_yyyymmdd(end);
    int pos_stock_1 = 0;
    int pos_stock_2 = 0;
    int index = 0; // this will get the end date of the period of n days to be analyzed at first
    
    for(;index<date.size();index++){
        if(date[index]>start_date){
            break;
        }
        else if(date[index]==start_date){
            break;
        }
        else{
            continue;
        }
    }
    
    int dummy = index;
    
    for(int iterator = 0;iterator<period;iterator++)
    {
        sum+=spread[index];
        sum_power_2+=(spread[index]*spread[index]);
        index--;
    }
    index = dummy;
    
    daily_cashflow<<"Date,Cashflow\n";
    order_statistics_2<<"Date,Order_dir,Quantity,Price\n";
    order_statistics_1<<"Date,Order_dir,Quantity,Price\n";
    int  i = 0;
    
    
    vector<vector<long double>> previous_data; // {a,mean,standard_deviation} if a = 1 then sell signal else if a = 0 then but signal
    int cur_previous_index = 0; // starting index for data to be analyzed
    
    while(date[index]<=end_date and index<date.size()){
        long double mean  = sum/period;
        long double standard_deviation = sqrt((sum_power_2/period)-(mean*mean));
        long double z_score = (spread[index]-mean)/standard_deviation;
        long double cash_flow = 0;
        
        
        int signal_type = -1; // if it is 1 then sell signal else if it is 0 buy signal
        int quan_sell = 0;
        int quan_buy = 0;
        
        if(z_score>threshold){
            signal_type=1;
            if(pos_stock_1>-x and pos_stock_2<x){
                quan_sell = 1;
            }
        }
        else if(z_score<-threshold){
            signal_type=0;
            if(pos_stock_1<x and pos_stock_2>-x)
            {
                quan_buy = 1;
            }
        }
        if(signal_type==1 and quan_sell>0)
        {
            if(cur_previous_index==previous_data.size() ){
                previous_data.push_back({1,mean,standard_deviation});
            }
            else if (previous_data[cur_previous_index][0]==1){
                previous_data.push_back({1,mean,standard_deviation});
            }
            else{
                cur_previous_index++;
            }
            
            
        }
        if(signal_type==0 and quan_sell>0){
            if(cur_previous_index==previous_data.size() ){
                previous_data.push_back({1,mean,standard_deviation});
            }
            else if (previous_data[cur_previous_index][0]==0){
                previous_data.push_back({0,mean,standard_deviation});
            }
            else{
                cur_previous_index++;
            }
            
        }
        vector<vector<long double>> new_previous_data;
        for(int i = cur_previous_index;i<previous_data.size();i++){
            long double new_z_score = (spread[index]-previous_data[cur_previous_index][1])/previous_data[cur_previous_index][2];
            if(abs(new_z_score)>abs(stop_loss))
            {
                if(previous_data[cur_previous_index][0]==1)
                {
                    quan_buy++;
                }
                else
                {
                    quan_sell++;
                }
            }
            else
            {
                new_previous_data.push_back(previous_data[cur_previous_index]);
            }
            
        }
        cur_previous_index = 0;
        previous_data = new_previous_data;
        if(quan_buy>quan_sell){
            quan_buy-=quan_sell;
            quan_sell=0;
        }
        else{
            quan_sell-=quan_buy;
            quan_buy=0;
        }
        if(quan_sell>0){
            if(pos_stock_1>-x and pos_stock_2<x)
            {
                order_statistics_1<<change_format_ddmmyyyy(date[index]);
                order_statistics_1<<",";
                order_statistics_1<<"SELL,";
                order_statistics_1<<quan_sell;
                order_statistics_1<<",";
                order_statistics_1<<price_1[index];
                order_statistics_1<<"\n";
                
                order_statistics_2<<change_format_ddmmyyyy(date[index]);
                order_statistics_2<<",";
                order_statistics_2<<"BUY,";
                order_statistics_2<<quan_sell;
                order_statistics_2<<",";
                order_statistics_2<<price_1[index];
                order_statistics_2<<"\n";
                
                cash_flow+=(price_1[index]*quan_sell);
                cash_flow-=(price_2[index]*quan_sell);
                pos_stock_1-=quan_sell;
                pos_stock_2+=quan_sell;
            }
        }
        if(quan_buy>0){
            if(pos_stock_1<x and pos_stock_2>-x){
                order_statistics_1<<change_format_ddmmyyyy(date[index]);
                order_statistics_1<<",";
                order_statistics_1<<"BUY,";
                order_statistics_1<<quan_buy;
                order_statistics_1<<",";
                order_statistics_1<<price_1[index];
                order_statistics_1<<"\n";
                
                order_statistics_2<<change_format_ddmmyyyy(date[index]);
                order_statistics_2<<",";
                order_statistics_2<<"SELL,";
                order_statistics_2<<quan_buy;
                order_statistics_2<<",";
                order_statistics_2<<price_1[index];
                order_statistics_2<<"\n";
                
                
                
                cash_flow-=(price_1[index]*quan_buy);
                cash_flow+=(price_2[index]*quan_buy);
                pos_stock_1+=quan_buy;
                pos_stock_2-=quan_buy;
            }
            
        }
        
        
        
        
        // updating daily_cash_flow
        daily_cashflow<<change_format_ddmmyyyy(date[index]);
        daily_cashflow<<",";
        daily_cashflow<<cash_flow;
        daily_cashflow<<"\n";
        
        net_change+=cash_flow;
        
        sum-= spread[index-period+1];
        sum_power_2-=(spread[index-period+1]*spread[index-period+1]);
        index++;
        sum+=spread[index];
        sum_power_2+=(spread[index]*spread[index]);
        
        
    }
    
    index--;
    net_change+=(price_1[index]*pos_stock_1);
    net_change+=(price_2[index]*pos_stock_2);
    
    
    ofstream final_pnl("final_pnl.txt");
    final_pnl<<net_change<<endl;
    
    
    
    daily_cashflow.close();
    order_statistics_1.close();
    order_statistics_2.close();
    final_pnl.close();
    
}



int main(int argc, char * argv[]) {
    
    
    // command line would be ./a.out symbol_1 symbol_2 start_date end_date n x threshold
    string symbol_1 = string(argv[1]);
    string symbol_2 = string(argv[2]);
    string start_date = string(argv[3]);
    string end_date = string(argv[4]);
    int n = stoi(string(argv[5]));
    long double x = stold(string(argv[6]));
    long double threshold = stold(string(argv[7]));
    
    fill_data(symbol_1,symbol_2);
    
    if(argc== 8 or (string(argv[8])==""))
    {
        solve1(start_date, end_date, n, threshold, symbol_1, symbol_2, x);
    }
    else
    {
        long double stop_loss_threshold = stold(string(argv[8]));
        cout<<"here"<<endl;
        solve2(start_date, end_date, n, threshold, symbol_1, symbol_2, x,stop_loss_threshold);
    }

    
    
    
    
    
    
    
    
    cout << "finished !\n";
    
    return 0;
}
