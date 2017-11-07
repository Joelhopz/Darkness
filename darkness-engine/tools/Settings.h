#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include <memory>

namespace tools
{
    enum class SettingsBackend
    {
        Binary,
        Json
    };

    class Settings;
    namespace implementation
    {
        class SettingsContainer : public std::enable_shared_from_this<SettingsContainer>
        {
        private:
            std::unordered_map<std::string, bool> m_bools;
            std::unordered_map<std::string, char> m_chars;
            std::unordered_map<std::string, short> m_shorts;
            std::unordered_map<std::string, int> m_ints;
            std::unordered_map<std::string, int64_t> m_bigints;
            std::unordered_map<std::string, unsigned char> m_uchars;
            std::unordered_map<std::string, unsigned short> m_ushorts;
            std::unordered_map<std::string, unsigned int> m_uints;
            std::unordered_map<std::string, uint64_t> m_biguints;
            std::unordered_map<std::string, float> m_floats;
            std::unordered_map<std::string, double> m_doubles;
            std::unordered_map<std::string, std::string> m_strings;

            std::unordered_map<std::string, std::vector<bool>> m_boolVectors;
            std::unordered_map<std::string, std::vector<char>> m_charVectors;
            std::unordered_map<std::string, std::vector<short>> m_shortVectors;
            std::unordered_map<std::string, std::vector<int>> m_intVectors;
            std::unordered_map<std::string, std::vector<int64_t>> m_bigintVectors;
            std::unordered_map<std::string, std::vector<unsigned char>> m_ucharVectors;
            std::unordered_map<std::string, std::vector<unsigned short>> m_ushortVectors;
            std::unordered_map<std::string, std::vector<unsigned int>> m_uintVectors;
            std::unordered_map<std::string, std::vector<uint64_t>> m_biguintVectors;
            std::unordered_map<std::string, std::vector<float>> m_floatVectors;
            std::unordered_map<std::string, std::vector<double>> m_doubleVectors;
            std::unordered_map<std::string, std::vector<std::string>> m_stringVectors;

        public:
            SettingsContainer(const std::string& groupName);
            std::vector<std::string> keys() const;
            std::vector<std::string> groups() const;

            template<typename T>
            void set(const std::string& key, const std::vector<T>& value);

            template<typename T>
            void set(const std::string& key, const T& value);

            template<typename T>
            T get(const std::string& key);

            template<typename T>
            bool hasKey(const std::string& key);

        protected:
            friend class Settings;
            std::shared_ptr<implementation::SettingsContainer> m_parent;
            std::string m_groupName;
            std::unordered_map<std::string, std::shared_ptr<implementation::SettingsContainer>> m_childs;

            void readJson(void* _obj);
            void writeJson(void* _writer);

            void writeBinary();
        };
    }

    class Settings
    {
    public:
        Settings();
        Settings(const std::string& settingsPath, SettingsBackend backend = SettingsBackend::Json);
        ~Settings();

        void save();

        Settings(const Settings&) = default;
        Settings(Settings&&) = default;
        Settings& operator=(const Settings&) = default;
        Settings& operator=(Settings&&) = default;

        std::vector<std::string> keys() const;
        std::vector<std::string> groups() const;

        void beginGroup(const std::string& group);
        void endGroup();

        template<typename T>
        void set(const std::string& key, const T& value)
        {
            m_currentNode->set(key, value);
            m_changed = true;
        }

        template<typename T>
        void set(const std::string& key, const std::vector<T>& value)
        {
            m_currentNode->set(key, value);
            m_changed = true;
        }

        template<typename T>
        T get(const std::string& key)
        {
            return m_currentNode->get<T>(key);
        }

        template<typename T>
        T get(const std::string& key, const T& defaultValue)
        {
            if(m_currentNode->hasKey<T>(key))
                return m_currentNode->get<T>(key);
            else
            {
                m_currentNode->set(key, defaultValue);
                m_changed = true;
                return defaultValue;
            }
        }
    private:
        std::shared_ptr<implementation::SettingsContainer> m_rootNode;
        std::shared_ptr<implementation::SettingsContainer> m_currentNode;
        std::string m_settingsPath;
        SettingsBackend m_backend;
        bool m_changed;

        void readJson();
        void readBinary();

        void writeJson();
        void writeBinary();
    };
}
