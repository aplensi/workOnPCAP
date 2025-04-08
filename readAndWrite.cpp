#include "readAndWrite.h"


SizedField::SizedField(size_t size) : m_size(size){}

std::vector<uint8_t> SizedField::read()
{
    std::vector<uint8_t> buffer(m_size);
    return buffer;
}

void packetIO::processRead(const std::string &filename, Field &reader)
{
    ownPcapReader fReader;
    fReader.readToBuffer(m_buffer, filename.c_str());
    readFromBuffer(m_buffer, reader);
}

// по функции processWrite
// 1. нужно вынести pcap-зависимости из этой функции
// опять же напрашивается разделение 
// - какой-то конвертор из pcap пакетную структуру
// - запись пакетной структуры в соответствии с указанным типом

void packetIO::processWrite(std::vector<uint8_t> buffer, Field &reader)
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

void packetIO::readFromBuffer(const std::vector<uint8_t>& buffer, Field& reader)
{
    size_t offset = 0;
    uint64_t length = 0;

    while (offset < buffer.size())
    {

        std::vector<uint8_t> sizeOfData = reader.read();
        if (offset + sizeOfData.size() > buffer.size()) {
            std::cerr << "Ошибка: недостаточно данных для чтения размера пакета." << std::endl;
            break;
        }

        for (uint8_t i = 0; i < sizeOfData.size(); i++) {
            length += static_cast<uint64_t>(buffer[offset + i]) << (8 * i);
        }
        offset += sizeOfData.size();

        if (offset + length > buffer.size()) {
            std::cerr << "Ошибка: недостаточно данных для чтения полезной нагрузки." << std::endl;
            break;
        }

        std::vector<uint8_t> payload(buffer.begin() + offset, buffer.begin() + offset + length);
        offset += length;

        std::cout << "Размер пакета: " << length << std::endl;
        for (uint8_t byte : payload) {
            std::cout << std::hex << static_cast<int>(byte) << " ";
        }
        std::cout << std::dec << std::endl << std::endl;

        length = 0;
    }
}

// общее:
// для указания типа пакета (он же тип пакетного файла) используется усложенненая конструкция
// - можно вынести в enum
// - можно указывать длину байтах

// использование вектора - на самый подходящий способ (хотя и рабочий)