#ifndef TACO_MODE_FORMAT_DIAGONAL_H
#define TACO_MODE_FORMAT_DIAGONAL_H

#include "taco/lower/mode_format_impl.h"

namespace taco {

// pos_iter = p -> (p, p+1)
// pos_access = (p, i_1, ... i_k) -> i_parent

class DiagModeFormat : public ModeFormatImpl {
  unsigned parentCoord;

public:
  // all diagonal levels are associated with a parent level
  DiagModeFormat(unsigned parent = 0)
      : ModeFormatImpl("diagonal", false /*isFull*/, true /*isOrdered*/,
                       true /*isUnique*/, true /*isBranchless*/,
                       true /*isCompact*/, false /*hasCoordValIter*/,
                       true /*hasCoordPosIter*/, false /*hasLocate*/,
                       false /*hasInsert*/, true /*hasAppend*/),
        parentCoord(parent) {}
  virtual ModeFunction posIterBounds(ir::Expr parentPos,
                                     Mode mode) const override {
    return ModeFunction(ir::Stmt(), {parentPos, ir::Add::make(parentPos, 1)});
  }

  virtual ModeFunction posIterAccess(ir::Expr, std::vector<ir::Expr> coords,
                                     Mode) const override {
    taco_iassert((parentCoord + 1) < coords.size());
    return ModeFunction(ir::Stmt(), {coords[parentCoord + 1], true});
  }
  virtual ir::Expr getSize(Mode mode) const override { return 1; }
  virtual ModeFormat
  copy(std::vector<ModeFormat::Property> properties) const override {
    return ModeFormat(std::make_shared<DiagModeFormat>());
  }

  virtual std::vector<ir::Expr> getArrays(ir::Expr, int) const override {
    return {};
  }
};

} // namespace taco

#endif
