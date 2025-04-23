#include <iostream>
#include <fstream>
#include <vector>
#include <cstring>
#include <map>
#include <sstream>
#include <iomanip>
using namespace std;

vector<string> attributes_of_table;

bool doesTableExists2(string tableName)
{
    fstream SchemaFile;
    SchemaFile.open("SchemaFile.txt",ios::in);
    if(SchemaFile)
    {
        string line;
        while(!SchemaFile.eof())
        {
            getline(SchemaFile,line);
            if(line[0]=='*')
            {
                string name = line.substr(1,line.size()-2);
                if(name==tableName)
                {
                    SchemaFile.close();
                    return true;
                }
            }
        }
    }
    else
        cout<<"Schema File doesn't exists"<<endl;
    return false;
}

void CreateTable(vector<string>&Tokens)
{
    //If control comes here means, table with specified name doesn't exists
    //So, Append the name of table and its attribute to schema file
    {
        fstream SchemaFile;
        SchemaFile.open("SchemaFile.txt",ios::app);

        SchemaFile << "*" << Tokens[2] << "*" << endl << "<<" << endl;
        SchemaFile << "pk: " << Tokens.back() << endl;

        int i = 3;
        while(i<Tokens.size()-3)
        {
            SchemaFile << Tokens[i] << " ";

            if(Tokens[i+1]=="varchar")
                {SchemaFile << Tokens[i+1] << " " << Tokens[i+2];  i+=3;}

            else if(Tokens[i+1]=="decimal")
                {SchemaFile << Tokens[i+1] << " " << Tokens[i+2] <<  " " << Tokens[i+3]; i+=4;}

            else
            {
                SchemaFile << Tokens[i+1];
                i+=2;
            }
            if(Tokens[i]=="check")
            {
                SchemaFile <<" " << Tokens[i] << " " ;
                SchemaFile << Tokens[i+1] << " " << Tokens[i+2] << " " << Tokens[i+3] ;
                i+=4;
                while(Tokens[i]=="AND" || Tokens[i]=="OR")
                {
                    SchemaFile <<" " << Tokens[i] << " " ;
                    SchemaFile << Tokens[i+1] << " " << Tokens[i+2] << " " << Tokens[i+3] ;
                    i+=4;
                }
            }
            SchemaFile << endl;

        }
        SchemaFile << ">>" << endl <<endl;
        SchemaFile.close();

        cout << "Table Created Successfully"<<endl;
        
    }
        
}

void delete_last_line()
{   
    ifstream fileIn( "SchemaFile.txt" );                   // Open for reading

    stringstream buffer;                             // Store contents in a std::string
    buffer << fileIn.rdbuf();
    string contents = buffer.str();

    fileIn.close();
    contents.pop_back();                                  // Remove last character

    ofstream fileOut( "SchemaFile.txt" , ios::trunc); // Open for writing (while also clearing file)
    fileOut << contents;                                  // Output contents with removed character
    fileOut.close(); 
}

void DropTable(vector<string>&Tokens)
{
    //drop table Students;
    fstream SchemaFile;
    SchemaFile.open("SchemaFile.txt",ios::in);
    
    fstream temp;
    temp.open("temp.txt",ios::out);
    
    string line;
    while(!SchemaFile.eof())
    {
        getline(SchemaFile,line);
        if(line[0]=='*' && line.substr(1,line.size()-2)==Tokens[2])// *Teacher*
        {
            while(line!=">>")
                getline(SchemaFile,line);

            getline(SchemaFile,line);
            getline(SchemaFile,line);
            
        }
        temp << line << endl;
    }
    
    cout<<"<"<<Tokens[2]<<"> Dropped Successfully"<<endl;
    
    temp.close();
    SchemaFile.close();
    remove("SchemaFile.txt");
    rename("temp.txt","SchemaFile.txt");

    char tableName[30];
    strcpy(tableName, (Tokens[2]+".txt").c_str());
    remove(tableName);

    delete_last_line();
}

void DescribeTable(vector<string>&Tokens)
{
    fstream SchemaFile;
    SchemaFile.open("SchemaFile.txt",ios::in);

    string line;
    while(!SchemaFile.eof())
    {
        getline(SchemaFile,line);
        if(line[0]=='*' && line.substr(1,line.size()-2)==Tokens[1])// *Teacher*
        {
            getline(SchemaFile,line);
            while(1)
            {
                getline(SchemaFile,line);
                if(line==">>")
                    break;
                cout<<line<<endl;
            }
            break;
        }    
    }    
}

