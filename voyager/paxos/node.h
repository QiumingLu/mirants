#ifndef VOYAGER_PAXOS_NODE_H_
#define VOYAGER_PAXOS_NODE_H_

#include "voyager/paxos/options.h"
#include "voyager/util/slice.h"

namespace voyager {
namespace paxos {

class Node {
 public:
  static bool Start(const Options& options,
                     Node** nodeptr);

  Node() { }
  virtual ~Node() { }

  virtual bool Propose(size_t group_idx,
                       const Slice& value,
                       uint64_t* new_instance_id) = 0;

 private:
  // No copying allowed
  Node(const Node&);
  void operator=(const Node&);
};

}  // namespace paxos
}  // namespace voyager

#endif  // VOYAGER_PAXOS_NODE_H_
