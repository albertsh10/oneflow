#ifndef ONEFLOW_CORE_OPERATOR_CONCAT_OP_H_
#define ONEFLOW_CORE_OPERATOR_CONCAT_OP_H_

#include "oneflow/core/operator/operator.h"

namespace oneflow {

class ConcatOp final : public Operator {
 public:
  OF_DISALLOW_COPY_AND_MOVE(ConcatOp);
  ConcatOp() = default;
  ~ConcatOp() = default;

  bool NeedOutWhenBackward() const override { return false; }
  void InitFromOpConf() override;

  const PbMessage& GetCustomizedConf() const override;

  void InferBlobDescs(std::function<BlobDesc*(const std::string&)> GetBlobDesc4BnInOp,
                      const ParallelContext* parallel_ctx) const override;
};

}  // namespace oneflow

#endif  // ONEFLOW_CORE_OPERATOR_CONCAT_OP_H_
