#ifndef OWNPCAPREADER_H
#define OWNPCAPREADER_H
#include <fstream>
#include <iostream>

class ownPcapReader
{
public:
    ownPcapReader(const char* file);
    ~ownPcapReader();
    int getLinkType();
    std::string getLinkTypeName();
    int getCountPackages();
private:
    struct PcapGlobalHeader {
        uint32_t magic_number;
        uint16_t version_major;
        uint16_t version_minor;
        int32_t thiszone;
        uint32_t sigfigs;
        uint32_t snaplen;
        uint32_t network;
    };
    struct PcapPacketHeader {
        uint32_t ts_sec;
        uint32_t ts_usec;
        uint32_t incl_len;
        uint32_t orig_len;
    };
    std::ifstream ifs;
    PcapGlobalHeader globalHeader;
    void readGlobalHeader();
    void countPackets();
    int packetCount;
    std::string getLinkTypeName(uint32_t linktype) const;
};

#endif // OWNPCAPREADER_H
