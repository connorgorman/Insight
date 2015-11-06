#ifndef TWEET_H
#define TWEET_H

#include <set>
#include "timestamp.h"
#include <iostream>
#include <sstream>

/*  Class Tweet
	Used to store all information relative to a particular tweet
*/

class Tweet {

public:

	Timestamp * timestamp;
	std::set<std::string> hashtags;
	bool hadUnicode;
	std::string text;

	Tweet(Timestamp * timestamp, std::string text, std::set<std::string> hashtags, bool hadUnicode) {
		this->timestamp = timestamp;
		this->text = text;
		this->hashtags = hashtags;
		this->hadUnicode = hadUnicode;
	}

	/*  Parameters: none
		Returns: string
		Function: Returns the proper format for printing to file
	*/
	std::string getTweetForFile() {
		std::stringstream fmt;
		fmt << text << " (timestamp: " << timestamp->timestampStr << ")\n";
		return fmt.str();
	}

	/*  Parameters: none
	Returns: void
	Function: Printout tweet for debugging purposes
	*/
	void printTweet(){
		std::cout << "Text: " << text << "\n\t HadUnicode: " << hadUnicode << std::endl;
		std::cout << "\t Hashtags: ";
		std::set<std::string>::iterator it1;
		for (it1=hashtags.begin(); it1!=hashtags.end(); ++it1) {
			std::cout << *it1 << " ";
		}
		std::cout << std::endl;
		std::cout << "\t Timestamp: ";
		this->timestamp->printTimestamp();
		std::cout << std::endl;
	}

};

#endif