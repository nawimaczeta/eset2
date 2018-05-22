#pragma once

#include "stdafx.h"
#include "ThreadContext.h"
#include "Argument.h"

namespace Evm {
	namespace Operation {
		using ArgumentPtr = unique_ptr<Argument::IArgument>;
		using ArgumentList = vector<ArgumentPtr>;

		struct IOperation {
			IOperation(string & opcode) : _opcode{ opcode } {}
			virtual ~IOperation() = default;
			virtual void execute(ThreadContext & thread) = 0;

			void pushArgument(ArgumentPtr arg) {
				_argList.push_back(move(arg));
			}
			string trace(ThreadContext & thread) const;
		protected:
			string _opcode;
			ArgumentList _argList;

			friend ostream & operator<<(ostream & os, IOperation & op);
		};

		using OperationPtr = unique_ptr<IOperation>;

		struct MovOperation : IOperation {
			using IOperation::IOperation;
			virtual void execute(ThreadContext & thread) override;
		};

		struct LoadConstOperation : IOperation {
			using IOperation::IOperation;
			virtual void execute(ThreadContext & thread) override;
		};

		struct MathOperation : IOperation {
			MathOperation(string & opcode, function<int64_t(int64_t, int64_t)> mathOperation) :
				IOperation{ opcode },
				_mathOperation{ mathOperation }
			{}
			virtual void execute(ThreadContext & thread) override;
		private:
			function<int64_t(int64_t, int64_t)> _mathOperation;
		};

		struct ConsoleWriteOperation : IOperation {
			using IOperation::IOperation;
			virtual void execute(ThreadContext & thread) override;
		};

		struct ConsoleReadOperation : IOperation {
			using IOperation::IOperation;
			virtual void execute(ThreadContext & thread) override;
		};

		struct CallOperation : IOperation {
			using IOperation::IOperation;
			virtual void execute(ThreadContext & thread) override;
		};

		struct JumpOperation : IOperation {
			using IOperation::IOperation;
			virtual void execute(ThreadContext & thread) override;
		};

		struct HltOperation : IOperation {
			using IOperation::IOperation;
			virtual void execute(ThreadContext & thread) override;
		};

		struct RetOperation : IOperation {
			using IOperation::IOperation;
			virtual void execute(ThreadContext & thread) override;
		};

		struct JumpEqualOperation : IOperation {
			using IOperation::IOperation;
			virtual void execute(ThreadContext & thread) override;
		};

		struct CreateThreadOperation : IOperation {
			using IOperation::IOperation;
			virtual void execute(ThreadContext & thread) override;
		};

		struct JoinOperation : IOperation {
			using IOperation::IOperation;
			virtual void execute(ThreadContext & thread) override;
		};

		struct SleepOperation : IOperation {
			using IOperation::IOperation;
			virtual void execute(ThreadContext & thread) override;
		};

		struct LockOperation : IOperation {
			using IOperation::IOperation;
			virtual void execute(ThreadContext & thread) override;
		};

		struct UnlockOperation : IOperation {
			using IOperation::IOperation;
			virtual void execute(ThreadContext & thread) override;
		};

		struct WriteOperation : IOperation {
			using IOperation::IOperation;
			virtual void execute(ThreadContext & thread) override;
		};

		struct ReadOperation : IOperation {
			using IOperation::IOperation;
			virtual void execute(ThreadContext & thread) override;
		};
	}
}