
#include "stdafx.h"
#include "Evm.h"

namespace Evm {

	ostream & operator<<(ostream & os, Evm & evm) {
		os << "EVM file:\nmagic " << string(begin(evm.header.magic), end(evm.header.magic)) << "\ncode size " << evm.header.codeSize <<
			"\ndata size " << evm.header.dataSize << "\ninit data size " << evm.header.initialDataSize <<
			"\nfile size " << evm.fileSize << "\npayload size " << evm.payload.size() << "\n";
		return os;
	}

	namespace File {
		static const uint32_t HEADER_SIZE = 20;
		static const char HEADER_MAGIC[8] = { 'E', 'S', 'E', 'T', '-', 'V' , 'M', '2' };

		/*
		Make evm object from given file
		*/
		unique_ptr<Evm> makeEvmFromFile(string filename)
		{
			ifstream ifs(filename, ios::binary);
			if (!ifs.is_open()) {
				throw runtime_error("Bad filename: " + filename);
			}

			auto evm = make_unique<Evm>();

			// get file size
			ifs.seekg(0, ifs.end);
			evm->fileSize = ifs.tellg();
			ifs.seekg(0, ifs.beg);

			if (evm->fileSize < HEADER_SIZE) {
				throw runtime_error("File size too small");
			}

			for (int i = 0; i < 8; i++) {
				evm->header.magic[i] = ifs.get();
			}

			ifs.read(reinterpret_cast<char *>(&evm->header.codeSize), 4);
			ifs.read(reinterpret_cast<char *>(&evm->header.dataSize), 4);
			ifs.read(reinterpret_cast<char *>(&evm->header.initialDataSize), 4);

			Bytes newPayload(evm->fileSize - HEADER_SIZE);
			ifs.read(reinterpret_cast<char *>(newPayload.data()), newPayload.size());

			evm->payload.swap(newPayload);

			return move(evm);
		}

		void validateEvm(Evm & evm)
		{
			if (!(evm.header.dataSize >= evm.header.initialDataSize)) {
				throw runtime_error("Bad evm file format: data size < initilaDataSize");
			}

			if (!equal(begin(evm.header.magic), end(evm.header.magic),
				begin(HEADER_MAGIC), end(HEADER_MAGIC))) {
				throw runtime_error("Bad evm file format: wrong magic");
			}

			uint32_t size = evm.header.codeSize + evm.header.initialDataSize + HEADER_SIZE;
			if (size != evm.fileSize) {
				throw runtime_error("Bad evm file format: values in header don't match file size");
			}
		}

		pair<Bytes::iterator, Bytes::iterator> extractCode(Evm & evm)
		{
			if (evm.header.codeSize > evm.payload.size()) {
				throw out_of_range{ "" };
			}
			auto codeBeginIt = begin(evm.payload);
			auto codeEndIt = codeBeginIt + evm.header.codeSize;

			return make_pair(codeBeginIt, codeEndIt);
		}
		pair<Bytes::iterator, Bytes::iterator> extractInitializedData(Evm & evm)
		{
			if (evm.header.codeSize + evm.header.initialDataSize > evm.payload.size()) {
				throw out_of_range{ "" };
			}
			auto codeBeginIt = begin(evm.payload) + evm.header.codeSize;
			auto codeEndIt = end(evm.payload);

			return make_pair(codeBeginIt, codeEndIt);
		}
	}
}