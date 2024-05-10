#pragma once

#include <algorithm>
#include <vector>
#include <array>
#include <deque>
#include <list>

/**
 * @brief Each multi-dimensional value in a key has a unique id that makes it retrievable from the initial dataset.
 * 
 * @tparam T type of data.
 * @tparam N number of dimensions.
 */
template<typename T, int N>
struct key{
    size_t          m_id;
    std::array<T,N> m_value;
};

/**
 * @brief Each note holds a key and a dataset to pass in successor nodes dusing construction. When the tree 
 * is fully constructed the datasets in all nodes are empty.
 * 
 * @tparam T type of data.
 * @tparam N number of dimensions.
 */
template<typename T, int N>
struct node_t {
    using key_type = key<T,N>;
    using dataset_type = std::deque<key_type>;
    
    key_type     m_key;
    dataset_type m_dataset;
    bool m_is_leaf;
    int  m_split_dim;
    node_t* m_left = nullptr;
    node_t* m_right = nullptr;

    node_t( dataset_type dataset ) : m_dataset(dataset) {
        m_is_leaf = true;
        m_left = nullptr;
        m_right = nullptr;
    }

    bool isLeaf()       const { return m_is_leaf; }
    int getID()         const { return m_key.m_id; }
    T getValue( int i ) const { return m_key.m_value.at(i); }
    node_t *getLeft()   const { return m_left; }
    node_t *getRight()  const { return m_right; }
};

template<typename T, int N>
node_t<T,N> *make_new_node( std::deque<key<T,N>> dataset, bool verbose = false ){
    if ( verbose )
        std::cout << "Called make_new_node" << std::endl;

    return new node_t<T,N>( dataset );
}

template<typename T, int N>
int find_dim_with_highest_stdev(  std::deque<key<T,N>> const& dataset, bool verbose = false ){
    if( dataset[0].m_value.size() > 1 )
        throw std::runtime_error("DIM SIZE MUST BE 1");
    return 0;
}

template<typename T, int N>
key<T,N> find_median_of_selected_dim(  std::deque<key<T,N>> &dataset, int dim, bool verbose = false ){

    // copy the values of the dim of interest in a vector and sort them; then find the median.
    std::vector<T> vec(dataset.size());
    for( size_t i(0); i<vec.size(); ++i )
        vec[i] = dataset[i].m_value[dim];
    std::sort(vec.begin(), vec.end());
    T median = vec[vec.size()/2];

    // loop over the dataset over the dim of interest and identify the key that corresponds to the median.
    int target;
    T closest = 1e9;
    key<T,N> median_key;
    for( size_t i(0); i < dataset.size(); ++i ){
        T diff = std::abs(dataset[i].m_value[dim] - median);
        if ( diff < closest ){
            median_key = dataset[i];
            target = i;
            closest = diff;
        }
    }

    // the key is removed from the dataset before it is returned as the output of this function.
    dataset.erase(dataset.begin()+target);

    return median_key;
}

template<typename T, int N>
void split_the_dataset_in_two_based_on_median( T median, int dim, std::deque<key<T,N>> &dataset,
    std::deque<key<T,N>> &dataset_left, std::deque<key<T,N>> &dataset_right ){
    dataset_left.clear();
    dataset_right.clear();
    while( dataset.size() > 0 ){
        if( dataset[0].m_value[dim] < median ){
            dataset_left.push_back(dataset[0]);
        } else {
            dataset_right.push_back(dataset[0]);
        }
        dataset.pop_front();
    }
}

template<typename T, int N>
void split_balanced(node_t<T,N> *node, bool verbose = false){
    if( node->m_dataset.size() > 0 ){
        // find the dim with the highest stdev
        int dim = find_dim_with_highest_stdev(node->m_dataset, verbose);
        node->m_split_dim = dim;
        
        // find the median key based on the selected dim
        key<T,N> median_key = find_median_of_selected_dim(node->m_dataset, dim, verbose);
        node->m_key = median_key;

        // split the dataset in two based on the median
        std::deque<key<T,N>> dataset_left, dataset_right;
        T median = median_key.m_value[dim];
        split_the_dataset_in_two_based_on_median(median, dim, node->m_dataset, dataset_left, dataset_right );
        
        // create left node and continue spliting
        if(dataset_left.size()>0){
            node->m_left = make_new_node(dataset_left, verbose);
            split_balanced(node->m_left, verbose);
        }

        // if ( verbose )
            std::cout << node->m_key.m_id << ", " << node->m_key.m_value[0] << std::endl;

        // create right node and continue spliting
        if(dataset_right.size()>0){
            node->m_right = make_new_node(dataset_right, verbose);
            split_balanced(node->m_right, verbose);   
        }
    }

    if ( node->m_left || node->m_right )
        node->m_is_leaf = false;

    if( node->isLeaf() )
        // if ( verbose )
            std::cout << "reached leaf node!" << std::endl;
}

template<typename T, int N>
void breakdown_left(node_t<T,N> *node, bool verbose = false){
    if( node->m_dataset.size() > 0 ){
        node->m_is_leaf = false;
        node->m_key = node->m_dataset[0];
        node->m_dataset.pop_front();
        node->m_left = make_new_node(node->m_dataset, verbose);
        node->m_dataset.clear();
        breakdown_left(node->m_left, verbose);
        if ( verbose )
            std::cout << node->m_key.m_id << ", " << node->m_key.m_value[0] << std::endl;
    } else {
        node->m_is_leaf = true;
    }

    if( node->isLeaf() )
        if ( verbose )
            std::cout << "reached leaf node!" << std::endl;
}

template<typename T, int N>
int make_kdtree( node_t<T,N>* root, std::deque<std::array<T,N>> const& data ){
    std::cout << "Called make_kdtree( std::deque )" << std::endl;

    std::deque<key<T,N>> dataset;

    for( size_t i(0); i < data.size(); ++i ){
        key<T,N> k;
        k.m_id = i;
        k.m_value = data[i];
        dataset.emplace_back( k );
    }

    root = make_new_node<T,N>( dataset );

    // breakdown_left(root,true);

    split_balanced(root);

    return 0;
}

template<typename T, int N>
int make_kdtree( node_t<T,N>* root, std::vector<std::array<T,N>> const& data ){
    std::cout << "Called make_kdtree( std::vector )" << std::endl;

    std::deque<std::array<T,N>> tempData;
    for( auto e : data )
        tempData.emplace_back(e);
    
    return make_kdtree<T,N>( root, tempData );
}
