#include "nucleotide.h"


CompactNucleotide Compress(const Nucleotide& n) {
    CompactNucleotide result{};

    result.Position = n.Position;
    result.ServiceInfo = n.ServiceInfo;
    result.ChromosomeNum = n.ChromosomeNum;

    for (const auto& s : symbols) {
        if (s == n.Symbol)
            break;
        ++result.Symbol;
    }

    result.GeneNum = n.GeneNum;

    if (n.IsMarked) {
        result.IsMarked = 0;
    } else {
        result.IsMarked = 1;
    }

    return result;
}


Nucleotide Decompress(const CompactNucleotide& cn) {
    Nucleotide result{};

    result.Position = cn.Position;
    result.ServiceInfo = cn.ServiceInfo;
    result.ChromosomeNum = cn.ChromosomeNum;
    result.Symbol = symbols.at(cn.Symbol);
    result.GeneNum = cn.GeneNum;
    result.IsMarked = marks.at(cn.IsMarked);

    return result;
}