int Count_no_Attributes(string TableName)
{
    //This function returns the number of attributes of the table-tableName
    fstream SchemaFile;
    SchemaFile.open("SchemaFile.txt",ios::in);
    int attributes=0;

    if(SchemaFile)
    {
        string line;
        while(!SchemaFile.eof())
        {
            getline(SchemaFile,line);
            if(line[0]=='*')
            {
                string name = line.substr(1,line.size()-2);
                if(name==TableName)
                {
                    getline(SchemaFile,line);//<<
                    getline(SchemaFile,line);//pk:

                    while(line!=">>")
                    {
                        attributes++;
                        getline(SchemaFile,line);
                    }
                    return attributes-1;
                }
            }
        }
    }
    else
    cout<<"Schema File not found"<<endl;

    return 0;
}
string ExtractCol(string tuple,int colno){
    //tuple = <103,S Y,24-02-2001>, colno=1, will return SY
    int comma=0;
    int i=1;
    while(comma<colno)
    {
        if(tuple[i]==',')
            comma++;
        i++;
    }

    string pk="";
    while(tuple[i]!=','&& tuple[i]!='>')
    {
        pk+=tuple[i];
        i++;
    }
    return pk;
}

bool isInt(string s)
{
    for(char c:s)
        if(!isdigit(c))
            return false;
    return true;
}
int checkValuesOrder(vector<string>&Tokens)
{
    //Getting datatypes of table from Schema File
    vector<string> datatypesInSchema;
    string tableName = Tokens[2];
    fstream SchemaFile;
    SchemaFile.open("SchemaFile.txt",ios::in);

    string line;
    string word;
    while(!SchemaFile.eof())
    {
        getline(SchemaFile,line);
        if(line[0]=='*' && line.substr(1,line.size()-2)==tableName)// *Teacher*
        {
            getline(SchemaFile,line);// <<
            getline(SchemaFile,line);// pk: roll
            while(1)
            {
                getline(SchemaFile,line);
                if(line==">>") break;
                istringstream ss(line);
                ss >> word;
                ss >> word;
                datatypesInSchema.push_back(word);
            }
        }    
    } 

    //Getting Datatypes from values(...) in tokens
    vector<string> datatypesInTokens;

    int i=0;
    while(Tokens[i]!="values")
        i++;
    i++;

    for(;i<Tokens.size();i++)
    {
        if(isalpha(Tokens[i][0]))
            datatypesInTokens.push_back("varchar");
        else if(Tokens[i][2]=='-')
            datatypesInTokens.push_back("date");
        else if(isInt(Tokens[i]))
            datatypesInTokens.push_back("int");
        else
            datatypesInTokens.push_back("decimal");
    }

    for(int i=0;i<datatypesInSchema.size();i++)
        if(datatypesInSchema[i]!=datatypesInTokens[i])
            return 0;
    /*
    cout<<"Schema datatypes: ";
    for(auto x:datatypesInSchema)
        cout<<x<<"  ";
    cout<<endl;

    cout<<"Tokens datatypes: ";
    for(auto x:datatypesInTokens)
        cout<<x<<"  ";
    cout<<endl;
    */
    return 1;

}
int InsertInto(vector<string>&Tokens)
{
    //If control comes here, means the specified table exists in schema file;
    //1.Checking whether primary key already exists in the table or not
    fstream table;
    table.open(Tokens[2]+".txt",ios::in);
    string tuple;
    string temp = Tokens[2]+".txt";
    
    while(table && !table.eof())
    {
        getline(table,tuple);
        if(Tokens[4]==ExtractCol(tuple,0))//0 means primary key
        {
            cout<<"PK already exists"<<endl;
            table.close();
            return 0;
        }
    }

    //error handling2 : Checking whether the values are specified according to the Schema..
    int noOfAttributes = Count_no_Attributes(Tokens[2]);
    if( Tokens.size()-4 < noOfAttributes )
    {cout<<"Less Values Specified"<<endl;return 0;}
        
    if( Tokens.size()-4 > noOfAttributes )
    {cout<<"More Values Specified"<<endl;return 0;}
    
    if(!checkValuesOrder(Tokens))
    {cout<<"Values not specified in proper order"<<endl;return 0;}

    //3.If the control comes here, there are no errors
    //  And we can append the tuple;
    fstream TABLE;
    TABLE.open(Tokens[2]+".txt",ios::app);
    TABLE <<"<";

    int i=4;
    while(i<Tokens.size()-1)
    {
        TABLE << Tokens[i] <<",";
        i++;
    }
    TABLE << Tokens[i] << ">" << endl;

    TABLE.close();
    return 1; 
}

