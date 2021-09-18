#include"Archiver.h"
#include<fstream>
#include<iostream>


Archiver::Archiver()
{
    huffTable.resize(vocabularySize);
    frequency.resize(vocabularySize, 0);
}

int Archiver::inCompressFile(std::string& fileName, std::string& outputFileName)
{
    std::ifstream file(fileName, std::ios::binary | std::ios::in);
    if (!file.is_open())
    {
        std::cerr << "Error! File doesn't open!" << std::endl;
        return 0; // 0 - ошибка
    }
    calculateFreq(file);
    //for (int i = 0; i < frequency.size(); ++i)	//удалить
    //	if (frequency[i] != 0)
    //		std::cout << (char)i << " " << frequency[i] << std::endl;

    short numUnicChar = createHuffTree();
    short serviceInfSize = numUnicChar * servInfSizeOfOneChar + 4;	//+4 это две short переменный в начале и концк файла
    createHuffTable();

    std::ofstream zipFile(outputFileName, std::ios::binary | std::ios::trunc);
    if (!zipFile.is_open())
    {
        std::cerr << "Error! Zip file doesn't open!" << std::endl;
        return 0; //ошибка
    }

    zipFile.write((const char*)&serviceInfSize, sizeof(serviceInfSize));
    writeServiceInf(zipFile);
    compress(file, zipFile);

    clearFrequency();
    clearHuffTree();
    clearHuffTable();
    file.close();
    zipFile.close();
    return 1;
}

int Archiver::inCompressFiles(std::vector<std::string>& fileNames)
{
    std::ifstream file;
    for (int i = 0; i < fileNames.size(); ++i)
    {
        for (int j = 0; j < frequency.size(); ++j)
            frequency[j] = 0;
        calculateFreq(file);
        //обнулить таблицу частот
    }
    //выше написан только подсчет частоты символов во всех файлах, надо дописать продолжение
    //добавление элементов в дерево ХаФФмана
    //добавить запись дерева Хаффмана в начало файла
    return 1;
}


int Archiver::outCompressArchive(std::string& zipFileName, std::string& outputFileName)
{
    std::ifstream zipFile(zipFileName, std::ios::binary | std::ios::in);	//файл с заархивированными данными
    if (!zipFile.is_open())
    {
        std::cerr << "Error! Zip file doesn't open!" << std::endl;
        return 0; //ошибка
    }
    short serviceInfSize, numUnicChar, numUselessBits;
    long long int fileSize;
    zipFile.seekg(0, std::ios::end);
    fileSize = zipFile.tellg();
    zipFile.seekg(-2, std::ios::end);
    zipFile.read((char*)&numUselessBits, sizeof(numUselessBits));
    zipFile.seekg(0, std::ios::beg);
    zipFile.read((char*)&serviceInfSize, sizeof(serviceInfSize));
    numUnicChar = (serviceInfSize - 4) / servInfSizeOfOneChar;	//4 байта служебной информации это 2 shot переменные в начале и конце файла
    long long int compressInfSize = fileSize - serviceInfSize;
    unsigned char tmpChar= 0;
    int bufferFrequency = 0;
    for (int i = 0; i < numUnicChar; ++i)
    {
        zipFile.read((char*)&tmpChar, sizeof(tmpChar));
        zipFile.read((char*)&bufferFrequency, sizeof(bufferFrequency));
        huffTree.push(tmpChar, bufferFrequency);
    }
    createHuffTree();

    std::ofstream file(outputFileName, std::ios::trunc | std::ios::binary);
    if (!file.is_open())
    {
        std::cerr << "Error! Output file doesn't open!" << std::endl;
        return 0; //ошибка
    }
    Node<unsigned char>* huffTreeRoot = huffTree.getRoot();
    Node<unsigned char>* currNode = huffTreeRoot;
    if (huffTreeRoot != nullptr)
    {
        unsigned char buffer = 0;
        unsigned char mask;
        for (int i = 0; i < compressInfSize - 1; ++i)  //
        {
            mask = 1 << (byteSize - 1);  //двоичный код маски 10000000, чтобы считать последний бит
            zipFile.read((char*)&buffer, sizeof(buffer));
            for (int j = 0; j < byteSize; ++j)
            {
                if (buffer & mask)
                    currNode = currNode->right;
                else
                    currNode = currNode->left;

                if (huffTree.isLeaf(currNode))
                {
                    file.write((char*)&currNode->data, sizeof(currNode->data));
                    currNode = huffTreeRoot;
                }
                mask >>= 1;
            }
        }

        mask = 1 << (byteSize - 1);	//двоичный код маски 10000000, чтобы считать последний бит
        zipFile.read((char*)&buffer, sizeof(buffer));
        for (int j = 0; j < byteSize - numUselessBits; ++j)
        {
            if (buffer & mask)
                currNode = currNode->right;
            else
                currNode = currNode->left;

            if (huffTree.isLeaf(currNode))
            {
                file.write((char*)&currNode->data, sizeof(currNode->data));
                currNode = huffTreeRoot;
            }
            mask >>= 1;
        }
    }
    clearHuffTree();
    file.close();
    zipFile.close();
    return 1;
}

