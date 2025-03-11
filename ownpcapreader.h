#ifndef OWNPCAPREADER_H
#define OWNPCAPREADER_H
#include <fstream>
#include <iostream>
#include <vector>
#include <iomanip>
#include <map>

#pragma pack(push, 1)
struct PcapGlobalHeader {
    uint32_t m_magic_number;
    uint16_t m_version_minor;
    int32_t m_thiszone;
    uint32_t m_sigfigs;
    uint16_t m_version_major;
    uint32_t m_snaplen;
    uint32_t m_network;
};
struct PcapPacketHeader {
    uint32_t m_ts_sec;
    uint32_t m_ts_usec;
    uint32_t m_incl_len;
    uint32_t m_orig_len;
};
struct Packet {
    uint32_t m_size;
    const uint8_t* m_data;
};
#pragma pack(pop)

enum FileType
{
    OneByte,
    TwoBytes,
    FourBytes,
    EightBytes
};

class ownPcapReader
{
public:
    ownPcapReader(const char* file);
    int getLinkType();
    int getCountPackages();
    std::map<int, int> getMapOfPackages();
    std::string getLinkTypeName();
    std::vector<uint8_t> getBuffer();
private:
    PcapGlobalHeader* m_globalHeader;
    const char* m_file;
    std::vector<uint8_t> m_buffer;
    std::vector<Packet> m_packages;
    std::map<int, int> m_sizes;
    void countPackets();
    void readPcapToBuffer();
    void createListOfSizes();
    void createListOfPackages();
    std::string getLinkTypeName(uint32_t linktype) const;
};

#endif // OWNPCAPREADER_H
