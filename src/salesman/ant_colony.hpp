#ifndef ANT_COLONY_HPP
#define ANT_COLONY_HPP

#include <vector>

#include "ant.hpp"
#include "path.hpp"
#include "tsmresult.hpp"

namespace s21 {

class AntColony {
public:
    AntColony(s21::Graph& graph)
        : path_(graph)
        , ants_(graph.MatrixSize(), Ant(0))
    {
    }

    ~AntColony() = default;

    void CreateAnts(size_t total_ants) {
        ants_.clear();
        for(size_t i = 0; i < total_ants; ++i)
            ants_.push_back(Ant(RandomNum(path_.Distance().Rows())));
    }

    void ReleaseAnts() {
        for(size_t i = 0; i < ants_.size(); ++i) {
            ants_[i].Run(path_);
        }
        for(size_t i = 0; i < ants_.size(); ++i) {
            path_.UpdatePheromones(ants_[i].Road());
        }
    }

    s21::TsmResult GetBestPath() {
        s21::TsmResult res;
        res.distance = INFINITY;
        for(size_t i = 0; i < ants_.size(); ++i) {
            if(path_.CalculatePathLength(ants_[i].Road()) < res.distance) {
                res.distance = path_.CalculatePathLength(ants_[i].Road());
                res.vertices = ants_[i].Road();
            }
        }
        return res;
    }

private:
    s21::Path path_;
    std::vector<Ant> ants_;

    size_t RandomNum(size_t max) {
        std::random_device rd;
        std::default_random_engine engine{rd()};
        std::uniform_int_distribution<int> distribution{0, static_cast<int>(max) - 1};
        return distribution(engine);
    }

}; // AntColony

}; // namespace s21

#endif // ANT_COLONY_HPP