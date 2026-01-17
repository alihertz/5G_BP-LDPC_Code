#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cmath>
#include <complex>
#include <limits>




#include "system.cpp"
#include "spa.cpp"



using namespace std;

#define GROW 133 
#define HROW 177
#define COL 309


#define msg_length 132
#define cw_length 308
#define cw_check 176




int main()
{
	// initialization decoder
	int max_block_num=10;
	int N;
	int M;

	// initialization other
	int number;
	fstream infile;
	
	// Input H
	vector<vector<int> > G;
	fstream Ginput_file;
	Ginput_file.open("G.txt");
	if(!Ginput_file){
		cout << "File G.txt is not exsit" << endl;
		return EXIT_FAILURE;
	}
	int Gnumber;
	int Gcount=0;
	vector <int> col;
	while(Ginput_file >> Gnumber){
		Gcount++;
		col.push_back(Gnumber);
		if (Gcount == 308){
			G.push_back(col);
			col.clear();
			Gcount=0;
		}
		

	}
	Ginput_file.close();


	// Input H
	vector<vector<int> > H;
	fstream Hinput_file;
	Hinput_file.open("H.txt");
	if(!Hinput_file){
		cout << "File H.txt is not exsit" << endl;
		return EXIT_FAILURE;
	}
	int Hnumber;
	int Hcount=0;
	vector <int> vcol;
	while(Hinput_file >> Hnumber){
		Hcount++;
		vcol.push_back(Hnumber);
		if (Hcount == 308){
			H.push_back(vcol);
			vcol.clear();
			Hcount=0;
		}
		

	}
	Hinput_file.close();

	// print out the H
	/*
	for (int j=0;j<H.size();j++){
		for(int i=0;i<H[j].size();i++)
			cout << H[j][i] << " ";
		cout << endl;
	}
	*/
	
	
	
	vector<int> cnd(1); //1 is length, 0 is value
	vector<int> vnd(1);

	//vector<int> cnd(HROW,0); // set the CND as an empty set
	//vector<int> vnd(COL,0); // set the VND as an empty set

	// read the file cndd.txt

	infile.open("cndd.txt",ios::in);
	if(!infile){
		cout << "The file is not existed" << endl;
		return EXIT_FAILURE;
	}
	
	// cout << "File is OK" << endl;
	while(infile >> number) 
		cnd.push_back(number);
	infile.close();

	// read the file vndd.txt
	
	infile.open("vndd.txt",ios::in);
	if(!infile){
		cout << "File vndd is not exist" << endl;
		return EXIT_FAILURE;
	}

	while(infile>>number)
		vnd.push_back(number);
	infile.close();
	
	int edge_num = cnd.size(); // size of edge 947 HARUSNYA 946
	
	
	vector <vector<int> > vnds(COL,vector<int>(1));
    vector <vector<int> > cnds(HROW,vector<int>(1));

    vector <vector<int> > sparse_vnds(COL,vector<int>(1));
    vector <vector<int> > sparse_cnds(HROW,vector<int>(1));

   
    //cout << vnd[6] << " this is first line" << endl;

    for(int i=1;i<edge_num;i++){ //disini benar
      // cout << "cnd in " << cnd[i] << " nilainya " << vnd[i] << endl;
        vnds[vnd[i]].push_back(cnd[i]);
        cnds[cnd[i]].push_back(vnd[i]);

        sparse_vnds[vnd[i]].push_back(i);
        sparse_cnds[cnd[i]].push_back(i);

        
	}
	
	/*
	*====================================================================================================================
	*====================================================================================================================
	*					START TO PERFORM ENCODER AND DECODER
	*====================================================================================================================
	*====================================================================================================================
	*/
	// Initialization
	const int Inf = numeric_limits<int>::max();
	const int minInf = numeric_limits<int>::min();
	vector<int> msg(133);
	vector<int> cw;
	vector<complex<double> > x;
	vector<complex<double> > rx;
	vector<double> y;
	vector<int> checksum;
	vector<vector<int> > HT;
	
	vector<int> compare_list;
	vector<int> add_list(1);
	vector<int> add_emp(1);

	vector<int> femp(947);
    vector<double> L(177);
    vector<double> emp(177);
    int sign=1;
    double D=0;
    vector<double> BETA(6);

	vector<double> bp_LLR;

	vector<double> result_LLR;
	vector<double> softBP(309);
	vector<int> hardBP(309);


	vector<int> check_eq; 

	int max_num_blocks=Inf;
	float snr = 1.5; // to check the correctness, change SNR to infinity
    double error_bp=0;
    int iter=100;
    int max_error=10;
	double blocks=0;
	double bp_bler;


    for (int block=1;block<=max_num_blocks;block++)
	{
		msg=random_message(block); // generate random message
		cw=encoder(msg,G);	 // codewords (0~307)	
		x=QPSKmodulator(cw); //tx=BPSKmodulator(cw);
		rx=awgn(x,snr,block);
		y=QPSKdemodulator(rx);
		
		// ======= BP Starting here ========
		bp_LLR=spa1(iter,y,sparse_vnds,sparse_cnds,edge_num,cnd,vnd);
		
		
		for(int i=0;i<309;i++)
		{
       	 softBP[i]=y[i];
		
		}

		
		//*add LLR to BP
		for(int j=1;j<edge_num;j++)
		{
			softBP[vnd[j]]+=bp_LLR[j];
		}


		for(int i=1;i<=cw_length;i++)
		{
			//BP
			if(softBP[i]<0){
				hardBP[i]=1;
			}
			else{
				hardBP[i]=0;
			}
		}
		

		

		if(ber(cw, hardBP)!=0)
		{
            error_bp++;
		}


		
		cout << "SNR=" << snr << ", " << block << "th blocks, BP=" << error_bp << endl;
		

        if(error_bp==max_error){
			blocks=block;
            break;
        }
		
		

		
    }
	bp_bler=error_bp/blocks;
	cout << "BLER BP : " << bp_bler << endl;

	return 0;
}

