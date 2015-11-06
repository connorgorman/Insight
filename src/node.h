#ifndef NODE_H
#define NODE_H

#include <map>
#include <queue>
#include "time_hashtag_pair.h"


/*  Class Node
	Stores the information for a particular node inthe graph
*/

class Node {

public:
	std::string hashtag;
	int degree;

	std::queue<Time_Hashtag_Pair *> timeHashtagPairs;
	std::map<std::string, unsigned int> mapOfEdges;

	Node(std::string hashtag) {
		this->hashtag = hashtag;
		this->degree = 0;
	}

	/*  Parameters: string, Timestamp *
		Returns: none
		Function: Adds edges to the node, but also removes edges if the hashtags are more than 60s past
	*/
	void addEdge(std::string hashtag, Timestamp * timestamp) {

		// If a new edge then increase the degree
		if (mapOfEdges.find(hashtag) == mapOfEdges.end()) {
			mapOfEdges[hashtag] = 1;
			degree++;
		} else {
			mapOfEdges[hashtag]++;
		}

		Time_Hashtag_Pair * thp = new Time_Hashtag_Pair(timestamp, hashtag);

		Time_Hashtag_Pair * front = timeHashtagPairs.front();
		std::map<std::string, unsigned int>::iterator it;
		
		// Iteratively compare to the first element in the queue. If its more than 60s remove. Repeat until none less than 60s
		while(!timeHashtagPairs.empty() && front->compareTo(thp) <= -60 ) {
			
			timeHashtagPairs.pop();
			
			// If there was only one instance of the edge left remove it. Otherwise decrement it
			if( mapOfEdges[front->hashtag] == 1 ) {
				it = mapOfEdges.find(front->hashtag);
				mapOfEdges.erase(it);
				degree--;
			} 
			else {
				mapOfEdges[front->hashtag]--;
			}
		}

		timeHashtagPairs.push(thp);
	} 
};

#endif
