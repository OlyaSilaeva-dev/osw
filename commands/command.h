#ifndef OSW_COMMAND_H
#define OSW_COMMAND_H

#include <iostream>
#include <sstream>
#include <string>
#include <filesystem>
#include <fstream>

#include "database.h"
#include "student.h"
#include "validation.h"

namespace fs = std::filesystem;

namespace command_pattern {

    student read_student_from_file(const std::string& file_name)
    {
        std::ifstream file;
        file.open(file_name);
        std::string name;
        uint32_t debts, id;
        file >> id >> name >> debts;
        return student(id, name, debts);
    }

    class command
    {

    protected:

        database<std::string, std::string, student> * _database;

        std::uint64_t _time_activity_should_be_finished;

    public:

        [[nodiscard]] std::int64_t get_time_and_date() const
        {
            return _time_activity_should_be_finished;
        }

    public:

        // передаем указатель на объект, который содержит саму базу данных (он уже должен быть создан)
        explicit command(database<std::string, std::string, student> * database, std::uint64_t const & _time = 0)
        {
            _time_activity_should_be_finished = _time;
            _database = database;
        }

    private:

        void clear()
        {
            _database = nullptr;
        }

    public:

        ~command()
        {
            clear();
        }

        command(command const & other)
        {
            clear();
            _database = other._database;
        }

        command &operator=(command const & other)
        {
            if (this != &other)
            {
                clear();
                _database = other._database;
            }
            return *this;
        }

        command(command && other) noexcept
        {
            clear();
            _database = other._database;
            other.clear();
        }

        command &operator=(command && other) noexcept {
            if (this != &other)
            {
                clear();
                _database = other._database;
                other.clear();
            }
            return *this;
        }

    public:

        virtual bool can_execute(std::string const&, std::uint64_t const & current_time = 0) = 0;

        virtual void execute(std::string const&, std::uint64_t const & current_time = 0) = 0;
    };

    class add_pool final:
            public command
    {

    private:

        std::string _pool_name;

    public:

        explicit add_pool(database<std::string, std::string, student> * database) : command(database) {}

    public:

        bool can_execute(std::string const & request, std::uint64_t const & current_time = 0) override
        {
            std::istringstream string_with_commands(request);
            std::string command;
            string_with_commands >> command;

            if (command == "ADD_POOL")
            {
                std::string pool_name;
                string_with_commands >> pool_name;
                if (!validation::is_name(pool_name)) return false;
                _pool_name = pool_name;
                return true;
            }
            return false;
        }

        void execute(std::string const& request, std::uint64_t const & current_time = 0)  override
        {
            if (_database->get_mode() == mode::IN_MEMORY_CACHE)
            {
                _database->add_pool(_pool_name);
                return;
            }

            std::string new_dir = "storage/" + _pool_name;
            std::filesystem::path path(new_dir);
            if (fs::exists(path))
            {
                std::string error = "Pool with name - " + _pool_name + " already exists\n";
                throw std::logic_error(error);
            }

            fs::create_directories(new_dir);
        }
    };

    class delete_pool final:
            public command {

    private:

        std::string _pool_name;

    public:

        delete_pool(database<std::string, std::string, student> * database) : command(database) {}

    public:

        bool can_execute(std::string const & request, std::uint64_t const & current_time = 0)  override {
            std::istringstream string_with_commands(request);
            std::string command;
            string_with_commands >> command;

            if (command == "DELETE_POOL")
            {
                std::string pool_name;
                string_with_commands >> pool_name;
                if (!validation::is_name(pool_name)) return false;
                _pool_name = pool_name;
                return true;
            }
            return false;
        }

        void execute(std::string const& request, std::uint64_t const & current_time = 0)  override
        {
            if (_database->get_mode() == mode::IN_MEMORY_CACHE)
            {
                _database->delete_pool(_pool_name);
                return;
            }

            std::string pool_str = "storage/" + _pool_name;
            fs::path pool_path(pool_str);
            if (!fs::exists(pool_path))
            {
                std::string error = "Pool with name - " + _pool_name + " doesn't exist\n";
                throw std::logic_error(error);
            }

            fs::remove_all(pool_str);
        }
    };


