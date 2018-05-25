//! @file	Operation.h
//! @author	Lukasz Iwanecki
//! @date	05.2018
//! @brief	Operation - abstraction which represent Evm instruction
//! 
//! IOperation interface represents Evm bytecode instruction. The operation interface has
//! execute() method which may be called by an application. Evm operation arguments
//! usually requires access to registers, thus the execution must be performed 
//! within a thread context. Each operation needs arguments which are represented
//! by @ref IArgument interface. The operations are capable of printing themselvs.
//! The opcode string is passed in constructor and the application may call trace()
//! function to get string with instruction opcode, printable list of arguments
//! and values of the arguments. Abstraction capable of making the Operations is
//! @ref IOperationFactory
#pragma once

#include "stdafx.h"
#include "ThreadContext.h"
#include "Argument.h"

//! @namespace Evm
//!
//! Main namespace of Evm library
namespace Evm {

	//! @namespace Operation
	//!
	//! Subnamespace with Operation classes, operation factories as well as other
	//! related functions
	namespace Operation {
		using ArgumentPtr = unique_ptr<Argument::IArgument>;
		using ArgumentList = vector<ArgumentPtr>;

		//! @brief IOperation interface. Abstraction of Evm instruction
		//!
		//! The class represents an Evm instruction. It provides three API methods,
		//! execute() - to run the operaction, trace() - to get printable representation
		//! and current values of arguemnts, and pushArgument() - to assign arguments.
		struct IOperation {
			//! @brief Constructor
			//!
			//! @param opcode Printable label of the instruction
			IOperation(string & opcode) : _opcode{ opcode } {}

			//! @brief Virtual destructor
			virtual ~IOperation() = default;

			//! @brief Execution of the instruction
			//!
			//! May by call by an application to execute the operation.
			//! The execution requires to be awared of thread context
			//! within the operation is executed.
			//! @param thread Context of Evm thread
			virtual void execute(ThreadContext & thread) = 0;

			//! @brief Assign argument to the operation
			//!
			//! The function is used to assign an argument to the operation.
			//! @note Arguments order has to match arguemnt order in documentation
			//! @param arg An argument
			void pushArgument(ArgumentPtr arg) {
				_argList.push_back(move(arg));
			}

			//! @brief Print the operation
			//!
			//! Return a strign with printable verson of the operation, arguments 
			//! and values of the arguments before execution
			//! @param thread Thread contex
			//! @note It is possible to print an operation with argument list
			//!		without current values. In that case the thread contex is not required
			//!		see << operator
			string trace(ThreadContext & thread) const;
		protected:
			string _opcode;				//!< Printable opcode
			ArgumentList _argList;		//!< Vector with arguments required by the instruction

			//! @brief Print the operation
			//!
			//! Stream output operator that prints the operation without
			//! runtime data
			friend ostream & operator<<(ostream & os, IOperation & op);
		};

		using OperationPtr = unique_ptr<IOperation>;

		//! @brief MathOperation Interface
		//!
		//! Math operation if a subinterface of IOperation that can execute mathematic
		//! instructions. It is good for three-argument operations like follows:
		//! arg3 <- arg1 # arg2
		//! where # is function given by an user. The interface takes lambda as
		//! an argument. At the moment the following instructions are implemented
		//! with this interface: add, sub, mul, dev, compare
		struct MathOperation : IOperation {
			//! @brief Constructor
			//!
			//! @param opcode Printable label of the instruction
			//! @param function Lambde with the essental operation
			MathOperation(string & opcode, function<int64_t(int64_t, int64_t)> mathOperation) :
				IOperation{ opcode },
				_mathOperation{ mathOperation }
			{}
			virtual void execute(ThreadContext & thread) override;
		private:
			function<int64_t(int64_t, int64_t)> _mathOperation;
		};

		//! @brief mov operation
		//!
		//! mov arg1, arg2;	arg2 <- arg1
		struct MovOperation : IOperation {
			using IOperation::IOperation;
			virtual void execute(ThreadContext & thread) override;
		};

		//! @brief loadConst operation
		//!
		//! loadConst constant,	arg1; arg1 <-constant
		struct LoadConstOperation : IOperation {
			using IOperation::IOperation;
			virtual void execute(ThreadContext & thread) override;
		};