vector<int> Find_Indices(vector<string>&Tokens,vector<string> attributes_of_table)
{
    //          0  1    2      3    4
    //Schema : id name gender mob email 
    
    //Query  : select name,id,email form Students where id>100;

    vector<int> indices_of_att_in_query;
    for(int i=1;Tokens[i]!="from";i++)
    {
        bool flag=true;
        for(int j=0;flag && j<attributes_of_table.size();j++)
            if(Tokens[i]==attributes_of_table[j])
            {
                indices_of_att_in_query.push_back(j);
                flag=false;
            }
    }
    return indices_of_att_in_query;
}


bool is_Where_True(string tuple,vector<string>&Tokens,int i)
{
    //cout<<"i:"<<i<<"   Tokens[i]:"<<Tokens[i]<<endl;
    if(i+1==Tokens.size())return true;
    //i is pointing to Students here;
    i++;//After this inc i will point to where

    int index=0;
    for(auto x:attributes_of_table)
    {
        if(x==Tokens[i+1])//Tokens[i+1]==marks
            break;
        else
            index++;
    }
    //after this index will be index of 'marks' in attributes_of_table

    bool isFloat = isdigit(Tokens[i+3][0]);
    
    if(isFloat)//Floats
    {   
        float typecasted = stof(ExtractCol(tuple,index));
        
        if(Tokens[i+2]==">") {return typecasted > stof(Tokens[i+3]);}
        if(Tokens[i+2]=="<") {return typecasted < stof(Tokens[i+3]);}
        if(Tokens[i+2]=="=") {return typecasted == stof(Tokens[i+3]);}
        if(Tokens[i+2]=="!=") {return typecasted != stof(Tokens[i+3]);}
    }
    else//Strings
    {
        if(Tokens[i+2]=="!=")     return ExtractCol(tuple,index) != (Tokens[i+3]);
        else if(Tokens[i+2]=="=") return ExtractCol(tuple,index) == (Tokens[i+3]);
    }
}

//Also VALID for where clause;
void Helper_Select(vector<string>&Tokens,string TableNameInQuery,int i)
{
    if(Tokens[1]=="*")
    {
        fstream file;
        file.open(TableNameInQuery+".txt",ios::in);
        int attributes = Count_no_Attributes(TableNameInQuery);
        
        bool flag=false;
        string tuple;
        while(file && !file.eof())
        {
            flag=true;
            getline(file,tuple);//<102,Dipak Yadav,99>
            if(tuple.size()==0)break;

            //cout<<"val:"<<Tokens[i]<<endl;
            if(is_Where_True(tuple,Tokens,i))
            {
                for(int j=0;j<attributes;j++)
                    cout<<left<<setw(25)<<ExtractCol(tuple,j);
                cout<<endl;
            }
        }
        if(!flag)
            cout<<"No Records yet"<<endl;
    }

    else
    {
        vector<int>indices_of_att_in_query = Find_Indices(Tokens,attributes_of_table);

        fstream file;
        file.open(TableNameInQuery+".txt",ios::in);
        
        bool flag=false;
        string tuple;

        while(file && !file.eof())
        {
            flag=true;
            getline(file,tuple);//<102,Dipak Yadav,99>
            if(tuple.size()==0)break;
            
            if(is_Where_True(tuple,Tokens,i)){
                for(int x:indices_of_att_in_query)
                    cout<<left<<setw(25)<<ExtractCol(tuple,x);
                cout<<endl;
            }
        }
        if(!flag)
            cout<<"No Records yet"<<endl;
    }
}

void FillingAttributesOfTable(string tableName)
{
    attributes_of_table.clear();
    fstream SchemaFile;
    SchemaFile.open("SchemaFile.txt",ios::in);
    string line;
    while(!SchemaFile.eof())
    {
        getline(SchemaFile,line);
        if(line[0]=='*')
        {
            string name = line.substr(1,line.size()-2);
            if(name==tableName)
            {
                getline(SchemaFile,line);//<<
                getline(SchemaFile,line);//pk: 

                while(line!=">>")
                {
                    getline(SchemaFile,line);
                    if(line==">>")break;

                    string temp="";
                    int i=0;
                    while(line[i]!=' ')
                    {
                        temp += line[i];
                        i++;
                    }
                    attributes_of_table.push_back(temp);
                }

                SchemaFile.close();
                break;
            }
        }
    }
}
void Select(vector<string>&Tokens)
{
    int i=0;
    while(Tokens[i]!="from") i++;
    i++;//After this increment i will point to Students

    string tableName = Tokens[i];
    //Check whether table with specified name exists in Schema file or not
    {
        if(doesTableExists2(tableName))
        {
            FillingAttributesOfTable(tableName);
            Helper_Select(Tokens,tableName,i);//i is pointing to Students;
        }
        else
            cout<<"table <"<<tableName<<"> table doesn't exists"<<endl;
    }
}


