#include <bits/stdc++.h>
using namespace std;
#define ll long long
#define pb push_back
#define len(v) (int)v.size() 
#define all(v) v.begin(),v.end()
#define rall(v) v.rbegin(),v.rend()
#ifdef Raptor
#include "debug.h"
#else
#define debug(...) 1
#define imie(...) 1;
#endif
struct matrix{
	int n,m;
	vector<vector<double>> a;
	matrix(vector<vector<double>> x) {
		n = len(x); m = len(x[0]);
		a.resize(n);
		for(int i=0;i<n;i++){
			a[i].resize(m);
			for(int j=0;j<m;j++){
				a[i][j] = x[i][j];
			}
		}
	}
	matrix inverse(){
		assert(n == m);
		vector<vector<double>> augmented(n, vector<double>(2*n));
        for(int i = 0; i < n; i++){
			augmented[i][i + n] = 1;
			for(int j = 0; j < n; j++){
				augmented[i][j] = a[i][j];
			}
		}
		for(int i = 0; i < n; i++){
			double divisor = augmented[i][i];
			if(divisor == 0) divisor = 0.01;
			for(int j = 0; j < 2*n; j++) augmented[i][j] /= divisor;
			for(int k = 0; k < n; k++){
				if(k != i){
					double factor = augmented[k][i];
					for(int j = 0; j < 2*n; ++j){
						augmented[k][j] -= factor * augmented[i][j];
                    }
                }
            }
        }
		vector<vector<double>> inversed(n, vector<double>(n));
        for (int i = 0; i < n; ++i) {
			for (int j = 0; j < n; ++j) {
				inversed[i][j] = augmented[i][j + n];
			}
		}
        return matrix(inversed);
	}
	matrix transpose(){
		vector<vector<double>> tr(m,vector<double> (n));
		for(int i=0;i<n;i++){
			for(int j=0;j<m;j++){
				tr[j][i] = a[i][j];
			}
		}
		return matrix(tr);
	}
	matrix operator *(matrix other){
		assert(other.n == m);
		vector<vector<double>> prod(n,vector<double>(other.m));
		for(int i=0;i<n;i++){
			for(int j=0;j<other.n;j++){
				for(int k=0;k<other.m;k++){
					prod[i][k] += a[i][j]*other.a[j][k];
				}
			}
		}
		return matrix(prod);
	}
};	
vector<string> dates;
vector<double> price;
string change_date_rep(string date){
	string new_date = "";
	new_date += date.substr(8,2);
	new_date += "/";
	new_date += date.substr(5,2);
	new_date += "/";
	new_date += date.substr(0,4); 
	return new_date;
}
string comparable(string date){
	string conv = "";
	conv += date.substr(6,4);
	conv += "-"; 
	conv += date.substr(3,2);
	conv += "-";
	conv += date.substr(0,2);
	return conv;
}
void get_dates(string symb){
	string line;
    ifstream file(symb + ".csv"); 
    int targetColumnIndex = -1;
    string targetColumnName = "DATE"; 
	while(std::getline(file, line)){ 
		if(targetColumnIndex == -1){
			stringstream ss(line);
			string columnName;
			int columnIndex = 0;
			while(std::getline(ss, columnName, ',')){
				if(columnName == targetColumnName){
					targetColumnIndex = columnIndex;
					break;
				}
				columnIndex++;
			}
			assert(targetColumnIndex != -1);
        }
        else{
			stringstream ss(line);
			string cell;
			int currentColumn = 0;
			while(std::getline(ss, cell, ',')){ 
				if(currentColumn == targetColumnIndex){  
					dates.push_back(change_date_rep(cell)); 
					break; 
				}
				currentColumn++;
			}
		}
	}
}
void get_close(string symb){ 
    string line;
    ifstream file(symb + ".csv"); 
    int targetColumnIndex = -1;
    string targetColumnName = "CLOSE"; 
	while(std::getline(file, line)){ 
		if(targetColumnIndex == -1){
			stringstream ss(line);
			string columnName;
			int columnIndex = 0;
			while(std::getline(ss, columnName, ',')){
				if(columnName == targetColumnName){
					targetColumnIndex = columnIndex;
					break;
				}
				columnIndex++;
			}
			assert(targetColumnIndex != -1);
        }
        else{
			stringstream ss(line);
			string cell;
			int currentColumn = 0;
			while(std::getline(ss, cell, ',')){ 
				if(currentColumn == targetColumnIndex){  
					price.push_back(stod(cell)); 
					break; 
				}
				currentColumn++;
			}
		}
	}
}
vector<double> train(string train_start_date, string start_date, string symb){
	ifstream trainfile("train.csv");
	vector<vector<string>> train_data;
    string line;
    while (getline(trainfile, line)) {
        stringstream ss(line);
        string cell;
        vector<string> row;
        while (getline(ss, cell, ',')) {
            row.push_back(cell);
        }
        train_data.push_back(row);
    }
    trainfile.close();
    train_data.erase(train_data.begin());
    reverse(all(train_data));
    int start = 0;
	for(int i=0;i<len(train_data);i++){
		if(train_data[i][0] >= comparable(train_start_date)){
			start = i;
			break;
		}
	}
	vector<vector<double>> mat_data;
	for(int st = start;st<len(train_data);st++){
		vector<double> row(1,1); 
		for(int i = 1;i<=6;i++){
			row.pb(stod(train_data[st-1][i]));
		}
		row.pb(stod(train_data[st][2]));
		mat_data.pb(row);
	}
	matrix X(mat_data);
	vector<vector<double>> ydata;
	for(int st = start;st<len(train_data);st++){
		ydata.pb({stod(train_data[st][1])});
	}
	matrix Y(ydata);
	matrix c1 = X.transpose()*X;
	matrix c2 = X.transpose()*Y;
	matrix B = c1.inverse()*c2;	
	
	ifstream testfile(symb + ".csv");
	vector<vector<string>> test_data;
	string lin;
    while (getline(testfile, lin)) {
        stringstream ss(lin);
        string cell;
        vector<string> row;
        while (getline(ss, cell, ',')) {
            row.push_back(cell);
        }
        test_data.push_back(row);
    }
    testfile.close();
    test_data.erase(test_data.begin());
    reverse(all(test_data));
    start = 0;
	for(int i=0;i<len(test_data);i++){
		if(test_data[i][0] >= comparable(start_date)){
			start = i;
			break;
		}
	}
	vector<vector<double>> test_vec;
	for(int st = start;st<len(test_data);st++){
		vector<double> row(1,1); 
		for(int i = 1;i<=6;i++){
			row.pb(stod(test_data[st-1][i]));
		}
		row.pb(stod(test_data[st][2]));
		test_vec.pb(row);
	}
	matrix X_test(test_vec);
	matrix Y_test = X_test*B; 
	matrix Y_test_transpose = Y_test.transpose();
	for(auto row : B.a){
		for(auto cell : row){
			cout<<cell<<" ";
		}
    }
	return Y_test_transpose.a[0];
}
void solve(int argc,char* argv[]){
	get_dates(string(argv[1]));
	get_close(string(argv[1]));
	reverse(all(dates));
	reverse(all(price));
	int x = stoi(argv[2]);
	double p = stod(argv[3]);
	string train_start_date = string(argv[4]);
	string start_date = string(argv[5]); 
	int start = 0;
	for(int i=0;i<len(dates);i++){
		if(comparable(dates[i]) >= comparable(start_date)){
			start = i;
			break;
		}
	}
	vector<double> predicted_price = train(train_start_date, start_date,string(argv[1]));
	ll offset = start; 
	ll position = 0;
	double cash = 0;
	ofstream order("order_statistics.csv");
	ofstream cashflow("daily_cashflow.csv");
	order<<"Date,Order_dir,Quantity,Price"<<endl;
	cashflow<<"Date,Cashflow"<<endl;
	for(start;start<len(dates);start++){
		if(predicted_price[start-offset] >= price[start]*((double)(100+p)/(double)100)){
			if(position < x){
				position++;
				cash -= price[start];
				order<<dates[start]<<","<<"BUY"<<","<<1<<","<<price[start]<<endl;
			}
		}
		if(predicted_price[start-offset] <= price[start]*((double)(100-p)/(double)100)){
			if(position > -x){
				position--;
				cash += price[start];
				order<<dates[start]<<","<<"SELL"<<","<<1<<","<<price[start]<<endl;
			}
		}
		cashflow<<dates[start]<<','<<cash<<endl;
	}
	// Squaring off 
	cash += price[len(price)-1]*position;
	order.close();
	cashflow.close();
	// Writing the final pnl
	ofstream pnl("final_pnl.txt");
	pnl<<cash<<endl;
	pnl.close();
}
int main(int argc, char*  argv[]){
	solve(argc, argv);
	
	return 0;
}
