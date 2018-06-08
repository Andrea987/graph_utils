#include "flags.hpp"
#include "graph.hpp"
#include "io.hpp"
#include <vector>

namespace {

std::unique_ptr<Graph> ReadBEL(int options) {
  std::vector<std::pair<node_t, node_t>> edges;
  {
    size_t a, b;
    Counter cnt("Reading edges");
    MemoryMappedFile f(flags::input_file);
    int m = f.data<int>()[0];
    for (int i = 0; i < m; i++) {
      a = f.data<int>()[2 * i + 1];
      b = f.data<int>()[2 * i + 2];
      if (a == b)
        continue;
      if (a < b || options & GraphReadOptions::BIDIRECTIONAL) {
        edges.emplace_back(node_t(a), node_t(b));
      }
      if (a > b || options & GraphReadOptions::BIDIRECTIONAL) {
        edges.emplace_back(node_t(b), node_t(a));
      }
      cnt++;
    }
  }
  {
    Counter cnt("Cleaning adj lists");
    std::sort(edges.begin(), edges.end());
    cnt++;
    edges.resize(std::unique(edges.begin(), edges.end()) - edges.begin());
    cnt++;
  }
  return std::make_unique<InMemoryGraph>(std::move(edges));
}

void WriteBEL(const Graph *g) { throw std::runtime_error("Not implemented"); }

GraphRegisterFormat r("bel", ReadBEL, WriteBEL);

} // namespace
