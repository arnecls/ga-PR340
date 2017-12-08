#pragma once

#include <unordered_map>
#include <mutex>
#include <string>
#include "command.h"

namespace util {

	//! @brief CStorage provides a thread safe key/value store.
	class CStorage {
	private:
		std::mutex mMapLock;
		std::unordered_map<std::string, std::string> mData;

	public:

		static const std::string NotFound;	//!< Returned by Execute/Get when a key was not found
		static const std::string Ok;		//!< Returned by Execute/Set
		static const std::string Invalid;	//!< Returned by Execute for anything but Get or Set

		//! @brief Store a given value for a given key.
		//! Exisiting keys will be overwritten.
		//! @param aKey Key of the value to store
		//! @param aValue Value to store
		void Set(const std::string& aKey, const std::string& aValue);

		//! @brief Get a value for a given key.
		//! @param aKey Key to search for
		//! @return Returns the corresponding value or NotFound
		const std::string& Get(const std::string& aKey);

		//! @brief Execute the given command against the storage.
		//! @param aCommand The command to execute. Set and Get are supported.
		//! @return Returns an appropriate response for a given command.
		const std::string& Execute(const SCommand& aCommand);
	};
}