/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BitcoinExchange.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jerperez <jerperez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/03 11:15:50 by jerperez          #+#    #+#             */
/*   Updated: 2024/05/03 17:51:48 by jerperez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "BitcoinExchange.hpp"
#include <iostream>
#include <fstream>
#include <exception>
#include <sstream>
#include <algorithm>
#include <iomanip>

#define	BE_TIMEFORMAT "%Y-%m-%d"
#define	BE_CSVHEADER "date,exchange_rate"
#define	BE_INHEADER "date | value"
#define	BE_EINFO "Error: "
#define	BE_EOPEN "could not open file."
#define	BE_ENEG "not a postitive number."
#define	BE_ELINE "not a valid line."
#define	BE_EVALUE "not a valid value."
#define	BE_EINPUTDATE "invalid date."
#define	BE_EINPUTLARGE "too large a number."
#define	BE_EHEADER "invalid header."
#define	BE_DBLEN 1613
#define	BE_DBDELIMITER ","
#define	BE_INDELIMITER " | "
#define	BE_FRACDEL "."

/* Exceptions
 *
 */

const char*	BitcoinExchange::CSVLine::InvalidLineException::what(void) const throw()
{
	return (BE_ELINE);
}

const char*	BitcoinExchange::CSVLine::InvalidDateException::what(void) const throw()
{
	return (BE_EINPUTDATE);
}

const char*	BitcoinExchange::CSVLine::InvalidValueException::what(void) const throw()
{
	return (BE_EVALUE);
}

/* CSVLine Canon
 *
 */

BitcoinExchange::CSVLine::CSVLine(void) : _value_int(0), _value_float(0) {}

BitcoinExchange::CSVLine::~CSVLine(void) {}

BitcoinExchange::CSVLine::CSVLine(const CSVLine& other)
{
	*this = other;
}

BitcoinExchange::CSVLine&	BitcoinExchange::CSVLine::operator=(const CSVLine& other)
{
	if (&other != this)
	{
		this->_time = other._time;
		this->_value_float = other._value_float;
		this->_value_int = other._value_int;
	}
	return (*this);
}


/* CSVLine Constructor
 *
 */

BitcoinExchange::CSVLine::CSVLine(std::string line, std::string delimiter)
{
	const size_t	pos = line.find(delimiter);

	if (std::string::npos == pos || line.length() == pos)
		throw InvalidLineException();
	if (false == this->parseDate(line.substr(0, pos)))
		throw InvalidDateException();
	if (false == this->parseRate(line.substr(pos + delimiter.length())))
		throw InvalidValueException();
}

/* CSVLine Parsing
 *
 */

bool	BitcoinExchange::CSVLine::parseDate(std::string date)
{
	return (strptime(date.c_str(), BE_TIMEFORMAT, &(this->_time)));
}

bool	BitcoinExchange::CSVLine::parseRate(std::string rate)
{
	const size_t	pos = rate.find(BE_FRACDEL);

	if (std::string::npos == pos)
	{
		std::stringstream ss(rate);
		ss >> this->_value_int;
		this->_value_float = 0;
	}
	else
	{
		std::stringstream ssi(rate.substr(0, pos));
		ssi >> this->_value_int;
		if (pos == rate.length())
			this->_value_float = 0;
		else
		{
			std::stringstream ssf(std::string("0.") + rate.substr(pos + 1));
			ssf >> this->_value_float;
		}
	}
	return (this->_value_float >= 0 && this->_value_int >= 0);
}



std::string	BitcoinExchange::CSVLine::getDate(void) const
{
	std::stringstream ss;

	ss 	<< (1900 + this->_time.tm_year) << "-"
		<< std::setw(2) << std::setfill('0') << (1 + this->_time.tm_mon) << "-"
		<< std::setw(2) << std::setfill('0') << (this->_time.tm_mday);
	return (ss.str());
}

std::string	BitcoinExchange::CSVLine::getValue(void) const
{
	std::stringstream	ssi;
	std::stringstream	ssf;
	std::string			sf;

	int		i;
	float	f;

	i = this->_value_int;
	f = this->_value_float;
	if (1 <= f)
	{
		i += (int)(this->_value_float);
		f -= (int)(this->_value_float);
	}
	ssi << i;
	if (0 < f)
	{
		ssf << f;
		sf = ssf.str();
		sf = sf.substr(sf.find(BE_FRACDEL));
	}
	else
		sf = "";
	return (ssi.str() + sf);
}

BitcoinExchange::CSVLine	BitcoinExchange::CSVLine::operator*(const CSVLine& other)
{
	CSVLine res;

	res._value_int = this->_value_int * other._value_int;
	res._value_float = this->_value_float * other._value_float \
		+ this->_value_float * other._value_int\
		+ other._value_float * this->_value_int;
	return (res);
}

