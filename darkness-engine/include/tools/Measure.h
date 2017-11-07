#pragma once

#include "tools/Debug.h"
#include <vector>
#include <chrono>
#include <string>
#include <map>

namespace tools
{
    namespace implementation
    {
        struct Measurement
        {
            const char* msg = nullptr;
            std::chrono::time_point<std::chrono::steady_clock> time;
            std::string stdMsg;
        };

        class MeasureStorage
        {
        public:
            bool addMeasure(const std::string& key, uint32_t period, std::vector<Measurement> measurements);
            void clearMeasure(const std::string& key);
            std::vector<double> average(const std::string& key);
        private:
            std::map<std::string, std::vector<std::vector<Measurement>>> m_storage;
        };

        extern MeasureStorage GlobalMeasurementStorage;
    }

    class Measure
    {
    public:
        Measure();

        void disable();

        void period(uint32_t period);

        void here(const char* msg);
        void here(const std::string& msg);
        ~Measure();
    private:
        bool m_active;
        uint32_t m_period;

        std::vector<implementation::Measurement> m_measurements;
    };
}
