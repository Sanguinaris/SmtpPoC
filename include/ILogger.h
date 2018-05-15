#pragma once
#include <string>

class ILogger {
public:
	virtual ~ILogger() = default;

public:
	virtual void Log(const std::string&) = 0;
};
