#include "stdafx.h"
#include "Operation.h"

namespace Evm {
	namespace Operation {
		void MovOperation::execute(ThreadContext & thread) {
			auto arg1Value = _arg1->getValue(thread);
			_arg2->setValue(thread, arg1Value);
		}

		void LoadConstOperation::execute(ThreadContext & thread) {
			_arg1->setValue(thread, _constant);
		}

		void MathOperation::execute(ThreadContext & thread) {
			// Get arguments, convert them to signed numbers
			uint64_t arg1Value = _arg1->getValue(thread);
			uint64_t arg2Value = _arg2->getValue(thread);
			int64_t arg1Signed = *reinterpret_cast<int64_t *>(&arg1Value);
			int64_t arg2Signed = *reinterpret_cast<int64_t *>(&arg2Value);

			// compute math operation, convert back to usigned
			int64_t result = _mathOperation(arg1Signed, arg2Signed);
			uint64_t resultUnsigned = *reinterpret_cast<uint64_t *>(&result);
			_arg3->setValue(thread, resultUnsigned);
		}

		void ConsoleWriteOperation::execute(ThreadContext & thread) {
			uint64_t value = _arg1->getValue(thread);
			ios_base::fmtflags flags{ cout.flags() };
			cout << "0x" << setfill('0') << setw(16) << hex << value << "\n";
			cout.flags(flags);
		}

		void ConsoleReadOperation::execute(ThreadContext & thread) {
			uint16_t input;
			ios_base::fmtflags flags{ cin.flags() };
			cin >> hex >> input;
			cin.flags(flags);
			_arg1->setValue(thread, input);
		}

		void JumpOperation::execute(ThreadContext & thread) {
			thread.programCounter(_address);
		}

		void CallOperation::execute(ThreadContext & thread) {
			thread.push(thread.programCounter());
			thread.programCounter(_address);
		}

		void HltOperation::execute(ThreadContext & thread) {
			thread.terminate();
		}

		void RetOperation::execute(ThreadContext & thread) {
			thread.programCounter(thread.pop());
		}

		void JumpEqualOperation::execute(ThreadContext & thread) {
			auto arg1Value = _arg1->getValue(thread);
			auto arg2Value = _arg2->getValue(thread);
			if (arg1Value == arg2Value) {
				thread.programCounter(_address);
			}
		}

		void CreateThreadOperation::execute(ThreadContext & thread) {
			uint64_t newThreadID = thread.application()->runNewThread(thread, _address);
			_arg1->setValue(thread, newThreadID);
		}

		void JoinOperation::execute(ThreadContext & thread) {
			uint64_t threadId = _arg1->getValue(thread);
			thread.application()->joinThread(threadId);
		}

		void SleepOperation::execute(ThreadContext & thread) {
			uint64_t ms = _arg1->getValue(thread);
			thread.sleep(ms);
		}

		void LockOperation::execute(ThreadContext & thread) {
			uint64_t lockID = _arg1->getValue(thread);
			thread.application()->lock(lockID);
		}

		void UnlockOperation::execute(ThreadContext & thread) {
			uint64_t lockID = _arg1->getValue(thread);
			thread.application()->unlock(lockID);
		}

		void WriteOperation::execute(ThreadContext & thread) {
			auto offset = _arg1->getValue(thread);
			auto numOfBytes = _arg2->getValue(thread);
			auto memoryAddress = _arg3->getValue(thread);
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

		void ReadOperation::execute(ThreadContext & thread) {
			auto offset = _arg1->getValue(thread);
			auto numOfBytes = _arg2->getValue(thread);
			auto memoryAddress = _arg3->getValue(thread);
			auto & file = thread.application()->inputFile();
			auto & memory = thread.application()->dataMemory();

			Bytes dataBuffer(numOfBytes);

			file.seekg(offset);
			file.read(reinterpret_cast<char *>(dataBuffer.data()), dataBuffer.size());
			auto bytesRead = file.gcount();
			_arg4->setValue(thread, static_cast<uint64_t>(bytesRead));
		}
	}
}