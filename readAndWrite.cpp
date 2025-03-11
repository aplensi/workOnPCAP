#include "readAndWrite.h"


SizedField::SizedField(size_t size) : m_size(size){}

std::vector<uint8_t> SizedField::read()
{
    std::vector<uint8_t> buffer(m_size);
    return buffer;
}

void processRead(const std::string &filename, Field &reader)
{
    std::ifstream ifs(filename, std::ios::binary);
    if (!ifs) {
        std::cerr << "Не удалось открыть файл." << std::endl;
        return;
    }

    std::vector<uint8_t> sizeOfData = reader.read();
    std::vector<uint8_t> buffer;
    uint64_t length = 0;
    while (true)
    {
        if(!ifs.read(reinterpret_cast<char*>(sizeOfData.data()), sizeOfData.size()))
        {
            break;
        }
        for(uint8_t i = 0; i < sizeOfData.size(); i++){
            length += static_cast<uint64_t>(sizeOfData[i])  << (8 * i);
        }

        buffer.resize(length);
        ifs.read(reinterpret_cast<char*>(buffer.data()), length);
        std::cout << "Размер пакета: " << length << std::endl;

        for(uint8_t byte : buffer){
            std::cout << std::hex << static_cast<int>(byte) << " ";
        }

        std::cout << std::dec << std::endl << std::endl;
        length = 0;
    }
}

void processWrite(std::vector<uint8_t> buffer, Field &reader)
{
    std::vector<uint8_t> sizeOfData = reader.read();
    std::ofstream ofs("file.sig"+std::to_string(sizeOfData.size() * 8),  std::ios::binary);
    std::vector<uint8_t> fileBuffer;
    const uint8_t* dataPtr = reinterpret_cast<const uint8_t*>(buffer.data());
    PcapPacketHeader* packetHeader;
    size_t offset = sizeof(PcapGlobalHeader);
    while(offset + sizeof(PcapPacketHeader) <= buffer.size())
    {
        packetHeader = reinterpret_cast<PcapPacketHeader*>(buffer.data() + offset);
        offset += sizeof(PcapPacketHeader);
        for (size_t i = 0; i < sizeOfData.size(); ++i) {
            if (i < sizeof(packetHeader->m_incl_len)) {
                sizeOfData[i] = (packetHeader->m_incl_len >> (i * 8)) & 0xFF;
            } else {
                sizeOfData[i] = 0;
            }
        }
        fileBuffer.insert(fileBuffer.end(), sizeOfData.begin(), sizeOfData.end());
        fileBuffer.insert(fileBuffer.end(), dataPtr + offset, dataPtr + offset + packetHeader->m_incl_len);

        offset += packetHeader->m_incl_len;
    }
    ofs.write(reinterpret_cast<const char*>(fileBuffer.data()), fileBuffer.size());
}