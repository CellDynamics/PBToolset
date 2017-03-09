/**
* \file C_MatlabExchange_Tests.cpp
* \brief Contains all tests for DumpAll
* \author PB
* \date 2014/09/26
*/

#include "MatlabExchange/C_MatlabExchange.hpp"
#include "gtest/gtest.h"

typedef unsigned short UINT16;

int main(int argc, char* argv[])
{
	int ret = 0;
	::testing::InitGoogleTest(&argc, argv);
	ret = RUN_ALL_TESTS();
	return ret;
}

/**
* \test C_MatlabExchange:_cantCreateFile
* \brief Try to Create nonexistent file.
* \post Expect exception ios_base::badbit
* \author PB
* \date 2014/09/26
*/
TEST(C_MatlabExchange,_cantCreateFile)
{
	bool exceptionThrown = false;
	try
	{
		C_MatlabExchange testObject("/df/dupa");	// drive z do not exist
	}
	catch(std::ios_base::failure& ex)
	{
		cerr << ex.what() << endl;
		exceptionThrown = true;
	}
	EXPECT_TRUE(exceptionThrown);
}

/**
* \test C_MatlabExchange:_canCreateFile
* \brief Try to Create existing file.
* \post Do not expect exception
* \author PB
* \date 2014/09/26
*/
TEST(C_MatlabExchange,_canCreateFile)
{
	bool exceptionThrown = false;
	try
	{
		C_MatlabExchange testObject("dupa");
	}
	catch(std::ios_base::failure& ex)
	{
		cerr << ex.what() << endl;
		exceptionThrown = true;
	}
	EXPECT_FALSE(exceptionThrown);
}

/**
* \test C_MatlabExchange:emptyFile
* \brief Test empty place for file structure created in constructor
* \post Do not expect exception and expects fiile with MAX_ENTRY*sizeof(ULONG)+sizeof(UINT) bytes
* \author PB
* \date 2014/09/27
*/
TEST(C_MatlabExchange,_emptyFile)
{
	bool exceptionThrown = false;
	unsigned int iloscWpisow;
	unsigned long entry = 0;
	try
	{
		C_MatlabExchange* testObject = new C_MatlabExchange("emptytest");	// create file
		delete testObject;									// close file
		// check content of the file
		std::ifstream readStream("emptytest");
		readStream.exceptions(readStream.exceptions()|ios_base::badbit|ios_base::failbit|ios_base::eofbit);	// throw
		//exceptions on every case
		readStream.read((char*)&iloscWpisow,sizeof(unsigned int));
		ASSERT_EQ(iloscWpisow,0);
		for(std::size_t a=0; a<MAX_ENTRY; ++a)
		{
			readStream.read((char*)&entry,sizeof(unsigned long));
			ASSERT_EQ(entry,0);
		}
		readStream.close();
	}
	catch(std::ios_base::failure& ex)
	{
		cerr << ex.what() << endl;
		exceptionThrown = true;
	}
	EXPECT_FALSE(exceptionThrown);
}

/**
* \test C_MatlabExchange:AddEntry1D
* \brief Test AddEntry1D method
* \post File with one entry containing chars from 0 to 255
* \author PB
* \date 2014/09/27
*/
TEST(C_MatlabExchange,_addEntry1D)
{
	bool exceptionThrown = false;
	float tabf[256];
	double tabd[256];
	UINT16 tab16[25];
	for(int a=0; a<256; ++a){
		tabf[a] = static_cast<float>(a);	// tab filled by increasing numbers
		tabd[a] = static_cast<double>(a);	// tab filled by increasing numbers
	}
	for(int a=0;a<25;a++)
		tab16[a] = static_cast<UINT16>(a);
	try
	{
		C_MatlabExchange testObject("matlab_addentry.dat");	// create file
		testObject.AddEntry1D<float>(tabf, 256, "float_256", "float");
		testObject.AddEntry1D<double>(tabd, 256, "double_256", "double");
		testObject.AddEntry1D<UINT16>(tab16, 25, "uint16_25", "ushort");
		std::cout << "Verify this test in Matlab" << std::endl;
	}
	catch(std::ios_base::failure& ex)
	{
		cerr << ex.what() << endl;
		exceptionThrown = true;
	}
	EXPECT_FALSE(exceptionThrown);
}

/**
* \test C_MatlabExchange:AddEntry2D
* \brief Test AddEntry1D method
* \post File with one entry containing chars from 0 to 255
* \author PB
* \date 2014/09/27
*/
TEST(C_MatlabExchange,_addEntry2D)
{
	bool exceptionThrown = false;
	float tabf[256];
	for(int a=0; a<256; ++a){
		tabf[a] = static_cast<float>(a);	// tab filled by increasing numbers
	}
	UINT16 tab16[100];
	for(int a=0;a<100;a++)
		tab16[a] = static_cast<UINT16>(a);
	try
	{
		C_MatlabExchange testObject("matlab_addentry2D.dat");	// create file
		testObject.AddEntry1D<float>(tabf, 256, "float_256", "float");
		testObject.AddEntry2D<UINT16>(tab16, 10, 10, "uint16_252D", "double");
		std::cout << "Verify this test in Matlab" << std::endl;
	}
	catch(std::ios_base::failure& ex)
	{
		cerr << ex.what() << endl;
		exceptionThrown = true;
	}
	EXPECT_FALSE(exceptionThrown);
}

/**
* \test C_MatlabExchange:AddEntryWrongDataType
* \brief Try to save unsupported data type
* \post Exception std::logic_error
* \author PB
* \date 2014/09/28
*/
TEST(C_MatlabExchange,AddEntryWrongDataType)
{
	bool exceptionThrown = false;
	unsigned long tab[256];
	for(int a=0; a<256; ++a)
		tab[a] = static_cast<unsigned long>(a);	// tab filled by increasing numbers
	try
	{
		C_MatlabExchange testObject("addentry");	// create file
		EXPECT_THROW(testObject.AddEntry1D<unsigned long>(tab, 256, "char_256","char"),std::logic_error);
	}
	catch(std::ios_base::failure& ex)	// check problem with file creating
	{
		cerr << ex.what() << endl;
		exceptionThrown = true;
	}
	EXPECT_FALSE(exceptionThrown);
}

/**
* \test C_MatlabExchange_readData:_readData
* \brief Try to save unsupported data type
* \post Exception std::logic_error
* \author PB
* \date 2014/09/28
*/
TEST(C_MatlabExchange_readData,_readData)
{
	bool exceptionThrown = false;
	std::unique_ptr<double[]> data;
	unsigned int rows,cols;
	double expected[] = {8,1,6,3,5,7,4,9,2};
	try
	{
		C_MatlabExchange::ReadData("magic3.dat", data, rows, cols);
		for(unsigned int a=0; a<rows*cols; a++)
		{
			std::cout << data[a] << ",";
			ASSERT_EQ(expected[a],data[a]);
		}
		std::cout << std::endl;
	}
	catch(std::ios_base::failure& ex)	// check problem with file creating
	{
		cerr << ex.what() << endl;
		exceptionThrown = true;
	}
	EXPECT_FALSE(exceptionThrown);
}