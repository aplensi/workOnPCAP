#include "readFile.h"

template <typename T> std::vector<uint8_t> TypedFieldReader<T>::read()
{
    T value;
    return std::vector<uint8_t>(sizeof(value));
}

SizedFieldReader::SizedFieldReader(size_t size) : m_size(size){}
std::vector<uint8_t> SizedFieldReader::read()
{
    std::vector<uint8_t> buffer(m_size);
    return buffer;
}

void processFile(const std::string &filename, FieldReader &reader)
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