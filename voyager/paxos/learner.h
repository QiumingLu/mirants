#ifndef VOYAGER_PAXOS_LEARNER_H_
#define VOYAGER_PAXOS_LEARNER_H_

#include <stdint.h>
#include "voyager/paxos/acceptor.h"
#include "voyager/paxos/paxos.pb.h"

namespace voyager {
namespace paxos {

class Config;
class Instance;

class Learner {
 public:
  Learner(Config* config, Instance* instance, Acceptor* acceptor);

  void SetInstanceId(uint64_t instance_id) { instance_id_ = instance_id; }
  uint64_t GetInstanceId() const { return instance_id_; }

  void OnNewChosenValue(const PaxosMessage& msg);
  void OnAskForLearn(const PaxosMessage& msg);
  void OnSendNowInstanceId(const PaxosMessage& msg);
  void OnComfirmForLearn(const PaxosMessage& msg);

  bool HasLearned() const { return has_learned_; }
  const std::string& GetLearnedValue() const { return learned_value_; }

  void NextInstance();

 private:
  void AskForLearn();
  void SendNowInstanceId(const PaxosMessage& msg);
  void ComfirmForLearn(const PaxosMessage& msg);
  void BroadcastMessageToFollower();

  Config* config_;
  Instance* instance_;
  Acceptor* acceptor_;

  uint64_t instance_id_;

  bool has_learned_;
  std::string learned_value_;

  // No copying allowed
  Learner(const Learner&);
  void operator=(const Learner&);
};

}  // namespace paxos
}  // namespace voyager

#endif  // VOYAGER_PAXOS_LEARNER_H_
