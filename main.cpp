#include "dbcontrol.h"
#include <sstream>

int main()
{
    DBcontrol dbc;
    std::cout << "Hello! Available commands:" << std::endl;
    dbc.printHelp();
    while (true)
    {
        std::cout << "main menu > ";
        std::string command;
        getline(std::cin, command);
        if (command.empty());
        else if (command == "quit")
        {
            break;
        }
        else
        {
            dbc.execute(command);
        }
    }
    return 0;
}
