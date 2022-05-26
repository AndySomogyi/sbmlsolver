//
// Created by Ciaran on 09/12/2021.
//

#ifndef ROADRUNNER_RRROADRUNNERMAP_H
#define ROADRUNNER_RRROADRUNNERMAP_H

#include "rrRoadRunner.h"
#include <unordered_map>
#include <thread>
#include "thread_pool.hpp"
#include "phmap.h"

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
    using ThreadSafeUnorderedMap = phmap::parallel_node_hash_map<std::string, std::unique_ptr<RoadRunner>>;

    /**
     * @brief Hash table designed for storing RoadRunner models. Expensive operations like building models
     * are done in parallel.
     * @details Loading RoadRunner models requires expensive builds, which can be prohibitive when loading
     * many models. RoadRunnerMap is a container for RoadRunner objects which abstracts the process of
     * loading or simulating in parallel.
     */
    class RoadRunnerMap {
    public:
        /**
         * @brief Default construct a RoadRunnerMap.
         * @details The default number of threads is 1, which can be changed with RoadRunnerMap::setNumThreads.
         */
        RoadRunnerMap() = default;

        /**
         * @brief Instantiate a RoadRunnerMap from a @param sbmlStringsOrFiles vector of sbml files, sbml strings or a mix thereof.
         * @param numThreads How many threads to use, if 1, the overhead of setting up multithreading pool is avoided.
         */
        explicit RoadRunnerMap(const std::vector<std::string> &sbmlStringsOrFiles, unsigned int numThreads);

        /**
         * @brief Returns the keys of the map in order of insertion.
         * @details Does a linear search over hashmap to build a vector of keys O(N).
         */
        std::vector<std::string> getKeys() const;

        /**
         * @brief return values of the map as a RoadRunnervector.
         * @details the RoadRunner* instances are still owned by the RoadRunnerMap
         * Linear complexity in the size of the map.
         */
        std::vector<RoadRunner *> getValues() const;

        /**
         * @brief get items of this map as vector of std::pair<std::string, RoadRuner*> types.
         * @details pointers are owned by the RoadRunnerMap -- so do not delete. O(N).
         */
        std::vector<std::pair<std::string, RoadRunner *>> getItems() const;

        /**
         * @brief Insert a @param roadRunner roadrunner model into the map.
         * @details The model must already be a unique pointer. The key for accessing the map
         * will be the model name. Loading another model with the same name will overwrite
         * the original.
         * @see void insert(const std::string &key, std::unique_ptr<RoadRunner> roadRunner);
         * @see void insert(const std::string &sbmlOrFile);
         * @see void insert(const std::string &key, const std::string &sbmlOrFile);
         * @see void insert(const std::vector<std::string> &sbmlStringsOrFiles);
         */
        void insert(std::unique_ptr<RoadRunner> roadRunner);

        /**
         * @brief The same as the std::unique_ptr<RoadRunner> overload but with a user provided key
         * @see void insert(std::unique_ptr<RoadRunner> roadRunner);
         * @see void insert(const std::string &sbmlOrFile);
         * @see void insert(const std::string &key, const std::string &sbmlOrFile);
         * @see void insert(const std::vector<std::string> &sbmlStringsOrFiles);
         */
        void insert(const std::string &key, std::unique_ptr<RoadRunner> roadRunner);

        /**
         * @brief insert a roadrunner model into the map using the model name as key.
         * @param sbmlOrFile either a sbml string or a valid file path to a sbml file.
         * @see void insert(std::unique_ptr<RoadRunner> roadRunner);
         * @see void insert(const std::string &key, std::unique_ptr<RoadRunner> roadRunner);
         * @see void insert(const std::string &key, const std::string &sbmlOrFile);
         * @see void insert(const std::vector<std::string> &sbmlStringsOrFiles);
         */
        void insert(const std::string &sbmlOrFile);

        /**
         * @brief insert a roadrunner model into the map using a user define key.
         * @param key The string to use for the key to the roadrunner model you are inserting.
         * @param sbmlOrFile either a sbml string or a valid file path to a sbml file.
         * @see void insert(std::unique_ptr<RoadRunner> roadRunner);
         * @see void insert(const std::string &key, std::unique_ptr<RoadRunner> roadRunner);
         * @see void insert(const std::string &sbmlOrFile);
         * @see void insert(const std::vector<std::string> &sbmlStringsOrFiles);
         */
        void insert(const std::string &key, const std::string &sbmlOrFile);

        /**
         * @brief Like the "insert(const std::string &sbmlOrFile);" overload, but pass in a vector
         * of sbml file or sbml strings.
         * @details
         * @see void insert(std::unique_ptr<RoadRunner> roadRunner);
         * @see void insert(const std::string &key, std::unique_ptr<RoadRunner> roadRunner);
         * @see void insert(const std::string &sbmlOrFile);
         * @see void insert(const std::string &key, const std::string &sbmlOrFile);
         */
        void insert(const std::vector<std::string> &sbmlStringsOrFiles);

        /**
         * @brief remove an element with @param key from the map
         * @details
         */
        void erase(const std::string &key);

        /**
         * @brief map iterator
         * @code
         *   for (auto&[modelName, rr]: rrm) {
         *      ...
         *   }
         * @endcode
         *
         * or
         * @code
         *    for (auto& [modelName, rr] : rrm) {
         *        actual[i] = rr->getInstanceID();
         *        i++;
         *    }
         * @endcode
         */
        ThreadSafeUnorderedMap::iterator begin();
        ThreadSafeUnorderedMap::const_iterator begin() const;

        /**
         * @brief
         * @code
         *   for (auto it = rrm.begin(); it != rrm.end(); ++it) {
         *      auto&[modelName, rr] = *it;
         *      i++;
         *  }
         * @endcode
         */
        ThreadSafeUnorderedMap::iterator end();
        ThreadSafeUnorderedMap::const_iterator end() const;

        /**
         * @brief find item with key equal to @param key .
         * @returns iterator to item if found or end if not found
         */
        ThreadSafeUnorderedMap::iterator find(const std::string &key);

        /**
         * @brief find item with key equal to @param key . Const version.
         * @returns iterator to item if found or end if not found
         */
        ThreadSafeUnorderedMap::const_iterator find(const std::string &key) const;

        /**
         * @brief return true if the map is empty
         */
        bool empty() const;

        /**
         * @brief returns the number of elements in the map
         */
        unsigned int size() const;

        /**
         * @brief empty the map so that there are 0 elements left
         */
        void clear();

        /**
         * @brief Getter operator. Returns a *borrowed* reference to a
         * roadruner model that is owned by the RoadRunnerMap.
         */
        RoadRunner *operator[](const std::string &key);

        /**
         * @brief get borrowed reference from map for roadrunner model with key equal to @param key .
         * @details returned pointer is owned by the RoadRunnerMap object.
         */
        RoadRunner *at(const std::string &key);

        RoadRunner *at(const std::string &key) const;

        /**
         * @brief wait for all tasks to finish before allowing program execution to continue
         */
        void wait_for_tasks();

        /**
         * @brief count the number of keys with value @param key .
         * @details Because there can be only 1 element with a particular key in the map,
         * the return value of count is guarenteed to be either 0 for not found, or 1 for found.
         */
        size_t count(const std::string &key);

        /**
         * @brief Reset the number of threads in the pool
         * @details
         *  - If we are asked for more than 0 threads and we have an existing thread pool, the pool will wait
         *    for current tasks to finish before resetting the pool with the new number of threads
         *  - If we are asked for more than 0 threads and we do not have an existing thread pool, creates one with
         *    the requested number of threads
         *  - If we are asked for 0 threads and we have an existing thread pool, the thread pool is destroyed.
         *  - If we are asked for 0 threads and we do not have an existing thread pool, does nothing
         */
        void setNumThreads(unsigned int n);

        /**
         * @brief get the number of threds in the thread_pool
         */
        unsigned int getNumThreads() const;

    private:
        /**
         * @brief Variable to hold the number of threads int the pool
         * @see RoadRunnerMap::setNumThreads
         * @see RoadRunnerMap::getNumThreads
         */
        unsigned int numThreads_ = 1;

        /**
         * @brief Load models in parallel, using numThread_ threads.
         * @details Uses a thread_pool under the hood to manage workloads.
         */
        void loadParallel(const std::vector<std::string> &sbmlFilesOrStrings);

        /**
         * @brief loads models in serial, without the overhead of having thread_pool
         */
        void loadSerial(const std::vector<std::string> &sbmlFilesOrStrings);

        /**
         * @brief a map type that does all the heavy lifting in this class.
         */
        ThreadSafeUnorderedMap rrMap_{};

        /**
         * @brief manages workloads using numThreads_ threads.
         */
        std::unique_ptr<thread_pool> pool = nullptr;
    };

}

#endif //ROADRUNNER_RRROADRUNNERMAP_H
