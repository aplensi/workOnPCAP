#include "ownpcapreader.h"

ownPcapReader::ownPcapReader(const char* file) : m_file(file)
{
    readPcapToBuffer();
    createListOfPackages();
    createListOfSizes();
}

int ownPcapReader::getLinkType()
{
    return m_globalHeader->m_network;
}

std::string ownPcapReader::getLinkTypeName()
{
    return getLinkTypeName(m_globalHeader->m_network);
}

std::vector<uint8_t> ownPcapReader::getBuffer()
{
    return m_buffer;
}

int ownPcapReader::getCountPackages()
{
    return m_packages.size();
}

std::map<int, int> ownPcapReader::getMapOfPackages()
{
    return m_sizes;
}

void ownPcapReader::readPcapToBuffer()
{
    std::ifstream ifs;
    ifs.open(m_file, std::ios::binary);
    if (!ifs) {
        std::cerr << "Не удалось открыть файл." << std::endl;
        return;
    }
    ifs.seekg(0, std::ios::end);
    std::streamsize fileSize = ifs.tellg();
    ifs.seekg(0, std::ios::beg);
    m_buffer.resize(fileSize);
    if (!ifs.read(reinterpret_cast<char*>(m_buffer.data()), fileSize)) {
        throw std::runtime_error("Ошибка чтения файла: " + std::string(m_file));
    }
    ifs.close();
}

void ownPcapReader::createListOfSizes()
{
    for(auto package : m_packages){
        if(m_sizes.count(package.m_size) == 0){
            m_sizes[package.m_size] = 1;
        } else{
            m_sizes[package.m_size] += 1;
        }
    }
}

void ownPcapReader::createListOfPackages()
{
    m_globalHeader = reinterpret_cast<PcapGlobalHeader*>(m_buffer.data());

    const uint8_t* dataPtr = reinterpret_cast<const uint8_t*>(m_buffer.data());
    PcapPacketHeader* header;
    Packet packet;
    size_t offset = sizeof(PcapGlobalHeader);
    int countOfPkg = 0;
    while(offset + sizeof(PcapPacketHeader) <= m_buffer.size())
    {
        header = reinterpret_cast<PcapPacketHeader*>(m_buffer.data() + offset);
        offset += sizeof(PcapPacketHeader);
        if (offset + header->m_incl_len > m_buffer.size()) {
            std::cout << "Ошибка: данные пакета выходят за пределы файла!" << std::endl;
            break;
        }
        offset += header->m_incl_len;
        countOfPkg++;
    }
    m_packages.reserve(countOfPkg);
    dataPtr = reinterpret_cast<const uint8_t*>(m_buffer.data());
    offset = sizeof(PcapGlobalHeader);
    while(offset + sizeof(PcapPacketHeader) <= m_buffer.size())
    {
        header = reinterpret_cast<PcapPacketHeader*>(m_buffer.data() + offset);
        offset += sizeof(PcapPacketHeader);
        if (offset + header->m_incl_len > m_buffer.size()) {
            std::cout << "Ошибка: данные пакета выходят за пределы файла!" << std::endl;
            break;
        }
        packet.m_size = header->m_incl_len;
        packet.m_data = dataPtr + offset;
        m_packages.emplace_back(packet);
        offset += header->m_incl_len;
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
