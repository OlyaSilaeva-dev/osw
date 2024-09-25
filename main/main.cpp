#include "handler.h"

int main(int argc, char * argv[])//<command file or --dialog> <mode> <index>
{
    std::string error = "Invalid command line arguments. Try again with <command file or --dialog> <mode> <index>;\n<mode> is --memory-cache or --file-system\n<index> is --id, --name, --debts or nothing\n";
    if ((argc != 3) && (argc != 4))
    {
        std::cerr << error << std::endl;
        return -1;
    }
    std::string dialog;
    bool is_dialog = false;

    fs::current_path("/mnt/c/src/GitHub/osw");
//    fs::current_path("C:/src/GitHub/osw");

    std::string file_name = "test.txt";
    file_name.assign(argv[1]);

    std::string mode = "--file-system";
    mode.assign(argv[2]);

    if (validation::is_dialog(file_name))
        is_dialog = true;

    key_type type = key_type::NAME;
    if (argc == 4)
    {
        std::string index;
        index.assign(argv[3]);
        if (!index.compare("--name")) type = key_type::NAME;
        else if (!index.compare("--debts")) type = key_type::DEBTS;
        else if (!index.compare("--id")) type = key_type::ID;
        else
        {
            std::cerr << "error";
            return -1;
        }
    }

    if (!mode.compare("--memory-cache"))
    {
        try
        {
            program::program(file_name, mode::IN_MEMORY_CACHE, type, is_dialog);
        }
        catch(const std::logic_error& e)
        {
            std::cerr << "\n\n" << e.what() << "\n\n";
            return -1;
        }
    }
    else if (!mode.compare("--file-system"))
    {
        try
        {
            program::program(file_name, mode::FILE_SYSTEM, type, is_dialog);
        }
        catch(const std::logic_error& e)
        {
            std::cerr << "\n\n" << e.what() << "\n\n";
            return -1;
        }
    }
    else
    {
        std::cerr << error;
        return -1;
    }

    std::cout << "Program has finished correcly\n";
    return 0;
}
