#ifndef TIMESTAMP_H
#define TIMESTAMP_H

#include <iostream>

/*  Class Timestamp
	Stores all information regarding the timestamp including a seconds version of the time
	as well the text interpretation
*/

class Timestamp {

public:

	long long timestamp;
	std::string timestampStr;

	Timestamp(long long timestamp, std::string timestampStr) {
		this->timestamp = timestamp;
		this->timestampStr = timestampStr;
	}

	/*  Parameters: none
		Returns: none
		Function: Prints the timestamp
	*/
	void printTimestamp() {
		int year = this->timestamp / 32140800 + 2000;
		int month = this->timestamp % 32140800 / 2678400;
		int day = this->timestamp % 32140800 % 2678400 / 86400;
		int hour = this->timestamp % 32140800 % 2678400 % 86400 / 3600;
		int min = this->timestamp % 32140800 % 2678400 % 86400 % 3600 / 60;
		int sec = this->timestamp % 32140800 % 2678400 % 86400 % 3600 % 60;
		std::cout << month <<":"<<day<<":"<<year << " " << hour << ":" << min << ":" << sec << std::endl;
	}

	/*  Parameters: none
		Returns: none
		Function: Used to determine if a tweet timestamp is valid
	*/
	bool isDateValid() {

		int year = this->timestamp / 32140800 + 2000;
		int month = this->timestamp % 32140800 / 2678400;
		int day = this->timestamp % 32140800 % 2678400 / 86400;
		int hour = this->timestamp % 32140800 % 2678400 % 86400 / 3600;
		int min = this->timestamp % 32140800 % 2678400 % 86400 % 3600 / 60;
		int sec = this->timestamp % 32140800 % 2678400 % 86400 % 3600 % 60;

		if(year < 0 || month < 0 || day < 0 || hour < 0 || min < 0 || sec < 0) {
			return false;
		}
		return true;

	}

	/*  Parameters: Timestamp *
		Returns: int
		Function: Raw distance between the times in seconds. Positive means first timestamp is greater than
	*/
	int compareTo(Timestamp * t2) {
		return this->timestamp - t2->timestamp;
	}

};


#endif