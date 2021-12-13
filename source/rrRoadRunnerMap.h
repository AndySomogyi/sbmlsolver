//
// Created by Ciaran on 09/12/2021.
//

#ifndef ROADRUNNER_RRROADRUNNERMAP_H
#define ROADRUNNER_RRROADRUNNERMAP_H

#include "rrRoadRunner.h"
#include <unordered_map>
#include <thread>
#include "thread_pool.hpp"

namespace rr {

    /**
     * Mutex for use in RoadRunnerMap. We need to be careful that
     * we don't cause a deadlock by interaction with rrMtx, which
     * is exclusively for use at the individual RoadRunner level.
     *
     */
    static std::mutex rrMapMtx;


    /**
     * Unordered map for use with RoadRunnerMap. This type will do the 
     * heavy lifting. Pointers to roadrunner models are stack allocated. 
     * If this is insufficient, then we'll convert this to unique_ptr. 
     */
    using UnorderedMap = std::unordered_map<std::string, std::unique_ptr<RoadRunner>>;

    class RoadRunnerMap {
    public:
        explicit RoadRunnerMap(const std::vector<std::string> &sbmlStringsOrFiles,
                      unsigned int numProc = std::thread::hardware_concurrency() - 1);

        std::vector<std::string> getKeys() const;

        void insert(std::unique_ptr<RoadRunner> roadRunner);

        void insert(const std::string &key, std::unique_ptr<RoadRunner> roadRunner);

        void insert(const std::string &key, const std::string &sbmlOrFile);

        void insert(const std::string &sbmlOrFile);

        void insert(const std::vector<std::string> &sbmlStringsOrFiles);

        void remove(const std::string &key);

        UnorderedMap::iterator begin() noexcept;

        UnorderedMap::iterator end() noexcept;

        bool empty() const;

        unsigned int size() const;

        void clear();

        RoadRunner *operator[](const std::string &key);

        RoadRunner *borrow(const std::string &key);

        std::unique_ptr<RoadRunner> steal(const std::string &key);

        size_t count(const std::string &key);

        UnorderedMap::iterator find(const std::string &key);

#if __cplusplus >= 202002L
        bool contains(const std::string& key);
#endif

        unsigned int numProc = 1;

    private:

        void loadParallel(const std::vector<std::string> &sbmlFilesOrStrings);

        std::mutex mutex;
        UnorderedMap rrMap_{};
        std::vector<std::string> keys_;
        thread_pool pool;
    };

}

#endif //ROADRUNNER_RRROADRUNNERMAP_H
