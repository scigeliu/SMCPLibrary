/*
 * SMCPUtility.hh
 *
 *  Created on: Jun 6, 2011
 *      Author: yuasa
 */

#ifndef SMCPUTILITY_HH_
#define SMCPUTILITY_HH_

#include <bitset>
#include <string>
#include "SMCPException.hh"

/** A class that collects utility methods used in the SMCP Library.
 */
class SMCPUtility {
public:
	/** Converts string to std::bitset<n>.
	 * Example usage:
	 * @code
	 	 std::string str="010100";
	 	 std::bitset<6> bits=SMCPUtility::createBitset<6>(str);
	 * @endcode
	 */
	template<int n>
	static std::bitset<n> createBitset(std::string str) throw (SMCPException) {
		std::bitset<n> result;
		if (str.size() != n && str.size() - 1 != n) {
			throw SMCPException("size error");
		}
		if (str[str.size() - 1] != '0' && str[str.size() - 1] != '1' && str[str.size() - 1] != 'b') {
			throw SMCPException("input format error");
		}

		for (unsigned int i = 0; i < n; i++) {
			if (str[i] == '1') {
				result.set(n - i - 1, 1);
			} else if (str[i] == '0') {
				result.set(n - i - 1, 0);
			} else {
				throw SMCPException("input format error");
			}
		}
		return result;
	}
};

#endif /* SMCPUTILITY_HH_ */
