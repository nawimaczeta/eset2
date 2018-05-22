#include "stdafx.h"
#include "Operation.h"

namespace Evm {
	namespace Operation {
		void MovOperation::execute(ThreadContext & thread) {
			auto arg1Value = _argList.at(0)->getValue(thread);
			_argList.at(1)->setValue(thread, arg1Value);
		}

		void LoadConstOperation::execute(ThreadContext & thread) {
			uint64_t constValue = _argList.at(0)->getValue(thread);
			_argList.at(1)->setValue(thread, constValue);
		}

		void MathOperation::execute(ThreadContext & thread) {
			// Get arguments, convert them to signed numbers
			uint64_t arg1Value = _argList.at(0)->getValue(thread);
			uint64_t arg2Value = _argList.at(1)->getValue(thread);
			int64_t arg1Signed = *reinterpret_cast<int64_t *>(&arg1Value);
			int64_t arg2Signed = *reinterpret_cast<int64_t *>(&arg2Value);

			// compute math operation, convert back to usigned
			int64_t result = _mathOperation(arg1Signed, arg2Signed);
			uint64_t resultUnsigned = *reinterpret_cast<uint64_t *>(&result);
			_argList.at(2)->setValue(thread, resultUnsigned);
		}

		void ConsoleWriteOperation::execute(ThreadContext & thread) {
			static mutex sem;

			{
				// mutex protection of console output.
				// In my opinion this is a shared resource and should be protected by
				// implementon
				lock_guard<mutex> lock(sem);
				uint64_t value = _argList.at(0)->getValue(thread);
				ios_base::fmtflags flags{ cout.flags() };
				cout << "0x" << setfill('0') << setw(16) << hex << value << "\n";
				cout.flags(flags);
			}
		}

		void ConsoleReadOperation::execute(ThreadContext & thread) {
			uint16_t input;
			ios_base::fmtflags flags{ cin.flags() };
			cin >> hex >> input;
			cin.flags(flags);
			_argList.at(0)->setValue(thread, input);
		}

		void JumpOperation::execute(ThreadContext & thread) {
			uint32_t address = static_cast<uint32_t>(_argList.at(0)->getValue(thread));
			thread.programCounter(address);
		}

		void CallOperation::execute(ThreadContext & thread) {
			uint32_t address = static_cast<uint32_t>(_argList.at(0)->getValue(thread));
			thread.push(thread.programCounter());
			thread.programCounter(address);
		}

		void HltOperation::execute(ThreadContext & thread) {
			thread.terminate();
		}

		void RetOperation::execute(ThreadContext & thread) {
			thread.programCounter(thread.pop());
		}

		void JumpEqualOperation::execute(ThreadContext & thread) {
			uint32_t address = static_cast<uint32_t>(_argList.at(0)->getValue(thread));
			auto arg1Value = _argList.at(1)->getValue(thread);
			auto arg2Value = _argList.at(2)->getValue(thread);
			if (arg1Value == arg2Value) {
				thread.programCounter(address);
			}
		}

		void CreateThreadOperation::execute(ThreadContext & thread) {
			uint32_t address = static_cast<uint32_t>(_argList.at(0)->getValue(thread));
			uint64_t newThreadID = thread.application()->runNewThread(thread, address);
			_argList.at(1)->setValue(thread, newThreadID);
		}

		void JoinOperation::execute(ThreadContext & thread) {
			uint64_t threadId = _argList.at(0)->getValue(thread);
			thread.application()->joinThread(threadId);
		}

		void SleepOperation::execute(ThreadContext & thread) {
			uint64_t ms = _argList.at(0)->getValue(thread);
			thread.sleep(ms);
		}

		void LockOperation::execute(ThreadContext & thread) {
			uint64_t lockID = _argList.at(0)->getValue(thread);
			thread.application()->lock(lockID);
		}

		void UnlockOperation::execute(ThreadContext & thread) {
			uint64_t lockID = _argList.at(0)->getValue(thread);
			thread.application()->unlock(lockID);
		}

		void WriteOperation::execute(ThreadContext & thread) {
			static mutex sem;

			{
				// mutex protection of console output.
				// In my opinion this is a shared resource and should be protected by
				// implementon. I decided to implement the lock to achieve good
				// execution of multithreaded_file_write.evm example
				lock_guard<mutex> lock(sem);
				auto offset = _argList.at(0)->getValue(thread);
				auto numOfBytes = _argList.at(1)->getValue(thread);
				auto memoryAddress = _argList.at(2)->getValue(thread);
				auto & file = thread.application()->inputFile();
				auto & memory = thread.application()->dataMemory();

				Bytes dataToWrite = memory.read(memoryAddress, numOfBytes);

				file.seekp(offset);
				file.write(reinterpret_cast<const char *>(dataToWrite.data()), dataToWrite.size());
				if (!file.good()) {
					throw InputFileRuntimeError{ thread.application()->inputFileName(), "Unable to write. Offset: " + to_string(offset) +
						" address: " + to_string(memoryAddress) + " bytes: " + to_string(numOfBytes) };
				}
			}
		}

		void ReadOperation::execute(ThreadContext & thread) {
			auto offset = _argList.at(0)->getValue(thread);
			auto numOfBytes = _argList.at(1)->getValue(thread);
			auto memoryAddress = _argList.at(2)->getValue(thread);
			auto & file = thread.application()->inputFile();
			auto & memory = thread.application()->dataMemory();

			Bytes dataBuffer(numOfBytes);

			file.seekg(offset);
			file.read(reinterpret_cast<char *>(dataBuffer.data()), dataBuffer.size());
			auto bytesRead = file.gcount();
			_argList.at(3)->setValue(thread, static_cast<uint64_t>(bytesRead));
		}
	}
}