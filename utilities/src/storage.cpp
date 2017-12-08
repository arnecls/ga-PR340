#include "storage.h"
#include "macros.h"

namespace util {
	const std::string CStorage::NotFound = "NOT FOUND";
	const std::string CStorage::Ok = "OK";
	const std::string CStorage::Invalid = "INVALID COMMAND";

	void CStorage::Set(const std::string& aKey, const std::string& aValue) {
		GUARD(mMapLock);
		mData[aKey] = aValue;
	}

	const std::string& CStorage::Get(const std::string& aKey) {
		GUARD(mMapLock); 
		auto itemIter = mData.find(aKey);
		if (itemIter != mData.end()) {
			return itemIter->second;
		}
		return CStorage::NotFound;
	}

	const std::string& CStorage::Execute(const SCommand& aCommand) {
		switch (aCommand.Type) {
		case ECommandType::Get:
			return Get(aCommand.Key);

		case ECommandType::Set:
			Set(aCommand.Key, aCommand.Value);
			return CStorage::Ok;

		default:
			return CStorage::Invalid;
		}
	}
}