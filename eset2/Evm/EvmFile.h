//! @file	EvmFile.h
//! @author	Lukasz Iwanecki
//! @date	05.2018
//! @brief	Evm file representation
//! 
//! The file contains EvmFile structure and a few functions 
//! that operates on this structure
#pragma once

#include "stdafx.h"

namespace Evm {
	namespace File {

		//! The structure describes Evm file. It contains Evm header, payload and file size
		struct EvmFile {
			//! Evm Header
			struct Header {
				char magic[8];			//!< Magic - first 8 bytes
				uint32_t codeSize;		//!< Size of code section
				uint32_t dataSize;		//!< Size of data section
				uint32_t initialDataSize;	//!< Size of section with init data
			};

			Header header;		//!< Evm header
			Bytes payload;		//!< Evm payload in bytes
			size_t fileSize;	//!< Real file size
		};

		//! @brief Evm file factory
		//!
		//! Generate EvmFile structure from given file.
		//! @param filename Filename to Evm file
		//! @return Pointer to EvmFile structure
		unique_ptr<EvmFile> makeEvmFromFile(const string & filename);

		//! @brief Validate Evm file
		//!
		//! Check if header and data in evm file is correct.
		//! @param evm Reference to EvmFile
		//! @throw EvmFileParseRuntimeError
		void validateEvm(const EvmFile & evm);

		//! @brief Get memory with program code from evm file
		//!
		//! Return a pair of iterators to code memory block.
		//! @param evm Reference to EvmFile
		//! @return A pair of iterators to code memory block
		pair<Bytes::const_iterator, Bytes::const_iterator> extractCode(const EvmFile & evm);

		//! @brief Get memory with initialized data from evm file
		//!
		//! Return a pair of iterators to initialized data memory block.
		//! @param evm Reference to EvmFile
		//! @return A pair of iterators to initialized data memory block
		pair<Bytes::const_iterator, Bytes::const_iterator> extractInitializedData(const EvmFile & evm);

		ostream & operator<<(ostream & os, EvmFile & evm);
	}
}