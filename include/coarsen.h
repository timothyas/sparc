#ifndef COARSEN_H
#define COARSEN_H

/* \brief Compute maximal matching for graph.
 *
 * \param g: original Graph object to be coarsened
 * \param colors: vector denoting node coloring
 * \param numColors: number of colors
 * \param nodeWeight: weight of each node (starts at 1, summed when coarsened)
 * \return Error code (0 = success)
 */
int mxm_shared(Graph& g, std::vector<int> &colors, int numColors);

/* \brief Compute colors for graph coarsening implementing openmp parallelism.
 *
 * \param g: original Graph object to be coarsened
 * \param colors: vector denoting node coloring
 * \param numColors: number of colors
 * \return Error code (0 = success)
 */
int colorGraph_shared(Graph& g, std::vector<int> &colors, int &numColors);

/* \brief Compute maximal independent set.
 *
 * \param g: original Graph object to be coarsened
 * \param finalRemoveList: vector of nodes which have already been assigned a color (1=remove,0=keep). This denotes nodes that have been colored already.
 * \param I: vector to be filled of maximal independent set
 * \return Error code (0 = success)
 */
int mis_shared(Graph& g, std::vector<int> finalRemoveList, std::vector<int> &I, int currentColor);


/* \brief Find unmatched nodes of a particular color.
 *
 * \param colors: list of colors for each node
 * \param currentColor: color value to find
 * \param matchList: list of matched status for each node 
 * \param unmatchedValue: value to look for unmatched nodes
 * \param nodeList: vector of unmatched nodes flagged with current color
 * \return Error code (0 = success)
 */
int doubleSelect_shared(std::vector<int> &colors, int currentColor, std::vector<int> matchList, int unmatchedValue, std::vector<int> &nodeList);

/* \brief Find unmatched nodes of a particular color.
 *
 * \param matchList: list of matched status for each node 
 * \param unmatchedValue: value to look for unmatched nodes
 * \param nodeList: vector of unmatched nodes
 * \return Error code (0 = success)
 */
int selectUnmatched_shared(std::vector<int> matchList, int unmatchedValue, std::vector<int> &nodeList);

/* \brief Performs parallel inclusive scan on vector. Does not use pass by reference since algorithm is recursive..
 *
 * \param a: vector of integers to be scanned
 * \return s: cumulative sum of vector a 
 */
std::vector<int> inclusiveScan_shared(std::vector<int> a);

#endif 
