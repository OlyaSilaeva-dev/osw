#include <fstream>
#include "client_logger.h"
#include <memory>

static std::unordered_map<std::string, std::pair<std::unique_ptr<std::ofstream>, size_t>> _all_streams;

client_logger::client_logger(const client_logger &other) {
    this->_format_string = other._format_string;
    this->_streams = other._streams;
    this->add_to_all_streams();
}

client_logger::client_logger(const client_logger &other, const std::string &format_string): _format_string(format_string)
{
    this->_streams = other._streams;
    this->add_to_all_streams();
}

client_logger &client_logger::operator=(const client_logger &other)
        {
    if (this != &other)
    {
        this->clear_object();
        this->_streams = other._streams;
        this->add_to_all_streams();
    }
    return *this;
}

client_logger::client_logger(client_logger &&other) noexcept
{
    this->_streams = std::move(other._streams);
}

client_logger &client_logger::operator=(client_logger &&other) noexcept
        {
    if (this != &other)
    {
        this->_streams = std::move(other._streams);
    }
    return *this;
}

client_logger::~client_logger() noexcept {
    this->clear_object();
}

void client_logger::add_to_all_streams()
{
    for (auto& pair: this->_streams)
    {
        ++_all_streams.find(pair.first)->second.second;
    }
}

void client_logger::clear_object()
{
    for (auto& pair: _streams)
    {
        auto it = _all_streams.find(pair.first);
        if (it != _all_streams.end() && --it->second.second) {
            it->second.first->close();
//            delete tmp.first;
            _all_streams.erase(it);
        }
    }
}

void client_logger::insert_in_stream(const std::string &path, std::set<logger::severity> severity_set)
{
    auto stream = std::make_unique<std::ofstream>(path);
    if (stream->is_open()) {
        this->_streams.emplace(path, std::make_pair(stream.get(), std::move(severity_set)));

        auto it = _all_streams.find(path);
        if (it != _all_streams.end())
        {
            ++it->second.second;
        }
        else
        {
            _all_streams.emplace(path, std::make_pair(std::move(stream), 1));
        }
    }
}

logger const *client_logger::log(const std::string &text, logger::severity severity) const noexcept
{
    for (const auto &entry : _streams)
    {
        if (entry.second.second.find(severity) != entry.second.second.end())
        {
            std::string path_of_log_file = entry.first;

            std::ostream* out;

            if (path_of_log_file.empty()) {
                out = &std::cout;
            } else {
                auto it = _all_streams.find(path_of_log_file);
                if (it != _all_streams.end() && it->second.first->is_open()) {
                    out = it->second.first.get();
                } else {
                    continue; // Skip if the stream is not found or not open
                }
            }

            for(int i = 0; _format_string[i] != '\0' && _format_string[i + 1] != '\0'; ++i)
            {
                if(_format_string[i] == '%' && _format_string[i + 1] == 'd')
                {
                    *out << "[" << logger::current_date_to_string() << "] ";
                }
                else if(_format_string[i] == '%' && _format_string[i + 1] == 't')
                {
                    *out << "[" << logger::current_time_to_string() << "] ";
                }
                else if(_format_string[i] == '%' && _format_string[i + 1] == 's')
                {
                    *out << "[" << logger::severity_to_string(severity) << "] ";
                }
                else if(_format_string[i] == '%' && _format_string[i + 1] == 'm')
                {
                    *out << text << ' ';
                }
            }
            *out << std::endl;
        }
    }

    return this;
}



