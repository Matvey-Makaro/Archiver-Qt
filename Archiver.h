#ifndef ARCHIVER_H
#define ARCHIVER_H

#include<vector>
#include<string>
#include<fstream>
#include"HuffmansTree.h"

class Archiver
{
public:
    Archiver();
    Archiver(std::vector<std::string>& fileNames, std::string& path);
    int inCompressFile(std::string& fileName, std::string& outputFileName);
    int inCompressFiles(std::vector<std::string>& fileNames);
    int outCompressArchive(std::string& zipFileName, std::string& outputFileName);
    ~Archiver();

private:
    static const int vocabularySize = 256; // кол-во символов, которые можно зашифровать в одном байте
    static constexpr int servInfSizeOfOneChar = sizeof(char) + sizeof(int);  // служебная информация - символ + его частота
    static constexpr int byteSize = sizeof(char) * 8;  //размер байта в битах
    //std::vector<std::string> fileNames; //удалить
    //std::vector<short> serviceInfSize;  //удалить
    HuffmansTree huffTree;
    std::vector<int> frequency;   //не забыть обнулять этот вектор
    std::vector<std::string> huffTable;

private:
    void calculateFreq(std::ifstream&);
    short createHuffTree();
    int createHuffTable();
    void writeServiceInf(std::ofstream&);
    void compress(std::ifstream&, std::ofstream&);
    void clearFrequency();
    void clearHuffTree();
    void clearHuffTable();
    //void unzip(std::fstream& zipFile, std::fstream& outputFile);  //удалить скорее всего
};
#endif // ARCHIVER_H
