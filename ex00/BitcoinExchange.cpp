/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BitcoinExchange.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jerperez <jerperez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/03 11:15:50 by jerperez          #+#    #+#             */
/*   Updated: 2024/05/03 11:51:36 by jerperez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "BitcoinExchange.hpp"
#include <iostream>

#define	BE_TIMEFORMAT "%Y_%m_%d"
#define	BE_EINFO "Error: "
#define	BE_EOPEN "could not open file."
#define	BE_ENEG "not a postitive number."
#define	BE_EINPUTDATE "invalid date."
#define	BE_EINPUTLARGE "too large a number."

BitcoinExchange::BitcoinExchange(void) {};
BitcoinExchange::~BitcoinExchange(void) {};

BitcoinExchange::BitcoinExchange(t_file dbrates, t_file intputvalues)
{
	this->_dbrates = dbrates;
	this->_intputvalues = intputvalues;
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

bool	BitcoinExchange::_parseDate(std::string date, tm* time)
{
	return (strptime(date.c_str(), BE_TIMEFORMAT, time));
}

void	BitcoinExchange::_error(std::string msg)
{
	std::cerr << BE_EINFO << msg << std::endl;
}

bool	BitcoinExchange::displayPrices(void)
{
	return (true);
}

	// 	typedef	struct		_t_date;
	// 	typedef	struct		_t_scalar;
	// 	BitcoinExchange(void);
	// 	t_file				_dbrates;
	// 	t_file				_intputvalues;
	// 	_t_date				_parseDate(std::string);
	// 	_t_scalar			_parseValue(std::string);
	// 	_t_scalar			_parseRate(std::string);
	// 	_t_scalar			_getPrice(_t_scalar, _t_scalar);
	// 	void				_error(std::string);
	// public:
	// 	BitcoinExchange(t_file, t_file);
	// 	~BitcoinExchange(void);
	// 	BitcoinExchange(const BitcoinExchange&);
	// 	BitcoinExchange	&operator=(const BitcoinExchange&);
	// 	bool	displayPrices(void);