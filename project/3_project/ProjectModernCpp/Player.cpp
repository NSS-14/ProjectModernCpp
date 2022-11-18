#include "Player.h"

Region Player::GetBase()
{
    return m_baseRegion;
}

void Player::SetBase(const Region& region)
{
    m_baseRegion = region;
}

const std::string& Player::GetName() const
{
    return m_name;
}

void Player::SetName(std::string& name)
{
    m_name = name;
}

const std::string& Player::GetPassword() const
{
    return m_password ;
}

void Player::SetPassword(std::string& password)
{
    m_password = password;
}

void Player::InsertRegion(const Region& region)
{
    m_ownedRegions.insert(std::make_pair(region.getCoordinates(),region));
}

Region Player::ExtractRegion(Region::Coordinates coordinates)
{
    if (coordinates == m_baseRegion.getCoordinates())
        return m_baseRegion;
    return m_ownedRegions[coordinates];
}