bool	BitcoinExchange::CSVLine::datecomp::operator()(const CSVLine& c0, const CSVLine& c1) const
{
	if (c0._time.tm_year != c1._time.tm_year)
	{
		return (c0._time.tm_year < c1._time.tm_year);
	}
	if (c0._time.tm_mon != c1._time.tm_mon)
	{
		return (c0._time.tm_mon < c1._time.tm_mon);
	}
	return (c0._time.tm_mday < c1._time.tm_mday);
	return (false);
}

bool	BitcoinExchange::CSVLine::operator<(int n)
{
	if (0 == this->_value_float && 0 != this->_value_int)
		return (this->_value_int < n);
	return (this->_value_float < n);
}

bool	BitcoinExchange::CSVLine::operator>(int n)
{
	if (0 == this->_value_float && 0 != this->_value_int)
		return (this->_value_int > n);
	return (this->_value_float > n);
}

BitcoinExchange::BitcoinExchange(void) {}
BitcoinExchange::~BitcoinExchange(void) {}

BitcoinExchange::BitcoinExchange(t_file dbrates, t_file intputvalues)
{
	this->_dbrates = dbrates;
	this->_csv = this->_parseCSV(BE_DBDELIMITER);
	this->_intputvalues = intputvalues;
}

std::vector<BitcoinExchange::CSVLine>	BitcoinExchange::_parseCSV(std::string delimiter)
{
	std::vector<CSVLine>	csv;
	std::string				line;
	std::ifstream			file(this->_dbrates.c_str());

	csv.reserve(BE_DBLEN);
	if (false == file.is_open())
	{
		this->_error(BE_EOPEN);
		return (csv);
	}
	if (!std::getline(file, line) || std::string(BE_CSVHEADER) != line)
	{
		this->_error(BE_EHEADER);
		return (csv);
	}
	while (std::getline(file, line))
	{
		CSVLine csvline(line, delimiter);
		this->_csv.push_back(csvline);
	}
	std::sort(this->_csv.begin(), this->_csv.end(), CSVLine::datecomp());
	return (csv);
}

BitcoinExchange::BitcoinExchange(const BitcoinExchange& other)
{
	*this = other;
}

BitcoinExchange&	BitcoinExchange::operator=(const BitcoinExchange& other)
{
	if (&other != this)
	{
		this->_dbrates = other._dbrates;
		this->_intputvalues = other._intputvalues;
	}
	return (*this);
}

// bool	BitcoinExchange::parseDate(std::string date, tm* time)
// {
// 	return (strptime(date.c_str(), BE_TIMEFORMAT, time));
// }

void	BitcoinExchange::_error(std::string msg)
{
	std::cerr << BE_EINFO << msg << std::endl;
}

bool	BitcoinExchange::displayPrices(void)
{
	if (false == this->_parseInput())
		return (false);
	return (true);
}

void	BitcoinExchange::_displayInfo(CSVLine input)
{
	std::vector<CSVLine>::iterator itb = this->_csv.begin();
	std::vector<CSVLine>::iterator ite = this->_csv.end();
	std::cout	<< input.getDate() << " => " << input.getValue()
				<< " = " << std::endl; //
	for (std::vector<CSVLine>::iterator it = itb; it != ite; ++it)
	{
		;
		// if ((*it).datecomp(*it, input))
		// 	;
	}
}

bool	BitcoinExchange::_parseInput(void)
{
	std::string		line;
	std::ifstream	file(this->_intputvalues.c_str());

	if (false == file.is_open())
	{
		this->_error(BE_EOPEN);
		return (false);
	}
	if (!std::getline(file, line) || std::string(BE_INHEADER) != line)
	{
		this->_error(BE_EHEADER);
		return (false);
	}
	while (std::getline(file, line))
	{
		try
		{
			CSVLine csvline(line, BE_INDELIMITER);
			if (csvline < 0)
				this->_error(BE_ENEG);
			else if (csvline > 1000)
				this->_error(BE_EINPUTLARGE);
			else
				_displayInfo(csvline);
		}
		catch(const std::exception& e)
		{
			this->_error(e.what());
		}
	}
	return (true);
}

	// 	typedef	struct		_t_date;
	// 	typedef	struct		_t_scalar;
	// 	BitcoinExchange(void);
	// 	t_file				_dbrates;
	// 	t_file				_intputvalues;
	// 	_t_date				parseDate(std::string);
	// 	_t_scalar			_parseValue(std::string);
	// 	_t_scalar			parseRate(std::string);
	// 	_t_scalar			_getPrice(_t_scalar, _t_scalar);
	// 	void				_error(std::string);
	// public:
	// 	BitcoinExchange(t_file, t_file);
	// 	~BitcoinExchange(void);
	// 	BitcoinExchange(const BitcoinExchange&);
	// 	BitcoinExchange	&operator=(const BitcoinExchange&);
	// 	bool	displayPrices(void);