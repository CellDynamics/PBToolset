/**
* \file C_MatlabExchange.hpp
* \brief Header file for C_Dump class
* \author PB
* \date 2014/09/25
*/

#ifndef C_DumpAll_h__
#define C_DumpAll_h__

#include <string>
#include <cstring>
#include <fstream>
#include <exception>
#include <typeinfo>
#include <memory>
using namespace std;

#define MAX_ENTRY 256 ///< Maximal number of entries in file

/**
* \enum dataType
* \brief Defines correct types of handled data
* \details Defines two types of data handled by library: \c double and \c float. Both are provided by pointers
* therefore tables as well as single values are supported.
* \author PB
* \date 2014/09/25
* \todo change type to smaller - need modifications in method and Matlab code because of size
*/
enum class dataType : unsigned int
{
	DOUBLE = 1,		///< table of doubles
	FLOAT,			///< table of floats
	USHORT,			///< 16bit unsigned integer
	DOUBLE2D,		///< 2D array of doubles
	FLOAT2D,		///< 2D array af floats
	USHORT2D,		///< 2D arrays of 16bit integers
	UNSUPPORTED		///< unsupported type
};

/**
* \enum dim
* \brief defines 1D or 2D datatypes passed to decodeType method
* \author PB
* \date 2014/10/06
*/
enum class dim : unsigned int
{
	d1D,
	d2D
};
/**
* \class C_MatlabExchange
* \brief Defines main interface for saving data for Matlab
* \details Represents object associated with file defined during construction. All data are saved to
* this file according to calls to AddEntry1D method
* \author PB
* \date 2014/09/25
* \todo Add exeptions for errors
*/
class C_MatlabExchange
{
public:
	/// Main constructor
	C_MatlabExchange(const char* filename);
	/// Adds 1D data of basic types
	template<typename T>
	void AddEntry1D(const T* data, unsigned int size, const char* name, const char* typeName);
	/// Adds 2D data of basic types
	template<typename T>
	void AddEntry2D(const T* data, unsigned int rows, unsigned int cols, const char* name, const char* typeName);
	/// Reads data from dat file
	static void ReadData(const char* filename,  std::unique_ptr<double[]>& _data, unsigned int& rows, unsigned int& cols );
	~C_MatlabExchange(void);
private:
	ofstream filedata;
	unsigned int lastpozindex;				//index ostatniego wpisu w offset
	unsigned long offset[MAX_ENTRY]; //offsety kolejnych wpisów
	/// Konwertuje string typeid na typ dataType
	dataType decodeType(std::string _type, dim _dim);
};

/**
* \brief Adds one dimmensional data to dump file
* \details Adds data on the end of file and then add entry in offsets table and modifies number of entries on the
* beginig of the file. Supports only basic data types defined in \c dataType
* \param[in] data pointer to data to be dumped to file
* \param[in] size size of the data pointed by \c data
* \param[in] name name of the entry assigned later in matlab
* \param[in] typeName name of the type according to dataType 
* \exception ios_base::badbit - on disk read error etc
* \exception ios_base::failbit - wrong conversion etc
* \exception std::logic_error - when MAX_ENTRY reached or unsuppoerted type of data
* \see dataType
* \ref ImportDumpFile.m
*/
template<typename T>
void C_MatlabExchange::AddEntry1D(const T* data,unsigned int size, const char* name, const char* typeName )
{
	dataType type;
	unsigned long ile;	// number of writen bytes
	unsigned int sl;	// length of data name

	if(lastpozindex >= MAX_ENTRY)
		throw std::logic_error("Maximal entry number reached");

	type = decodeType(typeName, dim::d1D);
	sl = static_cast<unsigned int>(strlen(name));
	// we know type, now start to write data to disk
	unsigned int itype = static_cast<unsigned int>(type);
	streamoff start = filedata.tellp(); // current position in file
	filedata.write((char*)&itype, sizeof(unsigned int)); // type of data
	filedata.write((char*)&size, sizeof(unsigned int)); // number of elements in writen table
	filedata.write((char*)data, sizeof(T)*size);	// table with data
	filedata.write((char*)&sl, sizeof(unsigned int));	// length of variable name
	filedata.write(name, sizeof(char)*sl);				// name of variable saved as binary
	// how many bytes has been written
	ile = static_cast<unsigned long>(filedata.tellp() - start);
	offset[lastpozindex] = ile; //ilosc zapisanych bajtów
	lastpozindex++;
}

/**
* \brief Adds two dimmensional data to dump file
* \details Adds data on the end of file and then add entry in offsets table and modifies number of entries on the
* beginig of the file. Supports only basic data types defined in \c dataType. Supports 2D data defined as 1D array with
* rows and cols. Array is row ordered
* \param[in] data pointer to data to be dumped to file
* \param[in] rows number of rows of data pointed by \c data
* \param[in] cols number of columns of data
* \param[in] name name of the entry assigned later in matlab
* \param[in] typeName name of the type according to dataType
* \exception ios_base::badbit - on disk read error etc
* \exception ios_base::failbit - wrong conversion etc
* \exception std::logic_error - when MAX_ENTRY reached or unsuppoerted type of data
* \see dataType
* \warning Do not check if size_of_array == rows*cols, assumes that it is true
*/
template<typename T>
void C_MatlabExchange::AddEntry2D(const T* data,unsigned int rows, unsigned int cols, const char* name, const char* typeName)
{
	dataType type;
	unsigned long ile;	// number of writen bytes
	unsigned int sl;	// length of data name

	if(lastpozindex >= MAX_ENTRY)
		throw std::logic_error("Maximal entry number reached");

	type = decodeType(typeName, dim::d2D);
	sl = static_cast<unsigned int>(strlen(name));
	// we know type, now start to write data to disk
	unsigned int itype = static_cast<unsigned int>(type);
	streamoff start = filedata.tellp(); // current position in file
	filedata.write((char*)&itype, sizeof(unsigned int)); // type of data
	filedata.write((char*)&rows, sizeof(unsigned int)); // number of elements in writen table
	filedata.write((char*)&cols, sizeof(unsigned int)); // number of elements in writen table
	filedata.write((char*)data, sizeof(T)*rows*cols);	// table with data
	filedata.write((char*)&sl, sizeof(unsigned int));	// length of variable name
	filedata.write(name, sizeof(char)*sl);				// name of variable saved as binary
	// how many bytes has been written
	ile = static_cast<unsigned long>(filedata.tellp() - start);
	offset[lastpozindex] = ile; //ilosc zapisanych bajtów
	lastpozindex++;
}
#endif