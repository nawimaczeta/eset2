#pragma once

#include "stdafx.h"

namespace Evm {

	struct Evm {
		struct Header {
			char magic[8];
			uint32_t codeSize;
			uint32_t dataSize;
			uint32_t initialDataSize;
		};

		Header header;
		Bytes payload;
		size_t fileSize;
	};

	ostream & operator<<(ostream & os, Evm & evm);

	namespace File {
		unique_ptr<Evm> makeEvmFromFile(string filename);
		void validateEvm(const Evm & evm);
		pair<Bytes::const_iterator, Bytes::const_iterator> extractCode(const Evm & evm);
		pair<Bytes::const_iterator, Bytes::const_iterator> extractInitializedData(const Evm & evm);
	}
}