#include <iostream>
#include <fstream>
#include <vector>
#include <stdlib.h>
#include <cmath>
#include <complex>
#include <random>
#include <cstdlib>
#include <typeinfo>
#include <stdio.h>

using namespace std;

//constexpr int FLOAT_MIN = 0;
//constexpr int FLOAT_MAX = 1;

//#define msg_length 4
//#define cw_length 5
//#define cw_check 4


#define msg_length 132
#define cw_length 308
#define cw_check 176



// encoder
vector<int> random_message(int block){
    vector<int> m;
    unsigned seed;
    seed = (unsigned)time(NULL);
    srand(seed+block);
    
    float ran;
    //ran=FLOAT_MIN +(float)(rand()) / ((float)(RAND_MAX/(FLOAT_MAX-FLOAT_MIN)));
    for(int i=1;i<=msg_length;i++){
        int FLOAT_MIN = 0;
        int FLOAT_MAX = 1;
        ran=FLOAT_MIN +(float)(rand()) / ((float)(RAND_MAX/(FLOAT_MAX-FLOAT_MIN)));
        m.push_back(round(ran));
    }
    //cout << "size in function " << m.size() << endl;
    return m;
}

vector<int> encoder(vector<int> const &m,vector <vector <int> > const &GM){
    vector <int> cw;
    int times;
    for (int i=0;i<cw_length;i++){
        int sum=0;
        for(int k=0;k<msg_length;k++){
            times=m[k]*GM[k][i];
            sum=sum+times;
        }
        cw.push_back(sum%2);
        //cout << "vector " << i << " sum=" << sum << endl;
        
    }


    return cw;
}

vector<double> BPSKmodulator(vector<int> const &cw){ 
    vector<double> tx; // codewords (0~307)	
    for (int i=0;i<cw.size();i++){
        tx.push_back((-cw[i]*2+1)*1/sqrt(2)); 
        cout << "cw = " << cw[i] << "qpsk = " << tx[i] << endl;
    }
    return tx;
}


vector <complex <double> > QPSKmodulator(vector<int> const &cw){ 
//    using namespace std::complex_literals;
    vector<complex <double> > x;
   
	double A=1/sqrt(2);	 // =0.707107
    
	int j=0;
    for (int n=0;n<cw.size();){
        if (cw[n]==0 && cw[n+1]==0){
            //auto a=0.707107 + 0.707107i;
            //x.push_back(a);
            complex <double> temp(A,A);
            x.push_back(temp);
	      //cout << "cw " << n << " and " << n+1 << "is " << cw[n] << " and " << cw[n+1] << " result : " << x[j] << " " << j+1 << endl;
	    }
        else if(cw[n]==0 && cw[n+1]==1){
            //auto a=-0.707107 + 0.707107i;
            //x.push_back(a);
            complex <double> temp(-A,A);
            x.push_back(temp);
	     //cout << "cw " << n << " and " << n+1 << "is " << cw[n] << " and " << cw[n+1] << " result : " << x[j] << " " << j+1 << endl;
        }
        else if(cw[n]==1 && cw[n+1]==0){
            //auto a=0.707107 - 0.707107i;
            //x.push_back(a);
            complex <double> temp(A,-A);
            x.push_back(temp);
	      //cout << "cw " << n << " and " << n+1 << "is " << cw[n] << " and " << cw[n+1] << " result : " << x[j] << " " << j+1 << endl;
        }
        else if(cw[n]==1 && cw[n+1]==1){
            //auto a=-0.707107 - 0.707107i;
            //x.push_back(a);
            complex <double> temp(-A,-A);
            x.push_back(temp);
	      //cout << "cw " << n << " and " << n+1 << "is " << cw[n] << " and " << cw[n+1] << " result : " << x[j] << " " << j+1 << endl;
        }

        n=n+2;
		j++;
            
    }
   return x;
}


vector<complex <double> >awgn(vector<complex <double> > x,float SNR,int blocknum){
//    using namespace std::complex_literals;
	vector<complex <double> > rx;
    
	unsigned seed;
    seed = (unsigned)time(NULL); 
    default_random_engine generator (seed+blocknum);
    normal_distribution<double> norm(0.0, sqrt(pow(10,-SNR/10.0)/2));
    //normal_distribution<double> norm(0.0, sqrt(pow(10,-SNR/10.0)));
    
    
	for(int n=0;n<x.size();n++){
        double A= real(x[n])+norm(generator);
        double B= imag(x[n])+norm(generator);
		complex <double> temp(A,B);
        rx.push_back(temp);

        // debug
        //auto a= 1.62811916875090 - 1.16822865741551i;
        //rx.push_back(a);

       //cout << "rx " << n << " is "<< rx[n] << " A is " << A << " and B is " << B << endl;
	}
    
    
    return rx;
}


