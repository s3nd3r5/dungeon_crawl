#include <iostream>
#include <string>
const int EXP_PER_LEVEL_MOD = 100;
const int EXP_PER_FLOOR_MOD = 50;
const int EXP_PER_KILL_MOD = 50;
const int BASE_HEALTH_QUOTIANT = 10;
const float ENEMIES_PER_FLOOR_MODIFIER = 1.5;
enum DamageType
{
	DARK,
	LIGHT,
	STRIKING,
	PIERCING,
	ELEMENTAL,
	NONE
};

class Weapon
{
public:
	Weapon(std::string name, int damage, DamageType damage_type)
	{
		this->name = name;
		this->damage = damage;
		this->damage_type = damage_type;
	}
	std::string name;
	int damage;
	DamageType damage_type;

};

class Armor
{
public:
	Armor(std::string name, DamageType weakness, DamageType immune, DamageType strong_against)
	{
		this->name = name;
		this->weakness = weakness;
		this->immune = immune;
		this->strong_against = strong_against;
	}
	std::string name;
	DamageType weakness;
	DamageType immune;
	DamageType strong_against;
};

class Actions
{
public:
	void clear()
	{
		this->defend = false;
		this->attack = false;
	}
	bool is_defending() { return this->defend; }
	bool is_attacking() { return this->attack; }
	void mark_defending() { this->defend = true; }
	void mark_attacking() { this->attack = true; }

	bool defend;
	bool attack;
};

class Character
{
public:
	Character(std::string name, int health, std::string class_name, Weapon* weapon, Armor* armor, int level)
	{
		this->name = name;
		this->health = health;
		this->class_name = class_name;
		this->weapon = weapon;
		this->armor = armor;		
		this->level = level;
		this->experience_points = 0;
		this->next_level_at = level * EXP_PER_LEVEL_MOD;
		this->action = new Actions();
	}

	void execute_action(Character* enemy)
	{
		if (this->action->is_attacking()) 
		{
			std::cout << this->name << " attacks!" << std::endl;
			this->attack(enemy);
		}
		else if (this->action->is_defending())
		{
			// Do nothing
			std::cout << this->name << " is defending!" << std::endl;
		}
		this->action->clear();

	}

	int defend(int damage, DamageType type)
	{
		if (this->action->is_defending())
		{
			std::cout << this->name << " has defended against the attack!" << std::endl;
			damage = damage * .75;
		}
		if (type == this->armor->weakness)
		{
		   return damage * 2;
		}
		else if (type == this->armor->immune)
		{
			return 0;
		}
		else if (type == this->armor->strong_against)
		{
			return damage / 2; 
		}
		else 
		{
			return damage;
		}
	}

	void take_damage(int damage, DamageType type) 
	{	

	   damage = this->defend(damage, type);
	   this->health = this->health - damage;
	   if (this->health < 0) 
	   {
		   this->health = 0;
	   }

	}

	bool is_alive()
	{
		return this->health > 0;
	}

	void attack(Character *c)
	{
		c->take_damage(this->weapon->damage, this->weapon->damage_type);
	}
	void gain_exp(int exp)
	{
		this->experience_points += exp;
		// You gained a level!
		if (this->experience_points > this->next_level_at)
		{
			this->experience_points %= this->next_level_at;
			this->level++;
			this->next_level_at = this->level * EXP_PER_LEVEL_MOD;
		}
		if (exp != 0)
		{
			this->gain_exp(0); //Check to see if you gained another level;
		}
	}
	void gain_exp(Character* c)
	{
		this->gain_exp(c->level * EXP_PER_KILL_MOD);
	}
	void print()
	{
		std::cout 
			<< "=== " << this->name << " ===" << std::endl 
			<< "Type: " << this->class_name << std::endl
			<< "Health: " << this->health << std::endl
			<< "Weapon: " << this->weapon->name << std::endl
			<< "Armor: " << this->armor->name << std::endl;
	}

	std::string name;
	std::string class_name;
	int health;
	int level;
	unsigned long experience_points;
	unsigned long next_level_at;
	Actions* action;
	Weapon* weapon;
	Armor* armor;
};