    class add_scheme final:
            public command
    {

    private:

        std::string _pool_name;

        std::string _scheme_name;

    public:

        add_scheme(database<std::string, std::string, student> * database) : command(database) {}

    public:

        bool can_execute(std::string const & request, std::uint64_t const & current_time = 0)  override
        {
            std::istringstream string_with_commands(request);
            std::string command;
            string_with_commands >> command;

            if (command == "ADD_SCHEME")
            {
                std::string pool_name;
                std::string scheme_name;
                string_with_commands >> pool_name >> scheme_name;
                if (!validation::is_name(pool_name) || !validation::is_name(scheme_name)) return false;
                _pool_name = pool_name;
                _scheme_name = scheme_name;
                return true;
            }
            return false;
        }

        void execute(std::string const& request, std::uint64_t const & current_time = 0)  override {

            if (_database->get_mode() == mode::IN_MEMORY_CACHE)
            {
                _database->add_scheme(_pool_name, _scheme_name);
                return;
            }

            std::string pool_path_str = "storage/" + _pool_name;
            fs::path pool_path(pool_path_str);
            if (!fs::exists(pool_path))
            {
                std::string error = "Pool with name - " + _pool_name + " doesn't exist\n";
                throw std::logic_error(error);
            }

            std::string scheme_path_str = pool_path_str + "/" + _scheme_name;
            fs::path scheme_path(scheme_path_str);
            if (fs::exists(scheme_path)) {
                std::string error = "Scheme with name - " + _pool_name + " " + _scheme_name + " already exists\n";
                throw std::logic_error(error);
            }

            fs::create_directories(scheme_path_str);
        }
    };

    class delete_scheme final:
            public command
    {

    private:

        std::string _pool_name;

        std::string _scheme_name;

    public:

        delete_scheme(database<std::string, std::string, student> * database) : command(database) {}

    public:

        bool can_execute(std::string const & request, std::uint64_t const & current_time = 0)  override
        {
            std::istringstream string_with_commands(request);
            std::string command;
            string_with_commands >> command;

            if (command == "DELETE_SCHEME")
            {
                std::string pool_name;
                std::string scheme_name;
                string_with_commands >> pool_name >> scheme_name;
                if (!validation::is_name(pool_name) || !validation::is_name(scheme_name)) return false;
                _pool_name = pool_name;
                _scheme_name = scheme_name;
                return true;
            }
            return false;
        }

        void execute(std::string const& request, std::uint64_t const & current_time = 0)  override
        {
            if (_database->get_mode() == mode::IN_MEMORY_CACHE)
            {
                _database->delete_scheme(_pool_name, _scheme_name);
                return;
            }

            std::string pool_path_str = "storage/" + _pool_name;
            fs::path pool_path(pool_path_str);
            if (!fs::exists(pool_path))
            {
                std::string error = "Pool with name - " + _pool_name + " doesn't exist\n";
                throw std::logic_error(error);
            }

            std::string scheme_path_str = pool_path_str + "/" + _scheme_name;
            fs::path scheme_path(scheme_path_str);
            if (!fs::exists(scheme_path))
            {
                std::string error = "Scheme with name - " + _pool_name + " " + _scheme_name + " doesn't exist\n";
                throw std::logic_error(error);
            }

            fs::remove_all(scheme_path_str);
        }
    };

