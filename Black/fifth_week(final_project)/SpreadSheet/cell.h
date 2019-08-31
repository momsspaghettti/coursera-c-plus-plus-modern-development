#pragma once
#include "common.h"


class Cell : public ICell {
public:


    Value GetValue() const override;

    std::string GetText() const override;

    std::vector<Position> GetReferencedCells() const override;

private:
    Value value_;
    std::string text_;
};