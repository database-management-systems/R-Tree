#include <iostream>
#include<vector>
#include<cstdlib>
#include<fstream>
#include<sstream>
#include<cmath>
#include <cstring>

using namespace std;

#define block_bytes_size 4096

struct Record {
    long long int id;
    char* name;
    char* bio;
    long long int m_id;
};
class Block{
private:
    vector<Record> records;
    Block* overflow;
    int record_size;
    int page_size;
public:
    Block(){
        overflow = NULL;
        records.clear();
        record_size = 0;
        page_size = 0;
    }
    Block(int size){
        overflow = NULL;
        record_size = size;
        records.clear();
        page_size = floor(block_bytes_size / size);
    }
    void insert_record(Record rc){
        if (records.size() < page_size) {
            records.push_back(rc);

        }else {
            if (overflow == NULL)
                overflow = new Block(record_size);
            overflow->insert_record(rc);
        }
    }
    void dump_record_block(vector<Record>& rc){
        for (int i = 0; i < records.size(); i++)
            rc.push_back(records[i]);
        records.clear();

        if (overflow) {
            overflow->dump_record_block(rc);
            delete overflow;
            overflow = NULL;
        }
    }
};

//class HashTable{
//private:
//    vector<Block*> big_bk; // Store all blocks.
//    int rec_size;
//    int max_capacity;
//    int rec_numbers;
//    int bits_num ;
//public:
//    HashTable(){
//        big_bk.clear();
//        rec_size = 0;
//        max_capacity = 0;
//        rec_numbers = 0;
//        bits_num = 0;
//    }
//    HashTable(int record_size){
//        rec_numbers = 0;
//        bits_num = 1;
//        max_capacity = floor(block_bytes_size / record_size);
//        this->rec_size = record_size;
//        big_bk.push_back(new Block(record_size));
//
//    };
//    unsigned long int Hashfunc(unsigned long int id){
//        int value;
//        value = id % bits_num;
//        return value;
//    }
//    int capacity() {
//        double percent = (double) rec_numbers / big_bk.size();
//        int cap = (100 * percent / max_capacity);
//        return cap;
//    }
//
//    void insert_record(Record* rc) {
//        unsigned long int hashkey = Hashfunc(rc->id);
//        if (big_bk.size() <= hashkey) {
//            hashkey -= (1 << (bits_num - 1));
//        }
//        big_bk[hashkey]->insert_record(*rc);
//        rec_numbers += 1;
//        while (80 <= capacity()) {
//            big_bk.push_back(new Block(rec_size));
//            bits_num = ceil(log2((double)big_bk.size()));
//            hashkey = big_bk.size() - 1 - (1 << (bits_num - 1));
//            vector<Record> rec;
//            big_bk[hashkey]->dump_record_block(rec);
//            for (int i = 0; i < rec.size(); i++) {
//                big_bk[Hashfunc(rec[i].id)]->insert_record(rec[i]);
//            }
//        }
//    }
//
//    int get_record_num(){
//        return rec_numbers;
//    }
//
//    void dump_table(vector<Record> &rc){
//        for (int i = 0; i < big_bk.size();i++){
//            big_bk[i]->dump_record_block(rc);
//        }
//    }
//};



vector<string> split(string str,string pattern)
{
    std::string::size_type pos;
    std::vector<std::string> result;
    str+=pattern;//Expand strings to make them easier to manipulate
    int size=str.size();

    for(int i=0; i<size; i++)
    {
        pos=str.find(pattern,i);
        if(pos<size)
        {
            std::string s=str.substr(i,pos-i);
            result.push_back(s);
            i=pos+pattern.size()-1;
        }
    }
    return result;
}

class Filehandler{
private:
    fstream fin;
    fstream fout;
    string file;
public:
    Filehandler(string file_name){
        file.assign(file_name);
    }

    void stc(string& str, char* c, int size) {
        for (int i = 0; i < str.length(); i++) {
            if (str[i] != '\0')
                c[i] = str[i];
            else
                break;
            if (i == size - 1)
                break;
        }
    }
    void readfile(vector<Record*>& emp) {
        fin.open(file.c_str(), ios::in);
        string line;
        Record* rc;
        vector<string> ve;
        //Read according to the space, end when encountered a whitespace
        while(getline(fin,line))
        {
            ve.push_back(line);
        }
        for(int i=0;i<ve.size();i++) {
            // Index the file
            string this_line  = ve[i];
            vector<string> kkkey = split(ve[i],",");
            rc = setemp(atoi(kkkey[0].c_str()), kkkey[1], kkkey[2], atoi(kkkey[3].c_str()));
            emp.push_back(rc);
        }
        fin.close();
    }
    Record* setemp(long int id, string name, string bio, long int m_id) {
        const int size = floor(500 / sizeof(char));
        Record* emp = new Record;
        emp->id = id;
        emp->name = new char[size + 1];;
        emp->bio = new char[size + 1];;
        emp->m_id = m_id;
        memset(emp->name, '\0', size + 1);
        memset(emp->bio, '\0', size + 1);
        stc(name, emp->name, size);
        stc(bio, emp->bio, size);

        return emp;
    }
    int get_rec_size(Record* rc){
        return sizeof(rc) + 2 * (floor(500 / sizeof(char)));
    }
    void savefile(HashTable* table){
        fout.open("EmployeeRTree", ios::out);
        vector<Record> rc;
        table->dump_table(rc);
        for (int i = 0; i < rc.size();i++){
            fout << rc[i].id << ","<< rc[i].name << ","<< rc[i].bio << ","<< rc[i].m_id << "\n";
        }
        fout.close();
    }
};

void create_table(HashTable *table, Filehandler *file){
    Record *rc;
    long int id = -1;
    long int m_id = 0;
    string name;
    string bio = "";

    cout << "Please enter the employee ID: ";
    cin >> id;
    cin.ignore();
    cout << "Please enter the employee name: ";
    getline(cin, name);
    cout << "Please enter the employee bio: ";
    getline(cin, bio);
    cout << "Please enter the employee's manager ID: ";
    cin >> m_id;
    cin.ignore();

    if (id >= 0 && m_id >= 0){
        rc = file->setemp(id, name, bio, m_id);
        table->insert_record(rc);
    }else
        cout << "Creating error......." << std::endl;
}

int main(int argc, char *argv[]) {

    string filename = "Employee.csv";
    vector<Record*> emp;
    int rec_size;
    char action;
    Filehandler newfile(filename);
    newfile.readfile(emp);
    rec_size = newfile.get_rec_size(emp[0]);
    HashTable ht(rec_size);
    for (int i=0; i<emp.size();i++)
        ht.insert_record(emp[i]);
    do{
        cout << "Total record currently count is: "<< ht.get_record_num() << endl;
        cout << "Enter \"C\": to insert a record." << endl;
        cout << "Enter \"0\" to save & quit." << endl;
        cin >> action;
        if(action == 'C' || action =='c'){
            create_table(&ht,&newfile);
        }
    }while (action !='0');
        newfile.savefile(&ht);
}


