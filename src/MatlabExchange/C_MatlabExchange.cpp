/**
* \file C_MatlabExchange.cpp
* \brief Class definition file for C_Dump class
* \author PB
* \date 2014/09/25
*/

#include "MatlabExchange/C_MatlabExchange.hpp"

/**
* \brief Main constructor
* \details Construct dump object associated with certain file
* \param[in] filename name of the data file to be created
* \exception ios_base::badbit - on disk read error etc
* \exception ios_base::failbit - wrong conversion etc
* \see C++ Programing language pp. 1080
*/
C_MatlabExchange::C_MatlabExchange(const char* filename)
{
	// setting error handling to throwing exceptions on these situations
	unsigned long datatowrite = 0;
	filedata.exceptions(filedata.exceptions() | ios_base::badbit | ios_base::failbit);
	filedata.open(filename,ios_base::binary);
	lastpozindex = 0;
	filedata.write((char*)&datatowrite,sizeof(unsigned int));	// miejsce na ilosc wpisów
	for(std::size_t a=0; a<MAX_ENTRY; ++a)
		filedata.write((char*)&datatowrite,sizeof(unsigned long)); // przesówanie wskaźnika plików
}

C_MatlabExchange::~C_MatlabExchange(void)
{
	filedata.seekp(ios::beg);		// begining of the file
	filedata.write((char*)&lastpozindex, sizeof(unsigned int));	// number of entries
	for(unsigned int a=0; a<lastpozindex; ++a)
		filedata.write((char*)&offset[a], sizeof(unsigned long));	// write table of offsets
	filedata.close();
}

/**
* \brief konwertuje łańcuch z typem zmennej na typ numeryczny
* \details Oeracja wymagana aby obsłużyć template metody \c AddEntry tak aby mogła ona zapisać kod typu danych w kontenerze
* w sposób możliwy do odczytania przez Matlaba
* \param[in] _type typ zmiennej zwrócony przez typeid (łańcuch)
* \param[in] _dim wymiarowość tablicy (1D lub 2D)
* \code{cpp}
* // decode type for 1D array
* type = decodeType(typeid(data).name(), dim::d1D);
* \endcode
* \return kod typu danych
* \retval dataType
* \exception std::logic_error - when MAX_ENTRY reached or unsuppoerted type of data
* \see dataType
*/
dataType C_MatlabExchange::decodeType(std::string _type, dim _dim)
{
	dataType type = dataType::UNSUPPORTED;  // type of data to write
	std::string name_of_type(_type);	// copy name of T to string.
	// mapping known types
	switch(_dim)
	{
	case dim::d1D:
		if(name_of_type.find("float")!=string::npos)	// it is float
			type = dataType::FLOAT;
		if(name_of_type.find("double")!=string::npos)	// it is double
			type = dataType::DOUBLE;
		if(name_of_type.find("ushort")!=string::npos)
			type = dataType::USHORT;
		break;
	case dim::d2D:
		if(name_of_type.find("float")!=string::npos)	// it is float
			type = dataType::FLOAT2D;
		if(name_of_type.find("double")!=string::npos)	// it is double
			type = dataType::DOUBLE2D;
		if(name_of_type.find("ushort")!=string::npos)
			type = dataType::USHORT2D;
		break;
	default:
		throw std::logic_error("Wrong dimmension");
	}
	if(type==dataType::UNSUPPORTED)
		throw std::logic_error("Unknown Type " + name_of_type);
	return type;
}

/**
* \brief Wczytuje plik z danumi \c dat stworzony w Matlabie
* \details Procedura allokuje pamięć potrzebną na przechowanie danych, funkcja wywołująca \c ReadData jest odpowiedzialna
* za zwonienie tej pamięci. Zwrcane są także wymiary macierzy. Format pliku:
* [UINT32 rows][UINT32 cols][double*(rows*cols) data]
* \param[in] filename nazwa pliku z danymi do wczytania
* \param[out] _data wskaźnik na zaalokowaną pamięć z danymi
* \param[out] rows liczba rzędów
* \param[out] cols liczba kolumn
* \retval void
* \exception std::logic_error - w przypadku problemu z odczytem pliku
* \exception ios_base::badbit - on disk read error etc
* \exception ios_base::failbit - wrong conversion etc
* \remarks Domyślnie obsługuje tylko typ \c double
* \ref ExportArrayToFile.m
*/
void C_MatlabExchange::ReadData(const char* filename, std::unique_ptr<double[]>& _data, unsigned int& rows, unsigned int& cols)
{
	std::ifstream input;
	input.exceptions(input.exceptions() | ios_base::badbit | ios_base::failbit);
	input.open(filename, ios_base::binary);
	// read rows and cols
	input.read((char*)&rows, sizeof(unsigned int));
	input.read((char*)&cols, sizeof(unsigned int));
	// allocate buffer
	double* data = new double[rows*cols];
	// read data
	input.read((char*)data, sizeof(double)*rows*cols);
	// make buffer unique
	std::unique_ptr<double[]> u_data(data);
	// move ownership to outside
	_data = std::move(u_data);
}