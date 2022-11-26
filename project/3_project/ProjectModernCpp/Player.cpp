#include "Player.h"

Player::Player(std::string name, std::string password)
    : m_name(name)
    , m_password(password)
{
    m_advantages.fill(true);
}

const Region& Player::GetBaseRegion()
{
    return m_baseRegion;
}
const std::string& Player::GetName() const
{
    return m_name;
}
const std::string& Player::GetPassword() const
{
    return m_password;
}

const Region& Player::GetRegion(const Region::Coordinates& coordinates)
{
    return m_ownedRegions[coordinates];
}

void Player::SetBaseRegion(const Region& region)
{
    m_baseRegion = region;
}
void Player::SetName(const std::string& name)
{
    m_name = name;
}
void Player::SetPassword(const std::string& password)
{
    m_password = password;
}

void Player::InsertRegion(const Region& region)
{
    m_ownedRegions.insert(std::make_pair(region.getCoordinates(),region));
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
        if (regionPair.second.getScore() >= 200) {
            m_advantages[advantageIndex] = false;
            return true;
        }
    }

    return false;
}
