﻿using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Runtime.Serialization;

namespace HearthstoneAI.Board
{
    [DataContract]
    class Hero
    {
        [DataMember]
        public int entity_id;

        [DataMember]
        public string card_id;

        [DataMember]
        public int max_hp;

        [DataMember]
        public int damage;

        [DataMember]
        public int armor;

        [DataMember]
        public int attack;

        [DataMember]
        public int attacks_this_turn;

        [DataMember]
        public Status status = new Status();

        [DataMember]
        public HeroPower hero_power = new HeroPower();

        [DataMember]
        public Enchantments enchantments = new Enchantments();
        
        public bool Parse(State.Game game, State.ReadOnlyEntity entity)
        {
            bool ret = true;

            this.entity_id = entity.Id;
            this.card_id = entity.CardId;

            this.max_hp = entity.GetTagOrDefault(State.GameTag.HEALTH, -1);
            this.damage = entity.GetTagOrDefault(State.GameTag.DAMAGE, 0);
            this.armor = entity.GetTagOrDefault(State.GameTag.ARMOR, 0);

            this.attack = entity.GetTagOrDefault(State.GameTag.ATK, 0);
            this.attacks_this_turn = entity.GetTagOrDefault(State.GameTag.NUM_ATTACKS_THIS_TURN, 0);

            ret = this.status.Parse(game, entity) && ret;

            State.ReadOnlyEntity hero_power;
            if (game.TryGetPlayerHeroPowerEntity(entity.Id, out hero_power))
            {
                this.hero_power.Parse(game, hero_power);
            }
            else
            {
                ret = false;
            }

            ret = this.enchantments.Parse(game, entity) && ret;

            return ret;
        }

        public override bool Equals(object obj)
        {
            Hero rhs = obj as Hero;
            if (rhs == null) return false;
            if (!this.entity_id.Equals(rhs.entity_id)) return false;
            if (!this.card_id.Equals(rhs.card_id)) return false;
            if (!this.max_hp.Equals(rhs.max_hp)) return false;
            if (!this.damage.Equals(rhs.damage)) return false;
            if (!this.armor.Equals(rhs.armor)) return false;
            if (!this.attack.Equals(rhs.attack)) return false;
            if (!this.attacks_this_turn.Equals(rhs.attacks_this_turn)) return false;
            if (!this.status.Equals(rhs.status)) return false;
            if (!this.hero_power.Equals(rhs.hero_power)) return false;
            if (!this.enchantments.Equals(rhs.enchantments)) return false;
            return true;
        }

        public override int GetHashCode()
        {
            return new
            {
                this.entity_id,
                this.card_id,
                this.max_hp,
                this.damage,
                this.armor,
                this.attack,
                this.attacks_this_turn,
                this.status,
                this.hero_power,
                this.enchantments
            }.GetHashCode();
        }
    }
}
