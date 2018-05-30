#include "nde_format.hpp"
#include "io.hpp"
#include <vector>

std::unique_ptr<Graph> ReadNDE(int options) {
  auto chg = SetupGraphInput();
  size_t N = nextInt();
  std::vector<std::vector<size_t>> graph(N);
  {
    size_t a, b;
    Counter cnt("Reading nodes");
    for (size_t i = 0; i < N; i++) {
      a = nextInt();
      b = nextInt();
      graph[a].reserve(b);
      cnt++;
    }
  }
  {
    size_t a, b;
    Counter cnt("Reading edges");
    while (true) {
      a = nextInt();
      b = nextInt();
      if (a == (size_t)EOF || b == (size_t)EOF)
        break;
      if (a == b)
        continue;
      if (a < b || options & GraphReadOptions::BIDIRECTIONAL) {
        graph[a].push_back(b);
      }
      if (a > b || options & GraphReadOptions::BIDIRECTIONAL) {
        graph[b].push_back(a);
      }
      cnt++;
    }
  }
  {
    Counter cnt("Cleaning adj lists");
    for (size_t i = 0; i < N; i++) {
      sort(graph[i].begin(), graph[i].end());
      graph[i].erase(std::unique(graph[i].begin(), graph[i].end()),
                     graph[i].end());
      cnt++;
    }
  }
  return std::make_unique<InMemoryGraph>(std::move(graph));
}

void WriteNDE(const Graph *g) {
  auto chg = SetupGraphOutput();
  write(g->size(), '\n');

  {
    Counter cnt("Outputting nodes");
    for (size_t i = 0; i < g->size(); i++) {
      write(i, ' ', g->degree(i), '\n');
      cnt++;
    }
  }
  {
    Counter cnt("Outputting edges");
    for (size_t i = 0; i < g->size(); i++) {
      for (size_t v : g->neighs(i)) {
        if (i < v) {
          write(i, ' ', v, '\n');
          cnt++;
        }
      }
    }
  }
}
