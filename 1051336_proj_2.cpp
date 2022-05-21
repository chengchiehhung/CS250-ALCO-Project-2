
#include<bitset>
# include <iostream>
# include <vector>
# include <map>

# include <string>

#define BIT_SIZE 3

using namespace std ;

map<string, int> Operation ; // 存入每個opertion裡面有幾個parameter
vector<vector<string> > Instruction ; // 切token的時候 每一行程式的每個token
map<string, int> Label_address ; // Label address -> map
map<int, vector<string> > Instruction_address ; // key -> address value -> 某行程式
map<string, int> Register_map ; // 每個Register 裡面的值
int gBranch_address ; // branch指令, Label的address
vector<vector<string> > Branch_Entry ; // 每個entry不同的預測狀態
vector<bitset<BIT_SIZE> > Branch_Bitstring ; // 每個entry裡面的 bitstring

int realTaken = 0, realNotTaken = 0, predict_sum = 0 ;
int predictTaken = 0, predictNotTaken = 0 ;

vector<string> GetToken() ;
void GetLine() ;
void skip_comment() ;
string read_string() ;


void skip_comment() {
    while( cin.peek() != '\n' && cin.peek() != -1 ) {
        cin.get() ;
    } // while
} // skip_comment

string read_string() {
    string a_string ;
    char onechar = cin.peek() ;
    while( cin.peek() != '\n' && cin.peek() != -1 ) {
        onechar = cin.peek() ;
        if ( onechar == ' ' || onechar == '\n' || onechar == ',' || onechar == ';' || onechar == '\t' ){
            break;
        } // if
        
        a_string += cin.get() ;
    } // while
    
//    cout << "string " << a_string << endl  ;
    return a_string ;
} // read_string()

int Get_Register( string reg ) {
    if ( Register_map.find( reg ) == Register_map.end() ) {
        Register_map[reg] = 0 ;
        return 0 ;
    } // if
    else {
        return Register_map[reg] ;
    } // else
} // Get_Register

void Perform_Add( vector<string> temp ) {
    string reg1 = temp[1], reg2 = temp[2], reg3 = temp[3] ;
    Register_map[reg1] = Get_Register( reg2 ) + Get_Register( reg3 ) ;
//    Register_map[reg1] = Register_map[reg2] + Register_map[reg3] ;
} // Perform_Add

void Perform_Addi( vector<string> temp ) {
    string reg1 = temp[1], reg2 = temp[2] ;
    int immed = stoi( temp[3] ) ;
    Register_map[reg1] = Get_Register( reg2 ) + immed ;
//    Register_map[reg1] = Register_map[reg2] + immed ;
} // Perform_Add

void Perform_Andi( vector<string> temp ) {
    string reg1 = temp[1], reg2 = temp[2] ;
    int immed = stoi( temp[3] ) ;
    Register_map[reg1] = Get_Register( reg2 ) & immed ;
//    Register_map[reg1] = Register_map[reg2] & immed ;
} // Perform_Andi

bool Perform_Beq( vector<string> temp ) {
    string reg1 = temp[1], reg2 = temp[2] ;
    string label = temp[3] + ":" ;
    if ( Get_Register( reg1 ) == Get_Register( reg2 ) ) {
        gBranch_address = Label_address[label] ;
        return true ;
    } // if
    else
        return false ;
} // Perform_Beq

bool Perform_Bne( vector<string> temp ) {
    string reg1 = temp[1], reg2 = temp[2] ;
    string label = temp[3] + ":" ;
    if ( Get_Register( reg1 ) != Get_Register( reg2 ) ) {
        gBranch_address = Label_address[label] ;
        return true ;
    } // if
    else
        return false ;
} // Perform_Bne

void Perform_Li( vector<string> temp ) {
    string reg1 = temp[1] ;
    int immed = stoi(temp[2]) ;
    Register_map[reg1] = immed ;
} // Perform_Li

void create_operation_map() {
    Operation.insert( pair<string, int>("add", 3 ) ) ;
    Operation.insert( pair<string, int>("andi", 3 ) ) ;
    Operation.insert( pair<string, int>("addi", 3 ) ) ;
    Operation.insert( pair<string, int>("beq", 3 ) ) ;
    Operation.insert( pair<string, int>("bne", 3 ) ) ;
    Operation.insert( pair<string, int>("li", 2 ) ) ;
} // create_inst_map


