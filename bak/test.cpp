#include<bits/stdc++.h>
// #include<fillOrdersManager.cpp>

using namespace std;

class test{
	private:
		int x;
		int y;
	public:
		test(int a, int b){
			x = a;
			y = b;
		}
		
		int twoTimes(){
			x *=2;
			y*= 2;
		}	

		void print(){
			cout<<x<<" "<<y<<endl;
		}
};


static double opDiv(double a, double b){
   return a/b;
}


int main(int argc, char const *argv[]){

	test t(0,1);
	t.print();
	t.twoTimes();
	t.print();

	std::vector<double> v = {4,3,2,1};
	std::vector<double> a = {0.1,0.2,0.3,0.4};
	std::vector<double> temp;
	transform(v.begin(),v.end(),a.begin(),back_inserter(temp),opDiv);
	cout<<temp[3]<<endl;
}