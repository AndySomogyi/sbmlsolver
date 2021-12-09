//
// Created by Ciaran on 09/12/2021.
//

#include "rrRoadRunnerMap.h"
#include "thread_pool.hpp"


namespace rr {


    RoadRunnerMap::RoadRunnerMap(const std::vector<std::string> &sbmlStringsOrFiles, unsigned int numProc)
            : pool(thread_pool(numProc)) {
        loadParallel(sbmlStringsOrFiles);
    }

    std::vector<std::string> RoadRunnerMap::getKeys() const {
        return keys_;
    }

    void RoadRunnerMap::insert(const std::string &key, std::unique_ptr<RoadRunner> roadRunner) {
        keys_.push_back(key);
        rrMap_.insert({key, std::move(roadRunner)});
    }

    void RoadRunnerMap::insert(std::unique_ptr<RoadRunner> roadRunner) {
        const std::string &k = roadRunner->getModelName();
        keys_.push_back(k);
        rrMap_.insert({k, std::move(roadRunner)});
    }

    void RoadRunnerMap::insert(const std::string &key, const std::string &sbmlOrFile) {
        keys_.push_back(key);
        rrMap_.insert({key, std::make_unique<RoadRunner>(sbmlOrFile)});
    }

    void RoadRunnerMap::insert(const std::string &sbmlOrFile) {
        auto rr = std::make_unique<RoadRunner>(sbmlOrFile);
        keys_.push_back(rr->getModelName());
        rrMap_.insert({rr->getModelName(), std::move(rr)});
    }
    void RoadRunnerMap::insert(const std::vector<std::string> &v) {
        loadParallel(v);
    }

    void RoadRunnerMap::remove(const std::string &key) {
        auto it = std::find(keys_.begin(), keys_.end(), key);
        if (it == keys_.end())
            return;
        keys_.erase(it);
    }

    UnorderedMap::iterator RoadRunnerMap::begin() noexcept {
        return rrMap_.begin();
    }

    UnorderedMap::iterator RoadRunnerMap::end() noexcept {
        return rrMap_.end();
    }

    bool RoadRunnerMap::empty() const {
        return rrMap_.empty();
    }

    unsigned int RoadRunnerMap::size() const {
        return rrMap_.size();
    }

    void RoadRunnerMap::clear() {
        rrMap_.clear();
    }

    RoadRunner *RoadRunnerMap::operator[](const std::string &key) {
        return rrMap_[key].get();
    }

    RoadRunner *RoadRunnerMap::borrow(const std::string &key) {
        return rrMap_.at(key).get();
    }

    std::unique_ptr<RoadRunner> RoadRunnerMap::steal(const std::string &key) {
        return std::move(rrMap_.at(key));

    }

    size_t RoadRunnerMap::count(const std::string &key) {
        return rrMap_.count(key);
    }

    UnorderedMap::iterator RoadRunnerMap::find(const std::string &key) {
        return rrMap_.find(key);
    }

#if __cplusplus >= 202002L
    bool RoadRunnerMap::contains(const std::string& key);
        return rrMap_.contains(key);
#endif

    /*******************************************************
     * Private
     */

    void RoadRunnerMap::loadParallel(const std::vector<std::string> &sbmlFilesOrStrings) {
        for (auto sbml: sbmlFilesOrStrings) {
            pool.push_task([&sbml, this]() -> void {
                insert(std::cref(sbml));
            });
        }
        /**
         * If we do not wait for all tasks to be complete, the destruction process will begin
         * and one thread will have deleted something in use by another. Then we become sad face.
         */
        pool.wait_for_tasks();
    }

}