		//! @brief consoleWrite operation
		//!
		//! consoleWrite arg1; Write arg1 to console, as hexadecimal value.
		struct ConsoleWriteOperation : IOperation {
			using IOperation::IOperation;
			virtual void execute(ThreadContext & thread) override;
		};

		//! @brief consoleRead operation
		//!
		//! consoleRead arg1; Read hexadecimal value from console and store to arg1
		struct ConsoleReadOperation : IOperation {
			using IOperation::IOperation;
			virtual void execute(ThreadContext & thread) override;
		};

		//! @brief call operation
		//!
		//! call address; Store address of instruction after the call to internal stack and
		//! continue execution at address
		struct CallOperation : IOperation {
			using IOperation::IOperation;
			virtual void execute(ThreadContext & thread) override;
		};

		//! @brief jump operation
		//!
		//! jump address; Move instruction pointer to address
		struct JumpOperation : IOperation {
			using IOperation::IOperation;
			virtual void execute(ThreadContext & thread) override;
		};

		//! @brief hlt operation
		//!
		//! hlt; End current thread. If initial thread is ended, end whole
		//! program.
		struct HltOperation : IOperation {
			using IOperation::IOperation;
			virtual void execute(ThreadContext & thread) override;
		};

		//! @brief ret operation
		//!
		//! ret; Take address from internal stack and continue execution from it
		struct RetOperation : IOperation {
			using IOperation::IOperation;
			virtual void execute(ThreadContext & thread) override;
		};

		//! @brief jumpEqual operation
		//!
		//! jumpEqual address, arg1, arg2;
		//! Move instruction pointer to address if arg1 == arg2.
		struct JumpEqualOperation : IOperation {
			using IOperation::IOperation;
			virtual void execute(ThreadContext & thread) override;
		};

		//! @brief createThread operation
		//!
		//! createThread address, arg1; Create a new thread, 
		//! starting at address and store it’s identifier to arg1.
		//! New thread starts with copy of current thread’s registers.
		struct CreateThreadOperation : IOperation {
			using IOperation::IOperation;
			virtual void execute(ThreadContext & thread) override;
		};

		//! @brief joinThread operation
		//!
		//! joinThread arg1; Wait till thread identified using arg1 ends and dispose its
		//! state. Threads will only be joined once.
		struct JoinOperation : IOperation {
			using IOperation::IOperation;
			virtual void execute(ThreadContext & thread) override;
		};

		//! @brief sleep operation
		//!
		//! sleep arg1; Delay execution of current thread by arg1 milliseconds
		struct SleepOperation : IOperation {
			using IOperation::IOperation;
			virtual void execute(ThreadContext & thread) override;
		};

		//! @brief lock operation
		//!
		//! lock arg1; Lock synchronization object identified by arg1
		struct LockOperation : IOperation {
			using IOperation::IOperation;
			virtual void execute(ThreadContext & thread) override;
		};

		//! @brief unlock operation
		//!
		//! unlock arg1; Unlock synchronization object identified by arg1
		struct UnlockOperation : IOperation {
			using IOperation::IOperation;
			virtual void execute(ThreadContext & thread) override;
		};

		//! @brief write operation
		//!
		//! write arg1, arg2, arg3; Write to binary output file using
		//! arg1 – offset in output file
		//!	arg2 – number of bytes to write
		//!	arg3 – memory address from which bytes will be written
		//!	If requested offset is larger than current output file size, file
		//!	should be padded with zeroes.
		struct WriteOperation : IOperation {
			using IOperation::IOperation;
			virtual void execute(ThreadContext & thread) override;
		};

		//! @brief read operation
		//!
		//! read arg1, arg2, arg3, arg4;
		//! Read from binary input file using
		//! arg1 – offset in input file
		//! arg2 – number of bytes to read
		//! arg3 – memory address to which read bytes will be stored
		//! After read operation, arg4 receives amount of bytes actually
		//! read – may be less than arg2, if not enough data exists in
		//! input file.
		struct ReadOperation : IOperation {
			using IOperation::IOperation;
			virtual void execute(ThreadContext & thread) override;
		};
	}
}