#ifndef OSW_COMMANDS_H
#define OSW_COMMANDS_H

#include "database.h"

namespace validation
{
    enum class command {
        UPDATE,
        ADD_PULL,
        ADD_SCHEME,
        ADD_COLLECTION,
        ADD_DATA,
        DELETE_PULL,
        DELETE_SCHEME,
        DELETE_COLLECTION,
        DELETE_DATA,
        READ_DATA,
        READ_DATA_BETWEEN,
    };

    int const commands_count = 11;

    std::string commands[] =
            {
            "UPDATE",
            "ADD_PULL",
            "ADD_SCHEME",
            "ADD_COLLECTION",
            "ADD_DATA",
            "DELETE_PULL",
            "DELETE_SCHEME",
            "DELETE_COLLECTION",
            "DELETE_DATA",
            "READ_DATA",
            "READ_DATA_BETWEEN",
            };

    bool is_dialog(const std::string& value) {
        if(!value.compare("--dialog")) return true;
        return false;
    }

    bool is_command(std::string value, command &_command) {
        for (auto i = 0; i < commands_count; ++i) {
            if (value.compare(commands[i]) == 0) {
                _command = (command)i;
                return true;
            }
        }
        return false;
    }

    bool is_id(std::string value, uint32_t &id) {
        for (auto & item : value) {
            if (!isdigit(item)) return false;
        }
        id = std::stoul(value);
        return true;
    }

    bool is_name(std::string value) {
        if (value.empty()) return false;
        for (auto & item : value) {
            if (!isalpha(item)) return false;
        }
        command _command;
        if (is_command(value, _command)) return false;
        return true;
    }
}



#endif //OSW_COMMANDS_H