vector<string> GetToken() {
    char onechar ;
    string test_string ;
    vector<string> tempLine ;
    while( cin.peek() != -1 ) {
        onechar = cin.peek() ;
        if ( onechar == ' ' || onechar == '\n' || onechar == ',' || onechar == ';' || onechar == '\t' ) {
            if ( onechar ==  ';' )
                skip_comment() ;
            else if ( onechar == '\n' ) {
                if ( tempLine.size() != 0 )
                    Instruction.push_back( tempLine ) ;
                tempLine.clear() ;
                cin.get() ;
            } // else if
            else
                cin.get() ;
        } // if
        else {
            test_string = read_string() ;
            tempLine.push_back( test_string ) ;
        } // else
        
    } // while
    
    return tempLine ;
} // GetToken

void Initial_Branch_Entry( int entry ) {
    vector<string> status ;
    int size = 1 << BIT_SIZE ;
    bitset<BIT_SIZE> bitstring ;
    for ( int j = 0 ; j < size ; j ++ ) {
        status.push_back("SN") ;
    } // for
    for ( int i = 0 ; i < entry ; i ++ ) {
        Branch_Entry.push_back( status ) ; // initial one entry
        Branch_Bitstring.push_back( bitstring ) ; // initial bitstring
    } // for
    
} // Initial_Branch_Entry

void Change_status( vector<string> & sign, int position, bool change ) { // 換狀態 SN -> WN -> WT -> ST
    if ( change == true ) {
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
    } // else
} // Change_status

bool Predict( bool guess , bool realtake ) { //
    if ( guess == true && realtake == true )
        return true ;
    else if ( guess == false && realtake == false )
        return true ;
    else
        return false ;
} // Predict

void Predict_Branch() {
    
} // Predict_Branch

void Analyze() {
    int address = 0 ;
    vector<string> tempInstruction ;
    for( int i = 0 ; i < Instruction.size() ; i ++ ) {
        if ( Operation.find(Instruction[i][0]) == Operation.end() && Instruction[i][0][Instruction[i][0].length()-1] == ':' ) { // 判斷是不是label
            //是label的話
            Label_address.insert( pair<string,int>(Instruction[i][0], address)) ;

            if ( Instruction[i].size() != 1 ) { // 如果label 後面是接instruction的話-> Label: add R1, R2, R3
                
                for( int j = 1 ; j < Instruction[i].size() ; j ++ ) {
                    tempInstruction.push_back( Instruction[i][j] ) ;
                } // for
                
                Instruction_address.insert( pair<int, vector<string> >(address, tempInstruction ) ) ; // address對應第幾行的程式
                address ++ ;
            } // if
        } // if
        else {
            for( int j = 0 ; j < Instruction[i].size() ; j ++ ) {
                tempInstruction.push_back( Instruction[i][j] ) ;
            } // for
            
            Instruction_address.insert( pair<int, vector<string> >(address, tempInstruction ) ) ; // address對應第幾行的程式
            address ++ ;
        } // else
        
        tempInstruction.clear() ;
    } // for
    
} // Analyze

