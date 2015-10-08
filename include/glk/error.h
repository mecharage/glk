#pragma once

#include <string>
#include <functional>
#include <stdexcept>

namespace glk {
	using LogHandler = std::function<void(std::string const&) >;
	LogHandler &logHandler();
	LogHandler &warnHandler();
	LogHandler &errorHandler();
	LogHandler &fatalHandler();

	void log(std::string const &msg);
	void warn(std::string const &msg);
	void error(std::string const &msg);
	void fatal(std::string const &msg, int exitCode = 1);

	struct LogSection {
		LogSection();
		LogSection(std::string const &title);
		~ LogSection();
	} ;

	enum class LogLevel {
		None, Errors, Warnings, Log
	} ;

	LogLevel setLogLevel(LogLevel level);

	struct FatalException final : std::exception {

		FatalException(std::string const &msg, int exitCode) : _msg(msg), _exitCode(exitCode) { }

		virtual char const *what() const noexcept override {
			return _msg.c_str();
		}

		int exitCode() const noexcept {
			return _exitCode;
		}

	private:
		std::string _msg;
		int _exitCode;
	} ;
}