    class add_collection final:
            public command
    {

    private:

        std::string _pool_name;

        std::string _scheme_name;

        std::string _collection_name;

    public:

        explicit add_collection(database<std::string, std::string, student> * database) : command(database) {}

    public:

        bool can_execute(std::string const & request, std::uint64_t const & current_time = 0)  override
        {
            std::istringstream string_with_commands(request);
            std::string command;
            string_with_commands >> command;

            if (command == "ADD_COLLECTION")
            {
                std::string pool_name;
                std::string scheme_name;
                std::string collection_name;
                string_with_commands >> pool_name >> scheme_name >> collection_name;
                if (!validation::is_name(pool_name) || !validation::is_name(scheme_name) || !validation::is_name(collection_name)) return false;
                _pool_name = pool_name;
                _scheme_name = scheme_name;
                _collection_name = collection_name;
                return true;
            }
            return false;
        }

        void execute(std::string const& request, std::uint64_t const & current_time = 0)  override
        {
            if (_database->get_mode() == mode::IN_MEMORY_CACHE)
            {
                _database->add_collection(_pool_name, _scheme_name, _collection_name);
                return;
            }

            std::string pool_path_str = "storage/" + _pool_name;
            fs::path pool_path(pool_path_str);
            if (!fs::exists(pool_path))
            {
                std::string error = "Pool with name - " + _pool_name + " doesn't exist\n";
                throw std::logic_error(error);
            }

            std::string scheme_path_str = pool_path_str + "/" + _scheme_name;
            fs::path scheme_path(scheme_path_str);
            if (!fs::exists(scheme_path))
            {
                std::string error = "Scheme with name - " + _pool_name + " " + _scheme_name + " doesn't exist\n";
                throw std::logic_error(error);
            }

            std::string collection_path_str = scheme_path_str + "/" + _collection_name;
            fs::path collection_path(collection_path_str);
            if (fs::exists(collection_path))
            {
                std::string error = "Collection with name - " + _pool_name + " " + _scheme_name + " " + _collection_name + " already exist\n";
                throw std::logic_error(error);
            }

            fs::create_directories(collection_path_str);
        }
    };

    class delete_collection final:
            public command
    {

    private:

        std::string _pool_name;

        std::string _scheme_name;

        std::string _collection_name;

    public:

        explicit delete_collection(database<std::string, std::string, student> * database) : command(database) {}

    public:

        bool can_execute(std::string const & request, std::uint64_t const & current_time = 0)  override
        {
            std::istringstream string_with_commands(request);
            std::string command;
            string_with_commands >> command;

            if (command == "DELETE_COLLECTION")
            {
                std::string pool_name;
                std::string scheme_name;
                std::string collection_name;
                string_with_commands >> pool_name >> scheme_name >> collection_name;
                if (!validation::is_name(pool_name) || !validation::is_name(scheme_name) || !validation::is_name(collection_name)) return false;
                _pool_name = pool_name;
                _scheme_name = scheme_name;
                _collection_name = collection_name;
                return true;
            }
            return false;
        }

        void execute(std::string const& request, std::uint64_t const & current_time = 0)  override
        {
            if (_database->get_mode() == mode::IN_MEMORY_CACHE)
            {
                _database->delete_collection(_pool_name, _scheme_name, _collection_name);
                return;
            }

            std::string pool_path_str = "storage/" + _pool_name;
            fs::path pool_path(pool_path_str);
            if (!fs::exists(pool_path))
            {
                std::string error = "Pool with name - " + _pool_name + " doesn't exist\n";
                throw std::logic_error(error);
            }

            std::string scheme_path_str = pool_path_str + "/" + _scheme_name;
            fs::path scheme_path(scheme_path_str);
            if (!fs::exists(scheme_path))
            {
                std::string error = "Scheme with name - " + _pool_name + " " + _scheme_name + " doesn't exist\n";
                throw std::logic_error(error);
            }

            std::string collection_path_str = scheme_path_str + "/" + _collection_name;
            fs::path collection_path(collection_path_str);
            if (!fs::exists(collection_path))
            {
                std::string error = "Collection with name - " + _pool_name + " " + _scheme_name + " " + _collection_name + " doesn't exist\n";
                throw std::logic_error(error);
            }

            fs::remove_all(collection_path_str);
        }
    };

