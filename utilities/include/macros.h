#pragma once

//! @brief Helper function for CONCAT. Do not use.
#define _CONCATENATE(dA, dB) dA ## dB

//! @brief Concatenate two identifiers
//! @param dA Left side of the resulting identifier
//! @param dB Right side of the resulting identifier
#define CONCAT(dA, dB) _CONCATENATE(dA, dB)

//! @brief Shortcut for a mutex based std::lock_guard
//! @param dLock The variable holding the mutex
#define GUARD(dLock) std::lock_guard<std::mutex> CONCAT(guard,__LINE__)(dLock);