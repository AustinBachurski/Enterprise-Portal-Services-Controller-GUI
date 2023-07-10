#include "Logging.hpp"

Logging::Logging()
{
    const std::chrono::zoned_time now{ std::chrono::current_zone(), std::chrono::system_clock::now() };
    m_filePath = m_directory + std::format("{:%F}", now) + ".txt";

    if (std::filesystem::exists(m_directory))
    {
        beginLogging(std::format("{:%r}", now));
    }
    else
    {
        createDirectory();
        beginLogging(std::format("{:%r}", now));
    }
}

void Logging::beginLogging(const std::string& now) const
{
    log("Logging started " + now);
}

void Logging::createDirectory() const
{
    std::filesystem::create_directory(m_directory);
}

std::string Logging::getFileName() const
{
    return m_filePath;
}

void Logging::log(const std::string input) const
{
    std::ofstream logFile(m_filePath, std::ios::out | std::ios::app); // Open file for writing. | Append data.

    if (logFile.is_open())
    {
        logFile << input << '\n';
    }
    else 
    {
        std::cout << "Error: Unable to open log file.\n";
    }
}