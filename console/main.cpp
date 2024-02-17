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
            t.first = 6;
            id = d->AddObject(t);
            delete d;
        }
        sleep(1);
        {
            database<test> d("test");

            auto objects = d.GetObjects();
            assertm(objects.size() > 0, "Somehow the object didn't save?");
            std::cout << "objects[0]: " << objects[0] << std::endl;
            auto obj = objects[0];
            obj.second = 1;
            d.EditObject(2, obj);
            objects = d.GetObjects();
            assertm(objects[1].second == 1, "The database didn't update correctly");
            for (int i = 0; i < objects.size(); i++)
            {
                std::cout << "objects[" << i << "]:" << objects[i] << std::endl;
            }
            obj = d.GetObject(2);
            assertm(obj.second == 1, "GetObject(1) seems borked");
            d.DeleteObject(1);
            d.DeleteObject(2);
        }
    }
    catch (std::exception &e)
    {
        std::cout << e.what() << std::endl;
    }
}
