#pragma once
#include "FlowControl/Manipulators/CardManipulator.h"

#include "state/State.h"

namespace FlowControl
{
	namespace Manipulators
	{
		inline state::Cards::Card & CardManipulator::GetCard()
		{
			return state_.GetMutableCard(card_ref_);
		}
	}
}