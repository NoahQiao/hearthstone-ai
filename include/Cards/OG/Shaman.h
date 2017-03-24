#pragma once

namespace Cards
{
	struct Card_OG_026 : MinionCardBase<Card_OG_026> {
		static void Battlecry(Contexts::OnPlay context) {
			context.manipulate_.Board().Player(context.card_.GetPlayerIdentifier()).GetResource().UnlockOverload();
		}
	};
}

REGISTER_CARD(OG_026)