//! @file	Application.h
//! @author	Lukasz Iwanecki
//! @date	05.2018
//! @brief	EVM application
//! 
//! EVM application abstraction. It is responsible for parsing and running evm file.
//! It also provides data and program memories, thread list, locks list input file as well as
//! interfaces to these objects.
//! To run the application, you should fill the CliConfiguration structure or let it to be
//! captured from a console by getCliConfiguration(). Create an Application object 
//! with the configuration as a parameter. Invoke run() method and then wait() to suspend 
//! the main thread until the evm execution is ready. The application throws Evm::RuntimeErrors
//! that shoul be cought and print to an user
#pragma once

#include "stdafx.h"
#include "Memory.h"
#include "BitBuffer.h"
#include "EvmFile.h"

struct ThreadContext;

//! @namespace Evm
//!
//! Main namespace of Evm library
namespace Evm {
	//! @brief Evm configuration
	//!
	//! Configuration structure for Evm Application. Can be filled by hand or captured from
	//! console by getCliConfiguration()
	struct CliConfiguration {
		string evmFileName;		//!< File name of .evm executable
		string inputFileName;	//!< File name of user input file (if it is required)
		bool inputFileIsGiven;	//!< True if the input file is given.
		bool trace;				//!< True if command execution trace is enabled
	};

	//! @brief Main EVM application class
	//!
	//! The class is responsible for parsing and execution of EVM files.
	//! It is responsible for parsing and running evm file.
	//! It also provides data and program memories, thread list, locks list input file as well as
	//! interfaces to these objects.
	struct Application {
		using ThreadContexPtr = unique_ptr<ThreadContext>;
		using ThreadList = vector<ThreadContexPtr>;
		using LockList = map<uint64_t, mutex>;

		//! @brief Constructor
		//!
		//! The constructor is initializes thread list, lock list and other internal structures.
		//! It also parses an evm file, validate it, and initializes data and program memories.
		//! @param config reference to configuration file
		//! @throw RuntimeError
		Application(CliConfiguration & config);

		//! @brief Destructor
		~Application();

		//! @brief Run the evm execution
		//!
		//! Running the evm file execution. It creates at least one thread. 
		//! @note This is non-blocking function
		//! @throw RuntimeError
		void run();

		//! @brief Wait for execution
		//!
		//! The function waits until the main evm thread is done.
		//! If the main thread is done, other threads are terminated and joined.
		//! @note This is blocking function
		//! @throw RuntimeError
		void wait();

		//! @brief Spawn new evm thread
		//!
		//! API function for evm library. The function creates new evm thread based on caller. 
		//! The new thread's program counter is set to address value.
		//! @note This is non-blocking function
		//! @param caller reference to caller thread
		//! @param address initial value to the new thread program counter
		//! @return new thread ID
		//! @throw RuntimeError
		uint64_t runNewThread(ThreadContext & caller, uint32_t address);

		//! @brief Join a thread
		//!
		//! API function for evm library. Suspend current thread execution until 
		//! a thread with given ID is done
		//! @note This is blocking function
		//! @param threadId id of a related thread
		//! @throw RuntimeError
		void joinThread(uint64_t threadId);

		//! @brief Obtain a lock
		//!
		//! API function for evm library. Obtain a lock with given ID.
		//! When the lock doesn't exist it will be created. If the lock is
		//! already obtained, the thread will be suspended.
		//! @note The function is blocking when the lock is already obtained\n
		//! otherwise is is non-blocking
		//! @param lockID ID of the lock
		//! @throw RuntimeError
		void lock(uint64_t lockID);

		//! @brief Release a lock
		//!
		//! API function for evm library. Release a lock with given ID.
		//! When the lock doesn't exist BadLockIDRuntimeError exception will be thrown
		//! @note This is non-blocking function
		//! @param lockID ID of the lock
		//! @throw RuntimeError
		void unlock(uint64_t lockID);

		//! @brief Get reference to data memory
		//!
		//! API function for evm library. Getter of data memory.
		//! @return reference to data memory object
		Utils::Memory & dataMemory();

		//! @brief Get reference to program memory
		//!
		//! API function for evm library. Getter of program memory.
		//! Program memory is bit accessed. Due to thread safety, it is read only memory.
		//! @return reference to program memory
		const Utils::BitBuffer & programMemory() const;

		//! @brief Get reference to input file
		//!
		//! The function returns reference to inpute file, but only if the file is given.
		//! If the file is not given be an user the function throws InputFileRuntimeError
		//! exception
		//! @return reference to input file
		//! @throw RuntimeError
		fstream & inputFile();

		//! @brief Get input file name
		//!
		//! The function returns input file name
		//! @return input file name
		const string & inputFileName() const;

		const CliConfiguration & configuartion() const;

		Application() = delete;
		Application(const Application &) = delete;
		Application(Application &&) = delete;
		Application & operator=(const Application &) = delete;
		Application & operator=(Application &&) = delete;

	private:
		const CliConfiguration _config;
		unique_ptr<File::EvmFile> _evm;	//!< Pointer to evm file structure
		const Utils::BitBuffer _programMemory;	//!< Program memory as bit buffer
		Utils::Memory _dataMemory;				//!< Data memory
		ThreadList _threadList;			//!< List of evm threads
		LockList _lockList;				//!< Directory with evm locks
		fstream _inputFileStream;		//!< Stream to the input file. 
										//!< Valid only when _isInputFileGiven is true.

		//! @brief Helper function. Open and parse evm file, it fill and validate.
		//!
		//! Used in constructor to initialize _evm object.
		unique_ptr<File::EvmFile> _parseEvmFile(const CliConfiguration & config) const;

		//! @brief Helper function. Initialize program memory with data from evm file
		Utils::BitBuffer _extractProgramMemory(const File::EvmFile & evm) const;
	};

	//! @brief Get evm configuration from cli
	//!
	//! The function parses arguments from console and initializes cliConfig structure.
	//! @param argc argc parameter from main() finction
	//! @param argv argv parameter from main() function
	//! @param cliConfig reference, output from function
	void getCliConfiguration(int argc, char** argv, CliConfiguration & cliConfig);

	//! @brief Get hardcoded evm configuration (For tests)
	void getCliConfiguration(CliConfiguration & cliConfig);
}