class Mage: public Character 
{
public:
	Mage(std::string name, int level): Character(
			name,
			BASE_HEALTH_QUOTIANT * 8,
			"Mage",
			new Weapon("Basic Staff", 1.5 * BASE_HEALTH_QUOTIANT, DamageType::ELEMENTAL),
			new Armor("Hunters Garb", DamageType::ELEMENTAL, DamageType::NONE, DamageType::NONE),
			level)
	{}
};

class Archer: public Character
{
public:
	Archer(std::string name, int level): Character(
			name,
			BASE_HEALTH_QUOTIANT * 11,
			"Archer",
			new Weapon("Basic Bow", 1 * BASE_HEALTH_QUOTIANT, DamageType::PIERCING),
			new Armor("Hunters Garb", DamageType::PIERCING, DamageType::NONE, DamageType::NONE),
			level)
	{}
};

class Warrior: public Character
{
public:
	Warrior(std::string name, int level): Character(
			name,
			BASE_HEALTH_QUOTIANT * 10,
			"Warrior",
			new Weapon("Basic Sword", 1.1 * BASE_HEALTH_QUOTIANT, DamageType::STRIKING),
			new Armor("Warriors Armor", DamageType::DARK, DamageType::NONE, DamageType::NONE),
			level)
	{}
};

void player_action(Character* player)
{
	std::string action;	
	std::cout 
		<< "What action do you want to take: " << std::endl
		<< "Attack, Defend: "; 
	std::cin >> action;
		
	if (action == "Attack")
	{
		player->action->mark_attacking();
	}
	else if (action == "Defend") 
	{
		player->action->mark_defending();
	}
}

void enemy_action(Character* enemy, Character* player)
{
	//implement logic
	enemy->action->mark_attacking();
}

Character* generate_enemy(int floor, int player_level)
{
	return new Character("Goblin",
			BASE_HEALTH_QUOTIANT * (floor + player_level),
			"Scout",
			new Weapon("Goblin Dagger", .75 * BASE_HEALTH_QUOTIANT, DamageType::PIERCING),
			new Armor("Tattered Garb", DamageType::STRIKING, DamageType::NONE, DamageType::NONE),
			(.5 * (floor + player_level)));
}
int main(void) 
{
	
	std::string name;	
	std::string class_name;
	Character* player;

	std::cout << "Setup your character: " << std::endl;
	std::cout << "What is your name: " << std::endl;
	
	std::cin >> name;

	std::cout << "What class do you want to be: " << std::endl;
	std::cout << "Mage, Warrior, Archer";
	std::cin >> class_name;

	if (class_name == "Mage") 
	{
		player = new Mage(name, 1);
	}
	else if (class_name == "Warrior")
	{
		player = new Warrior(name, 1);
	}
	else if (class_name == "Archer")
	{
		player = new Archer(name, 1);
	}

	bool is_running = true;
	int floor = 1;
	
	do {
		int enemies_per_floor = int(floor * ENEMIES_PER_FLOOR_MODIFIER);
		while(enemies_per_floor-- > 0)
		{
			Character* enemy = generate_enemy(floor, player->level);
			std::cout 
				<< "You have encountered a level " << enemy->level  
				<< " " << enemy->class_name 
				<< " on your travels" 
				<< std::endl;
			do {
				enemy->print();
				player->print();

				// Priority Queue based on stats?
				player_action(player);
				enemy_action(enemy, player);

				if (player->level > enemy->level) 
				{
					player->execute_action(enemy);
					enemy->execute_action(player);
				}
				else
				{
					enemy->execute_action(player);
					player->execute_action(enemy);
				}

			} while(enemy->is_alive());
			std::cout << player->name << " has defeated the " << enemy->name << " " << enemy->class_name << "!" << std::endl;
			player->gain_exp(enemy);
			delete enemy;
		}
		std::cout << player->name << " advances deeper into the dungeon" << std::endl;
		player->gain_exp(floor * EXP_PER_FLOOR_MOD);
	} while(is_running);
	
	return 0;
}
