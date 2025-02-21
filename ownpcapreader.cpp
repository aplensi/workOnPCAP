#include "ownpcapreader.h"

ownPcapReader::ownPcapReader(const char *file) : packetCount(0)
{
    ifs.open(file, std::ios::binary);
    if (!file) {
        std::cerr << "Не удалось открыть файл." << std::endl;
        return;
    }

    readGlobalHeader();
    countPackets();
    createListOfPackages();
}

ownPcapReader::~ownPcapReader()
{
    ifs.close();
}

int ownPcapReader::getLinkType()
{
    return globalHeader.network;
}

std::string ownPcapReader::getLinkTypeName()
{
    return getLinkTypeName(globalHeader.network);
}

int ownPcapReader::getCountPackages()
{
    return packetCount;
}

void ownPcapReader::readGlobalHeader()
{
    ifs.read(reinterpret_cast<char*>(&globalHeader), sizeof(globalHeader));
}

void ownPcapReader::countPackets()
{
    PcapPacketHeader packetHeader;
    while (ifs.read(reinterpret_cast<char*>(&packetHeader), sizeof(packetHeader))) {
        packetCount++;
        std::vector<char> packetData(packetHeader.incl_len);
        ifs.read(reinterpret_cast<char*>(packetData.data()), packetHeader.incl_len);
        const char* headerPtr = reinterpret_cast<const char*>(&packetHeader);
        buffer.insert(buffer.end(), headerPtr, headerPtr + sizeof(PcapPacketHeader));
        buffer.insert(buffer.end(), packetData.begin(), packetData.end());
    }
}

void ownPcapReader::createListOfPackages()
{
    const uint8_t* dataPtr = reinterpret_cast<const uint8_t*>(buffer.data());
    std::vector<Packet> packets;
    size_t offset = 0;
    int i = 0;
    while(offset + sizeof(PcapPacketHeader) <= buffer.size())
    {
        const PcapPacketHeader* header = reinterpret_cast<const PcapPacketHeader*>(dataPtr + offset);
        offset += sizeof(PcapPacketHeader);
        if (offset + header->incl_len > buffer.size()) {
            std::cout << "Ошибка: данные пакета выходят за пределы файла!" << std::endl;
            break;
        }
        Packet packet;
        packet.size = header->incl_len;
        packet.data = dataPtr + offset;
        packets.push_back(packet);
        offset += header->incl_len;
        i++;
    }
}

std::string ownPcapReader::getLinkTypeName(uint32_t linktype) const
{
    switch (linktype) {
        case 0: return "LINKTYPE_NULL";
        case 1: return "LINKTYPE_ETHERNET";
        case 6: return "LINKTYPE_IEEE802_5";
        case 7: return "LINKTYPE_ARCNET_BSD";
        case 8: return "LINKTYPE_SLIP";
        case 9: return "LINKTYPE_PPP";
        case 10: return "LINKTYPE_FDDI";
        case 100: return "LINKTYPE_RAW";
        case 101: return "LINKTYPE_IEEE802_11";
        case 104: return "LINKTYPE_C_HDLC";
        case 105: return "LINKTYPE_IEEE802_11_RADIO";
        case 108: return "LINKTYPE_PPP_HDLC";
        case 113: return "LINKTYPE_LINUX_SLL";
        case 114: return "LINKTYPE_LTALK";
        default: return "UNKNOWN";
    }
}
