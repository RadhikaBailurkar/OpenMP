#include <fstream>
#include <iostream>
#include <string>
#include <cstring>
#include <complex>
#include <utility>
#include <cmath>
#include <limits.h>
#include <set>
#include <vector>
#include <map>
#include <cctype>
#include <queue>
#include <stdio.h>
#include <cstdio>
#include <stack>
#include <algorithm>
#include <list>
#include <sys/time.h>
#include <ctime>
#include <time.h>
#include <stdlib.h>
#include <numeric>
#include <memory.h>
#include <omp.h>
#include<streambuf>
//#define all(a) (a).begin(),(a).end()
//#define gcd __gcd
//#define bitcount __builtin_popcount
#define d 256 //for 256 bit ASCII number

using namespace std;

typedef std::vector<int> vi;
typedef std::vector<std::string> vs;
typedef std::pair<int, int> pii;
typedef std::set<int> si;
typedef std::map<std::string, int> msi;

int Word(string text, string pattern, int prime,int threads) {
  int textlength = text.length() ;
  int pattlength = pattern.length() ;
  int p = 0 ; //hash value for pattern
  int t = 0 ; //hash value for text
  int h = 1 ;
  int c ;
  int total=0;
  // h = (d^(pattlength-1)) % prime
  for(int i=0;i<pattlength-1;i++)
    h = (d * h) % prime ;
  for(int i=0;i<pattlength;i++) {
    p = (d * p + pattern[i]) % prime ; //ascii of letters is being used here
  }
  int len = text.length();
  omp_set_num_threads(threads);
  int dev = len/threads;
  #pragma omp parallel for private(c,t) reduction(+:total)
  for(int l=0;l<threads;l++) {
    t=0;
    cout<<"Thread no "<<omp_get_thread_num()<<endl;
    int start = dev*l;
    int stop = dev*(l+1)+1;
    //calculating hash values for pattern and first window of text
    for(int i=start;i<start+pattlength;i++) {
       t = (d * t + text[i]) % prime ;
    }

    for(int i=start;i<stop&&i<textlength;i++) {
      c = 0; //dont forget this ...whooff!
      if(p==t) { //if hash matches do naive approach
        for(int j=0;j<pattlength;j++) {
          c++ ;
          if(text[i+j]!=pattern[j])
            break ;
        }
        if(c==pattlength) {
          total=total+1;
        }
      }
      //calcualting hash of next text window
      if(i < textlength-pattlength) {
        t = (d * (t - text[i] * h) + text[i+pattlength]) % prime ;
        if(t<0)  //only if it turns out to be negative
          t = t + prime ;
      }
    }
  }
  return total;
}

int main(int argc, char const *argv[]) {
  string pattern ;
  //cout<<"Enter text"<<endl;
  //getline(cin,text);
  ifstream t("input.txt");
  string text((istreambuf_iterator<char>(t)),
                 istreambuf_iterator<char>());
  //cout<<text;
  //cin>>text;
  //int n;
  //cin>>n;
  int prime = 101 ; //random prime number
  //while(n--!=0) {
  cout<<"Type word to search"<<endl;
    cin>>pattern;
int threads = 1;
while(threads<=70){
	ofstream file;
	string fname = "output.txt";
	file.open(fname,ios_base::app);
double start = omp_get_wtime();
    int k = Word(text, pattern, prime,threads) ;
    double elapsed = omp_get_wtime() - start;
cout<<threads<<"------------------------"<<elapsed<<endl;
	file<<threads<<" "<<elapsed<<"\n";
	file.close();
    cout<<k<<endl;
threads+=10;
}
    //cout<<elapsed<<endl;
  //}
  return 0;
}
