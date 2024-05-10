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

    node_t<T,N> & operator=( node_t<T,N> const& ) = delete;
    node_t<T,N>( node_t<T,N> const& ) = delete;

    node_t( dataset_type dataset ) : m_dataset(dataset) {
        m_is_leaf = true;
        m_left = nullptr;
        m_right = nullptr;
    }

    ~node_t() = default;

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
int64_t find_index_of_key( std::deque<key<T,N>> const& dataset, key<T,N> const& k, bool verbose = false ){
    int64_t index = 0;
    while ( index < (int64_t)dataset.size() ){
        if ( dataset[index].m_id == k.m_id )
            return index;
        ++index;
    }

    return -1;
}

template<typename T, int N>
T find_min( std::deque<key<T,N>> const& dataset, int dim, bool verbose = false ){
    T min_value = 1e9;
    for( key<T,N> const& e : dataset )
        if ( e.m_value[dim] < min_value )
            min_value = e.m_value[dim];
    return min_value;
}

template<typename T, int N>
T find_max( std::deque<key<T,N>> const& dataset, int dim, bool verbose = false ){
    T max_value = -1e9;
    for( key<T,N> const& e : dataset )
        if ( e.m_value[dim] > max_value )
            max_value = e.m_value[dim];
    return max_value;
}

template<typename T, int N>
int find_dim_with_highest_spread(  std::deque<key<T,N>> const& dataset, bool verbose = false ){
    
    T max_spread = 0;
    int max_dim = 0;
    for( int dim(0); dim < N; ++dim ){
        T min_value = find_min(dataset,dim,verbose);
        T max_value = find_max(dataset,dim,verbose);
        T spread = std::abs(max_value - min_value);
        if ( spread > max_spread ){
            max_spread = spread;
            max_dim = dim;
        }
    }
    
    return max_dim;
}

template<typename T, int N>
key<T,N> find_median_of_selected_dim(  std::deque<key<T,N>> & dataset, int dim, bool verbose = false ){

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
void split_the_dataset_in_two_based_on_median( T median, int dim, std::deque<key<T,N>> & dataset,
    std::deque<key<T,N>> & dataset_left, std::deque<key<T,N>> & dataset_right ){
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
void split_balanced(node_t<T,N> * node, bool verbose = false){
    if( node->m_dataset.size() > 0 ){
        // find the dim with the highest stdev
        int dim = find_dim_with_highest_spread(node->m_dataset, verbose);
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

        if ( verbose )
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
        if ( verbose )
            std::cout << "reached leaf node!" << std::endl;
}

template<typename T, int N>
node_t<T,N> * build_kdtree( std::deque<std::array<T,N>> const& data ){
    std::cout << "Called build_kdtree( std::deque )" << std::endl;

    std::deque<key<T,N>> dataset;

    for( size_t i(0); i < data.size(); ++i ){
        key<T,N> k;
        k.m_id = i;
        k.m_value = data[i];
        dataset.emplace_back( k );
    }

    node_t<T,N> * root = new node_t<T,N>( dataset );

    split_balanced(root);

    return root;
}

template<typename T, int N>
node_t<T,N> * build_kdtree( std::vector<std::array<T,N>> const& data ){
    std::cout << "Called build_kdtree( std::vector )" << std::endl;

    std::deque<std::array<T,N>> tempData;
    for( auto e : data )
        tempData.emplace_back(e);
    
    return build_kdtree<T,N>( tempData );
}

template<typename T, int N>
node_t<T,N> * build_kdtree( std::list<std::array<T,N>> const& data ){
    std::cout << "Called build_kdtree( std::list )" << std::endl;

    std::deque<std::array<T,N>> tempData;
    for( auto e : data )
        tempData.emplace_back(e);
    
    return build_kdtree<T,N>( tempData );
}

template<typename T, int N>
void print_kdtree( node_t<T,N> * node ){
    if(node->m_left)
        print_kdtree(node->m_left);
    
    std::cout << node->m_key.m_value[0] << " is_leaf?  " << (node->isLeaf() ? "yes" : "no") << "  ";
    if(!node->isLeaf()){
        if(node->m_left)
            std::cout << node->m_left->m_key.m_value[0] << " ";
        if(node->m_right)
            std::cout << node->m_right->m_key.m_value[0] << " ";
    }
    std::cout << std::endl;

    if(node->m_right)
        print_kdtree(node->m_right);
}

template<typename T, int N>
void destroy_kdtree( node_t<T,N> * node, bool verbose = false ){
    if (verbose)
        std::cout << "destroying node " << node->getID() << std::endl;

    if (node == nullptr) return;

    if (node->m_left)
        destroy_kdtree(node->m_left,verbose);

    if (node->m_right)
        destroy_kdtree(node->m_right,verbose);
    
    delete node;
}