vector<double> QPSKdemodulator(vector <complex <double> > const &rx){ // belum selesai
//    using namespace std::complex_literals;
    vector<double> y(309);
    double D=pow(2,-0.5);

    // it is already correct
    /*
    vector<complex<double>> y;
    for(int n=0;n<rx.size();n++){
        double A= 2*imag(rx[n])/D;
        double B= 2*real(rx[n])/D;
		complex <double> temp(A,B);
        y.push_back(temp);
       cout << "y " << n << " is "<< y[n] << " A is " << A << " and B is " << B << endl;
	}
    */

  //  vector<double> y;
    int nn=1;
    for(int n=0;n<rx.size();n++){

		double A= 2*imag(rx[n])/D;
        y[n+nn]=A;
        nn++;
        double B= 2*real(rx[n])/D;
        y[n+nn]=B;
	}
        
        
    

    

    return y;
    
}

vector<int> cht(vector<int> const &dcw, vector <vector <int> > const &H){
        vector<vector <int> > HT;
        vector<int> column;
        vector<int> checksum;
        for(int j=0;j<cw_length;j++){
            for(int i=0;i<cw_check;i++){
                column.push_back(H[i][j]);
               
            }
            HT.push_back(column);
            column.clear();
        }

        int times;
        for (int k=0;k<cw_check;k++){
        int sum=0;
        for(int i=0;i<cw_length;i++){
            times=dcw[i]*HT[i][k];
            sum=sum+times;
        }
        checksum.push_back(sum%2);
        }
        /*
       cout << endl;
       cout << "check sum" << endl;
        for(int i=0;i<checksum.size();i++){
			cout << checksum[i] << " "; // his SPA code is not run correctly
		}
        
       */

        return checksum;

    }
    

vector<vector <int> > H_transpose(vector<vector <int> > const &H){
        vector<vector <int> > HT;
        vector<int> column;
        
        for(int j=0;j<cw_length;j++){
            for(int i=0;i<cw_check;i++){
                column.push_back(H[i][j]);
                
            }
            HT.push_back(column);
            column.clear();
           // cout << endl;
        }

        for(int j=0;j<cw_length;j++){
            for(int i=0;i<cw_check;i++){
            //    cout << H[i][j] << " ";
                
            }
           // cout << endl;
        }


        return HT;

    }

    int ber(vector<int> const &cw, vector<int> const &dcw){
        int ber=0;
        for(int n=0;n<cw.size();n++){
            if(cw[n]!=dcw[n+1])
                ber++;
        }
        return ber;
    }













    //void print(vector<double> const &y, vector<int> const &cw, vector<complex<double>> const &rx){
    void print(vector<double> const &y, vector<int> const &cw){    

        
    cout << "codeword" << endl;
    for(int n=0;n<cw.size();n++){
        cout << cw[n] << " ";
    }
    cout <<endl;

    /*
    cout << "awgn" << endl;
    for(int n=0;n<rx.size();n++){
        cout << rx[n] << " ";
    }
    cout <<endl;
    */
   
    cout << "demodulated" << endl;
    for(int n=1;n<y.size();n++){
        cout << y[n] << " ";
    }
    
    cout <<endl;
    /*
    cout << "decoded codeword " << endl;
    for(int n=0;n<y.size();n++){
        if ( y[n]>=0)
            cout << "0 " ;
        else
            cout << "1 " ;
    }
    */

}

void print2(vector<int> const &cw, vector<complex <double> > const &x, vector <complex <double> > const &rx, vector<double> const &y){
    int i=0;
    for(int n=0;n<cw.size();n++){
        
        if (n%2==1){
            
            cout << i << ". symbol " << cw[i] << " " << cw[i+1] << ", x: " << x[i] << ", rx: " << rx[i] << ", mapped: " << y[i] << " " << y[i+1] << endl;
            i++;
        }
    }
}