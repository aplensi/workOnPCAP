#include "ownpcapreader.h"
#include "readFile.h"

int main(int argc, char *argv[])
{

    std::cout << "\033c";
    ownPcapReader ownreader("record.pcap");

    SizedFieldReader reader(4);
    processFile("file.sig32", reader);

    std::map<int, int> sizes = ownreader.getMapOfPackages();
    for(auto [size, count] : sizes){
        std::cout << size << " => " << count << std::endl;
    }

    std::cout << std::endl << "Количество пакетов: " << ownreader.getCountPackages() << std::endl;
    std::cout << "LinkType: " << ownreader.getLinkType() << std::endl;
    std::cout << "LinkType name: " << ownreader.getLinkTypeName() << std::endl;
    
}
