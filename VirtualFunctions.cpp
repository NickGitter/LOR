#include <iostream>
#include <vector>

class Weapon 
{
private:
    int _damageValue;
public:
    int getDamageValue() const 
    {
        return _damageValue;
    }
    Weapon(int damageValue) :_damageValue{ damageValue } 
    {
    }
};

class MeeleWeapon : public Weapon 
{
private:
    int _critChance;
public:
    MeeleWeapon(int damageValue, int critChance) :Weapon{ damageValue }, 
        _critChance(critChance)
    {
    }

    int getCritChance() const 
    {
        return _critChance;
    }
};

class GameUnit 
{
private:
    std::string _name;
public:
    GameUnit(const std::string& name) : _name(name) 
    {
    }
    GameUnit(const std::string&& name) : _name(name)
    {
    }
    GameUnit(const char* name) : _name(name)
    {
    }
public:
    std::string& getName() 
    {
        return _name;
    }
};

class BattleUnit : public GameUnit
{
protected:
    int _maxHealth{ 500 };
    int _health{ 500 };
    bool _isAlive{ true };
public:
    BattleUnit(const std::string& name, int health = 100) : GameUnit(name), _maxHealth{ health }, _health { health }, _isAlive{ true }
    {
    }
    BattleUnit(const std::string&& name, int health = 100) : GameUnit(name), _maxHealth{ health }, _health { health }, _isAlive{ true }
    {
    }
    BattleUnit(const char* name, int health = 100) : GameUnit(name), _maxHealth{ health }, _health{health}, _isAlive{ true }
    {
    }
public:
    virtual void attackOnAlive(BattleUnit* p_target) = 0;
    virtual void attackOnUndead(BattleUnit* p_target) = 0;
    virtual void attack(BattleUnit* p_target) 
    {
        if (isAlive())
        {
            if (p_target->isAlive())
            {
                attackOnAlive(p_target);
                p_target->onHit(this);
                if (!p_target->isAlive())
                    onKill(p_target);
            }
            else
            {
                attackOnUndead(p_target);
            }
        }
    }
    virtual void onHit(BattleUnit* p_damager) 
    {
        int damage = p_damager->getDamage();
        
        _health -= damage;
        
        std::cout << "Unit " << p_damager->getName() << " damage by " << damage << " total health = " << _health << std::endl;
        
        if (_health <= 0)
        {
            dead();
        }
    }
    virtual int getDamage() = 0;
    bool isAlive() const
    {
        return _isAlive;
    }
    void dead()
    {
        if (_isAlive) 
        {
            onDeath();
        }
        _isAlive = false;
    }
    virtual void onDeath() = 0;
    virtual void onKill(BattleUnit* p_target) 
    {
        if (_health < _maxHealth) 
        {
            _health += 100;
            if (_health > _maxHealth)
                _health = _maxHealth;
        }
    }
};

class Warrior : public BattleUnit
{
private:
    MeeleWeapon* _weapon{ nullptr };
public:
    void setWeapon(MeeleWeapon* weapon) 
    {
        _weapon = weapon;
    }

public:
    Warrior(const std::string& name,int health = 100) : BattleUnit(name,health)
    {
    }
    Warrior(const std::string&& name, int health = 100) : BattleUnit(name, health)
    {
    }
    Warrior(const char* name, int health = 100) : BattleUnit(name, health)
    {
    }
public:
    virtual void attackOnAlive(BattleUnit* p_target)
    {
        std::cout << "Warrior " << getName() << " attacked " << p_target->getName() << std::endl;
    }
    virtual void attackOnUndead(BattleUnit* p_target)
    {
        std::cout << "I cant fight with undead" << std::endl;
    }
    virtual void attack(BattleUnit* p_target)
    {
        BattleUnit::attack(p_target);
    }
    virtual void onHit(BattleUnit* p_damager) 
    {
        std::cout << "Warrior "<< getName() << " has been attacked by " << p_damager->getName() << std::endl;
        BattleUnit::onHit(p_damager);
    }
    
    virtual int getDamage() 
    {
        int damage = (rand() % 6) + 10;
        int critChance = 15;

        if (_weapon)
        {
            damage += _weapon->getDamageValue();
            critChance += _weapon->getCritChance();

            if (critChance >= 50)
                critChance = 50;

        }

        if ((rand()%100) < critChance)
            damage *= 2;
        return damage;
    }

    virtual void onDeath() 
    {
        std::cout << "I am running to VALHALA" << std::endl;
    }
    virtual void onKill(BattleUnit* p_target) 
    {
        std::cout << "Warrior " << getName() << " killed " << p_target->getName() << std::endl;
        BattleUnit::onKill(p_target);
    }
};

class Mage : public BattleUnit
{
private:
    int _intelligent{ 0 };
public:
    
