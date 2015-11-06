#ifndef TIME_HASHTAG_PAIR
#define TIME_HASHTAG_PAIR

#include <string>
#include "timestamp.h"

/*  Class Time_Hashtag_Pair
	Stores a pair between the time a hashtag was used and the timestamp
*/

class Time_Hashtag_Pair {

public:
	Timestamp * timestamp;
	std::string hashtag;

	Time_Hashtag_Pair(Timestamp * timestamp, std::string hashtag) {
		this->timestamp = timestamp;
		this->hashtag = hashtag;
	}

	/*  Parameters: Timestamp *
		Returns: int
		Function: Raw distance between the times in seconds. Positive means first timestamp is greater than
	*/
	int compareTo(Time_Hashtag_Pair * thp) {
		return this->timestamp->compareTo(thp->timestamp);
	}
};

#endif
