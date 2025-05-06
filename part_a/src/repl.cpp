#include <iostream>
#include <sstream>
#include <string>
#include "StorageEngine/lsmtree.h"

using namespace std;

/**
 * @brief Runs a Read-Eval-Print Loop (REPL) for the key-value store.
 *
 * Allows users to interact with an LSMTree-backed key-value store
 * using commands: SET, GET, and DEL.
 * Type 'EXIT' to quit the REPL.
 */
void runREPL()
{
    LSMTree store("/sstabledata");

    cout << "Welcome to the Key-Value Store REPL. Supported commands: SET, GET, DEL.\n";
    cout << "Type 'EXIT' to quit.\n";

    string line;
    while (true)
    {
        cout << "User> ";
        getline(cin, line);

        if (line == "EXIT" || line == "exit")
        {
            break;
        }

        stringstream ss(line);
        string command, key, value;
        ss >> command;

        if (command == "SET" || command == "set")
        {
            ss >> key >> value;
            if (key.empty() || value.empty())
            {
                cout << "Invalid SET command. Usage: SET <key> <value>\n";
                continue;
            }
            store.set(key, value);
            cout << "OK\n";
        }
        else if (command == "GET" || command == "get")
        {
            ss >> key;
            if (key.empty())
            {
                cout << "Invalid GET command. Usage: GET <key>\n";
                continue;
            }
            string result = store.get(key);
            result = (result == "DELETED") ? "NULL" : result;
            cout << result << "\n";
        }
        else if (command == "DEL" || command == "del")
        {
            ss >> key;
            if (key.empty())
            {
                cout << "Invalid DEL command. Usage: DEL <key>\n";
                continue;
            }
            store.remove(key);
            cout << "Deleted\n";
        }
        else
        {
            cout << "Unknown command. Supported commands: SET, GET, DEL.\n";
        }
    }

    cout << "Exiting REPL. Goodbye!\n";
}

/**
 * @brief Main function to start the REPL.
 *
 * @return Exit status code.
 */
int main()
{
    runREPL();
    return 0;
}
