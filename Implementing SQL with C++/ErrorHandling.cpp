#include <vector>
#include <string>
#include <fstream>
#include <iostream>
using namespace std;

class TableChecker {
public:
    bool doesTableExists(string tableName) {
        fstream SchemaFile;
        SchemaFile.open("SchemaFile.txt", ios::in);
        if (SchemaFile) {
            string line;
            while (!SchemaFile.eof()) {
                getline(SchemaFile, line);
                if (line[0] == '*') {
                    string name = line.substr(1, line.size() - 2);
                    if (name == tableName) {
                        return true;
                    }
                }
            }
        } else {
            cout << "Schema File doesn't exist" << endl;
        }
        SchemaFile.close();
        return false;
    }
};

class ErrorChecker {
public:
    ErrorChecker() {}
    bool ErrorsChecking(vector<string>& Tokens) {
        if (Tokens.empty())
            return false;

        TableChecker tableChecker;

        if (Tokens[0] == "create" && Tokens[1] == "table") {

            //Error 1 : Checking whether table with specified name exists in Schema file or not
            if (tableChecker.doesTableExists(Tokens[2])) {
                cout << "table <" << Tokens[2] << "> already exists" << endl;
                cout << "Table not created" << endl;
                return false;
            }

            //Error 2 : Checking whether PK attribute is defined or not
            int Tsize = Tokens.size();
            if (Tokens[Tsize - 3] != "primary" && Tokens[Tsize - 2] != "key") {
                cout << "Defining PK is mandatory" << endl;
                cout << "Table not created" << endl;
                return false;
            }
        }

        else if (Tokens[0] == "drop" && Tokens[1] == "table") {
            //Error 1 : Checking whether table with specified name exists in Schema file or not
            if (!tableChecker.doesTableExists(Tokens[2])) {
                cout << "table <" << Tokens[2] << "> doesn't exists" << endl;
                cout << "Table not dropped" << endl;
                return false;
            }
        }

        else if (Tokens[0] == "describe") {
            //Error 1 : Checking whether table with specified name exists in Schema file or not
            if (!tableChecker.doesTableExists(Tokens[1])) {
                cout << "table <" << Tokens[1] << "> doesn't exists" << endl;
                cout << "Table cannot be described" << endl;
                return false;
            }
        }

        else if (Tokens[0] == "insert" && Tokens[1] == "into") {
            //Error 1 : Checking whether table with specified name exists in Schema file or not
            if (!tableChecker.doesTableExists(Tokens[2])) {
                cout << "table <" << Tokens[2] << "> doesn't exists" << endl;
                cout << "Tuple not inserted" << endl;
                return false;
            }
            //Other errors handling done in insert function itself.
        }
        
        else if(Tokens[0]=="select")
        {
            //All errors are handled in Select function itself
        }

        else if(Tokens[0]=="help" && Tokens[1]=="tables")
        {

        }

        else if(Tokens[0]=="help")
        {}

        else if (Tokens[0] == "update") {
            if (!tableChecker.doesTableExists(Tokens[1])) {
                cout << "table <" << Tokens[1] << "> doesn't exists" << endl;
                cout << "0 rows affected" << endl;
                return false;
            }
        }
            else if(Tokens[0]=="quit")
        {}
        else
        {}
        return true;
    }
};