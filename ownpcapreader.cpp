#include "ownpcapreader.h"

ownPcapReader::ownPcapReader(const char* file) : m_file(file)
{
    readPcapToBuffer();
    createListOfPackages();
    writePacketsToFile(FileType::FourBytes);
    writePacketsToFile(FileType::TwoBytes);
    writePacketsToFile(FileType::OneByte);
    writePacketsToFile(FileType::EightBytes);
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

void ownPcapReader::writePacketsToFile(FileType type)
{
    std::ofstream ofs;
    switch(type){
        case FileType::OneByte:
            ofs.open("file.sig8", std::ios::binary);
            break;
        case FileType::TwoBytes:
            ofs.open("file.sig16", std::ios::binary);
            break;
        case FileType::FourBytes:
            ofs.open("file.sig32", std::ios::binary);
            break;
        case FileType::EightBytes:
            ofs.open("file.sig64", std::ios::binary);
            break;
    }
    std::vector<uint8_t> fileBuffer;
    const uint8_t* dataPtr = reinterpret_cast<const uint8_t*>(m_buffer.data());
    uint16_t packet16;
    uint8_t packet8;
    uint64_t packet64;
    PcapPacketHeader* header;
    size_t offset = sizeof(PcapGlobalHeader);
    while(offset + sizeof(PcapPacketHeader) <= m_buffer.size())
    {
        header = reinterpret_cast<PcapPacketHeader*>(m_buffer.data() + offset);
        offset += sizeof(PcapPacketHeader);
        if (offset + header->m_incl_len > m_buffer.size()) {
            std::cout << "Ошибка: данные пакета выходят за пределы файла!" << std::endl;
            break;
        }

        switch(type){
            case FileType::OneByte:
                packet8 = static_cast<uint8_t>(header->m_incl_len);
                fileBuffer.insert(fileBuffer.end(), reinterpret_cast<uint8_t*>(&packet8), reinterpret_cast<uint8_t*>(&packet8) + sizeof(packet8));
                fileBuffer.insert(fileBuffer.end(), dataPtr + offset, dataPtr + offset + packet8);
                break;
            case FileType::TwoBytes:
                packet16 = static_cast<uint16_t>(header->m_incl_len);
                fileBuffer.insert(fileBuffer.end(), reinterpret_cast<uint8_t*>(&packet16), reinterpret_cast<uint8_t*>(&packet16) + sizeof(packet16));
                fileBuffer.insert(fileBuffer.end(), dataPtr + offset, dataPtr + offset + packet16);
                break;
            case FileType::FourBytes:
                fileBuffer.insert(fileBuffer.end(), reinterpret_cast<uint8_t*>(&header->m_incl_len), reinterpret_cast<uint8_t*>(&header->m_incl_len) + sizeof(header->m_incl_len));
                fileBuffer.insert(fileBuffer.end(), dataPtr + offset, dataPtr + offset + header->m_incl_len);
                break;
            case FileType::EightBytes:
                packet64 = static_cast<uint64_t>(header->m_incl_len);
                fileBuffer.insert(fileBuffer.end(), reinterpret_cast<uint8_t*>(&packet64), reinterpret_cast<uint8_t*>(&packet64) + sizeof(packet64));
                fileBuffer.insert(fileBuffer.end(), dataPtr + offset, dataPtr + offset + packet64);
                break;
        }

        offset += header->m_incl_len;
    }
    ofs.write(reinterpret_cast<const char*>(fileBuffer.data()), fileBuffer.size());
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
