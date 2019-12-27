#include "Log.h"

static Logger::Ptr g_instance;
static std::mutex g_mutex;

Logger::Ptr Logger::sharedInstance() {
    std::lock_guard<std::mutex> lock(g_mutex);
    if (!g_instance) {
        g_instance = std::make_shared<Logger>();
    }

    return g_instance;
}

void Logger::write(const std::string& message) {
    std::lock_guard<std::mutex> lock(g_mutex);
    std::cout << message << std::endl;
}