    class add_value final:
            public command
    {

    private:

        std::string _pool_name;

        std::string _scheme_name;

        std::string _collection_name;

        uint32_t _id;

        std::string _name;

        uint32_t _number_of_depts;

        std::string _key;

    public:

        add_value(database<std::string, std::string, student> * database, std::uint64_t const & _time = 0) : command(database, _time) {}

    public:

        bool can_execute(std::string const & request, std::uint64_t const & current_time = 0)  override
        {
            std::istringstream string_with_commands(request);
            std::string command;
            string_with_commands >> command;

            if (command == "ADD_DATA")
            {
                if (current_time != 0 && this->get_time_and_date() < current_time) return false;
                std::string pool_name;
                std::string scheme_name;
                std::string collection_name;
                std::string id_str, name, number_of_depts_str;
                uint32_t id, number_of_depts;
                string_with_commands >> pool_name >> scheme_name >> collection_name >> id_str >> name >> number_of_depts_str;
                if (!validation::is_name(pool_name) || !validation::is_name(scheme_name) || !validation::is_name(collection_name)) return false;
                if (!validation::is_id(id_str, id) || !validation::is_name(name) || !validation::is_id(number_of_depts_str, number_of_depts)) return false;
                _pool_name = pool_name;
                _scheme_name = scheme_name;
                _collection_name = collection_name;
                _id = id;
                _number_of_depts = number_of_depts;
                _name = name;

                switch (_database->get_key_type())
                {
                    case key_type::ID:
                        _key = id_str;
                        break;

                    case key_type::DEBTS:
                        _key = number_of_depts_str;
                        break;

                    case key_type::NAME:
                        _key = name;
                        break;

                    default:
                        break;
                }
                return true;
            }
            return false;
        }

        void execute(std::string const& request, std::uint64_t const & current_time = 0)  override
        {
            if (_database->get_mode() == mode::IN_MEMORY_CACHE)
            {
                _database->add_value(_pool_name, _scheme_name, _collection_name, _key, _id, _name, _number_of_depts);
                return;
            }

            std::string pool_path_str = "storage/" + _pool_name;
            fs::path pool_path(pool_path_str);
            if (!fs::exists(pool_path))
            {
                std::string error = "Pool with name - " + _pool_name + " doesn't exist\n";
                throw std::logic_error(error);
            }

            std::string scheme_path_str = pool_path_str + "/" + _scheme_name;
            fs::path scheme_path(scheme_path_str);
            if (!fs::exists(scheme_path))
            {
                std::string error = "Scheme with name - " + _pool_name + " " + _scheme_name + " doesn't exist\n";
                throw std::logic_error(error);
            }

            std::string collection_path_str = scheme_path_str + "/" + _collection_name;
            fs::path collection_path(collection_path_str);
            if (!fs::exists(collection_path))
            {
                std::string error = "Collection with name - " + _pool_name + " " + _scheme_name + " " + _collection_name + " doesn't exist\n";
                throw std::logic_error(error);
            }

            std::string student_path_str = collection_path_str + "/" + _key;
            fs::path student_path(student_path_str);
            if (fs::exists(student_path))
            {
                std::string error = "Student in " + _pool_name + " " + _scheme_name + " " + _collection_name + " with key - " + _key +  "already exists\n";
                throw std::logic_error(error);
            }

            std::ofstream file(student_path_str);
            file << _id << " " << _name << " " << _number_of_depts;
        }
    };

    class delete_value final:
            public command
    {

    private:

        std::string _pool_name;

        std::string _scheme_name;

        std::string _collection_name;

        std::string _key;

    public:

        delete_value(database<std::string, std::string, student> * database, std::uint64_t const & _time = 0) : command(database, _time)
        {}

    public:

        bool can_execute(std::string const & request, std::uint64_t const & current_time = 0)  override
        {
            std::istringstream string_with_commands(request);
            std::string command;
            string_with_commands >> command;

            if (command == "DELETE_DATA")
            {
                if (current_time != 0 && this->get_time_and_date() < current_time) return false;
                std::string pool_name;
                std::string scheme_name;
                std::string collection_name;
                std::string key;
                uint32_t tmp;
                string_with_commands >> pool_name >> scheme_name >> collection_name >> key;
                if (!validation::is_name(pool_name) || !validation::is_name(scheme_name) || !validation::is_name(collection_name)) return false;
                switch (_database->get_key_type())
                {
                    case key_type::ID:
                        if (!validation::is_id(key, tmp)) return false;
                        break;

                    case key_type::DEBTS:
                        if (!validation::is_id(key, tmp)) return false;
                        break;

                    case key_type::NAME:
                        if (!validation::is_name(key)) return false;
                        break;

                    default:
                        break;
                }
                _pool_name = pool_name;
                _scheme_name = scheme_name;
                _collection_name = collection_name;
                _key = key;
                return true;
            }
            return false;
        }

        void execute(std::string const& request, std::uint64_t const & current_time = 0)  override
        {
            if (_database->get_mode() == mode::IN_MEMORY_CACHE)
            {
                _database->delete_value(_pool_name, _scheme_name, _collection_name, _key);
                return;
            }

            std::string pool_path_str = "storage/" + _pool_name;
            fs::path pool_path(pool_path_str);
            if (!fs::exists(pool_path))
            {
                std::string error = "Pool with name - " + _pool_name + " doesn't exist\n";
                throw std::logic_error(error);
            }

            std::string scheme_path_str = pool_path_str + "/" + _scheme_name;
            fs::path scheme_path(scheme_path_str);
            if (!fs::exists(scheme_path))
            {
                std::string error = "Scheme with name - " + _pool_name + " " + _scheme_name + " doesn't exist\n";
                throw std::logic_error(error);
            }

            std::string collection_path_str = scheme_path_str + "/" + _collection_name;
            fs::path collection_path(collection_path_str);
            if (!fs::exists(collection_path))
            {
                std::string error = "Collection with name - " + _pool_name + " " + _scheme_name + " " + _collection_name + " doesn't exist\n";
                throw std::logic_error(error);
            }

            std::string student_path_str = scheme_path_str + "/" + _key;
            fs::path customer_path(collection_path_str);
            if (!fs::exists(customer_path))
            {
                std::string error = "Student in " + _pool_name + " " + _scheme_name + " " + _collection_name + " with key - " + _key +  "doesn't exist\n";
                throw std::logic_error(error);
            }

            fs::create_directory(student_path_str);
        }
    };

