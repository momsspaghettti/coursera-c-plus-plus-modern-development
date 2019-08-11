#include "nucleotide.h"
#include "UnitTestsFramework.h"


void TestSize() {
    ASSERT(sizeof(CompactNucleotide) <= 8)
}


void TestCompressDecompress() {
    Nucleotide source{};

    source.Symbol = 'T';
    source.Position = 1'000'000'000;
    source.ChromosomeNum = 48;
    source.GeneNum = 1'000;
    source.IsMarked = true;
    source.ServiceInfo = 'Test';

    CompactNucleotide compressedSource = Compress(source);
    Nucleotide decompressedSource = Decompress(compressedSource);

    ASSERT(source == decompressedSource)
}


void TestNucleotide() {
    TestRunner tr;

    RUN_TEST(tr, TestSize);
    RUN_TEST(tr, TestCompressDecompress);
}