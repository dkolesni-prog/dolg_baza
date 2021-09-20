//
// Created by Daniil Kolesnik on 14/09/2021.
//

#include "Database.h"
#include <fstream>

void Database::load()
{
    if (_name.empty())
    {
        std::cout << "error: invalid db name" << std::endl;
    }
    else if (std::ifstream stream("dbs/" + _name + ".txt"); !stream)
    {
        std::cout << "error: couldn't load file \"" << _name << "\"" << std::endl;
    }
    else
    {
        while (stream)
        {
            auto c = stream.get();
            if (stream.eof())
            {
                break;
            }
            Faculty* f = Faculty::create(c);
            if (!f)
            {
                std::cout << "error: unexpected faculty type: " << c << std::endl;
                _clear();
                return;
            }
            _records.push_back(f);
            f->deserialize(stream);
            stream.get(); // \n
        }
        _loaded = true;
        std::cout << "db \"" << _name << "\" loaded" << std::endl;
    }
}

void Database::save()
{
    if (_name.empty())
    {
        std::cout << "error: invalid db name" << std::endl;
    }
    else
    {
        std::ofstream stream("dbs/" + _name + ".db");
        if (!stream)
        {
            std::cout << "error: couldn't load file \"" << _name << "\"" << std::endl;
        }
        for (auto& faculty : _records)
        {
            stream << faculty->getId();
            faculty->serialize(stream);
        }
        std::cout << "db " << _name << " saved successfully" << std::endl;
    }
}
