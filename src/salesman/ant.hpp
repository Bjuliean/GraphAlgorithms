#ifndef ANT_HPP
#define ANT_HPP

#include <cmath>
#include <random>

#include "path.hpp"

namespace s21 {

class Ant {
public:
    Ant() = delete;

    Ant(size_t vertex, double alpha = 2.0f, double beta = 1.0f)
        : alpha_(alpha)
        , beta_(beta)
        , road_()
        , possible_ways_()
    {
        road_.push_back(vertex);
    }

    ~Ant() = default;

    void Run(s21::Path& path) {
        while(CanMove(path)) {
            std::vector<std::pair<size_t, double>> ways_chance;
            double wish_sum = 0;

            for(auto it : possible_ways_) {
                double path_factor = std::pow(std::pow(path.Distance()(road_.back(), it), alpha_), -1);
                double pheromone_factor = std::pow(path.Pheromones()(road_.back(), it), beta_);
                double attraction = path_factor * pheromone_factor;
                wish_sum += attraction;
                ways_chance.push_back(std::make_pair(it, attraction));
            }

            for(size_t i = 0; i < ways_chance.size(); ++i) {
                ways_chance[i].second /= wish_sum;
            }

            if(ways_chance.size() > 0) road_.push_back(RandomDirection(ways_chance));

        }

        if(path.Distance()(road_.back(), road_.front()) != 0) {
            road_.push_back(road_.front());
        }

    }

    inline std::vector<size_t>& Road() { return road_; }

private:
    double alpha_;
    double beta_;
    std::vector<size_t> road_;
    std::vector<size_t> possible_ways_;


    bool CanMove(s21::Path& path) {
        possible_ways_.clear();
        size_t current_vertex = road_.back();
        for(size_t i = 0; i < path.Distance().Rows(); ++i) {
            if(i != current_vertex && path.Distance()(current_vertex, i) != 0 && !Contains(road_, i)) {
                possible_ways_.push_back(i);
            }
        }
        return !possible_ways_.empty();
    }

    bool Contains(std::vector<size_t>& vec, size_t el) {
        for(auto it : vec) {
            if(it == el) return true;
        }
        return false;
    }

    size_t RandomDirection(std::vector<std::pair<size_t, double>>& vec) {
        std::random_device rd;
        std::default_random_engine eng(rd());
        std::uniform_int_distribution<int> rnd(0, 100);
        double rand_val = static_cast<double>(rnd(eng)) / 100.0;
        std::sort(vec.begin(), vec.end(), Comparator);
        size_t res = vec.back().first;

        for(size_t i = 0; i < vec.size(); ++i) {
            if(rand_val < vec[i].second) {
                res = vec[i].first;
                break;
            }
        }

        return res;
    }

    static bool Comparator(std::pair<size_t, double>& lhs, std::pair<size_t, double>& rhs) {
        return lhs.second < rhs.second;
    }


}; // Ant

}; // namespace s21

#endif // ANT_HPP