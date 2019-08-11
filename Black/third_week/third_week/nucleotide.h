#pragma once
#include <cstddef>
#include <array>


struct Nucleotide {
    char Symbol;
    size_t Position;
    int ChromosomeNum;
    int GeneNum;
    bool IsMarked;
    char ServiceInfo;
};


const std::array<char, 4> symbols = {
       'A',
       'T',
       'G',
       'C'
};


const std::array<bool, 2> marks = {
        true,
        false
};


struct CompactNucleotide {
    uint32_t Position;
    char ServiceInfo;
    uint8_t ChromosomeNum:6;
    uint8_t Symbol:2;
    uint16_t GeneNum:15;
    uint8_t IsMarked:1;
};


inline bool operator==(const Nucleotide& lhs, const Nucleotide& rhs) {
    return (lhs.Symbol == rhs.Symbol)
           && (lhs.Position == rhs.Position)
           && (lhs.ChromosomeNum == rhs.ChromosomeNum)
           && (lhs.GeneNum == rhs.GeneNum)
           && (lhs.IsMarked == rhs.IsMarked)
           && (lhs.ServiceInfo == rhs.ServiceInfo);
}


CompactNucleotide Compress(const Nucleotide& n);
Nucleotide Decompress(const CompactNucleotide& cn);

void TestNucleotide();