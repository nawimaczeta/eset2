#pragma once

#include "stdafx.h"

namespace Evm {
	namespace File {

		struct EvmFile {
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

		unique_ptr<EvmFile> makeEvmFromFile(const string & filename);
		void validateEvm(const EvmFile & evm);
		pair<Bytes::const_iterator, Bytes::const_iterator> extractCode(const EvmFile & evm);
		pair<Bytes::const_iterator, Bytes::const_iterator> extractInitializedData(const EvmFile & evm);

		ostream & operator<<(ostream & os, EvmFile & evm);

	}
}