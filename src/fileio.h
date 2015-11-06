#ifndef FILEIO
#define FILEIO

#include <queue>
#include <set>
#include <fstream>
#include <iostream>
#include "timestamp.h"
#include "tweet.h"

/*  Parameters: string
	Returns: int
	Function: Convert month to integer
*/

int MonthToInt(std::string month){

	if(month == "Jan") {
		return 1;
	} else if (month == "Feb"){
		return 2;
	} else if (month == "Mar"){
		return 3;
	} else if (month == "Apr"){
		return 4;
	} else if (month == "May"){
		return 5;
	} else if (month == "Jun"){
		return 6;
	} else if (month == "Jul"){
		return 7;
	} else if (month == "Aug"){
		return 8;
	} else if (month == "Sep"){
		return 9;
	} else if (month == "Oct"){
		return 10;
	} else if (month == "Nov"){
		return 11;
	} else if (month == "Dec"){
		return 12;
	}

	return -1;
}

/*  Parameters: string
	Returns: Timestamp *
	Function: Parse timestamp string and return a timestamp object
*/
Timestamp * getTimeStampObject(std::string timestamp) {

	std::string tempStr = timestamp.substr(0,3);
	int month = MonthToInt(tempStr);

	tempStr = timestamp.substr(4, 2);
	int day = atoi(tempStr.c_str());

	tempStr = timestamp.substr(7, 2);
	int hour = atoi(tempStr.c_str());

	tempStr = timestamp.substr(10, 2);
	int min = atoi(tempStr.c_str());

	tempStr = timestamp.substr(13, 2);
	int sec = atoi(tempStr.c_str());

	tempStr = timestamp.substr(22, 4);
	int year = atoi(tempStr.c_str());

	// Start years at 2000 so its smaller. Computes the seconds of the object
	long long endTime = (year - 2000) * 32140800 + month * 2678400  + day * 86400 + hour * 3600 + min * 60 + sec * 1; 
	return new Timestamp(endTime, timestamp);
}

/*  Parameters: string *
	Returns: bool
	Function: Alter the string so that it no longer has escape sequeneces and unicode. Returns if had unicode
*/
bool cleanString(std::string *root) {

	bool foundUnicode = false;
  	int found = (*root).find("\\");
  	while (found != std::string::npos) {

  		if( (*root)[found+1] == 'u' ) {
  			(*root).erase(found, 6);
  			foundUnicode = true;
  		} else if ( (*root)[found+1] == 't' || (*root)[found+1] == 'n' || (*root)[found+1] == 'r' ) {
  			(*root).erase(found, 2);
  		} else {
  			(*root).erase(found,1);
  		}

  		found = (*root).find("\\", found);
  	}

  	return foundUnicode;
}

/*  Parameters: string, char
	Returns: string
	Function: Gets the substring from the start of the string to the last instance of the character where its no escaped
*/
std::string findFirstUnescapedChar(std::string root, char c) {
	int foundIdx = -1;
	for(int i = 0; i < root.size(); i++) {
		if( root[i] == c && root[i-1] != '\\') {
			foundIdx = i;
			break;
		}
	}
	return root.substr(0, foundIdx);
}

/*  Parameters: string, set<string>
	Returns: bool
	Function: Parses tweet and puts all hashtags into the set of hashtags. Returns if it had unicode
*/
bool getHashtags(std::string tweet, std::set<std::string> &hashtags) {

	int hashtagIndex = tweet.find("\"hashtags\":");
	// String doesn't have the word hashtags in it. Invalid type
	if( hashtagIndex == -1)
		return false;

	// Shift number of characters over from startPos
	hashtagIndex += 12;

	// Empty list of hashtags
	if( tweet[hashtagIndex] == ']' ) {
		return false;
	}

	int endBracket = tweet.find("]", hashtagIndex);
	int textPos = tweet.find("\"text\":\"", hashtagIndex)+8; // gets the current position
	int prevTextPos = textPos - 1; // keeps found from looping

	bool hasUnicode = false;

	// While you found hashtag but have no moved on to another json field and have no looped around the string
	while( textPos < endBracket && textPos > prevTextPos ) {

		//Find the string and cut it at the end quote to get the real hashtag then make it lowercase
		int textEnd = tweet.find("\"", textPos);
		std::string hashtag = tweet.substr(textPos, textEnd - textPos);
		std::transform(hashtag.begin(), hashtag.end(), hashtag.begin(), ::tolower);

		// Remove all unicode from the hashtag
		if( cleanString(&hashtag)) {
			hasUnicode = true;
		}

		hashtags.insert(hashtag);

		endBracket = tweet.find("]", endBracket+1);
		prevTextPos = textPos;
		textPos = tweet.find("\"text\":\"", textPos+1)+8;
	}

	return hasUnicode;
}

/*  Parameters: string
	Returns: Timestamp *
	Function: Parses tweet and returns the timestamp from it
*/
Timestamp * getCreatedTime(std::string tweet){
	int createdIndex = tweet.find("\"created_at\":");

	// If created_at isn't found then it is a misformed tweet
	if( createdIndex == -1)
		return NULL;

	createdIndex += 18;
	std::string createdTime = tweet.substr(createdIndex, 26);
	Timestamp * timestamp = getTimeStampObject(createdTime);

	return timestamp;
}

/*  Parameters: string, string *
	Returns: bool
	Function: Takes the tweet and stores the tweet that is cleaned. Returns if the tweet text was valid
*/
bool getFinalText(std::string tweet, std::string *finalText) {

	int textIndex = tweet.find("\"text\":");
	if( textIndex == -1)
		return false;

	textIndex += 8;
	int tempTextEnd = 140 * 6; // Max length of twitter tweet if it was all unicode
	std::string longText = tweet.substr(textIndex, tempTextEnd);
	*finalText = findFirstUnescapedChar(longText, '\"');
	return true;
}

/*  Parameters: string, queue<Tweet *>
	Returns: none
	Function: Open the file parameters and iterate over all the tweets and add them in order to the queue
*/
void readTweets(std::string filename, std::queue<Tweet *> &tweetQueue) {

	std::ifstream infile(filename.c_str());

	std::string tweet;
	int count = 0;

	//Iterate over every line in the file
	while(getline(infile, tweet)) {

		std::set<std::string> hashtags;
		std::string finalText;
		Timestamp * timestamp = getCreatedTime(tweet);

		//If there is no timestamp then continue because the tweet it malformed
		if( timestamp == NULL)
			continue;

		bool hashtagUnicode = getHashtags(tweet, hashtags);
		bool validText = getFinalText(tweet, &finalText);
		
		// If the tweet text is not valid then ignore the tweet
		if( !validText )
			continue;

		bool textUnicode = cleanString(&finalText);

		// If either the hashtag or the text had unicode then mark as having unicode
		bool hadUnicode = hashtagUnicode | textUnicode;

		// Create the new tweet and add it to the queue
		Tweet * tweet = new Tweet(timestamp, finalText, hashtags, hadUnicode);
		tweetQueue.push(tweet);

	}
}

/*  Parameters: string, int
	Returns: string
	Function: Open the file and find a particular line for debugging and return the string of that line
*/
std::string readLine(std::string filename, int lineNum) {

	std::ifstream infile(filename.c_str());

	std::string tweet;

	int count = 1;
	while(getline(infile, tweet)) {
		if( count == lineNum)
			return tweet;
		count++;
	}
}

#endif
