#include <bits/stdc++.h>
using namespace std;
int main(){
    string title, in;
    double vAcc=0,v2Acc=0,cAcc=0,tAcc=0,x,y,z,h;
    map<double,int> dictV,dictV2,dictC,dictT;
    int cont=0;
    pair<int,double> v,v2,c,t;
    getline(cin,title);
    while(getline(cin,in)){
        cont++;
        stringstream s(in);
        string v_,v2_,c_,t_;
        getline(s,v_,',');
        getline(s,v2_,',');
        getline(s,c_,',');
        getline(s,t_,',');
        x=stof(v_);
        h=stof(v2_);
        y=stof(c_);
        z=stof(t_);
        v=max({++dictV[stof(v_)],x},v);
        v2=max({++dictV2[stof(v_)],h},v2);
        c=max({++dictC[stof(c_)],y},c);
        t=max({++dictT[stof(t_)],z},t);
        vAcc+=x;
        v2Acc+=h;
        cAcc+=y;
        tAcc+=z;
    }
    cout<<"Moda:"<<v.second<<' '<<v2.second<<' '<<c.second<<' '<<t.second<<endl;
    cout<<"Media:"<<vAcc/cont<<' '<<v2Acc/cont<<' '<<cAcc/cont<<' '<<tAcc/cont<<endl;
}