#include "mainwindow.h"
#include <QApplication>
#include "binarygenerator.h"
#include <iostream>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();

    /*BinaryGenerator generator;
    generator.init(std::vector<char>({1,2}));
    generator.setLimitLecture(100ull*8ull*1024ull*1024ull);

    QTime time;
    time.start();
    generator.startGeneration();
    int elapsed = time.elapsed();
    std::cout << "Temps : " << elapsed << "secondes." << std::endl;
    std::vector<unsigned long long> data = generator.getProgression();
    unsigned int total = 0;
    for (unsigned int i = 0; i < data.size()-1; i++)
        total += data[i];
    std::cout << "Total : " << total << std::endl;
    return 0;*/
}
