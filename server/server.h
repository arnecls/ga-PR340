#pragma once

#include <cstdint>
#include "storage.h"

class CServer {
private:

	util::CStorage mStorage;

public:

	//! @brief Starts listening for connections.
	//! This function should be blocking.
	//! @param aPort Port to listen to
	void Start(uint16_t aPort);

	//! @brief Stop the server
	void Stop();
};