    Mage(const std::string& name, int health = 100) : BattleUnit(name, health)
    {
    }
    Mage(const std::string&& name, int health = 100) : BattleUnit(name, health)
    {
    }
    Mage(const char* name, int health = 100) : BattleUnit(name, health)
    {
    }
public:
    void upIntelligent() 
    {
        if (_intelligent < 20)
        {
            ++_intelligent;
        }
    }
    virtual void attackOnAlive(BattleUnit* p_target)
    {
        std::cout << "Mage " << getName() << " attacked " << p_target->getName() << std::endl;
    }
    virtual void attackOnUndead(BattleUnit* p_target)
    {
        std::cout << "I cant cast magic on undead" << std::endl;
    }
    virtual void attack(BattleUnit* p_target)
    {
        BattleUnit::attack(p_target);
    }
    virtual void onHit(BattleUnit* p_damager)
    {
        std::cout << "Mage " << getName() << " has been attacked by " << p_damager->getName() << std::endl;
        BattleUnit::onHit(p_damager);
    }
    virtual int getDamage()
    {
        return 14 + _intelligent*4;
    }
    virtual void onDeath()
    {
        std::cout << "You cant defeat my soul" << std::endl;
    }
    virtual void onKill(BattleUnit* p_target)
    {
        std::cout << "Mage " << getName() << " killed " << p_target->getName() << std::endl;
        BattleUnit::onKill(p_target);
        std::cout << "Total health after kill: " << _health << std::endl;
    }
};

class Group 
{
private:
    std::string _name;
    std::vector<BattleUnit*> group;
public:
    std::string& getName()
    {
        return _name;
    }
    Group(const std::string& name) : _name{ name } 
    {
    }
    void addGameUnit(BattleUnit* p)
    {
        group.push_back(p);
    }
    void attackGroup(Group* targetGroup) 
    {
        if (this != targetGroup) 
        {
            if (targetGroup)
            {
                for (int i = 0; i < group.size(); ++i)
                {
                    for (int j = 0; j < targetGroup->group.size(); ++j)
                    {
                        group[i]->attack(targetGroup->group[j]);
                    }
                }
            }
        }
    }
    int getAliveUnitCount()
    {
        int aliveUnitCount = 0;
        for (int i = 0; i < group.size(); ++i)
        {
            if (group[i]->isAlive())
                ++aliveUnitCount;
        }
        return aliveUnitCount;
    }
};

class BattleArea 
{
private:
    std::vector<Group*> area;
public:
    void addGroup(Group* p)
    {
        area.push_back(p);
    }

    void fight()
    {
        int aliveCount;
        do 
        {
            aliveCount = 0;
            for (int i = 0; i < area.size(); ++i)
            {
                for (int j = 0; j < area.size(); ++j)
                {
                    if (i != j)
                    {
                        area[i]->attackGroup(area[j]);
                    }
                }
            }
            for (int i = 0; i < area.size(); ++i) 
            {
                if (area[i]->getAliveUnitCount() > 0)
                    ++aliveCount;
            }
        } while (aliveCount > 1);

        for (int i = 0; i < area.size(); ++i)
        {
            if (area[i]->getAliveUnitCount() > 0)
                std::cout << "Group \"" << area[i]->getName() << "\" is winner" << std::endl;;
        }
    }
};

class BattleField  
{
private:
    std::vector<BattleUnit*> army;
public:
    BattleField()
    {
    }
    void addGameUnit(BattleUnit* p)
    {
        army.push_back(p);
    }
    void attack() 
    {
        for (int i = 0; i < army.size(); ++i) 
        {
            for (int j = 0; j < army.size(); ++j) 
            {
                if (i != j)
                    army[i]->attack(army[j]);
            }
        }
    }
    void fight() 
    {
        int countOfAlive = 0;
        do 
        {
            attack();
            countOfAlive = 0;
            for (int i = 0; i < army.size(); ++i)
            {
                if (army[i]->isAlive())
                    ++countOfAlive;
            }
        } while (countOfAlive > 1);
        for (int i = 0; i < army.size(); ++i)
        {
            if (army[i]->isAlive())
                std::cout << army[i]->getName() << " The winner" << std::endl;
        }
    }

};

int main()
{
    srand(time(0));

    Mage Gendalf("Gendalf", 600);
    Gendalf.upIntelligent();
    Gendalf.upIntelligent();
    Gendalf.upIntelligent();

    Mage Sauron("Sauron", 700);

    Warrior Aragorn("Aragorn", 500);
    MeeleWeapon Sword(10, 10);
    Aragorn.setWeapon(&Sword);

    Warrior Gimly("Gimly", 700);
    MeeleWeapon Mace(5, 12);

    Gimly.setWeapon(&Mace);

    BattleArea ba;
    Group grLight("The light guys");
    Group grDark("Bad boys");

    grLight.addGameUnit(&Gendalf);
    grLight.addGameUnit(&Aragorn);

    grDark.addGameUnit(&Gimly);
    grDark.addGameUnit(&Sauron);

    ba.addGroup(&grLight);
    ba.addGroup(&grDark);

    ba.fight();
    /*
    BattleField bf;

    bf.addGameUnit(&Gendalf);
    bf.addGameUnit(&Sauron);
    bf.addGameUnit(&Aragorn);
    bf.addGameUnit(&Gimly);

    bf.fight();
    */
    return 0;
}