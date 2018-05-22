#pragma once

#include "stdafx.h"
#include "ThreadContext.h"
#include "Argument.h"
#include "Application.h"

namespace Evm {
	namespace Operation {
		using ArgumentPtr = unique_ptr<Argument::IArgument>;
		using ArgumentList = vector<ArgumentPtr>;

		struct IOperation {
			virtual ~IOperation() = default;
			virtual void execute(ThreadContext & thread) = 0;

			void pushArgument(ArgumentPtr arg) {
				_argList.push_back(move(arg));
			}
		protected:
			ArgumentList _argList;
		};

		using OperationPtr = unique_ptr<IOperation>;

		struct MovOperation : IOperation {
			virtual void execute(ThreadContext & thread) override;
		};

		struct LoadConstOperation : IOperation {
			virtual void execute(ThreadContext & thread) override;
		};

		struct MathOperation : IOperation {
			MathOperation(function<int64_t(int64_t, int64_t)> mathOperation) :
				_mathOperation{ mathOperation }
			{}
			virtual void execute(ThreadContext & thread) override;
		private:
			function<int64_t(int64_t, int64_t)> _mathOperation;
		};

		struct ConsoleWriteOperation : IOperation {
			virtual void execute(ThreadContext & thread) override;
		};

		struct ConsoleReadOperation : IOperation {
			virtual void execute(ThreadContext & thread) override;
		};

		struct CallOperation : IOperation {
			virtual void execute(ThreadContext & thread) override;
		};

		struct JumpOperation : IOperation {
			virtual void execute(ThreadContext & thread) override;
		};

		struct HltOperation : IOperation {
			virtual void execute(ThreadContext & thread) override;
		};

		struct RetOperation : IOperation {
			virtual void execute(ThreadContext & thread) override;
		};

		struct JumpEqualOperation : IOperation {
			virtual void execute(ThreadContext & thread) override;
		};

		struct CreateThreadOperation : IOperation {
			virtual void execute(ThreadContext & thread) override;
		};

		struct JoinOperation : IOperation {
			virtual void execute(ThreadContext & thread) override;
		};

		struct SleepOperation : IOperation {
			virtual void execute(ThreadContext & thread) override;
		};

		struct LockOperation : IOperation {
			virtual void execute(ThreadContext & thread) override;
		};

		struct UnlockOperation : IOperation {
			virtual void execute(ThreadContext & thread) override;
		};

		struct WriteOperation : IOperation {
			virtual void execute(ThreadContext & thread) override;
		};

		struct ReadOperation : IOperation {
			virtual void execute(ThreadContext & thread) override;
		};
	}
}