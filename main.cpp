#include "ownpcapreader.h"
#include "readAndWrite.h"

int main(int argc, char *argv[])
{

    std::cout << "\033c";
    ownPcapReader ownreader("record.pcap");
    packetIO pIo;

    SizedField field(4);
    TypedField<uint32_t> field1;
    pIo.processWrite(ownreader.getBuffer(), field);
    pIo.processRead("file.sig32", field1);

    // std::map<int, int> sizes = ownreader.getMapOfPackages();
    // for(auto [size, count] : sizes){
    //     std::cout << size << " => " << count << std::endl;
    // }

    std::cout << std::endl << "Количество пакетов: " << ownreader.getCountPackages() << std::endl;
    std::cout << "LinkType: " << ownreader.getLinkType() << std::endl;
    std::cout << "LinkType name: " << ownreader.getLinkTypeName() << std::endl;
    
}
