#include "command.h"
#include <unordered_map>

namespace util {

	enum class EContext {
		Type, Key, Value, Undefined
	};

	static const std::unordered_map<std::string, ECommandType> gNameToType = {
		{ "SET", ECommandType::Set },
		{ "set", ECommandType::Set },
		{ "GET", ECommandType::Get },
		{ "get", ECommandType::Get },
		{ "QUIT", ECommandType::Quit },
		{ "quit", ECommandType::Quit }
	};

	std::string getString(const char* pStart, const char* pEnd) {
		if (*pStart == '"') {
			pStart++;
			pEnd--;
		}
		return std::string(pStart, pEnd);
	}

	ECommandType getType(const char* pStart, const char* pEnd) {
		auto ident = getString(pStart, pEnd);
		auto typeIter = gNameToType.find(ident);
		if (typeIter != gNameToType.end()) {
			return typeIter->second;
		}
		return ECommandType::Invalid;
	}

	void update(SCommand& arParsedCommand, EContext& arState, const char* pLastMatch, const char* pChar) {
		// It is always "TYPE [KEY] [VALUE]\n", so we can parse in exactly that order.
		// We will switch to Undefined when a command has completed parsing.
		switch (arState) {
		case EContext::Type:
			arParsedCommand.Type = getType(pLastMatch, pChar);
			arState = (arParsedCommand.Type == ECommandType::Quit)
				? EContext::Undefined // Quit has no parameters
				: EContext::Key;
			break;

		case EContext::Key:
			arParsedCommand.Key = getString(pLastMatch, pChar);
			arState = (arParsedCommand.Type == ECommandType::Get)
				? EContext::Undefined // Get has no value
				: EContext::Value;
			break;

		case EContext::Value:
			arParsedCommand.Value = getString(pLastMatch, pChar);
			arState = EContext::Undefined; // could return here but we want to validate the \n
			break;
		}
	}

	SCommand ParseCommand(const char* pString) {
		auto state = EContext::Type;
		bool inQuotedString = false;
		bool escapedChar = false;
		bool inWhitespace = true;

		SCommand parsedCommand;
		const char* pLastMatch = pString;
		
		for (const char* pChar = pString; *pChar != 0; ++pChar) {
			switch (*pChar) {
			default:
				escapedChar = false;
				inWhitespace = false;
				break;

			case '"':
				if (!escapedChar) {
					inQuotedString = !inQuotedString;
				}
				escapedChar = false;
				inWhitespace = false;
				break;

			case '\\':
				escapedChar = !escapedChar; // escaped backslash requires toggle
				inWhitespace = false;
				break;

			case '\t':
		    case ' ':
				escapedChar = false;
				if (inQuotedString) {
					break; // ignore whitespace in quoted strings
				}
				if (inWhitespace) {
					pLastMatch = pChar + 1; // skip additional whitespace
					break;
				}

				update(parsedCommand, state, pLastMatch, pChar);

				pLastMatch = pChar + 1;
				inWhitespace = true;
				break;

			case '\n':
				if (!inQuotedString) {
					update(parsedCommand, state, pLastMatch, pChar);
					return parsedCommand; // ### return, valid parse ###
				}
				escapedChar = false;
				inWhitespace = false;
				break;
			}
		}

		return InvalidCommand;
	}
}