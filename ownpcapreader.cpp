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
        ifs.seekg(packetHeader.incl_len, std::ios::cur);
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
