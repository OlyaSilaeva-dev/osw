#ifndef OSW_STUDENT_H
#define OSW_STUDENT_H

#include <string>
#include <utility>

class student
{ 

private:

    class debts
    {
    
    public:

        enum debt_type
        {
            Exam,
            Test,
            Labs,
            Coursework
        };

    private:

        uint32_t _debts_number;

        debt_type _type;

    public:

        uint32_t get_debts_number() const noexcept
        {
            return _debts_number;
        }

        std::string get_depts_type() const noexcept
        {
            switch (_type)
            {
                case Exam:
                    return "Exam";
                case Test:
                    return "Test";
                case Labs:
                    return "Labs";
                case Coursework:
                    return "Coursework";
                default:
                    return "Unknown_type";
            }
        }
    
    public:

        debts(uint32_t debts_number, debt_type type = Exam) : _debts_number(debts_number), _type(type) {}

        ~debts() {}

        debts(debts const & other) : _debts_number(other._debts_number), _type(other._type) {}

        debts &operator=(debts const & other)
        {
            if (this == &other) return *this;
            _debts_number = other._debts_number;
            _type = other._type;
            return *this;
        }

        debts(debts && other) noexcept  : _debts_number(other._debts_number), _type(other._type) {}

        debts &operator=(debts && other) noexcept
        {
            if (this == &other) return *this;
            _debts_number = other._debts_number;
            _type = other._type;
            return *this;
        }
    };

private:

    uint32_t _id;

    std::string _name;

    debts _debts;

public:

    uint32_t get_id() const noexcept
    {
        return _id;
    }

    std::string get_name() const noexcept
    {
        return _name;
    }

    uint32_t get_debts_number() const noexcept
    {
        return _debts.get_debts_number();
    }

    std::string get_depts_type() const noexcept
    {
        return _debts.get_depts_type();
    }
    
public:

    student(uint32_t const & id, std::string name, uint32_t const & debts_number = 0, debts::debt_type type = debts::debt_type::Exam):
            _debts(debts_number, type), _name(std::move(name)), _id(id)
    {}

    ~student() {}

    student(student const & other) : _name(other._name), _debts(other._debts), _id(other._id) {}

    student &operator=(student const & other)
    {
        if (this == &other) return *this;
        _name = other._name;
        _debts = other._debts;
        _id = other._id;
        return *this;
    }

    student(student && other) noexcept  : _name(std::move(other._name)), _debts(std::move(other._debts)), _id(other._id) {}

    student &operator=(student && other) noexcept
    {
        if (this == &other) return *this;
        _name = std::move(other._name);
        _debts = std::move(other._debts);
        _id = other._id;
        return *this;
    }
};

#endif