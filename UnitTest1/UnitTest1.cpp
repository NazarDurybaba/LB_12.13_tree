#include "pch.h"
#include "CppUnitTest.h"
#include "../LB_12.13_tree.cpp"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTest1
{
	TEST_CLASS(UnitTest1)
	{
	public:
		
        TEST_METHOD(TestMethod1)
        {
            Trie trie;

            // Test adding a guest
            Guest guest1 = { "Smith", "John", "A", 101 };
            trie.addGuest(guest1);

            vector<Guest> foundGuests;
            bool found = trie.findGuest("Smith", foundGuests);

            Assert::IsTrue(found, L"Guest should be found");
            Assert::AreEqual(size_t(1), foundGuests.size(), L"There should be one guest found");
            Assert::AreEqual(string("John"), foundGuests[0].firstName, L"First name should be John");
            Assert::AreEqual(string("A"), foundGuests[0].middleName, L"Middle name should be A");
            Assert::AreEqual(101, foundGuests[0].roomNumber, L"Room number should be 101");

            // Test editing the guest
            trie.deleteGuest("Smith", "John", "A");
            Guest guest2 = { "Smith", "John", "B", 102 };
            trie.addGuest(guest2);

            foundGuests.clear();
            found = trie.findGuest("Smith", foundGuests);

            Assert::IsTrue(found, L"Guest should be found after edit");
            Assert::AreEqual(size_t(1), foundGuests.size(), L"There should be one guest found after edit");
            Assert::AreEqual(string("John"), foundGuests[0].firstName, L"First name should be John after edit");
            Assert::AreEqual(string("B"), foundGuests[0].middleName, L"Middle name should be B after edit");
            Assert::AreEqual(102, foundGuests[0].roomNumber, L"Room number should be 102 after edit");

            // Test deleting the guest
            trie.deleteGuest("Smith", "John", "B");
            foundGuests.clear();
            found = trie.findGuest("Smith", foundGuests);

            Assert::IsFalse(found, L"Guest should not be found after deletion");
            Assert::AreEqual(size_t(0), foundGuests.size(), L"There should be no guests found after deletion");
        }
	};
}
