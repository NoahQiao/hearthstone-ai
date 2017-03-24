#pragma once

#include "FlowControl/FlowContext.h"
#include "FlowControl/Manipulate.h"

namespace FlowControl {
	inline void FlowContext::PrepareSpecifiedTarget(state::State & state, state::CardRef card_ref, const state::Cards::Card & card, state::targetor::Targets const & target_info)
	{
		assert(!specified_target_.IsValid());
		std::vector<state::CardRef> targets;
		target_info.Fill(Manipulate(state, *this), targets);
		specified_target_ = action_parameters_.GetSpecifiedTarget(state, card_ref, card, targets);
		assert(specified_target_.IsValid());
	}

	inline void FlowContext::AddDeadEntryHint(state::State & state, state::CardRef ref)
	{
		int play_order = state.GetCardsManager().Get(ref).GetPlayOrder();

		dead_entity_hints_.insert(std::make_pair(play_order, ref));
	}

	inline bool FlowContext::Empty() const
	{
		if (!dead_entity_hints_.empty()) return false;
		if (destroyed_weapon_.IsValid()) return false;
		return true;
	}
}