    class read_between_value final:
            public command
    {

    private:

        std::string _pool_name;

        std::string _scheme_name;

        std::string _collection_name;

        std::string _min_key;

        std::string _max_key;

    public:

        explicit read_between_value(database<std::string, std::string, student> * database, std::uint64_t const & _time = 0) : command(database, _time) {}

    public:

        bool can_execute(std::string const & request, std::uint64_t const & current_time = 0)  override;

        void execute(std::string const& request, std::uint64_t const & current_time = 0)  override
        {
            if (_database->get_mode() == mode::IN_MEMORY_CACHE)
            {
                auto res = _database->read_value_between(_pool_name, _scheme_name, _collection_name, _min_key, _max_key);
                std::cout << "\nValues with key from " << _min_key << " to " << _max_key << std::endl;
                for (auto & item : res) std::cout << "id: " << item.value.get_id() << ", name: " << item.value.get_name() << ", balance: " << item.value.get_debts_number() << std::endl;
                return;
            }

            std::string pool_path_str = "storage/" + _pool_name;
            fs::path pool_path(pool_path_str);
            if (!fs::exists(pool_path))
            {
                std::string error = "Pool with name - " + _pool_name + " doesn't exist\n";
                throw std::logic_error(error);
            }

            std::string scheme_path_str = pool_path_str + "/" + _scheme_name;
            fs::path scheme_path(scheme_path_str);
            if (!fs::exists(scheme_path))
            {
                std::string error = "Scheme with name - " + _pool_name + " " + _scheme_name + " doesn't exist\n";
                throw std::logic_error(error);
            }

            std::string collection_path_str = scheme_path_str + "/" + _collection_name;
            fs::path collection_path(collection_path_str);
            if (!fs::exists(collection_path))
            {
                std::string error = "Collection with name - " + _pool_name + " " + _scheme_name + " " + _collection_name + " doesn't exist\n";
                throw std::logic_error(error);
            }

            fs::directory_iterator begin("collection_path");
            fs::directory_iterator end;
            std::cout << "\nValues with key from " << _min_key << " to " << _max_key << std::endl;
            for (; begin != end; ++ begin)
            {
                if (begin->path().string() >= (_min_key + ".txt") &&
                    begin->path().string() <=  (_max_key + ".txt"))
                {
                    std::string file_name = (begin->path()).string();
                    student the_student = read_student_from_file(file_name);
                    std::cout << "id: " << the_student.get_id() << ", name: " << the_student.get_name() << ", balance: " << the_student.get_debts_number() << std::endl;
                }
            }
        }
    };

