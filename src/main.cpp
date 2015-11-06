
#include <fstream>
#include <sstream>
#include <string>
#include <iostream>
#include <set>
#include <algorithm>
#include <string>
#include <utility>
#include <queue>
#include <stdlib.h>     /* atoi */
#include <map>
#include <chrono>
#include "timestamp.h"
#include "fileio.h"
#include "node.h"

using namespace std;

map<string, Node *> globalMapOfNodes;

/*  Parameters: string, string, Timestamp *
	Returns: none
	Function: Adds a connection between two hashtags and removes all ones not in 60s timestamp
*/
void addConnection(string hashtag1, string hashtag2, Timestamp * timestamp) {

	//Add new node to graph if necessary
	if (globalMapOfNodes.find(hashtag1) == globalMapOfNodes.end()) {
		Node * node = new Node(hashtag1);
		globalMapOfNodes[hashtag1] = node;
	} 
	if (globalMapOfNodes.find(hashtag2) == globalMapOfNodes.end()) {
		Node * node = new Node(hashtag2);
		globalMapOfNodes[hashtag2] = node;
	} 

	//Call the nodes and add the edge
	globalMapOfNodes[hashtag1]->addEdge(hashtag2, timestamp);
	globalMapOfNodes[hashtag2]->addEdge(hashtag1, timestamp);
}

/*  Parameters: Tweet *
	Returns: none
	Function: Adds all edges of possible unique pairs of hashtags to the graph
*/
void generateConnections(Tweet * tweet) {
	std::set<string>::iterator it1;
	std::set<string>::iterator it2;

	for (it1=tweet->hashtags.begin(); it1!=tweet->hashtags.end(); ++it1) {
		for (it2 = it1; it2!=tweet->hashtags.end(); ++it2) {
			if( *it1 != *it2) {
				addConnection(*it1, *it2, tweet->timestamp);
			}
		}
	}
}

/*  Parameters: none
	Returns: double
	Function: Gets the average degree of the nodes in the graph
*/
double getAverageDegree() {

	std::map<std::string, Node *>::iterator it;

	double average = 0.0;
	int count = 0.0;
	for(it = globalMapOfNodes.begin(); it != globalMapOfNodes.end(); ++it) {
		average += (it->second)->degree;
		count++;
	}

	if( count == 0 )
		count = 1;

	return average / count;
}

int main(int argc, char * argv[]){

	queue<Tweet *> tweetQueue;

	auto start = std::chrono::system_clock::now();

	// For testing purposes, add command line arguments
	if( argc == 2 && *argv[1] == '1')
		readTweets("../tweet_input/test_tweets.txt", tweetQueue);
	else if ( argc == 2 && *argv[1] == '2' )
		readTweets("../tweet_input/tweets_new.txt", tweetQueue);
	else
		readTweets("../tweet_input/tweets.txt", tweetQueue);

	int unicodeCount = 0;
	int count = tweetQueue.size();

	// Open both output files
	ofstream ft1("../tweet_output/ft1.txt");
	ofstream ft2("../tweet_output/ft2.txt");

	// Iterate over all tweets in order from time of arrival
	while(!tweetQueue.empty()) {

		Tweet * t = tweetQueue.front();
		tweetQueue.pop();

		//Write the information to file1
		ft1 << t->getTweetForFile();

		if( t->hadUnicode)
			unicodeCount++;

		//Generate all the connections for the graph
		generateConnections(t);

		//Get the average degree and write to the file
		double average = getAverageDegree();
		ft2 << average << '\n';

		//Clean up datastructures
		delete t->timestamp;
		delete t;
	}

	ft1 << '\n' << unicodeCount << " tweets contained unicode.";

	ft1.close();
	ft2.close();

	auto end = std::chrono::system_clock::now();
	auto elapsed = std::chrono::duration_cast<std::chrono::microseconds>(end - start);	

	double secondsElapsed = elapsed.count() * 0.000001;

	cout << "Tweets Processed per Second: " << count / secondsElapsed << endl;

	// Remove nodes before exit
	std::map<string, Node *>::iterator it;
	for (it=globalMapOfNodes.begin(); it!=globalMapOfNodes.end(); ++it) {
		delete it->second;
	}

	return 0;	
}