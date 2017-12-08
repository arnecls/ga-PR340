#pragma once

#include <string>

namespace util {

	//! @brief ECommandType holds the different supported commands for the server.
	enum class ECommandType : char {
		Invalid, //!< Unknown or invalid command
		Set,     //!< Write a key/value pair
		Get,     //!< Read a value for a given key
		Quit     //!< Close the connection
	};
	
	//! @brief SCommand represents a command sent to the server.
	//! You can parse a command from a string using ParseCommand.
	struct SCommand {
		ECommandType Type; //!< Type of the command
		std::string Key;   //!< Optional: key passed to the command
		std::string Value; //!< Optional: value passed to the command
	};

	//! @brief InvalidCommand is used by the parser if a command could not be
	//! parsed or is unknown.
	static const SCommand InvalidCommand {
		ECommandType::Invalid
	};

	//! @brief Parse a command string into an SCommand struct.
	//! This function will parse the commands GET, SET and QUIT. The expected
	//! Fields are TYPE, KEY and VALUE. All fields can be put in quotes and are
	//! expected to be separated by " " or "\t". Conesqutive whitespaces will 
	//! be ignored. The escape character is "\". A command is considered 
	//! terminated by the character '\n'.
	//!
	//! @param apString Command string to be parsed.
	//! @return Returns the parsed command or InvalidCommand on error.
	SCommand ParseCommand(const char* apString);
}