    bool read_between_value::can_execute(const std::string &request, const uint64_t &current_time) {
        std::istringstream string_with_commands(request);
        std::string command;
        string_with_commands >> command;

        if (command == "READ_DATA_BETWEEN")
        {
            if (current_time != 0 && this->get_time_and_date() < current_time) return false;
            std::string pool_name;
            std::string scheme_name;
            std::string collection_name;
            std::string min_key, max_key;
            uint32_t tmp;
            string_with_commands >> pool_name >> scheme_name >> collection_name >> min_key >> max_key;
            if (!validation::is_name(pool_name) || !validation::is_name(scheme_name) || !validation::is_name(collection_name)) return false;
            switch (_database->get_key_type())
            {
                case key_type::ID:
                    if (!validation::is_id(min_key, tmp)) return false;
                    if (!validation::is_id(max_key, tmp)) return false;
                    break;

                case key_type::DEBTS:
                    if (!validation::is_id(min_key, tmp)) return false;
                    if (!validation::is_id(max_key, tmp)) return false;
                    break;

                case key_type::NAME:
                    if (!validation::is_name(min_key)) return false;
                    if (!validation::is_name(max_key)) return false;
                    break;

                default:
                    break;
            }
            _pool_name = pool_name;
            _scheme_name = scheme_name;
            _collection_name = collection_name;
            _min_key = min_key;
            _max_key = max_key;
            return true;
        }
        return false;
    }

    class read_value final:
            public command
    {

    private:

        std::string _pool_name;

        std::string _scheme_name;

        std::string _collection_name;

        std::string _key;

    public:

        read_value(database<std::string, std::string, student> * database, std::uint64_t const & _time = 0) : command(database, _time)
        {}

    public:

        bool can_execute(std::string const & request, std::uint64_t const & current_time = 0)  override
        {
            std::istringstream string_with_commands(request);
            std::string command;
            string_with_commands >> command;

            if (command == "READ_DATA")
            {
                if (current_time != 0 && this->get_time_and_date() < current_time) return false;
                std::string pool_name;
                std::string scheme_name;
                std::string collection_name;
                std::string key;
                uint32_t tmp;
                string_with_commands >> pool_name >> scheme_name >> collection_name >> key;
                if (!validation::is_name(pool_name) || !validation::is_name(scheme_name) || !validation::is_name(collection_name)) return false;
                switch (_database->get_key_type())
                {
                    case key_type::ID:
                        if (!validation::is_id(key, tmp)) return false;
                        break;

                    case key_type::DEBTS:
                        if (!validation::is_id(key, tmp)) return false;
                        break;

                    case key_type::NAME:
                        if (!validation::is_name(key)) return false;
                        break;

                    default:
                        break;
                }
                _pool_name = pool_name;
                _scheme_name = scheme_name;
                _collection_name = collection_name;
                _key = key;
                return true;
            }
            return false;
        }

        void execute(std::string const& request, std::uint64_t const & current_time = 0)  override
        {
            if (_database->get_mode() == mode::IN_MEMORY_CACHE)
            {
                auto res = _database->read_value(_pool_name, _scheme_name, _collection_name, _key);
                std::cout << "\nValue with key  " << _key << std::endl;
                std::cout << "id: " << res.get_id() << ", name: " << res.get_name() << ", debts: " << res.get_debts_number() << std::endl;
                return;
            }

            std::string pool_path_str = "storage/" + _pool_name;
            fs::path pool_path(pool_path_str);
            if (!fs::exists(pool_path))
            {
                std::string error = "Pool with name - " + _pool_name + " doesn't exist\n";
                throw std::logic_error(error);
            }

            std::string scheme_path_str = pool_path_str + "/" + _scheme_name;
            fs::path scheme_path(scheme_path_str);
            if (!fs::exists(scheme_path))
            {
                std::string error = "Scheme with name - " + _pool_name + " " + _scheme_name + " doesn't exist\n";
                throw std::logic_error(error);
            }

            std::string collection_path_str = scheme_path_str + "/" + _collection_name;
            fs::path collection_path(collection_path_str);
            if (!fs::exists(collection_path))
            {
                std::string error = "Collection with name - " + _pool_name + " " + _scheme_name + " " + _collection_name + " doesn't exist\n";
                throw std::logic_error(error);
            }

            std::string file_name = collection_path_str + "/" + _key;
            fs::path file_path(file_name);
            if (!fs::exists(file_path))
            {
                std::string error = "Student with key - " + _key + " doesn't exist\n";
                throw std::logic_error(error);
            }
            std::cout << "\nValue with key  " << _key << std::endl;
            student the_student = read_student_from_file(file_name);
            std::cout << "id: " << the_student.get_id() << ", name: " << the_student.get_name() << ", debts_number: " << the_student.get_debts_number() << std::endl;
        }
    };

