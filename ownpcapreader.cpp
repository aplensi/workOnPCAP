#include "ownpcapreader.h"

ownPcapReader::ownPcapReader(const char* file) : file(file)
{
    readPcapToBuffer();
    createListOfPackages();
    writePacketsToFile();
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
    return packages.size();
}

void ownPcapReader::readPcapToBuffer()
{
    std::ifstream ifs;
    ifs.open(file, std::ios::binary);
    if (!file) {
        std::cerr << "Не удалось открыть файл." << std::endl;
        return;
    }
    ifs.seekg(0, std::ios::end);
    std::streamsize fileSize = ifs.tellg();
    ifs.seekg(0, std::ios::beg);
    buffer.resize(fileSize);
    if (!ifs.read(buffer.data(), fileSize)) {
        throw std::runtime_error("Ошибка чтения файла: " + std::string(file));
    }
    ifs.close();
}

void ownPcapReader::createListOfPackages()
{
    std::memcpy(&globalHeader, buffer.data(), sizeof(PcapGlobalHeader));

    const uint8_t* dataPtr = reinterpret_cast<const uint8_t*>(buffer.data());
    size_t offset = sizeof(PcapGlobalHeader);
    while(offset + sizeof(PcapPacketHeader) <= buffer.size())
    {
        PcapPacketHeader header;
        std::memcpy(&header, buffer.data() + offset, sizeof(PcapPacketHeader));
        offset += sizeof(PcapPacketHeader);
        if (offset + header.incl_len > buffer.size()) {
            std::cout << "Ошибка: данные пакета выходят за пределы файла!" << std::endl;
            break;
        }
        Packet packet;
        packet.size = header.incl_len;
        packet.data = dataPtr + offset;
        packages.push_back(packet);
        offset += header.incl_len;
    }
}

void ownPcapReader::writePacketsToFile()
{
    std::ofstream ofs2Bytes("2bytes.bin", std::ios::binary);
    std::ofstream ofs4Bytes("4bytes.bin", std::ios::binary);

    const uint8_t* dataPtr = reinterpret_cast<const uint8_t*>(buffer.data());
    size_t offset = sizeof(PcapGlobalHeader);
    while(offset + sizeof(PcapPacketHeader) <= buffer.size())
    {
        PcapPacketHeader header;
        std::memcpy(&header, buffer.data() + offset, sizeof(PcapPacketHeader));
        offset += sizeof(PcapPacketHeader);
        if (offset + header.incl_len > buffer.size()) {
            std::cout << "Ошибка: данные пакета выходят за пределы файла!" << std::endl;
            break;
        }
        uint16_t packet16 = static_cast<uint16_t>(header.incl_len);
        ofs2Bytes.write(reinterpret_cast<const char*>(&packet16), sizeof(packet16));
        ofs2Bytes.write(reinterpret_cast<const char*>(dataPtr + offset), packet16);
        ofs4Bytes.write(reinterpret_cast<const char*>(&header.incl_len), sizeof(header.incl_len));
        ofs4Bytes.write(reinterpret_cast<const char*>(dataPtr + offset), header.incl_len);
        offset += header.incl_len;
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