Archiver::~Archiver() {}


void Archiver::calculateFreq(std::ifstream& file)
{
    long long int fileSize;
    file.clear();
    file.seekg(0, std::ios::end);
    fileSize = file.tellg();
    file.seekg(0, std::ios::beg);
    unsigned char tmp;
    for (long long int i = 0; i < fileSize; ++i)
    {
        file.read((char*)&tmp, sizeof(tmp));
        //std::cout << tmp;
        ++frequency[tmp];
    }
    std::cout << std::endl;
}


short Archiver::createHuffTree()
{
    short counter = 0;
    for (int i = 0; i < frequency.size(); ++i)
        if (frequency[i] != 0)
        {
            huffTree.push((unsigned char)i, frequency[i]);
            ++counter;
        }
    huffTree.create();
    return counter;
}

int Archiver::createHuffTable() { return huffTree.createHuffTable(huffTable); }


void Archiver::writeServiceInf(std::ofstream& zipFile)
{
    for (int i = 0; i < frequency.size(); ++i)
    {
        if (frequency[i] != 0)
        {
            zipFile << (unsigned char)i;
            zipFile.write((const char*)&frequency[i], sizeof(frequency[i]));
        }
    }
}

void Archiver::compress(std::ifstream& file, std::ofstream& zipFile)
{
    long long int fileSize;
    file.clear();
    file.seekg(0, std::ios::end);
    fileSize = file.tellg();
    file.seekg(0, std::ios::beg);
    short counter = 0;
    unsigned char buffer = 0, newChar = 0;
    std::string newCode;
    for (int i = 0; i < fileSize; ++i)
    {
        file.read((char*)&buffer, sizeof(buffer));
        newCode = huffTable[buffer];
        for (int j = 0; j < newCode.size(); ++j)
        {
            if (newCode[j] == '1')
                newChar |= 1;
            ++counter;
            if (counter == byteSize)
            {
                zipFile.write((const char*)&newChar, sizeof(newChar));
                counter = 0;
                newChar = 0;
            }
            newChar <<= 1;
        }
    }

    if (counter != 0)  //если кол-во новых бит не кратно 8
    {
        newChar <<= (byteSize - counter - 1);    //дополняем последний символ до 8 бит нулями, -1 потому что при выходе из цикла сверху newСhar уже сместился на 1 бит влево
        zipFile.write((const char*)&newChar, sizeof(newChar));
    }
    short numUselessBits = byteSize - counter;
    zipFile.write((const char*)&numUselessBits, sizeof(numUselessBits));  //записываем в конец файла кол-во не значимых бит в последнем символе
}

void Archiver::clearFrequency()
{
    for (int i = 0; i < frequency.size(); ++i)
        frequency[i] = 0;
}

void Archiver::clearHuffTree() { huffTree.clear(); }
void Archiver::clearHuffTable()
{
    for (int i = 0; i < huffTable.size(); ++i)
        huffTable[i] = "";
}
