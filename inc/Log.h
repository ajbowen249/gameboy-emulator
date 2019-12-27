#ifndef __Log_h__
#define __Log_h__

#include <iostream>
#include <string>
#include <memory>
#include <mutex>

#define LOG(data) \
    Logger::sharedInstance()->write(data); \

class Logger {
public:
    typedef std::shared_ptr<Logger> Ptr;

    static Ptr sharedInstance();
    void write(const std::string& message);
};

#endif // __Log_h__