    class update_value final:
            public command
    {

    private:

        std::string _pool_name;

        std::string _scheme_name;

        std::string _collection_name;

        std::string _key;

        std::string _new_name;

        std::string _new_number_debts_str;

        uint32_t _new_number_debts;

    public:

        update_value(database<std::string, std::string, student> * database, std::uint64_t const & _time = 0) : command(database, _time) {}

    public:

        bool can_execute(std::string const & request, std::uint64_t const & current_time = 0) override
        {
            std::istringstream string_with_commands(request);
            std::string command;
            string_with_commands >> command;

            if (command == "UPDATE")
            {
                if (current_time != 0 && this->get_time_and_date() < current_time) return false;
                std::string pool_name;
                std::string scheme_name;
                std::string collection_name;
                std::string key;
                std::string new_name, new_number_debts;
                uint32_t tmp, number_debts;
                string_with_commands >> pool_name >> scheme_name >> collection_name >> key >> new_name >> new_number_debts;
                if (!validation::is_name(pool_name) || !validation::is_name(scheme_name) || !validation::is_name(collection_name)) return false;
                if (!validation::is_name(new_name) || !validation::is_id(new_number_debts, number_debts)) return false;

                switch (_database->get_key_type())
                {
                    case key_type::ID:
                        if (!validation::is_id(key, tmp)) return false;
                        break;

                    case key_type::DEBTS:
                        if (!validation::is_id(key, tmp)) return false;
                        break;

                    case key_type::NAME:
                        if (!validation::is_name(key)) return false;
                        break;

                    default:
                        break;
                }
                _pool_name = pool_name;
                _scheme_name = scheme_name;
                _collection_name = collection_name;
                _key = key;
                _new_name = new_name;
                _new_number_debts_str = new_number_debts;
                _new_number_debts = number_debts;
                return true;
            }
            return false;
        }

        void execute(std::string const& request, std::uint64_t const & current_time = 0) override
        {
            if (_database->get_mode() == mode::IN_MEMORY_CACHE)
            {
                auto res = _database->delete_value(_pool_name, _scheme_name, _collection_name, _key);
                switch (_database->get_key_type())
                {
                    case key_type::DEBTS:
                        _key = _new_number_debts_str;
                        break;

                    case key_type::NAME:
                        _key = _new_name;
                        break;

                    default:
                        break;

                }
                _database->add_value(_pool_name, _scheme_name, _collection_name, _key, res.get_id(), _new_name, _new_number_debts);
                return;
            }

            std::string pool_path_str = "storage/" + _pool_name;
            fs::path pool_path(pool_path_str);
            if (!fs::exists(pool_path))
            {
                std::string error = "Pool with name - " + _pool_name + " doesn't exist\n";
                throw std::logic_error(error);
            }

            std::string scheme_path_str = pool_path_str + "/" + _scheme_name;
            fs::path scheme_path(scheme_path_str);
            if (!fs::exists(scheme_path))
            {
                std::string error = "Scheme with name - " + _pool_name + " " + _scheme_name + " doesn't exist\n";
                throw std::logic_error(error);
            }

            std::string collection_path_str = scheme_path_str + "/" + _collection_name;
            fs::path collection_path(collection_path_str);
            if (!fs::exists(collection_path))
            {
                std::string error = "Collection with name - " + _pool_name + " " + _scheme_name + " " + _collection_name + " doesn't exist\n";
                throw std::logic_error(error);
            }

            std::string file_name = collection_path_str + "/" + _key;
            fs::path file_path(file_name);
            if (!fs::exists(file_path))
            {
                std::string error = "Student with key - " + _key + " doesn't exist\n";
                throw std::logic_error(error);
            }
            auto res = read_student_from_file(file_name);
            fs::remove_all(file_path);
            switch (_database->get_key_type())
            {
                case key_type::DEBTS:
                    _key = _new_number_debts_str;
                    break;

                case key_type::NAME:
                    _key = _new_name;
                    break;

                default:
                    break;
            }

            file_name = collection_path_str + "/" + _key;

            std::ofstream file(file_name);
            file << res.get_id() << " " << _new_name << " " << _new_number_debts;
        }
    };
};

#endif //OSW_COMMAND_H
