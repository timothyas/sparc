/**
 * \file
 * \author Gopal Yalla and Tim Smith
 * 
 * \brief Defines functions used to perform graph coarsening.
 *
 * Multilevel graph partitioning requires coarsening if the number of vertices is large. 
 * Coarsening requires maximal matching which requires coloring with requires maximal
 * independent sets. This file implements those functions to be used in the page 
 * rank problem. A short description of each algorithm is given below. 
 *
 * Coarsening
 * -----------
 *
 * An example of coarsening a graph using maximal matching is shown below:
 *
 * \htmlonly
 * <IMG SRC="coarsening.png" width=600px height=500px align="middle"/>
 * \endhtmlonly
 *
 * - Vertices in coarse graph:
 *   - Edges in maximal matching
 *   - Nodes not incident to the matching
 *   .
 * - Edges in coarse graph: 
 *   - Merge all edges between two coarse nodes
 *   .
 * - Weights in coarse graph: 
 *   - Vertex weight: Sum of group vertices' weights
 *   - Edge weight: Sum of edges
 *   .
 * .
 *
 * Edge Matching
 * -----------
 *
 * The main ingredient in maximal matching is edge matching.
 * This requires finding a subset of edges such that no two edges
 * in the set share a vertex. A vertex is considered matched if it is
 * incident to an edge in the matching. 
 *
 * Maximal matching is non-unique and is completed if the matching property
 * is lost if one more edge is added. Note, this is different that maximum 
 * matching where the goal is to match with the maximum number of edges. 
 *
 * An example of edge matching is shown below: 
 *
 * \htmlonly
 * <IMG SRC="edge_matching.png" width=600px height=500px align="middle"/>
 * \endhtmlonly
 *
 * Maximal Independent Set
 * -----------------------
 * 
 * Edge matching requires two more algorithms; the first being choosing a 
 * maximal independent set of vertices, i.e., a subset of vertices so that 
 * no two vertices share an edge. The set is considered maximal if independence
 * is violated if one more vertex were to be added to the set. Again, this set
 * is not unique. An example of a vertex maximal independent set is shown below: 
 *
 * \htmlonly
 * <IMG SRC="mis.png" width=700px height=300px align="middle"/>
 * \endhtmlonly
 *
 * Coloring
 * ---------
 *
 * Edge matching requires each vertex to be labeled. The process of labeling 
 * vertices is known as coloring. We assign a different color to vertices that 
 * share and edge. Maximal Independent Sets is used to pick vertices to color of
 * label at each stage. An example of colored graphs is shown below: 
 *
 * \htmlonly
 * <IMG SRC="coloring.png" width=700px height=200px align="middle"/>
 * \endhtmlonly
 *
 *
 */

#ifndef COARSEN_H
#define COARSEN_H

/**
 * \brief Compute maximal matching for graph.
 *
 * \param g: original Graph object to be coarsened
 * \param colors: vector denoting node coloring
 * \param numColors: number of colors
 * \param nodeWeight: weight of each node (starts at 1, summed when coarsened)
 * \return Error code (0 = success)
 */
int mxm_shared(Graph& g, std::vector<int> &colors, int numColors);

/**
 * \brief Compute colors for graph coarsening implementing openmp parallelism.
 *
 * \param g: original Graph object to be coarsened
 * \param colors: vector denoting node coloring
 * \param numColors: number of colors
 * \return Error code (0 = success)
 */
int colorGraph_shared(Graph& g, std::vector<int> &colors, int &numColors);

/**
 * \brief Compute maximal independent set.
 *
 * \param g: original Graph object to be coarsened
 * \param finalRemoveList: vector of nodes which have already been assigned a color (1=remove,0=keep). This denotes nodes that have been colored already.
 * \param I: vector to be filled of maximal independent set
 * \return Error code (0 = success)
 */
int mis_shared(Graph& g, std::vector<int> finalRemoveList, std::vector<int> &I, int currentColor);


/**
 * \brief Find unmatched nodes of a particular color.
 *
 * \param colors: list of colors for each node
 * \param currentColor: color value to find
 * \param matchList: list of matched status for each node 
 * \param unmatchedValue: value to look for unmatched nodes
 * \param nodeList: vector of unmatched nodes flagged with current color
 * \return Error code (0 = success)
 */
int doubleSelect_shared(std::vector<int> &colors, int currentColor, std::vector<int> matchList, int unmatchedValue, std::vector<int> &nodeList);

/**
 * \brief Find unmatched nodes of a particular color.
 *
 * \param matchList: list of matched status for each node 
 * \param unmatchedValue: value to look for unmatched nodes
 * \param nodeList: vector of unmatched nodes
 * \return Error code (0 = success)
 */
int selectUnmatched_shared(std::vector<int> matchList, int unmatchedValue, std::vector<int> &nodeList);

/** 
 * \brief Performs parallel inclusive scan on vector. Does not use pass by reference since algorithm is recursive..
 *
 * \param a: vector of integers to be scanned
 * \return s: cumulative sum of vector a 
 */
std::vector<int> inclusiveScan_shared(std::vector<int> a);

#endif 
