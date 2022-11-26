#include "Player.h"

Player::Player(std::string name, std::string password)
    : m_name(name)
    , m_password(password)
{
    /* EMPTY */
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
    return m_password ;
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

Region Player::ExtractRegion(const Region::Coordinates& coordinates)
{
    auto region = m_ownedRegions.extract(coordinates);

    if (region)
        return std::move(region.mapped());
    throw "Region not found.";
}
