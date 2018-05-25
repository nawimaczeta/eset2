//! @file	EvmFile.cpp
//! @author	Lukasz Iwanecki
//! @date	05.2018
//! 
//! Evm file related functions and structures
#include "stdafx.h"
#include "EvmFile.h"
#include "RuntimeError.h"

namespace Evm {
	namespace File {
		static const uint32_t HEADER_SIZE = 20;		//!< Header size in bytes
		static const char HEADER_MAGIC[8] = 
		{ 'E', 'S', 'E', 'T', '-', 'V' , 'M', '2' };	//!< Evm file magic

		unique_ptr<File::EvmFile> makeEvmFromFile(const string & filename)
		{
			ifstream ifs(filename, ios::binary);
			if (!ifs.is_open()) {
				throw EvmFileParseRuntimeError{ "Bad filename: " + filename };
			}

			auto evm = make_unique<EvmFile>();

			// get file size
			ifs.seekg(0, ifs.end);
			evm->fileSize = ifs.tellg();
			ifs.seekg(0, ifs.beg);

			if (evm->fileSize < HEADER_SIZE) {
				throw EvmFileParseRuntimeError{ "File size too small" };
			}

			for (int i = 0; i < 8; i++) {
				evm->header.magic[i] = ifs.get();
			}

			// Fill EvmFile::Header with values from the file
			ifs.read(reinterpret_cast<char *>(&evm->header.codeSize), 4);
			ifs.read(reinterpret_cast<char *>(&evm->header.dataSize), 4);
			ifs.read(reinterpret_cast<char *>(&evm->header.initialDataSize), 4);

			// Get payload
			Bytes newPayload(evm->fileSize - HEADER_SIZE);
			ifs.read(reinterpret_cast<char *>(newPayload.data()), newPayload.size());

			evm->payload.swap(newPayload);

			return move(evm);
		}

		void validateEvm(const EvmFile & evm)
		{
			if (!(evm.header.dataSize >= evm.header.initialDataSize)) {
				throw EvmFileParseRuntimeError{ "Bad header: data size < initialized data size" };
			}

			if (!equal(begin(evm.header.magic), end(evm.header.magic),
				begin(HEADER_MAGIC), end(HEADER_MAGIC))) {
				throw EvmFileParseRuntimeError{ "Bad header: magic incorrect" };
			}

			uint32_t size = evm.header.codeSize + evm.header.initialDataSize + HEADER_SIZE;
			if (size != evm.fileSize) {
				throw EvmFileParseRuntimeError{ "Bad header: values in header don't match file size" };
			}

			if (evm.header.codeSize > evm.payload.size()) {
				throw EvmFileParseRuntimeError{ "Code size from header doesn't match real payload size" };
			}
		}

		pair<Bytes::const_iterator, Bytes::const_iterator> extractCode(const EvmFile & evm)
		{
			if (evm.header.codeSize > evm.payload.size()) {
				throw EvmFileParseRuntimeError{ "Code size from header doesn't match real payload size" };
			}
			auto codeBeginIt = begin(evm.payload);
			auto codeEndIt = codeBeginIt + evm.header.codeSize;

			return make_pair(codeBeginIt, codeEndIt);
		}
		pair<Bytes::const_iterator, Bytes::const_iterator> extractInitializedData(const EvmFile & evm)
		{
			if (evm.header.codeSize + evm.header.initialDataSize > evm.payload.size()) {
				throw EvmFileParseRuntimeError{ "Code size from header doesn't match real payload size" };
			}
			auto codeBeginIt = begin(evm.payload) + evm.header.codeSize;
			auto codeEndIt = end(evm.payload);

			return make_pair(codeBeginIt, codeEndIt);
		}

		ostream & operator<<(ostream & os, EvmFile & evm) {
			os << "EVM file:\nmagic " << string(begin(evm.header.magic), end(evm.header.magic)) << "\ncode size " << evm.header.codeSize <<
				"\ndata size " << evm.header.dataSize << "\ninit data size " << evm.header.initialDataSize <<
				"\nfile size " << evm.fileSize << "\npayload size " << evm.payload.size() << "\n";
			return os;
		}
	}
}