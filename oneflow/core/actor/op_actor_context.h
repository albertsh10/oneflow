#ifndef ONEFLOW_CORE_ACTOR_OP_ACTOR_CONTEXT_H_
#define ONEFLOW_CORE_ACTOR_OP_ACTOR_CONTEXT_H_

namespace oneflow {

namespace actor {

class MsgHandler;

class OpActorCtx {
 public:
  using ProducedRegstType = HashMap<int64_t, std::vector<std::unique_ptr<Regst>>>;

  int64_t act_id() const { return act_id_; }
  std::unique_ptr<DeviceCtx>& mut_device_ctx() { return device_ctx_; }
  const ParallelContext* parallel_ctx() const { return parallel_ctx_.get(); }

  void Init(const TaskProto&, const ThreadCtx&);
  void UpdateWithRegstMsg(const ActorMsg&);
  void UpdateWithEordMsg(const ActorMsg&);
  void UpdateWithCmdMsg(const ActorMsg&);

  bool IsReady4Act() const;
  void Act();
  void HandleRegstMsgAfterAct();
  bool NoLongerConsumeRegst() const;

  void ProcessMsgFromConsumers();
  void RecvAllProducedMsg();

  MsgHandler initial_msg_handler() const;

 protected:
  OpActorCtx() = default;
  virtual ~OpActorCtx() = default;

  void SetInitMsgHandler(MsgHanler handler);
  void InsertRegstPattern(RegstPatternWrapperIf*);

 private:
  virtual void VirtualHandleRegstPattern(const TaskProto&) = 0;
  virtual void VirtualSetMsgHandler() = 0;

  int64_t actor_id_;
  int64_t act_id_;
  std::unique_ptr<ParallelContext> parallel_ctx_;
  std::unique_ptr<DeviceCtx> device_ctx_;
  MsgHandler initial_msg_handler_;
  HashMap<std::string, std::vector<int64_t>> name2regst_desc_id_;
  std::vector<ExecKernel> exec_kernel_vec_;
  ProducedRegstType produced_regsts_;

  HashMap<std::string, std::unique_ptr<RegstPatternWrapperIf>> wrappers_;
  HashMap<int64_t, RegstPatternWrapperIf*> regst_desc_id2wrapper_;
};

}

}

#endif // ONEFLOW_CORE_ACTOR_OP_ACTOR_CONTEXT_H_
