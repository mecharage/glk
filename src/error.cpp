#include <string>
#include <functional>
#include <iostream>

#include <glk/error.h>
#include <glk/util.h>
#include <glk/stringLiterals.h>

namespace {

	glk::LogLevel &logLevel() {
		static glk::LogLevel logLevel = glk::LogLevel::Warnings;
		return logLevel;
	}

	std::string const &logIndentIncrement() {
		static std::string const logIndentIncrement = "  ";
		return logIndentIncrement;
	}

	std::string &logIndent() {
		static std::string logIndent;
		return logIndent;
	}
}

namespace glk {

	LogHandler &logHandler() {
		static LogHandler logHandler = [](auto &&msg) {
			std::clog << msg << '\n';
		};
		return logHandler;
	}

	LogHandler &warnHandler() {
		static LogHandler warnHandler = [](auto &&msg) {
			std::clog << msg << '\n';
		};
		return warnHandler;
	}

	LogHandler &errorHandler() {
		static LogHandler errorHandler = [](auto &&msg) {
			std::cerr << msg << '\n';
		};
		return errorHandler;
	}

	LogHandler &fatalHandler() {
		static LogHandler fatalHandler = [](auto &&msg) {
			std::cerr << msg << std::endl;
		};
		return fatalHandler;
	}

	LogLevel setLogLevel(LogLevel level) {
		LogLevel oldLevel = logLevel();
		logLevel() = level;
		return oldLevel;
	}

	std::string indent(std::string const &msg) {
		std::string result = logIndent();
		result.reserve(logIndent().size() + msg.size());
		for (char const *p = msg.c_str(); *p; ++p) {
			result += *p;
			if (*p == '\n')
				result += "       "s + logIndentIncrement();
		}
		return result;
	}

	void log(std::string const &msg) {
		if (val(logLevel()) >= val(LogLevel::Log))
			logHandler()("       "s + indent(msg));
	}

	void warn(std::string const &msg) {
		if (val(logLevel()) >= val(LogLevel::Warnings))
			warnHandler()("[WARN ]"s + indent(msg));
	}

	void error(std::string const &msg) {
		if (val(logLevel()) >= val(LogLevel::Errors))
			errorHandler()("[ERROR]"s + indent(msg));
	}

	void fatal(std::string const &msg, int exitCode) {
		fatalHandler()("[FATAL]"s + indent(msg));
		throw FatalException(msg, exitCode);
	}

	LogSection::LogSection() {
		logIndent() += logIndentIncrement();
	}

	LogSection::LogSection(const std::string & title) {
		log("["s + title + "]");
		logIndent() += logIndentIncrement();
	}

	LogSection::~LogSection() {
		logIndent().resize(logIndent().size() - logIndentIncrement().size());
	}
}
