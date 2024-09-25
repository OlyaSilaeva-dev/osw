#ifndef OSW_DIALOG_H
#define OSW_DIALOG_H

#include "validation.h"
#include <iostream>

namespace dialog
{
    std::string dialog_commands[] =
            {
                    "ADD_DATA <pool_name> <scheme_name> <collection_name> <student_id> <student_name> <student_debts>",
                    "READ_DATA <pool_name> <scheme_name> <collection_name> <student_key>",
                    "READ_BETWEEN <pool_name> <scheme_name> <collection_name> <min student_key> <max student_key>",
                    "UPDATE <pool_name> <scheme_name> <collection_name> <student_key> <new student_name> <new student_debts>",
                    "DELETE_DATA <pool_name> <scheme_name> <collection_name> <student_key>",
                    "ADD_POOL <pool_name>",
                    "DELETE_POOL <pool_name>",
                    "ADD_SCHEME <pool_name> <scheme_name>",
                    "DELETE_SCHEME <pool_name> <scheme_name>",
                    "ADD_COLLECTION <pool_name> <scheme_name> <collection_name>",
                    "DELETE_COLLECTION <pool_name> <scheme_name> <collection_name>",
                    "INSERT_FILE",
                    "QUIT"
            };

    std::string start = "Welcome to the command center!\n";

    void interactive_dialog()
    {
        std::cout << "\nYou can choose these commands:\n\n";
        for (auto & item : dialog_commands) std::cout << item << std::endl;
    };

    std::string parse_file()
    {
        std::cin.sync();
        std::string file_name;
        std::cout << "ENTER FILE NAME HERE >> ";
        std::cin >> file_name;
        return file_name;
    }
};

#endif //OSW_DIALOG_H
