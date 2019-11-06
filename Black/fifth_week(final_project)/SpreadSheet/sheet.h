#pragma once

#include "cell.h"
#include "common.h"

#include <functional>

class Sheet : public ISheet {
public:
    ~Sheet();

    void SetCell(Position pos, std::string text) override;

    const ICell *GetCell(Position pos) const override;

    ICell *GetCell(Position pos) override;

    void ClearCell(Position pos) override;

    void InsertRows(int before, int count) override;

    void InsertCols(int before, int count) override;

    void DeleteRows(int first, int count) override;

    void DeleteCols(int first, int count) override;

    Size GetPrintableSize() const override;

    void PrintValues(std::ostream &output) const override;

    void PrintTexts(std::ostream &output) const override;

    const Cell *GetConcreteCell(Position pos) const;

    Cell *GetConcreteCell(Position pos);

private:
    void MaybeIncreaseSizeToIncludePosition(Position pos);

    void PrintCells(std::ostream &output,
                    const std::function<void(const ICell &)> &printCell) const;

    void UpdateFormulas(
            const std::function<IFormula::HandlingResult(IFormula &)> &update);

    Size GetActualSize() const;

    std::vector<std::vector<std::unique_ptr<Cell>>> cells_;
};