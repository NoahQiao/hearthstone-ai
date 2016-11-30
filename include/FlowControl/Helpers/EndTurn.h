#pragma once

#include "FlowControl/Result.h"
#include "FlowControl/Manipulate.h"
#include "FlowControl/Helpers/DamageDealer.h"
#include "FlowControl/Helpers/EntityDeathHandler.h"
#include "FlowControl/Manipulators/HeroManipulator.h"

namespace FlowControl
{
	namespace Helpers
	{
		template <class ActionParameterGetter, class RandomGenerator>
		class OnTurnEnd
		{
		public:
			OnTurnEnd(state::State & state, FlowContext & flow_context, ActionParameterGetter & action_parameters, RandomGenerator & random)
				: state_(state), flow_context_(flow_context), action_parameters_(action_parameters), random_(random)
			{

			}

			Result Go()
			{
				Result rc = kResultNotDetermined;

				state_.event_mgr.TriggerEvent<state::Events::EventTypes::OnTurnEnd>();
				if ((rc = EntityDeathHandler(state_, flow_context_).ProcessDeath()) != kResultNotDetermined) return rc;

				if (state_.turn == 89) return kResultDraw;
				++state_.turn;

				state_.ChangePlayer();

				state_.GetCurrentPlayer().resource_.IncreaseTotal();
				state_.GetCurrentPlayer().resource_.Refill();
				// TODO: overload

				state_.event_mgr.TriggerEvent<state::Events::EventTypes::OnTurnStart>();
				if ((rc = EntityDeathHandler(state_, flow_context_).ProcessDeath()) != kResultNotDetermined) return rc;

				Manipulate(state_, flow_context_).CurrentHero().DrawCard(random_);
				if ((rc = EntityDeathHandler(state_, flow_context_).ProcessDeath()) != kResultNotDetermined) return rc;

				return kResultNotDetermined;
			}

		private:
			Helpers::DamageDealer GetDamageDealer() { return DamageDealer(state_, flow_context_); }

		private:
			state::State & state_;
			FlowContext & flow_context_;
			ActionParameterGetter & action_parameters_;
			RandomGenerator & random_;
		};
	}
}