vector<int>GetIndices(string tableName, map<string,string>mpp)
{
    FillingAttributesOfTable(tableName);
    vector<int>ans;
    
    for(auto kv=mpp.begin();kv!=mpp.end();kv++)
    {
        int count=0;
        for(auto x:attributes_of_table)
        {   
            //cout<<"attr:"<<x<<"     kv:"<<kv->first<<endl;
            if(kv->first==x)
                {ans.push_back(count);break;}
            count++;
        }
    }
    return ans;
}
vector<string> split (string s, char delim) {
    vector<string> result;
    stringstream ss (s);
    string item;

    while (getline (ss, item, delim)) {
        result.push_back (item);
    }

    return result;
}
void UpdateTable(vector<string>&Tokens)
{
    string tableName = Tokens[1];
    //Finding position of where in Tokens
    bool isWhere=false;
    int i;
    for( i=3;i<Tokens.size();i++)
    {   
        if(Tokens[i]=="where")
            {isWhere=true;break;}
    }
    i--;
    //Checking if PK is getting updated .. 
        //  if where is not present
        //  and yes PK is getting updated, then dont update because each record will get same PK.
    
    fstream SchemaFile;
    SchemaFile.open("SchemaFile.txt",ios::in);

    //Reading PK from Schema File
    string PK_fromSchema="";
    string line;
    while(SchemaFile && !SchemaFile.eof())
    {
        getline(SchemaFile,line);
        if(line[0]=='*')//*Students*
        {
            if(tableName==line.substr(1,line.size()-2))
            {
                getline(SchemaFile,line);//<<
                getline(SchemaFile,line);//pk: 

                PK_fromSchema = line.substr(4,line.size()-4);
                SchemaFile.close();
                break;
            }
        }
    }

    //Geting Keys and Values from the Tokens
    map<string,string> mpp;
    bool flag=true;
    
    for(int i=3;i<Tokens.size();i+=3)
        mpp[Tokens[i]] = Tokens[i+2];
                
    //Checking if PK attribute is being updated or not;
    for(auto x : mpp)
        if(x.first==PK_fromSchema)
        {
            cout<<"Cannot be Updated, same PK would get set"<<endl;
            return;
        }
        
    fstream temp;
    temp.open("temp.txt",ios::out);

    fstream table;
    table.open(tableName+".txt",ios::in);

    string tuple;
    vector<int>indices = GetIndices(tableName,mpp);//indices of keys, which we will be updating
    
    cout<<endl;
    FillingAttributesOfTable(tableName);

    while(table && !table.eof())
    {
        getline(table,tuple);
        if(tuple.size()==0)break;
        vector<string>parsed = split(tuple.substr(1,tuple.size()-2),',');
        if(is_Where_True(tuple,Tokens,i))
        {
            for(int index:indices)
                parsed[index] = mpp[attributes_of_table[index]];
        }   
        temp << "<";
        for(auto ele:parsed)
            temp << ele << ",";
        temp << ">" << endl;
    }

    table.close();
    temp.close();

    char Tname[30];
    strcpy(Tname, (Tokens[1]+".txt").c_str());
    remove(Tname);
    rename("temp.txt",Tname);
}


void DeleteFrom(vector<string>&Tokens)
{
    
    //Where Clause not present, 
        //So delete the table itself..
        //But don't remove its details from the schema file..
    if(Tokens.size()==3)
    {
        fstream table;
        table.open(Tokens[2]+".txt",ios::in);
        string line;
        int count=0;
        while(table && !table.eof())
            {getline(table,line);count++;}
        table.close();
        cout<<count-1<<" rows affected"<<endl;
        char tableName[30];
        strcpy(tableName, (Tokens[2]+".txt").c_str());
        remove(tableName);
        return ;
    }

    //if control comes here, means where clause is present

    int originalCount=0;
    int duplicateCount=0;

    FillingAttributesOfTable(Tokens[2]);
    fstream temp;
    temp.open("temp.txt",ios::out);

    fstream table;
    table.open(Tokens[2]+".txt",ios::in);

    string tuple;
    
    while(table && !table.eof())
    {
        getline(table,tuple);
        if(tuple.size()==0)break;
        originalCount++;
        if(!is_Where_True(tuple,Tokens,2))
            {temp << tuple <<endl;duplicateCount++;}
    }
    table.close();
    temp.close();

    char tableName[30];
    strcpy(tableName, (Tokens[2]+".txt").c_str());
    remove(tableName);
    rename("temp.txt",tableName);

    cout<<originalCount-duplicateCount<<" rows affected"<<endl;
}