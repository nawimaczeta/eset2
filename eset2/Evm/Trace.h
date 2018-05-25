//! @file	Trace.h
//! @author	Lukasz Iwanecki
//! @date	05.2018
//! @brief	Trace class declaration
//! 
//! Trace class is responsible for logging execution trace from threads.
#pragma once

#include "stdafx.h"

using namespace chrono;

namespace Evm {
	namespace Utils {
		//! Trace class is responsible for logging execution trace from threads.
		struct Trace {
			//! @Constructor
			//!
			//! @param filename File name of log file
			//! @param true if trace is enabled
			Trace(const string & filename, bool enableTrace) {
				if (enableTrace) {
					_outputFile.open(filename, ios::trunc);
				}
			}

			//! @brief Destructor
			~Trace() {
				if (_outputFile.is_open()) _outputFile.close();
			}

			//! @brief Log message
			//!
			//! Log message @ref msg to file. Timestamp is added at beginning
			//! of the line.
			//! @param msg Message to be logged
			template <typename T>
			void log(T & msg) {
				if (_outputFile.is_open()) {
					auto timestamp = duration_cast<milliseconds>(
						system_clock::now().time_since_epoch()
						).count();
					_outputFile << timestamp << " " << msg << "\n";
				}
			}

			//! @brief Log message
			//!
			//! Log message @ref msg to file. Add timestamp and program counter
			//! @param programCounter Value of program counter
			//! @param msg Message to be logged
			template <typename T>
			void log(uint32_t programCounter, T & msg) {
				ostringstream oss;
				oss << "0x" << setfill('0') << setw(8) << hex << 
					programCounter << ": " << msg;
				log(oss.str());
			}
		private:
			ofstream _outputFile;		//!< Log file
		};
	}
}