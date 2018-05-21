
#include "stdafx.h"
#include "Application.h"
#include "tclap/CmdLine.h"

namespace Evm {
	Application::Application(CliConfiguration & config) :
		_evm{ _parseEvmFile(config) },
		_programMemory{ _extractProgramMemory(*_evm) },
		_dataMemory{ _evm->header.dataSize },
		_inputFileName{config.inputFileName},
		_isInputFileGiven{ config.inputFileIsGiven }
	{
		cout << *_evm << "\n";

		// Copy initialized data to beginning of data memory
		auto initDataIts = File::extractInitializedData(*_evm);
		_dataMemory.write(0, initDataIts.first, initDataIts.second);

		// open input file if it is given by an user
		if (_isInputFileGiven) {
			_inputFileStream.open(_inputFileName, fstream::in | fstream::out | fstream::binary | fstream::app);
			if (!_inputFileStream.is_open()) {
				throw InputFileRuntimeError{ _inputFileName, "Unable to open" };
			}
		}
	}

	Application::~Application()
	{
		if (_isInputFileGiven) {
			_inputFileStream.close();
		}
	}

	void Application::run()
	{
		//cout << "Application: spawn main thread\n";
		// create first thread
		_threadList.emplace_back(make_unique<ThreadContext>(this));
		_threadList.at(0)->run();
		//cout << "Application: main thread is alive!\n";
	}

	void Application::wait()
	{
		// join main thread
		//cout << "Application: joining with main thread\n";
		_threadList.at(0)->join();
		//cout << "Application: main thread is over\n";
	}

	uint64_t Application::runNewThread(ThreadContext & caller, uint32_t address)
	{
		// create new Thread from the caller
 		_threadList.emplace_back(make_unique<ThreadContext>(caller, address));
		uint64_t newThreadId = _threadList.size() - 1;
		_threadList.at(newThreadId)->run();
		return newThreadId;
	}

	void Application::joinThread(uint64_t threadId)
	{
		try {
			ThreadContext & thread = *_threadList.at(threadId);
			thread.join();
		}
		catch (out_of_range & e) {
			(void)e;
			throw UnknownThreadRuntimeError(threadId);
		}
	}

	void Application::lock(uint64_t lockID)
	{
		// get mutex or create new if it doesn't exist
		mutex & m = _lockList[lockID];
		m.lock();
	}

	void Application::unlock(uint64_t lockID)
	{
		try {
			mutex & m = _lockList.at(lockID);
			m.unlock();
		}
		catch (out_of_range & e) {
			(void)e;
			throw BadLockIDRuntimeError(lockID);
		}
	}

	Memory & Application::dataMemory()
	{
		return _dataMemory;
	}

	const BitBuffer & Application::programMemory() const
	{
		return _programMemory;
	}

	fstream & Application::inputFile()
	{
		if (!_isInputFileGiven) {
			throw InputFileRuntimeError{ "Unknown user file. Give user file when lunch the application (-i filename)" };
		}
		return _inputFileStream;
	}

	const string & Application::inputFileName() const
	{
		return _inputFileName;
	}

	unique_ptr<Evm> Application::_parseEvmFile(const CliConfiguration & config) const
	{
		auto evm = File::makeEvmFromFile(config.evmFileName);
		File::validateEvm(*evm);
		return move(evm);
	}

	BitBuffer Application::_extractProgramMemory(const Evm & evm) const
	{
		auto its = File::extractCode(evm);
		return{ its.first, its.second };
	}

	void getCliConfiguration(int argc, char ** argv, CliConfiguration & cliConfig)
	{
		try {
			TCLAP::CmdLine cmd("Command description message", ' ', "0.9");
			TCLAP::UnlabeledValueArg<string> evmFilenameArg("evm", "evm file name", true, "", "filename");
			TCLAP::ValueArg<std::string> filenameArg("i", "input_file", "Input file", false, "in", "filename");
			cmd.add(evmFilenameArg);
			cmd.add(filenameArg);

			cmd.parse(argc, argv);

			cliConfig.evmFileName = evmFilenameArg.getValue();
			cliConfig.inputFileIsGiven = filenameArg.isSet();
			cout << "inputFileIsGiven? " << cliConfig.inputFileIsGiven << "\n";
			cliConfig.inputFileName = filenameArg.getValue();
		}
		catch (TCLAP::ArgException &e)  // catch any exceptions
		{
			throw CliConfigurationRuntimeError{ e.error() + " for arg " + e.argId() };
		}
	}
	void getCliConfigurationHardcoded(CliConfiguration & cliConfig)
	{
		//const string EVM_FILE_NAME{ "input/math.evm" };
		//const string EVM_FILE_NAME{ "input/memory.evm" };
		//const string EVM_FILE_NAME{ "input/xor.evm" };
		//const string EVM_FILE_NAME{ "input/xor-with-stack-frame.evm" };
		//const string EVM_FILE_NAME{ "input/fibonacci_loop.evm" };
		//const string EVM_FILE_NAME{ "input/threadingBase.evm" };
		//const string EVM_FILE_NAME{ "input/philosophers.evm" };
		//const string EVM_FILE_NAME{ "input/lock.evm" };
		//const string EVM_FILE_NAME{ "input/pseudorandom.evm" };
		//const string EVM_FILE_NAME{ "input/sleep_test.evm" };
		const string EVM_FILE_NAME{ "input/multithreaded_file_write.evm" };
		//const string EVM_FILE_NAME{ "input/my/multithreaded_file_write.evm" };
		//const string EVM_FILE_NAME{ "input/crc.evm" };

		cliConfig.evmFileName = EVM_FILE_NAME;
		cliConfig.inputFileName = "input/multithreaded_file_write.bin";
		//cliConfig.inputFileName = "input/my/multithreaded_file_write.bin";
		//cliConfig.inputFileName = "input/crc.bin";
		//cliConfig.inputFileIsGiven = true;
		cliConfig.inputFileIsGiven = true;
	}
}