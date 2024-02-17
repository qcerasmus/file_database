#include <cassert>
#include <fstream>
#include <iostream>

#include "database.h"
#include "test_struct.h"

#define assertm(exp, msg) assert(((void)msg, exp))
int main()
{
    try
    {
        {
            database<test> *d = new database<test>("test");

            test t;
            t.first = 5;
            t.second = 10;
            for (int i = 0; i < 10; i++)
                t.test_string[i] = 'a' + i;

            auto id = d->AddObject(t);
            assertm(id == 1, "This has to be id 1");
            delete d;
        }
        sleep(1);
        {
            database<test> d("test");

            auto objects = d.GetObjects();
            assertm(objects.size() == 1, "Somehow the object didn't save?");
            std::cout << "objects[0]->first = " << objects[0].first << " objects[0].second = " << objects[0].second << " objects[0].test_string = " << objects[0].test_string
                      << std::endl;
            auto obj = objects[0];
            obj.second = 1;
            d.EditObject(1, obj);
            objects = d.GetObjects();
            assertm(objects[0].second == 1, "The database didn't update correctly");
            std::cout << "objects[0]->first = " << objects[0].first << " objects[0].second = " << objects[0].second << " objects[0].test_string = " << objects[0].test_string
                      << std::endl;
            obj = d.GetObject(1);
            assertm(objects[0].second == 1, "The database didn't update correctly");
            std::cout << "objects[0]->first = " << objects[0].first << " objects[0].second = " << objects[0].second << " objects[0].test_string = " << objects[0].test_string
                      << std::endl;
            d.DeleteObject(1);
        }
    }
    catch (std::exception &e)
    {
        std::cout << e.what() << std::endl;
    }
}
