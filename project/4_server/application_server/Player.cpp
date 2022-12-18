#include "Player.h"

Player::Player()
    : m_advantages()
    , m_baseRegion()
    , m_ownedRegions()
{
    m_advantages.fill(true);
}

Player::Player(std::string name, std::string password)
    : User(1,name,password)
    , m_advantages()
    , m_baseRegion()
    , m_ownedRegions()
{
    m_advantages.fill(true);
}

Player::Player(const Player& player)
{
    *this = player;
}

Player::Player(const User& user)
    : User(user)
    , m_advantages()
    , m_baseRegion()
    , m_ownedRegions()
{
    m_advantages.fill(true);
}

Player& Player::operator=(const Player& player)
{
    m_advantages = player.m_advantages;
    m_baseRegion = player.m_baseRegion;
    m_ownedRegions = player.m_ownedRegions;
    m_id = player.m_id;
    m_name = player.m_name;
    m_password = player.m_password;

    return *this;
}

bool Player::operator==(const Player& player)
{
    return m_advantages==player.m_advantages
        && m_baseRegion==player.m_baseRegion
        && m_ownedRegions== player.m_ownedRegions
        && m_id==player.m_id
        && m_name==player.m_name
        && m_password==player.m_password;
}

const Region& Player::GetBaseRegion()
{
    return m_baseRegion;
}


const Region& Player::GetRegion(const Region::Coordinates& coordinates) const
{
    if (m_baseRegion.GetCoordinates() == coordinates)
    {
        return m_baseRegion;
    }
    return m_ownedRegions.at(coordinates);
}

unsigned int Player::GetScore() const
{
    unsigned int totalScore = 0;

    totalScore += m_baseRegion.GetScore();

    for (const auto& element : m_ownedRegions)
    {
        totalScore += element.second.GetScore();
    }
    return totalScore;
}

const std::array<bool, 3>& Player::GetAdvantages() const
{
    return m_advantages;
}

void Player::SetBaseRegion(const Region& region)
{
    m_baseRegion = region;
}


Region& Player::SetRegion(const Region::Coordinates& coordinates)
{
    if (m_baseRegion.GetCoordinates() == coordinates)
    {
        return m_baseRegion;
    }
    return m_ownedRegions[coordinates];
}

void Player::SetAdvantages(const std::array<bool, 3>& advantages)
{
    m_advantages = advantages;
}

void Player::InsertRegion(const Region& region)
{
    m_ownedRegions.insert(std::make_pair(region.GetCoordinates(),region));
}

bool Player::HasRegion(const Region::Coordinates& coordinates)
{
    if (m_ownedRegions.count(coordinates))
        return true;
    return false;
}

Region Player::ExtractRegion(const Region::Coordinates& coordinates)
{
    auto region = m_ownedRegions.extract(coordinates);

    if (region)
        return std::move(region.mapped());
    throw "Region not found.";
}

bool Player::UseAdvantage(uint8_t advantageIndex)
{
    if (!m_advantages[advantageIndex])
        return false;
    
    for (const std::pair<Region::Coordinates, Region>& regionPair : m_ownedRegions) {
        if (regionPair.second.GetScore() >= 200) {
            m_advantages[advantageIndex] = false;
            return true;
        }
    }

    return false;
}
