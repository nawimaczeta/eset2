
#include "stdafx.h"
#include "Application.h"

namespace Evm {

	BitBuffer Application::_getInstructions(Evm & evm)
	{
		auto its = File::extractInstructions(evm);
		return{ its.first, its.second };
	}

}