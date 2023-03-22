#include <thread>
#include <vector>
#include <iostream>
#include <string>
#include <string_view>
#include <algorithm>
#include "LongestPalindromeSubstring.hpp"

LongestPalindromeInSubstringSolver::LongestPalindromeInSubstringSolver(const std::string& str, const bool UsingMultiThreadedImplementation = false)
	:
	sForward{ str }, sBackward{ str.crbegin(), str.crend() }, MultiThreaded{ UsingMultiThreadedImplementation }
{
}

const std::string LongestPalindromeInSubstringSolver::Solve()
{
	std::string result1{ "" };
	std::string result2{ "" };

	if (MultiThreaded == false)
	{
		// check both ways for any potential special cases both ways one after the other.
		Implementation(sForward, result1);
		Implementation(sBackward, result2);

		return (result1.size() > result2.size() ? result1 : result2);
	}
	else
	{
		// check both ways for any potential special cases both ways at the same time.
		std::thread thread1{ &LongestPalindromeInSubstringSolver::Implementation, this, std::cref(sForward), std::ref(result1) };
		std::thread thread2{ &LongestPalindromeInSubstringSolver::Implementation, this, std::cref(sBackward), std::ref(result2) };

		thread1.join();
		thread2.join();

		return (result1.size() > result2.size() ? result1 : result2);
	}
}
const bool IsPalindrome(const std::string_view& view) // minimal copying because of string_view
{
	return std::equal(view.cbegin(), view.cend(), view.crbegin());
}
void LongestPalindromeInSubstringSolver::Implementation(const std::string& stringToSearch, std::string& result)
{
	const std::string_view view{ stringToSearch }; // O(1)
	std::string_view temp{ "" };
	std::string_view res{ "" };
	const size_t size = view.size();

	size_t tempResSize = res.size();
	size_t tempTempSize = temp.size();

	for (size_t i = 0; i < size; i++) // start at start of view and move start index of view down to the end.
	{
		// loop i = 0, j = 4 1234
		// loop i = 0, j = 3 123
		// loop i = 0, j = 2 12  ....

		// loop i = 1, j = 4 234
		// loop i = 1, j = 3 23
		// loop i = 1, j = 2 2

		
		for (size_t j = size; j >= i; j--) // shrink the end of the current view down 1
		{
			temp = view.substr(i, j);
			if (temp.size() < res.size()) // early exit
			{
				// Any subsequent shrinks here point would have lower size than currently stored result
				break; // regardless if palindrome or not, no need to continue testing here. It's too small.
			}
			if (IsPalindrome(temp)) // if the temp string is equal or greater in size to res, test if palindrome.
			{
				if (temp.size() >= res.size()) // if temp is greater in size than result, store it instead and break.
				{
					res = temp; // minimal copying because string_view
					// because it's greater or equal in size, any subsequent shrinks would be less than the current result making them invalid.
				}
				break; // if the string_view temp is shrunk anymore from this point forward, it cannot be the longest.
			}
		}
	}

	// Worst case is O(n^2). Early exit puts it's runtime around O(log(n))
	// Best case is O(n).
	
	result = res;
}

int main()
{
	std::vector<std::string> Tests{ "palinilappalinilapPPPPPPPPPPPPPPPPPPP","bwwhawkawahhawacaksckawdjawd", "MissMaryMack" };

	const bool WantToTestWithMultiThreading = true;

	const size_t TestCount = Tests.size();
	for (size_t i = 0; i < TestCount; i++)
	{
		std::cout << "Test " << i << ": \n" << LongestPalindromeInSubstringSolver(Tests[i], WantToTestWithMultiThreading).Solve() << "\n\n";
	}

	return 0;
}