#pragma once

#include <cstdint>
#include <filesystem>
#include <fstream>
#include <map>
#include <memory>
#include <set>
#include <vector>

template <typename T>
class database
{
  public:
    explicit database(const std::string &name)
    {
        std::filesystem::create_directory("Databases");
        FileName_ = "Databases/" + name + ".dat";
        if (std::filesystem::exists(FileName_))
        {
            ReloadFromFile();
        }
        else
        {
            DatabaseFile_ = std::make_shared<std::ofstream>(FileName_, std::ios::binary);
        }
    }
    database(const database &) = delete;
    database(const database &&) = delete;
    database &operator=(const database &) = delete;
    database &operator=(const database &&) = delete;
    database &operator==(const database &&) = delete;

    ~database()
    {
        DatabaseFile_->close();
    }

    [[nodiscard]] std::uint64_t AddObject(const T &object)
    {
        const auto pos = getUnoccupiedPosition();

        OccupiedPlaces_[++Id_] = pos;
        DatabaseFile_->seekp(static_cast<long long>(pos * (sizeof(T) + sizeof(std::uint64_t))));
        DatabaseFile_->write(reinterpret_cast<const char *>(&Id_), sizeof(std::uint64_t));
        DatabaseFile_->write(reinterpret_cast<const char *>(&object), sizeof(T));
        DatabaseFile_->flush();

        return Id_;
    }
    T GetObject(std::uint64_t id)
    {
        const auto pos = findDatabasePositionFromId(id);
        std::ifstream tempReader(FileName_, std::ios::binary);
        auto *tempMemoryBuffer = new char[sizeof(T)];
        tempReader.seekg(static_cast<long long>((pos * (sizeof(T) + sizeof(std::uint64_t))) + sizeof(std::uint64_t)));
        tempReader.read(&tempMemoryBuffer[0], sizeof(T));
        const auto tempObject = *(reinterpret_cast<T *>(&tempMemoryBuffer[0]));
        delete[] tempMemoryBuffer;
        tempReader.close();

        return tempObject;
    }
    void EditObject(std::uint64_t id, const T &object)
    {
        const auto pos = findDatabasePositionFromId(id);

        DatabaseFile_->seekp(static_cast<long long>((pos * (sizeof(T) + sizeof(std::uint64_t))) + sizeof(std::uint64_t)));
        DatabaseFile_->write(reinterpret_cast<const char *>(&object), sizeof(T));
        DatabaseFile_->flush();
    }
    void DeleteObject(std::uint64_t id)
    {
        const auto pos = findDatabasePositionFromId(id);

        DatabaseFile_->seekp(static_cast<long long>(pos * (sizeof(T) + sizeof(std::uint64_t))));
        for (std::size_t i = 0; i < sizeof(T) + sizeof(std::uint64_t); i++)
            DatabaseFile_->put(0x00);
        OccupiedPlaces_.erase(id);
        FragmentedSegments_.insert(pos);
        DatabaseFile_->flush();
    }
    void ReloadFromFile()
    {
        LargestNumber_ = -1;
        FragmentedSegments_.clear();
        OccupiedPlaces_.clear();
        std::ifstream tempReader(FileName_, std::ios::binary);
        const std::uintmax_t fileSize = std::filesystem::file_size(FileName_);
        if (fileSize % (sizeof(T) + sizeof(std::uint64_t)) != 0)
        {
            return;
        }

        auto *tempMemoryBuffer = new char[fileSize];
        tempReader.read(&tempMemoryBuffer[0], static_cast<long long>(fileSize));
        tempReader.close();

        for (std::size_t i = 0; i < fileSize / (sizeof(T) + sizeof(std::uint64_t)); i++)
        {
            std::uint64_t *tempId = reinterpret_cast<std::uint64_t *>(&tempMemoryBuffer[i * (sizeof(T) + sizeof(std::uint64_t))]);
            if (*tempId == 0)
            {
                FragmentedSegments_.insert(i);
            }
            else
            {
                OccupiedPlaces_[*tempId] = i;
                Id_ = *tempId;
            }
            LargestNumber_++;
        }
        delete[] tempMemoryBuffer;

        DatabaseFile_ = std::make_shared<std::ofstream>(FileName_, std::ios::binary | std::ios::in);
    }
    [[nodiscard]] std::vector<T> GetObjects() const
    {
        std::vector<T> objects;
        std::ifstream tempReader(FileName_, std::ios::binary);
        const std::uintmax_t fileSize = std::filesystem::file_size(FileName_);
        auto *tempMemoryBuffer = new char[fileSize];
        tempReader.read(&tempMemoryBuffer[0], static_cast<long long>(fileSize));
        tempReader.close();

        for (std::size_t i = 0; i < fileSize / (sizeof(T) + sizeof(std::uint64_t)); i++)
        {
            std::uint64_t *tempId = reinterpret_cast<std::uint64_t *>(&tempMemoryBuffer[i * sizeof(T)]);
            if (*tempId != 0)
            {
                T *tmpObject = reinterpret_cast<T *>(&tempMemoryBuffer[i * (sizeof(T) + sizeof(std::uint64_t)) + sizeof(uint64_t)]);
                objects.emplace_back(*tmpObject);
            }
        }
        delete[] tempMemoryBuffer;

        return objects;
    }

  private:
    std::uint64_t Id_ = 0;
    std::map<std::uint64_t, std::size_t> OccupiedPlaces_;
    std::string FileName_;
    long long LargestNumber_ = -1;
    std::set<std::size_t> FragmentedSegments_;
    std::shared_ptr<std::ofstream> DatabaseFile_;
    std::size_t ObjectSize_;

    [[nodiscard]] std::size_t getUnoccupiedPosition()
    {
        if (FragmentedSegments_.empty())
            return ++LargestNumber_;

        const auto result = *FragmentedSegments_.begin();
        FragmentedSegments_.erase(FragmentedSegments_.begin());

        return result;
    }
    [[nodiscard]] std::size_t findDatabasePositionFromId(std::uint64_t id)
    {
        if (OccupiedPlaces_.contains(id))
            return OccupiedPlaces_[id];

        throw std::runtime_error("The order was not in the database.");
    }
};
