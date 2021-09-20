//
// Created by Daniil Kolesnik on 14/09/2021.
//

#include "faculty.h"

Faculty* Faculty::create(int c)
{
    if (c == BaseFaculty::id())
    {
        return new BaseFaculty();
    }
    else if (c == BranchFaculty::id())
    {
        return new BranchFaculty();
    }
    else
    {
        return nullptr;
    }
}

std::string _getStr(std::istream& stream, char delimiter /*= '\n'*/)
{
    std::string str;
    getline(stream, str, delimiter);
    return str;
}
