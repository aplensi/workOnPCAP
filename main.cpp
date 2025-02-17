#include <QCoreApplication>
#include "pcapreader.h"
#include "ownpcapreader.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    std::cout << "1)libpcap\n"
                 "2)ownReader" << std::endl;
    int choice;
    std::cin >> choice;

    if(choice == 1){
        std::cout << "\033c";
        pcapReader reader("record.pcap");
        std::cout << "Количество пакетов: " << reader.getCountPackages() << std::endl;
        std::cout << "LinkType: " << reader.getLinkType() << std::endl;
        std::cout << "LinkType name: " << reader.getLinkTypeName() << std::endl;
    }
    else{
        std::cout << "\033c";
        ownPcapReader ownreader("record.pcap");
        std::cout << "Количество пакетов: " << ownreader.getCountPackages() << std::endl;
        std::cout << "LinkType: " << ownreader.getLinkType() << std::endl;
        std::cout << "LinkType name: " << ownreader.getLinkTypeName() << std::endl;
    }

    return a.exec();
}
