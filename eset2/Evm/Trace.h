#pragma once

#include "stdafx.h"

using namespace chrono;

namespace Evm {
	namespace Utils {
		struct Trace {
			Trace(string & filename, bool enableTrace) {
				if (enableTrace) {
					_outputFile.open(filename, ios::trunc);
				}
			}
			~Trace() {
				if (_outputFile.is_open()) _outputFile.close();
			}

			template <typename T>
			void log(T & msg) {
				if (_outputFile.is_open()) {
					auto timestamp = duration_cast<milliseconds>(
						system_clock::now().time_since_epoch()
						).count();
					_outputFile << timestamp << " " << msg << "\n";
				}
			}

			template <typename T>
			void log(uint32_t programCounter, T & msg) {
				ostringstream oss;
				oss << "0x" << setfill('0') << setw(8) << hex << 
					programCounter << ": " << msg;
				log(oss.str());
			}
		private:
			ofstream _outputFile;
		};
	}
}