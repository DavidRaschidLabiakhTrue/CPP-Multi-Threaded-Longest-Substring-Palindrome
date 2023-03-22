#pragma once

#include <string>
#include <string_view>

class LongestPalindromeInSubstringSolver
{

public:
	LongestPalindromeInSubstringSolver(const std::string& str, const bool UsingMultiThreadedImplementation);
	const std::string Solve();

private:
	void Implementation(const std::string& stringToSearch, std::string& result);

	const bool MultiThreaded;

	const std::string sForward;
	const std::string sBackward;

	std::thread tForward;
	std::thread tBackward;
};

const bool IsPalindrome(const std::string_view& view);