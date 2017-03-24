#pragma once

// http://www.hearthpwn.com/cards?filter-set=3&filter-premium=1&filter-class=1&sort=-cost&display=1
// finished: Argent Squire

namespace Cards
{
	struct Card_CS2_231 : public MinionCardBase<Card_CS2_231> {};

	struct Card_CS2_188_Enchant : public EnchantmentForThisTurn<Attack<2>> {
		static constexpr EnchantmentTiers tier = EnchantmentTiers::kEnchantmentTier1;
	};
	struct Card_CS2_188 : public MinionCardBase<Card_CS2_188> {
		static auto GetSpecifiedTargets(Contexts::SpecifiedTargetGetter context) {
			return Targets().Minion().Targetable();
		}
		static void Battlecry(Contexts::OnPlay context) {
			Manipulate(context).Card(context.GetTarget()).Enchant().Add<Card_CS2_188_Enchant>();
		}
	};

	struct Card_EX1_009_Enchant : public Enchantment<Attack<5>> {
		static constexpr EnchantmentTiers tier = EnchantmentTiers::kEnchantmentTier1;
	};
	struct Card_EX1_009 : public MinionCardBase<Card_EX1_009, Taunt> {
		template <typename Context>
		static auto GetEnrageTargets(Context&& context) {
			context.new_targets.insert(context.card_ref_);
		}
		Card_EX1_009() {
			Enrage<Card_EX1_009_Enchant>();
		}
	};

	struct Card_EX1_008 : public MinionCardBase<Card_EX1_008, Shield> {};

	struct Card_NEW1_025 : public MinionCardBase<Card_NEW1_025> {
		static void Battlecry(Contexts::OnPlay context) {
			state::PlayerIdentifier opponent = context.card_.GetPlayerIdentifier().Opposite();
			state::CardRef weapon = context.manipulate_.Board().Player(opponent).GetWeaponRef();
			if (!weapon.IsValid()) return;
			Manipulate(context).Weapon(weapon).Damage(context.card_ref_, 1);
		}
	};


	struct Card_EX1_089 : public MinionCardBase<Card_EX1_089> {
		static void Battlecry(Contexts::OnPlay context) {
			AnotherPlayer(context).GetResource().GainEmptyCrystal();
		}
	};

	struct Card_NEW1_038_Enchant : public Enchantment<Attack<1>, MaxHP<1>> {
		static constexpr EnchantmentTiers tier = EnchantmentTiers::kEnchantmentTier1;
	};
	struct Card_NEW1_038 : public MinionCardBase<Card_NEW1_038> {
		template <typename Context>
		static bool HandleEvent(state::CardRef self, Context&& context) {
			Manipulate(context).Card(self).Enchant().Add<Card_NEW1_038_Enchant>();
			return true;
		};

		Card_NEW1_038() {
			RegisterEvent<InPlayZone, NonCategorized_SelfInLambdaCapture,
				state::Events::EventTypes::OnTurnEnd>();
		}
	};

	struct Card_EX1_020 : public MinionCardBase<Card_EX1_020, Shield> {};
	struct Card_CS1_069 : public MinionCardBase<Card_CS1_069, Taunt> {};

	struct Card_CS2_203 : public MinionCardBase<Card_CS2_203> {
		static auto GetSpecifiedTargets(Contexts::SpecifiedTargetGetter context) {
			return Targets().Minion().Targetable();
		}
		static void Battlecry(Contexts::OnPlay context) {
			return Manipulate(context).Minion(context.GetTarget()).Silence();
		}
	};

	struct Card_EX1_390_Enchant : public Enchantment<Attack<3>> {
		static constexpr EnchantmentTiers tier = EnchantmentTiers::kEnchantmentTier1;
	};
	struct Card_EX1_390 : public MinionCardBase<Card_EX1_390, Taunt> {
		template <typename Context>
		static auto GetEnrageTargets(Context&& context) {
			context.new_targets.insert(context.card_ref_);
		}
		Card_EX1_390() {
			Enrage<Card_EX1_390_Enchant>();
		}
	};

	struct Card_EX1_564 : public MinionCardBase<Card_EX1_564> {
		static auto GetSpecifiedTargets(Contexts::SpecifiedTargetGetter context) {
			return Targets().Minion().Targetable();
		}
		static void Battlecry(Contexts::OnPlay context) {
			if (!context.GetTarget().IsValid()) return;

			state::CardRef new_ref = Manipulate(context).Minion(context.card_ref_)
				.BecomeCopyof(context.GetTarget());

			*context.new_card_ref = new_ref;
			*context.new_card = &context.manipulate_.Board().GetCard(new_ref);
		}
	};
}

REGISTER_CARD(NEW1_025)
REGISTER_CARD(EX1_564)
REGISTER_CARD(EX1_008)
REGISTER_CARD(EX1_009)
REGISTER_CARD(EX1_390)
REGISTER_CARD(CS2_188)
REGISTER_CARD(CS2_231)
REGISTER_CARD(CS2_203)

REGISTER_CARD(EX1_089)
REGISTER_CARD(NEW1_038)
REGISTER_CARD(EX1_020)
REGISTER_CARD(CS1_069)
