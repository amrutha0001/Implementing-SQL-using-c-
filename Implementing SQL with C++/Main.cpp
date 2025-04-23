#include <iostream>
#include <fstream>
#include <vector>
#include <cstring>
#include <set>
#include <algorithm>
#include "1.cpp"
#include "HelpCommands.cpp" 
#include "ErrorHandling.cpp"
using namespace std;

class QueryParser {
private:
    set<string> keywords;
    vector<string> Tokens;

public:
    QueryParser() {
        keywords = {
            "create", "table", "primary", "key",
            "int", "varchar", "date", "decimal",
            "drop",
            "describe",
            "insert", "into", "values",
            "help", "tables",
            "select", "from", "where", "and", "or",
            "update",
            "delete",
            "quit"
        };
    }

    void parseIntoTokens(string Query) {
        string temp = "";

        for (int i = 0; i < Query.size(); i++) {
            char c = Query[i];

            if (c == '"') {
                i++;
                while (Query[i] != '"') {
                    temp += Query[i];
                    i++;
                }

                if (temp != "")
                    Tokens.push_back(temp);
                temp = "";
            } else if (c == ' ' || c == '(' || c == ')' || c == ',' || c == ';' || c == '*') {
                if (temp != "")
                    Tokens.push_back(temp);
                if (c == '*') // for handling special case in select
                    Tokens.push_back("*");
                temp = "";
            } else if (Query[i] == '!' && Query[i + 1] == '=') {
                if (temp != "")
                    Tokens.push_back(temp);
                temp = "";

                Tokens.push_back("!=");
                i++;
            } else if (c == '<' || c == '>' || c == '=') { // OR other operators
                if (temp != "")
                    Tokens.push_back(temp);
                temp = "";

                Tokens.push_back(string(1, c));
            } else {
                temp += c;
            }
        }
        if (temp != "")
            Tokens.push_back(temp);
    }

    vector<string> getTokens() const {
        return Tokens;
    }

    void convertIntoSmallerCase() {
        for (int i = 0; i < Tokens.size(); i++) {
            string x = Tokens[i];
            transform(x.begin(), x.end(), x.begin(), ::tolower);
            if (keywords.find(x) != keywords.end())
                Tokens[i] = x;
        }
    }
};

class QueryExecutor {
public:
    static void execute(vector<string> Tokens) {
        if (Tokens.empty())
            return;

        if (Tokens[0] == "create" && Tokens[1] == "table") {
            CreateTable(Tokens);
        } else if (Tokens[0] == "drop" && Tokens[1] == "table") {
            DropTable(Tokens);
        } else if (Tokens[0] == "describe") {
            DescribeTable(Tokens);
        } else if (Tokens[0] == "help" && Tokens[1] == "tables") {
            HelpTablesCommand helpTables; // Instantiate HelpTablesCommand
            helpTables.displayHelp(); // Display help for tables
        } else if (Tokens[0] == "help") {
            HelpSpecificCommand helpSpecific(Tokens[1]); // Instantiate HelpSpecificCommand with the command name
            helpSpecific.displayHelp(); // Display specific help for the command
        } else if (Tokens[0] == "insert" && Tokens[1] == "into") {
            int res = InsertInto(Tokens);
            res == 1 ? cout << "Tuple inserted successfully" << endl : cout << "Tuple not inserted" << endl;
        } else if (Tokens[0] == "select") {
            Select(Tokens);
        } else if (Tokens[0] == "update") {
            UpdateTable(Tokens);
        } else if (Tokens[0] == "delete" && Tokens[1] == "from") {
            DeleteFrom(Tokens);
        } else if (Tokens[0] == "quit") {
            cout << "Program terminated successfully." << endl;
            exit(0);
        } else {
            cout << "INVALID QUERY" << endl;
        }
    }
};


int main() {
    system("cls");
    string Query;

    while (true) {
        QueryParser parser;
        cout << endl << ">> ";

        getline(cin, Query);
        cout << endl;

        if (Query.back() != ';') {
            cout << "; missing at the end" << endl;
            continue;
        }

        parser.parseIntoTokens(Query);
        parser.convertIntoSmallerCase();
        auto Tokens = parser.getTokens();
        ErrorChecker errorcheck;
        bool noerrors = errorcheck.ErrorsChecking(Tokens);

        if (noerrors)
            QueryExecutor::execute(Tokens);
    }

    return 0;
}