#pragma once

#include "ILogger.h"

class ConsoleLogger : public ILogger {
public:
	void Log(const std::string& str) override;
};