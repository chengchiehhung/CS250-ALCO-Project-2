//
//  main.cpp
//  low 3-bit predictor
//
//  Created by Larrykuo on 2022/5/14.
//

# include <iostream>
# include <vector>
# include <bitset>

#define BIT_SIZE 3

using namespace std ;



void Change_status( vector<string> & sign, int position, char change ) { // 換狀態 SW -> WN -> WT -> ST
    if ( change == 'T' ) {
        if ( sign[position] == "SN" ) {
            sign[position] = "WN" ;
        } // if
        else if ( sign[position] == "WN" ) {
            sign[position] = "WT" ;
        } // else if
        else if ( sign[position] == "WT" ) {
            sign[position] = "ST" ;
        } // else if
        else if ( sign[position] == "ST" ) {
            sign[position] = "ST" ;
        } // else if
    } // if
    else { // change == "N"
        if ( sign[position] == "ST" ) {
            sign[position] = "WT" ;
        } // if
        else if ( sign[position] == "WT" ) {
            sign[position] = "WN" ;
        } // else if
        else if ( sign[position] == "WN" ) {
            sign[position] = "SN" ;
        } // else if
        else if ( sign[position] == "SN" ) {
            sign[position] = "SN" ;
        } // else if
    } //
} // Change_status

bool Predict( char guess , char realtake ) { //
    if ( guess == realtake )
        return true ;
    else
        return false ;
} // Predict

void Run( string question ) {
    // 3 bit -> 8 status
    int size = 1 << BIT_SIZE ;
    int now_value = 0 ;
    string now_predict ;
    vector<string> status ;
    for ( int i = 0 ; i < size ; i ++ ) {
        status.push_back("SN") ;
    } // for
    
    
    bitset<BIT_SIZE> bitstring ; // all bits initial to 0
    // take 3 bits to predict
    // 3 bits will point to a sign and it shows the prodiction is taken or not taken
    // if it's right or wrong, it will change the sign to another sign
    // according to the history NT to change 3 bits value
    
    for( int i = 0 ; i < question.size() ; i ++ ) {
        cout << i+1 << " Round." << " Now is " << question[i] << endl ;
        now_value = static_cast<int>(bitstring.to_ulong()) ;
//        cout << bitstring.to_ulong() << endl ;
        if ( status[now_value][1] == 'T' ) {
            now_predict = "Taken" ;
            cout << "Predict: " << now_predict ;
        } // if
        else if ( status[now_value][1] == 'N' ) {
            now_predict = "NotTaken" ;
            cout << "Predict: " << now_predict ;
        } // else if
        
        if ( Predict( question[i], status[now_value][1] ) ) {
            cout << "  prediction" << endl ;
        } // if
        else {
            cout << "  mispredict" << endl ;
        } // else
        
        Change_status( status, now_value, question[i]) ;
        for ( string i : status ) {
            cout << i << " " ;
        } // for
        cout << endl ;
            
        bitstring = bitstring << 1 ;
        if ( question[i] == 'T' )
            bitstring[0] = 1 ;

        cout << "bitstring: " << bitstring << endl ;
        
        // change 3 bit
        
    } // for
} // Run

int main(int argc, const char * argv[]) {
    // insert code here...
    cout << "Hello, World!\n";
    string question ;
    cin >> question ;
    // input  TTTTNNTNTNTNTNTNT
    Run( question ) ;
//    bitset<3> test("010") ;
//    cout << test << endl ;
//    test = test << 1 ;
//    cout << test << endl ;
    
    return 0;
}
