//! @file	Memory.h
//! @author	Lukasz Iwanecki
//! @date	05.2018
//! @brief	Memory class declaration
#pragma once

#include "stdafx.h"

namespace Evm {
	namespace Utils {
		//! @breif Memory class
		//!
		//! Memory class represents a block of RAM. Mamory is given in constructor.
		//! A class provides read and write functions
		struct Memory {
			//! @breif Constructor
			//!
			//! @param size Memory size
			Memory(uint64_t size) :
				_memory(size)
			{}

			//! @brief Write data block
			//!
			//! Write a bloc of data from @ref data to memory under @ref address.
			//! @param address Address in memory
			//! @param data Data block to write
			//! @throw out_of_range
			void write(uint64_t address, const Bytes & data);

			//! @brief Write data block
			//!
			//! Write a bloc of data from [beg; end) to memory under @ref address.
			//! @param address Address in memory
			//! @param beg Begin iterator
			//! @param end End iterator
			//! @throw out_of_range
			void write(uint64_t address, Bytes::const_iterator beg, Bytes::const_iterator end);

			//! @brief Read data block
			//!
			//! Read @ref size bytes from @ref address and return them as a vactor
			//! @param address Address in memory
			//! @param size Number of bytes to read
			//! @return Data block
			//! @throw out_of_range
			Bytes read(uint64_t address, uint64_t size) const;

		private:
			Bytes _memory;		//!< Memory
			bool _outOfMemory(uint64_t address, uint64_t size) const;
		};
	}
}