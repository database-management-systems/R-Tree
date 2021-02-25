#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "RTree.h"

using namespace std;

typedef int ValueType;
typedef long long CoordType;

struct Record {
    long long int id;
    char* name;
    char* bio;
    long long int m_id;
};

struct Rect
{
    Rect()  {}

    Rect(CoordType a_minX, CoordType a_minY, CoordType a_maxX, CoordType a_maxY)
    {
        min[0] = a_minX;
        min[1] = a_minY;

        max[0] = a_maxX;
        max[1] = a_maxY;
    }


    CoordType min[2];
    CoordType max[2];
};


bool MySearchCallback(ValueType id)
{
    cout << "Hit data rect " << id << "\n";
    return true; // keep going
}

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
};


int main( int argc, char* argv[] )
{
    string filename = "Employee.csv";
    vector<Record*> emp;

    Filehandler newfile(filename);

    typedef std::vector< Rect > RectVector;
    RectVector rectVector;

    string file;
    fstream fin;
    fstream fout;
    char action;

    /** readfile **/
    file.assign(filename);
    fin.open(file.c_str(), ios::in);
    string line;
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
        rectVector.push_back( Rect( atoi(kkkey[0].c_str()), atoi(kkkey[0].c_str()), atoi(kkkey[0].c_str()) + atoi(kkkey[3].c_str()), atoi(kkkey[0].c_str()) + atoi(kkkey[3].c_str()) ) );
    }
    fin.close();

    /** search R-Tree **/
    // Rect search_rect(6, 4, 10, 6);
    // nhits = tree.Search(search_rect.min, search_rect.max, MySearchCallback);
    // cout << "Search resulted in " << nhits << " hits\n";

    do{
        cout << "Total record currently count is: "<< rectVector.size() << endl;
        cout << "Enter \"C\": to insert a record." << endl;
        cout << "Enter \"0\" to save & quit." << endl;
        cin >> action;
        if(action == 'C' || action =='c'){
            int id = -1;
            int m_id = 0;
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
                rectVector.push_back( Rect( id, id, id + m_id, id + m_id ) );
            }else
                cout << "Creating error......." << std::endl;
        }
    }while (action !='0');

    typedef RTree<ValueType, CoordType, 2, float> MyTree;
    MyTree tree;

    unsigned int i, nhits;
    cout << "number of rectangles is " << rectVector.size() << "\n";

    for(i=0; i<rectVector.size(); i++)
    {
        tree.Insert(rectVector[i].min, rectVector[i].max, i); // Note, all values including zero are fine in this version
    }

    /** test output
    int itIndex = 0;
    MyTree::Iterator it;
    for( tree.GetFirst(it);
         !tree.IsNull(it);
         tree.GetNext(it) )
    {
      int value = tree.GetAt(it);

      CoordType boundsMin[2] = {0,0};
      CoordType boundsMax[2] = {0,0};
      it.GetBounds(boundsMin, boundsMax);
      cout << "it[" << itIndex++ << "] " << value << " = (" << boundsMin[0] << "," << boundsMin[1] << "," << boundsMax[0] << "," << boundsMax[1] << ")\n";
    }

    // Iterator test, alternate syntax
    itIndex = 0;
    tree.GetFirst(it);
    while( !it.IsNull() )
    {
      CoordType value = *it;
      ++it;
      cout << "it[" << itIndex++ << "] " << value << "\n";
    }
    **/

    //save file
    tree.Save("EmployeeRTree");

    return 0;
}

