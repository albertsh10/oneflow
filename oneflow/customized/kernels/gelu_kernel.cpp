#include "oneflow/core/framework/framework.h"

namespace oneflow {

template<typename T>
class CpuGeluKernel final : public user_op::OpKernel {
 public:
  CpuGeluKernel() = default;
  ~CpuGeluKernel() = default;

 private:
  void Compute(user_op::KernelComputeContext* ctx) const override {
    const user_op::Tensor* in = ctx->Tensor4ArgNameAndIndex("in", 0);
    user_op::Tensor* out = ctx->Tensor4ArgNameAndIndex("out", 0);
    const int32_t elem_cnt = in->shape().elem_cnt();
    const T* in_ptr = in->dptr<T>();
    T* out_ptr = out->mut_dptr<T>();
    T inv_sqrt2 = std::sqrt(0.5);
    FOR_RANGE(int32_t, i, 0, elem_cnt) {
      out_ptr[i] = 0.5 * in_ptr[i] * (1.0 + std::erf(inv_sqrt2 * in_ptr[i]));
    }
  };

  bool AlwaysComputeWhenAllOutputsEmpty() const override { return false; }
};

#define REGISTER_CPU_GELU_KERNEL(dtype)                                             \
  REGISTER_USER_KERNEL("gelu").SetCreateFn<CpuGeluKernel<dtype>>().SetIsMatchedHob( \
      (user_op::HobDeviceType() == DeviceType::kCPU)                                \
      & (user_op::HobDataType("out", 0) == GetDataType<dtype>::value));

REGISTER_CPU_GELU_KERNEL(float)
REGISTER_CPU_GELU_KERNEL(double)

template<typename T>
class CpuGeluGradKernel final : public user_op::OpKernel {
 public:
  CpuGeluGradKernel() = default;
  ~CpuGeluGradKernel() = default;

 private:
  void Compute(user_op::KernelComputeContext* ctx) const override {
    const user_op::Tensor* x = ctx->Tensor4ArgNameAndIndex("x", 0);
    const user_op::Tensor* dy = ctx->Tensor4ArgNameAndIndex("dy", 0);
    user_op::Tensor* dx = ctx->Tensor4ArgNameAndIndex("dx", 0);
    const int32_t elem_cnt = x->shape().elem_cnt();
    const T* x_ptr = x->dptr<T>();
    const T* dy_ptr = dy->dptr<T>();
    T* dx_ptr = dx->mut_dptr<T>();
    T inv_sqrt2 = std::sqrt(0.5);
    T coef = std::sqrt(2.0 / std::acos(-1.0));
    FOR_RANGE(int32_t, i, 0, elem_cnt) {
      dx_ptr[i] = 0.5
                  * (1.0 + std::erf(inv_sqrt2 * x_ptr[i])
                     + x_ptr[i] * coef * std::exp(-0.5 * x_ptr[i] * x_ptr[i]))
                  * dy_ptr[i];
    }
  };

  bool AlwaysComputeWhenAllOutputsEmpty() const override { return false; }
};

#define REGISTER_CPU_GELU_GRAD_KERNEL(dtype)                          \
  REGISTER_USER_KERNEL("gelu_grad")                                   \
      .SetCreateFn<CpuGeluGradKernel<dtype>>()                        \
      .SetIsMatchedHob((user_op::HobDeviceType() == DeviceType::kCPU) \
                       & (user_op::HobDataType("dx", 0) == GetDataType<dtype>::value));

REGISTER_CPU_GELU_GRAD_KERNEL(float)
REGISTER_CPU_GELU_GRAD_KERNEL(double)

}  // namespace oneflow