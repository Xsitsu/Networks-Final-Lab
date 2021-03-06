#include "packetreader.hpp"

#include "packetall.hpp"

namespace Network
{

PacketReader::PacketReader()
{

}

PacketReader::~PacketReader()
{

}

Packet::Base* PacketReader::ReadPacket(unsigned int packetSize, void *data)
{
    uint32_t connectionId = PacketReader::Read32(data);
    PacketFamily family = PacketReader::ReadFamily(data);
    PacketAction action = PacketReader::ReadAction(data);

    Packet::Base *ret = nullptr;

    switch (family)
    {
    case PacketFamily::FAMILY_CONNECT:
        {
        Packet::Connect *packet = new Packet::Connect(connectionId, action);
        packet->SetAssignedId(PacketReader::Read32(data));
        ret = packet;
        break;
        }
    case PacketFamily::FAMILY_DISCONNECT:
        {
        Packet::Disconnect *packet = new Packet::Disconnect(connectionId, action);
        ret = packet;
        break;
        }
    case PacketFamily::FAMILY_TERRAIN:
        {
        Packet::Terrain *packet = new Packet::Terrain(connectionId, action);

        uint8_t terrainSize = PacketReader::Read8(data);
        for (uint8_t i = 0; i < terrainSize; i++)
        {
            Packet::Terrain::TerrainData tData;
            tData.type = PacketReader::Read8(data);
            tData.posX = PacketReader::Read32(data);
            tData.posY = PacketReader::Read32(data);
            tData.sizeX = PacketReader::Read32(data);
            tData.sizeY = PacketReader::Read32(data);
            packet->AddTerrainData(tData);
        }

        ret = packet;
        break;
        }
    case PacketFamily::FAMILY_PLAYER:
        {
        Packet::Player *packet = new Packet::Player(connectionId, action);

        uint8_t numPlayers = PacketReader::Read8(data);
        for (uint8_t i = 0; i < numPlayers; i++)
        {
            Packet::Player::PlayerData pData;
            pData.playerId = PacketReader::Read32(data);
            pData.direction = PacketReader::Read8(data);
            pData.posX = PacketReader::Read32(data);
            pData.posY = PacketReader::Read32(data);
            pData.velX = PacketReader::Read32(data);
            pData.velY = PacketReader::Read32(data);
            packet->AddPlayerData(pData);
        }

        ret = packet;
        break;
        }
    default:
        {
        Packet::Base *packet = new Packet::Base(connectionId, family, action);
        ret = packet;
        break;
        }
    }

    return ret;
}


PacketFamily PacketReader::ReadFamily(void *&data)
{
    PacketFamily *d = static_cast<PacketFamily*>(data);
    PacketFamily val = *d;
    d++;
    data = d;
    return val;
}

PacketAction PacketReader::ReadAction(void *&data)
{
    PacketAction *d = static_cast<PacketAction*>(data);
    PacketAction val = *d;
    d++;
    data = d;
    return val;
}

uint32_t PacketReader::Read32(void *&data)
{
    uint32_t *d = static_cast<uint32_t*>(data);
    uint32_t val = *d;
    d++;
    data = d;
    return ntohl(val);
}

uint16_t PacketReader::Read16(void *&data)
{
    uint16_t *d = static_cast<uint16_t*>(data);
    uint16_t val = *d;
    d++;
    data = d;
    return ntohs(val);
}

uint8_t PacketReader::Read8(void *&data)
{
    uint8_t *d = static_cast<uint8_t*>(data);
    uint8_t val = *d;
    d++;
    data = d;
    return val;
}

}
