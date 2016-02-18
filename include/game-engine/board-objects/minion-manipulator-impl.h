#include "game-engine/board.h"
#include "game-engine/board-objects/minion.h"
#include "game-engine/board-objects/minion-manipulator.h"

inline int GameEngine::BoardObjects::MinionManipulator::GetHP() const
{
	return this->minion.stat.GetHP();
}

inline int GameEngine::BoardObjects::MinionManipulator::GetMaxHP() const
{
	return this->minion.stat.GetMaxHP();
}

inline int GameEngine::BoardObjects::MinionManipulator::GetAttack() const
{
	return this->minion.stat.GetAttack();
}

inline void GameEngine::BoardObjects::MinionManipulator::TakeDamage(int damage, bool poisonous)
{
	if (this->minion.stat.IsShield()) {
		this->minion.stat.SetShield(false);
	}
	else {
		this->minion.stat.SetHP(this->minion.stat.GetHP() - damage);

		if (poisonous) {
			this->GetMinions().MarkPendingRemoval(*this);
		}

		this->HookMinionCheckEnraged();
	}
}

inline bool GameEngine::BoardObjects::MinionManipulator::IsForgetful() const
{
	return this->minion.stat.IsForgetful();
}

inline void GameEngine::BoardObjects::MinionManipulator::AttackedOnce()
{
	this->minion.attacked_times++;
	if (this->minion.stat.IsStealth()) this->minion.stat.SetStealth(false);
}

inline void GameEngine::BoardObjects::MinionManipulator::SetFreezeAttacker(bool freeze)
{
	this->minion.stat.SetFreezeAttacker(freeze);
}

inline void GameEngine::BoardObjects::MinionManipulator::SetFreezed(bool freezed)
{
	this->minion.stat.SetFreezed(freezed);
}

inline bool GameEngine::BoardObjects::MinionManipulator::IsFreezeAttacker() const
{
	return this->minion.stat.IsFreezeAttacker();
}

inline bool GameEngine::BoardObjects::MinionManipulator::IsFreezed() const
{
	return this->minion.stat.IsFreezed();
}

inline bool GameEngine::BoardObjects::MinionManipulator::IsPoisonous() const
{
	return this->minion.stat.IsPoisonous();
}

inline void GameEngine::BoardObjects::MinionManipulator::AddAttack(int val)
{
	this->minion.stat.SetAttack(this->minion.stat.GetAttack() + val);
}

inline void GameEngine::BoardObjects::MinionManipulator::IncreaseCurrentAndMaxHP(int val)
{
#ifdef DEBUG
	if (val < 0) throw std::runtime_error("should we trigger heal? enrage effect? damaged effect? use TakeDamage() for that.");
#endif
	this->minion.stat.SetMaxHP(this->minion.stat.GetMaxHP() + val);
	this->minion.stat.SetHP(this->minion.stat.GetHP() + val);

	// no need to check enrage, since we add the hp and max-hp by the same amount
}

inline void GameEngine::BoardObjects::MinionManipulator::DecreaseMaxHP(int val)
{
	this->minion.stat.SetMaxHP(this->minion.stat.GetMaxHP() - val);
	this->minion.stat.SetHP(std::min(this->minion.stat.GetHP(), this->minion.stat.GetMaxHP()));

	this->HookMinionCheckEnraged(); // might become un-enraged if max-hp lowered to current-hp
}

inline void GameEngine::BoardObjects::MinionManipulator::AddOnDeathTrigger(Minion::OnDeathTrigger func)
{
	this->minion.triggers_on_death.push_back(func);
}

inline std::list<GameEngine::BoardObjects::Minion::OnDeathTrigger> GameEngine::BoardObjects::MinionManipulator::GetAndClearOnDeathTriggers()
{
	std::list<GameEngine::BoardObjects::Minion::OnDeathTrigger> ret;
	this->minion.triggers_on_death.swap(ret);
	return ret;
}

inline void GameEngine::BoardObjects::MinionManipulator::SetMinionStatFlag(MinionStat::Flag flag, bool val)
{
	this->minion.stat.SetFlag(flag, val);
}

inline void GameEngine::BoardObjects::MinionManipulator::AddAura(Aura * aura)
{
	this->minion.auras->Add(*this, aura);
}

inline void GameEngine::BoardObjects::MinionManipulator::ClearAuras()
{
	this->minion.auras->Clear(*this);
}

inline void GameEngine::BoardObjects::MinionManipulator::AddEnchantment(Enchantment<MinionManipulator> * enchantment, EnchantmentOwner * owner)
{
	this->minion.enchantments->Add(enchantment, owner, *this);
}

inline void GameEngine::BoardObjects::MinionManipulator::RemoveEnchantment(Enchantment<MinionManipulator> * enchantment)
{
	this->minion.enchantments->Remove(enchantment, *this);
}

inline void GameEngine::BoardObjects::MinionManipulator::ClearEnchantments()
{
	this->minion.enchantments->Clear(*this);
}

inline void GameEngine::BoardObjects::MinionManipulator::HookAfterMinionAdded(MinionManipulator & added_minion)
{
	this->minion.auras->HookAfterMinionAdded(*this, added_minion);
}

inline void GameEngine::BoardObjects::MinionManipulator::HookMinionCheckEnraged()
{
	auto & minion = this->minion;
	if (this->GetHP() < this->GetMaxHP()) {
		minion.auras->HookAfterOwnerEnraged(*this); // enraged
	}
	else if (this->GetHP() == this->GetMaxHP()) {
		minion.auras->HookAfterOwnerUnEnraged(*this); // un-enraged
	}
	else {
		throw std::runtime_error("hp should not be larger than max-hp");
	}
}

inline void GameEngine::BoardObjects::MinionManipulator::TurnStart(bool owner_turn)
{
	this->minion.summoned_this_turn = false;
	this->minion.attacked_times = 0;
}

inline void GameEngine::BoardObjects::MinionManipulator::TurnEnd(bool owner_turn)
{
	if (owner_turn) {
		// check thaw
		// Note: if summon in this turn, and freeze it, then the minion will not be unfrozen
		if (this->minion.attacked_times == 0 && !this->minion.summoned_this_turn) {
			if (this->IsFreezed()) this->minion.stat.SetFreezed(false);
		}
	}

	this->minion.enchantments->TurnEnd(*this);
}

inline bool GameEngine::BoardObjects::MinionManipulator::IsPlayerSide() const
{
	return &this->board.object_manager.player_minions == &this->minions;
}

inline bool GameEngine::BoardObjects::MinionManipulator::IsOpponentSide() const
{
	return &this->board.object_manager.opponent_minions == &this->minions;
}