//
// Created by Ciaran on 09/12/2021.
//

#include "rrRoadRunnerMap.h"
#include "thread_pool.hpp"


namespace rr {


    RoadRunnerMap::RoadRunnerMap(const std::vector<std::string> &sbmlStringsOrFiles, unsigned int numThreads)
            : numThreads_(numThreads) {
        // we don't want multiple threads trying to use constructor at the same time
//        std::lock_guard lock(rrMapMtx);
        if (numThreads_ == 0){
            std::ostringstream err;
            err << "User has requested " << numThreads_ << " threads. Please choose a number greater than 0.";
            rrLogErr << err.str();
            throw std::invalid_argument(err.str());
        }
        // provide a way to avoid the overhead of creating and managing the thread_pool
        // For instance, if we are only loading 3 models, the overhead might not be worth it
        if (numThreads_ > 1){
            rrLogInfo << "Instantiating a map with " << numThreads_ << " threads";
            pool = std::make_unique<thread_pool>(numThreads);
            loadParallel(sbmlStringsOrFiles);
        }

        else {
            rrLogInfo << "Instantiating a map in serial mode, 1 thread.";
            loadSerial(sbmlStringsOrFiles);
        }
    }

    std::vector<std::string> RoadRunnerMap::getKeys() const {
        std::vector<std::string> keys(size());
        int i = 0;
        for (auto& [name, ptr ]: rrMap_){
            keys[i++] = name;
        }
        return keys;
    }

    std::vector<RoadRunner*> RoadRunnerMap::getValues() const{
        std::vector<RoadRunner*> values(size());
        int i = 0;
        for (auto& [name, ptr ]: rrMap_){
            values[i++] = ptr.get();
        }
        return values;
    }

    std::vector<std::pair<std::string, RoadRunner*>> RoadRunnerMap::getItems() const{
        std::vector<std::pair<std::string, RoadRunner*>> v(size());
        int i = 0;
        for (auto& [name, ptr ]: rrMap_){
            v[i++] = {name, ptr.get()};
        }
        return v;
    }

    void RoadRunnerMap::insert(std::unique_ptr<RoadRunner> roadRunner) {
        const std::string &k = roadRunner->getModelName();
        std::lock_guard<std::mutex> lock(rrMapMtx);
        rrMap_.insert({k, std::move(roadRunner)});
    }

    void RoadRunnerMap::insert(const std::string &key, std::unique_ptr<RoadRunner> roadRunner) {
        try {
            std::lock_guard<std::mutex> lock(rrMapMtx);
            rrMap_.insert({key, std::move(roadRunner)});
        } catch (std::exception &e) {
            rrLogErr << "Model with key \"" << key << "\" did not load due to the following exception: \n " << e.what();
        }
    }

    void RoadRunnerMap::insert(const std::string &sbmlOrFile) {
        try {
            auto rr = std::make_unique<RoadRunner>(sbmlOrFile);
            std::lock_guard<std::mutex> lock(rrMapMtx);
            rrMap_.insert({rr->getModelName(), std::move(rr)});
        } catch (std::exception &e) {
            rrLogErr << "Model with key sbmlOrFile \"" << sbmlOrFile
                     << "\" did not load due to the following exception \n" << e.what();
        }
    }

    void RoadRunnerMap::insert(const std::string &key, const std::string &sbmlOrFile) {
        try {
            std::lock_guard<std::mutex> lock(rrMapMtx);
            rrMap_.insert({key, std::make_unique<RoadRunner>(sbmlOrFile)});
        } catch (std::exception &e) {
            rrLogErr << "Model with key \"" << key << "\" and sbmlOrFile : \n" << sbmlOrFile
                     << "\n did not load due to the following exception: \n" << e.what();
        }
    }

    void RoadRunnerMap::insert(const std::vector<std::string> &v) {
        if (numThreads_ == 1){
            loadSerial(v);
        } else {
            loadParallel(v);
        }
    }

    void RoadRunnerMap::erase(const std::string &key) {
        std::lock_guard<std::mutex> lock(rrMapMtx);
        rrMap_.erase(key);
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

    size_t RoadRunnerMap::count(const std::string &key) {
        return rrMap_.count(key);
    }

    void RoadRunnerMap::setNumThreads(unsigned int n){
        // do not do anything if the user asks for the same number of threads
        // that already exist
        if (n == numThreads_)
            return;
        numThreads_ = n;

        // if we are asked for more than 0 threads and we have an existing
        // thread_pool, reset the number of threads in the thread_pool
        if (numThreads_ != 0 && pool != nullptr){
            // reset is a method of thread_pool -- do not confuse with the method in unique_ptr.
            pool->reset(numThreads_);
        }

        // if we are asked for more than 0 threads and we do not yet have a thread_pool,
        // we must create one.
        else if (numThreads_ != 0 && pool == nullptr){
            pool = std::make_unique<thread_pool>(numThreads_);
        }

        // if we are asked to 0 threads and we have a existing thread_pool, delete it
        else if (numThreads_ == 0 && pool != nullptr){
            // reset is a method of unique_ptr -- do not confuse with the method in thread_pool.
            pool.reset();
        }

        // if we are asked for 0 threads and we do not yet have any, do nothing
        else if (numThreads_ == 0 && pool == nullptr){
            return;
        }
    }

    unsigned int RoadRunnerMap::getNumThreads() const{
        return numThreads_;
    }

    void RoadRunnerMap::wait_for_tasks() {
        pool->wait_for_tasks();
    }

    /*******************************************************
     * Private
     */

    void RoadRunnerMap::loadParallel(const std::vector<std::string> &sbmlFilesOrStrings) {
        for (auto &sbml: sbmlFilesOrStrings) {
            pool->push_task([&sbml, this]() -> void {
                insert(std::cref(sbml));
            });
        }
        /**
         * If we do not wait for all tasks to be complete, the destruction process will begin
         * and one thread will have deleted something in use by another. Then we become sad face.
         */
        pool->wait_for_tasks();
    }

    void RoadRunnerMap::loadSerial(const std::vector<std::string> &sbmlFilesOrStrings){
        for (const auto& sbml : sbmlFilesOrStrings){
            insert(sbml);
        }
    }

    ThreadSafeUnorderedMap::iterator RoadRunnerMap::begin() {
        return rrMap_.begin();
    }
    ThreadSafeUnorderedMap::const_iterator RoadRunnerMap::begin() const {
        return rrMap_.begin();
    }

    ThreadSafeUnorderedMap::iterator RoadRunnerMap::end() {
        return rrMap_.end();
    }

    ThreadSafeUnorderedMap::const_iterator RoadRunnerMap::end() const{
        return rrMap_.end();
    }

    ThreadSafeUnorderedMap::iterator RoadRunnerMap::find(const std::string& key){
        return rrMap_.find(key);
    }

    ThreadSafeUnorderedMap::const_iterator RoadRunnerMap::find(const std::string& key) const{
        return rrMap_.find(key);
    }

    RoadRunner *RoadRunnerMap::operator[](const string &key) {
        return at(key);
    }

    RoadRunner* RoadRunnerMap::at(const std::string& key){
        return rrMap_.at(key).get();
    }

    RoadRunner* RoadRunnerMap::at(const std::string& key) const {
        return rrMap_.at(key).get();
    }


//    RoadRunner *RoadRunnerMap::steal( const std::string &key){
//        auto rr = rrMap_.at(key).get();
//        for (auto it=rrMap_.begin(); it != rrMap_.end(); ++it){
//            if (it->first == key){
//                rrMap_.erase(it);
//                break;
//            }
//        }
//        return rr;
//    }


}