void Perform( int entry ) {
    int pc = 0 ;
    bool branch = false, branch_prediction ;
    vector<string> current_inst ;
    int current_entry = 0, now_value ;
    Register_map["R1"] = 0 ;
    while ( pc < Instruction_address.size()  ) {
        cout << "PC : " << pc << "\t" ;
        // get instruction
        current_inst = Instruction_address[pc] ;
        cout << "Current Instruction : " ;
        for( auto i : current_inst ) { // 輸出現在的 instruction
            cout << i << " " ;
        } // for
        cout << endl ;
        branch = false ;
        // predict branch
        // 1. 先看是哪一個entry pc%entry -> current_entry
        // 2. 依照Branch_Bitstring 去找 Branch_Entry 裡面的sign 去做預測
        
        current_entry = pc % entry ; // 得知這個是哪一個entry
        now_value = static_cast<int>( Branch_Bitstring[current_entry].to_ulong() ) ; // 將那3個bit轉換成int
        cout << "Entry:" << current_entry << "  " ;
        cout << Branch_Bitstring[current_entry] << " " ; // 輸出現在的3bit
        for ( auto i : Branch_Entry[current_entry] ) { // 輸出現在的 sign
            cout << i << " " ;
        } // for
        
        
        if ( Branch_Entry[current_entry][now_value][1] == 'T' ) { // 預測
            branch_prediction = true ;
            
            cout << "\tPredict: T\t" ;
        } // if
        else {
            branch_prediction = false ;
            
            cout << "\tPredict: N\t" ;
        } // else

        // perform instruction
        if ( current_inst[0] == "li" ) {
            Perform_Li( current_inst ) ;
        } // if
        else if ( current_inst[0] == "addi" ) {
            Perform_Addi( current_inst ) ;
        } // else if
        else if ( current_inst[0] == "beq" ) {
            branch = Perform_Beq( current_inst ) ;
            if( branch )  // 統計
                realTaken ++ ;
            else
                realNotTaken ++ ;

            if ( branch_prediction )
                predictTaken ++ ;
            else
                predictNotTaken ++ ;
            
            if ( Predict( branch, branch_prediction ))
                predict_sum ++ ;
     
        } // else if
        else if ( current_inst[0] == "bne" ) {
            branch = Perform_Bne( current_inst ) ;
            if( branch )  // 統計
                realTaken ++ ;
            else
                realNotTaken ++ ;

            if ( branch_prediction )
                predictTaken ++ ;
            else
                predictNotTaken ++ ;
            
            if ( Predict( branch, branch_prediction ))
                predict_sum ++ ;
            
        } // else if
        else if ( current_inst[0] == "add" ) {
            Perform_Add( current_inst ) ;
        } // else if
        else if ( current_inst[0] == "andi" ) {
            Perform_Addi( current_inst ) ;
        } // else if
        
        if ( branch )
            cout << "Real: T" << endl ;
        else
            cout << "Real: N" << endl ;
        
        // predict or mispredict
        if ( Predict( branch, branch_prediction )) {
            cout << "Prediction" ;
        } // if
        else {
            cout << "Misprediction" ;
        } // else
        
        // change branch history status
        Change_status( Branch_Entry[current_entry], now_value, branch ) ;
        // change 3 bit value
        Branch_Bitstring[current_entry] = Branch_Bitstring[current_entry] << 1 ;
        if ( branch ) {
            
            Branch_Bitstring[current_entry][0] = 1 ;
//            cout << "jump " ;
            pc = gBranch_address ;
        } // if
        else {
            
            pc ++ ;
        } // else
        
        cout << endl << endl ;
    } // while
    
    cout << "PC : " << pc << endl ;
} // Perform


int main(int argc, const char * argv[]) {
    int entry = 0 ;
    create_operation_map() ;
    cout << "Please input entry (entry > 0)" << endl ;
    cin >> entry ;
    cin.get() ;
    GetToken() ; // 讀入所有的token
    
    for ( int i = 0 ; i < Instruction.size() ; i ++ ) {
        cout << "第" << i << "行 " ;
        for ( int j = 0 ; j < Instruction[i].size() ; j ++ ) {
            cout  <<  Instruction[i][j] << " "  ;
        } // for
        cout << endl ;
    } // for

    cout << "Analyze" << endl ;
    Analyze() ; // 加上各個物件的位置，包含label's address, instruction's address
    
    
    for ( auto i : Instruction_address ) {
        cout << i.first << "  " ;
        for( auto j : i.second ) {
            cout << j << " " ;
        } // for
        cout << endl ;
    } // for
    
    cout << "label" << endl ;
    for ( auto i : Label_address ) {
        cout << i.second << "  " << i.first << endl ;

    } // for
    
    cout << "Analyze End" << endl ;
    
    
    cout << "Perform Start" << endl ;
    Initial_Branch_Entry( entry ) ;
    Perform( entry ) ;
    
    cout << "Perform End" << endl << endl ;
    
    cout << "Taken: " << realTaken << " NotTaken: " << realNotTaken << endl ;
    cout << "Predict Taken: " << predictTaken << "Predict Not Taken: " << predictNotTaken << endl ;
    cout << "Success predict: " << predict_sum << "/" << realTaken + realNotTaken <<  endl ;
    return 0;
}
