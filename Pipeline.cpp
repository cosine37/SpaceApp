#include<iostream>
#include<fstream>
#include<sstream>
#include<math.h>
#include <iomanip>
using namespace std;
string lon, lat,thi;
int total;
struct Level{
	int year;
	int day;
	double change;
};

Level levels[1000];

struct Date{
	int month;
	int day;
};


int diff(int y1, int d1, int y2, int d2){
	if (y1==y2){
		return d2-d1;
	} else {
		if (y1%4==0){
			return 366-d1+d2;
		} else {
			return 365-d1+d2;
		}
	}
}

Date dateRawToDate(int year, int date){
	int cur = 1;
	int dom[13] = {0,31,28,31,30,31,30,31,31,30,31,30,31};
	if (year % 4 == 0){
		dom[2] = 29;
	}
	while (date>dom[cur]){
		date = date-dom[cur];
		cur++;
	}
	Date ans;
	ans.month = cur;
	ans.day = date;
	return ans;
}

void input(string fileName){
	ifstream in;
	in.open(fileName.c_str());
	string s;
	double dtime, level;
	/*
	if (!in){
		cout<<"cannot read file"<<endl;	
	}*/
	int i=0;
	while (in>>s){
		in>>s;
		in>>dtime;
		in>>s;
		in>>s;
		in>>level;
		for (int i=0; i<6; i++){
			in>>s;
		}
		double dyear = trunc(dtime);
		//cout<<dyear<<endl;
		double ddate = dtime-dyear;
		double daysOfYear;
		int year = (int)dyear;
		if (year%4==0){
			daysOfYear = 366;
		} else {
			daysOfYear = 365;
		}
		int dateRaw = (int)round(daysOfYear * ddate);
		levels[i].year=year;
		levels[i].day=dateRaw;
		levels[i].change=level;
		i++;
	}
	total=i;
}

void output(){
	double x,incr,n;
	int i,j;
	ofstream out("result.csv");
	for (i=0;i<total-1;i++){
		x=diff(levels[i].year,levels[i].day,levels[i+1].year,levels[i+1].day);
		n=(int)x;
		incr=(levels[i+1].change-levels[i].change)/x;
		int ty,td,tmo,tda;
		double tincr,tj;
		Date d;
		for (j=0;j<n;j++){
			ty=levels[i].year;
			td=levels[i].day+j;
			tj=(double)j;
			tincr=levels[i].change+incr*tj;
			if (ty%4==0){
				if (td>366){
					ty=ty+1;
					td=td-366;
				}
			} else {
				if (td>365){
					ty=ty+1;
					td=td-365;
				}
			}
			d=dateRawToDate(ty,td);
			out<<ty;
			if (d.month>9){
				out<<d.month;
			} else {
				out<<"0";
				out<<d.month;
			}
			if (d.day>9){
				out<<d.day;
			} else {
				out<<"0";
				out<<d.day;
			}
			out<<",";
			out<<tincr<<endl;
		}
	}
}

void inputOld(string fileName){
	ifstream in;
	ofstream out("result1.csv");
	out<<"lon,lat,thick"<<endl;
	in.open(fileName.c_str());
	string line,token;
	int skipLines=10;
	int i,j;
	j=0;
	while (getline(in,line)){
		j++;
		if (j<11) {
			continue;
		}
		istringstream ss(line);
		i=0;
		while (getline(ss, token, ',')){
			if (i==1){
				lon=token;
			}
			if (i==2){
				lat=token;				
			}
			if (i==3){
				thi=token;
			} 
			i++;
			if (i==4){
				out<<lon<<","<<lat<<","<<thi<<endl;
				break;
			}
		}
		
		
	}
	out.close();
}
void cutdown(){
	ifstream in;
	ofstream out("files.txt");
	in.open("files_raw.txt");
	string line,s;
	while (getline(in,line)){
		s=line.substr(67);
		out<<s<<endl;
	} 
}

void generateJSArray(){
	ifstream in;
	ofstream out("fileNames.txt");
	in.open("filename.txt");
	string line;
	int total=1193;
	int i;
	string names[3000];
	for (i=0;i<total;i++){
		in>>line;
		names[i]=line;
	}
	out<<"[";
	cout<<"total="<<total<<endl;
	for (i=0;i<total-1;i++){
		out<<"\""<<names[i]<<"\","; 
	}
	out<<"\""<<names[i]<<"\"]"; 
} 
struct Density {
	int year,month,day;
	double d;
};
Density densities[5000];
int findNextValid(int x){
	while (1==1){
		x++;
		if (densities[x].d!=-999.99) return x; 
	}
}
void getCO2(string filename){
	ifstream in;
	ofstream out("co2.csv");
	in.open(filename.c_str());
	string s;
	int year,month,day,total,i;
	double co2, oldco2;
	total=0;
	while (in>>s){
		in>>year;
		in>>month;
		in>>day;
		for (i=0;i<3;i++) {
			in>>s;
		}
		in>>co2;
		//out<<year;
		for (i=0;i<8;i++){
			in>>s;
		}
		densities[total].year=year;
		densities[total].month=month;
		densities[total].day=day;
		densities[total].d=co2;
		total++;
		/*
		if (month<10) out<<"0";
		out<<month;
		if (day<10) out<<"0";
		out<<day<<","<<co2<<endl;
		*/	
	}
	int l,last;
	double n,diff;
	bool flag;
	for (i=0;i<total;i++){
		out<<densities[i].year;
		if (densities[i].month<10) out<<"0";
		out<<densities[i].month;
		if (densities[i].day<10) out<<"0";
		out<<densities[i].day<<",";
		if (densities[i].d==-999.99){
			if (flag){
				last=i-1;
				l=findNextValid(i);
				n=l-i+1;
				diff=densities[l].d-densities[i-1].d;
				flag=false;
			}
			densities[i].d=densities[i-1].d+diff/n;
		} else {
			flag=true;
		}
		out<<setprecision(5)<<densities[i].d<<endl;
	}
}
int main(){
	//input("sealevel.txt");
	//output();
	generateJSArray();
	//getCO2("co2_10-17.txt");
} 
