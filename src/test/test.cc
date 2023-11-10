#include <gtest/gtest.h>

#include "../algorithms/s21_graph_algorithms.h"

TEST(BFS, test1) {
  s21::Graph a;
  s21::GraphAlgorithms b;
  a.LoadGraphFromFile("test/test1.txt");
  std::vector<int> res = b.BreadthFirstSearch(a, 1);
  std::vector<int> check = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11};
  for (size_t i = 0; i < check.size(); ++i) EXPECT_EQ(res[i], check[i]);
}

TEST(BFS, test2) {
  s21::Graph a;
  s21::GraphAlgorithms b;
  a.LoadGraphFromFile("test/test2.txt");
  std::vector<int> res = b.BreadthFirstSearch(a, 1);
  std::vector<int> check = {1, 3, 6, 7, 2, 4, 5};
  for (size_t i = 0; i < check.size(); ++i) EXPECT_EQ(res[i], check[i]);
}

TEST(DFS, test1) {
  s21::Graph a;
  s21::GraphAlgorithms b;
  a.LoadGraphFromFile("test/test1.txt");
  std::vector<int> res = b.DepthFirstSearch(a, 1);
  std::vector<int> check = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11};
  for (size_t i = 0; i < check.size(); ++i) EXPECT_EQ(res[i], check[i]);
}

TEST(DFS, test2) {
  s21::Graph a;
  s21::GraphAlgorithms b;
  a.LoadGraphFromFile("test/test2.txt");
  std::vector<int> res = b.DepthFirstSearch(a, 1);
  std::vector<int> check = {1, 3, 6, 2, 4, 5, 7};
  for (size_t i = 0; i < check.size(); ++i) EXPECT_EQ(res[i], check[i]);
}

TEST(BetweenTwoVert, test1) {
  s21::Graph a;
  s21::GraphAlgorithms b;
  a.LoadGraphFromFile("test/test1.txt");
  int res = b.GetShortestPathBetweenVertices(a, 1, 5);
  EXPECT_EQ(res, 16);
}

TEST(BetweenTwoVert, test2) {
  s21::Graph a;
  s21::GraphAlgorithms b;
  a.LoadGraphFromFile("test/test2.txt");
  int res = b.GetShortestPathBetweenVertices(a, 1, 4);
  EXPECT_EQ(res, 17);
}

TEST(BetweenAllVert, test1) {
  s21::Graph a;
  s21::GraphAlgorithms b;
  a.LoadGraphFromFile("test/test2.txt");
  s21::Matrix<int> res = b.GetShortestPathsBetweenAllVertices(a);
  std::vector<size_t> check = {0, 41, 2, 17, 16, 12, 8};

  for (size_t i = 0; i < check.size(); ++i) EXPECT_EQ(check[i], res(0, i));
}

TEST(SpanningTree, test1) {
  s21::Graph a;
  s21::GraphAlgorithms b;
  a.LoadGraphFromFile("test/test2.txt");
  s21::Matrix<int> res = b.GetLeastSpanningTree(a);
  std::vector<size_t> check = {0, 0, 2, 0, 0, 12, 8};
  a.ExportGraphToDot("ttt");
  a.ExportGraphToDot("tt.dot");

  for (size_t i = 0; i < check.size(); ++i) EXPECT_EQ(check[i], res(0, i));
}

TEST(TSM, test1) {
  s21::Graph a;
  s21::GraphAlgorithms b;
  a.LoadGraphFromFile("test/test1.txt");
  s21::TsmResult res = b.SolveTravelingSalesmanProblem(a);
  EXPECT_TRUE(res.distance < 300.0);
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}