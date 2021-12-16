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
         * @brief Default construct a RoadRunnerMap
         * @details
         */
        RoadRunnerMap() = default;

        /**
         * @brief Instantiate a RoadRunnerMap from a @param sbmlStringsOrFiles vector of sbml files, sbml strings or a mix thereof.
         * @param numThreads How many threads to use.
         */
        explicit RoadRunnerMap(const std::vector<std::string> &sbmlStringsOrFiles, unsigned int numThreads);

        /**
         * @brief Returns the keys of the map in order of insertion.
         * @details The order of insertion can be different on different runs since they are multithreaded.
         * The keys are maintained as a string vector as a member variable to RoadRunnerMap. Therefore
         * accessing the vector of keys is O(1) not O(N).
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
        void remove(const std::string &key);

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
         *   for (auto it = rrm.begin(); it != rrm.end(); ++it) {
         *      auto&[modelName, rr] = *it;
         *      i++;
         *  }
         * @endcode
         */
        UnorderedMap::iterator begin() noexcept;

        /**
         * @brief
         * @code
         *   for (auto it = rrm.begin(); it != rrm.end(); ++it) {
         *      auto&[modelName, rr] = *it;
         *      i++;
         *  }
         * @endcode
         */
        UnorderedMap::iterator end() noexcept;

        /**
         * @brief keys iterator
         * @code
         *   RoadRunnerMap rrm(sbmlFiles, 1);
         *   auto it = rrm.findKey("case00001");
         *   // first make sure key is in the map
         *   ASSERT_NE(it, rrm.keysEnd());
         * @endcode
         */
        std::vector<std::string>::iterator keysBegin();

        /**
         * @brief Iterate using the keys of the map
         * @details
         */
        std::vector<std::string>::iterator keysEnd();

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
         * @brief Remove item @param key from the RoadRunnerMap and return the
         * corresponding RoadRunner model.
         * @details the returned RoadRunner pointer is a borrowed reference, owned by
         * the RoadRunnerMap.
         */
        std::unique_ptr<RoadRunner> popKey(const std::string &key);

        /**
         * @brief Getter operator. Returns a *borrowed* reference to a
         * roadruner model that is owned by the RoadRunnerMap.
         * @details Implemented using the RoadRunnerMap::borrow method
         */
        RoadRunner *operator[](const std::string &key);

        /**
         * @brief Getter operator. Returns a *borrowed* reference to a
         * roadruner model that is owned by the RoadRunnerMap.
         * @see RoadRunnerMap::steal
         */
        RoadRunner *borrow(const std::string &key);

        /**
         * @brief Take a roadrunner object out of the map.
         * @details the returned pointer is owned by the caller
         * and must be deleted.
         * @note not returning a unique_ptr<RoadRunner> because
         * it plays havok with swig.
         */
        RoadRunner* steal(const std::string &key);

        /**
         * @brief count the number of keys with value @param key.
         * @details Because there can be only 1 element with a particular key in the map,
         * the return value of count is guarenteed to be either 0 for not found, or 1 for found.
         */
        size_t count(const std::string &key);

        /**
         * @brief Find an iterator to a std::pair<std::string, std::unique_ptr<RoadRunner>> given the @param key
         * @details
         */
        UnorderedMap::iterator find(const std::string &key);

        /**
         * @brief Find an iterator to a std::string at position @param key
         * @details
         */
        std::vector<std::string>::iterator findKey(const std::string &key);


#if __cplusplus >= 202002L
        /**
         * @brief returns true if key is in the model. Same as count but only exists in >C++20
         * @details
         */
        bool contains(const std::string& key);
#endif

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
         * @brief Mutex for thread sync.
         * @details Mainly we cannot access the rrMap_ variable from multiple threads at the same time.
         */

        std::mutex mutex;

        /**
         * @brief a map type that does all the heavy lifting in this class.
         */
        UnorderedMap rrMap_{};

        /**
         * @brief a vector to store the keys of the map. Note, this is an augmentation that enables
         * O(1) access to the keys. The keys_ variable is always maintained and accurate.
         */
        std::vector<std::string> keys_;

        /**
         * @brief manages workloads using numThreads_ threads.
         */
        std::unique_ptr<thread_pool> pool = nullptr;
    };

}

#endif //ROADRUNNER_RRROADRUNNERMAP_H
