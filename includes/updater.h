#pragma once

#include <algorithm>
#include <cstdint>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>

#include "json.hpp"

struct update_struct
{
    int length;
    int newLength;
    std::string fieldStatus;
};

inline void from_json(const nlohmann::json &j, update_struct &update)
{
    j.at("length").get_to(update.length);
    j.at("newLength").get_to(update.newLength);
    j.at("fieldStatus").get_to(update.fieldStatus);
}

template <typename T>
class updater
{
  public:
    updater()
    {
    }

    void update(const std::string &filename, const std::string &update_filename, const std::string &new_filename)
    {
        if (!std::filesystem::exists(filename))
        {
            throw std::runtime_error("the database file[" + filename + "] could not be found.");
        }

        if (!std::filesystem::exists(update_filename))
        {
            throw std::runtime_error("the update file[" + update_filename + "] could not be found.");
        }

        std::ifstream ifs(update_filename);
        m_Updates = nlohmann::json::parse(ifs);
        calculate_struct_sizes();

        auto filesize = std::filesystem::file_size(filename);
        auto remainder = filesize % m_OldStructSize;
        if (remainder != 0)
        {
            throw std::runtime_error("the old structure doesn't match the file provided");
        }

        m_NumberOfItemsInDatabase = (filesize / m_OldStructSize);

        write_new_file(filename, new_filename);
    }
    void calculate_struct_sizes()
    {
        for (auto &update : m_Updates)
        {
            std::transform(update.fieldStatus.begin(), update.fieldStatus.end(), update.fieldStatus.begin(), ::toupper);
            if (update.fieldStatus == "UNCHANGED")
            {
                m_OldStructSize += update.length;
                m_NewStructSize += update.length;
                continue;
            }

            if (update.fieldStatus == "REMOVED")
            {
                m_OldStructSize += update.length;
                continue;
            }

            if (update.fieldStatus == "ADDED")
            {
                m_NewStructSize += update.newLength;
                continue;
            }

            if (update.fieldStatus == "CHANGED")
            {
                m_NewStructSize += update.newLength;
                m_OldStructSize += update.length;
                continue;
            }
        }
        m_OldStructSize += sizeof(std::uint64_t);
        m_NewStructSize += sizeof(std::uint64_t);
    }
    void write_new_file(const std::string &filename, const std::string &new_filename)
    {
        char *buffer = new char[m_OldStructSize];
        char *newBuffer = new char[m_NewStructSize];

        std::ifstream file(filename, std::ios::binary);
        std::ofstream newFile(new_filename, std::ios::binary);
        for (auto i = 0; i < m_NumberOfItemsInDatabase; i++)
        {
            std::uint64_t index = 0;
            file.seekg(i * m_OldStructSize);
            file.read(buffer, m_OldStructSize);
            newFile.write(buffer, sizeof(std::uint64_t));
            newFile.flush();
            index += sizeof(std::uint64_t);
            for (auto &update : m_Updates)
            {
                std::transform(update.fieldStatus.begin(), update.fieldStatus.end(), update.fieldStatus.begin(), ::toupper);
                if (update.fieldStatus == "UNCHANGED")
                {
                    newFile.write(buffer + index, update.length);
                    newFile.flush();
                    index += update.length;
                    continue;
                }

                if (update.fieldStatus == "REMOVED")
                {
                    index += update.length;
                    continue;
                }

                if (update.fieldStatus == "ADDED")
                {
                    for (int i = 0; i < update.newLength; i++)
                        newFile.put(0x00);
                    newFile.flush();
                    continue;
                }

                if (update.fieldStatus == "CHANGED")
                {
                    if (update.newLength > update.length)
                    {
                        newFile.write(buffer + index, update.length);
                        for (int i = 0; i < update.newLength - update.length; i++)
                            newFile.put(0x00);
                        newFile.flush();
                        index += update.length;
                    }
                    else
                    {
                        newFile.write(buffer + index, update.newLength);
                        newFile.flush();
                        index += update.newLength;
                    }
                }
            }
        }
        file.close();
        newFile.close();

        if (std::filesystem::file_size(new_filename) % m_NewStructSize != 0)
        {
            throw std::runtime_error("the new file is not the expected size...");
        }
    }

    std::int64_t m_OldStructSize = 0;
    std::int64_t m_NewStructSize = 0;
    std::int64_t m_NumberOfItemsInDatabase = 0;
    std::vector<update_struct> m_Updates;
};
