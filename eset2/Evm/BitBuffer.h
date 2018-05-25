//! @file	BitBuffer.h
//! @author	Lukasz Iwanecki
//! @date	05.2018
//! @brief	BitBuffer class
#pragma once

#include "stdafx.h"

namespace Evm {
	namespace Utils {
		//! BitBuffer class provides bit access to memory block. Memory block is a vector of bytes.
		//! Getter @ref getU64(), @ref getU32(), @ref getU16() and @ref getU8() functions 
		//! return required number of bytes from the data block. The main purpose of this 
		//! class is usage as program memory in Evm application
		struct BitBuffer {
			//! @brief Constructor
			//!
			//! Get vector as input memory block
			//! @param data Input vector
			BitBuffer(const Bytes & data);

			//! @brief Constructor
			//!
			//! Get a pair of iterators. Data in range [begin; end) is copied as memory block
			//! @param begin Begin iterator
			//! @param begin End iterator
			BitBuffer(Bytes::const_iterator begin, Bytes::const_iterator end);

			//! @brief Get up to 64 bits and return them as 64-bit number
			//!
			//! Acquire size number of bits from memory block, starting from
			//! address @ref bit address. Return the number as 64-bit number.
			//! The bits can be aquired in normal or reversed order
			//! @param bitAddress Address in bits of the first bit
			//! @param size Size in bits
			//! @param reversed True - reversed bit order
			//! @throw out_of_range
			uint64_t getU64(uint32_t bitAddress, uint32_t size, bool reversed = false) const;

			//! @brief Get up to 32 bits and return them as 32-bit number
			//!
			//! Acquire size number of bits from memory block, starting from
			//! address @ref bit address. Return the number as 32-bit number.
			//! The bits can be aquired in normal or reversed order
			//! @param bitAddress Address in bits of the first bit
			//! @param size Size in bits
			//! @param reversed True - reversed bit order
			//! @throw out_of_range
			uint32_t getU32(uint32_t bitAddress, uint32_t size, bool reversed = false) const;

			//! @brief Get up to 16 bits and return them as 16-bit number
			//!
			//! Acquire size number of bits from memory block, starting from
			//! address @ref bit address. Return the number as 16-bit number.
			//! The bits can be aquired in normal or reversed order
			//! @param bitAddress Address in bits of the first bit
			//! @param size Size in bits
			//! @param reversed True - reversed bit order
			//! @throw out_of_range
			uint16_t getU16(uint32_t bitAddress, uint32_t size, bool reversed = false) const;

			//! @brief Get up to 8 bits and return them as 8-bit number
			//!
			//! Acquire size number of bits from memory block, starting from
			//! address @ref bit address. Return the number as 8-bit number.
			//! The bits can be aquired in normal or reversed order
			//! @param bitAddress Address in bits of the first bit
			//! @param size Size in bits
			//! @param reversed True - reversed bit order
			//! @throw out_of_range
			uint8_t getU8(uint32_t bitAddress, uint32_t size, bool reversed = false) const;

			//! @brief Return number of bits in BitBuffer
			//!
			//! @return Number of bits
			uint32_t size() const;
		private:
			const Bytes _data;	//!< memory block
		};
	}
}