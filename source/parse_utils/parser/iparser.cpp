/******************************************************************************
 *  Copyright (C) 2001  Michael D. Lowis
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *****************************************************************************/
/******************************************************************************
 * Includes and Prototypes
 *****************************************************************************/
#include <stdio.h>
#include "iparser.h"
#include "cork.h"

using namespace std;

/******************************************************************************
 * Public Functions
 *****************************************************************************/
IParser::IParser() : input(NULL)
{
}

IParser::~IParser()
{
    if(input != NULL)
    {
        delete input;
    }
}

void IParser::setInput(char* in)
{
    input = _new istringstream( string( in ) );
}

void IParser::setInput(string& in)
{
    input = _new istringstream( in );
}

void IParser::setInput(istream* in)
{
    input = in;
}


