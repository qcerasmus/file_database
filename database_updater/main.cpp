#include <iostream>
#include "test_struct.h"
#include "updater.h"

void PRINT_HELP()
{
    std::cout << "Usage: database_updater {filename} {update_description} {output_filename}\n";
}

int main(int argc, char **argv)
{
    /* if (argc != 4)
    {
        PRINT_HELP();
        return 0;
    }

    std::string filename = argv[1];
    std::string update_filename = argv[2];
    std::string output_filename = argv[3];
    */
    updater<test> asdf;
    asdf.update("test.dat", "updates.json", "new.dat");
    std::cout << asdf.m_OldStructSize << "->" << asdf.m_NewStructSize << '\n';
}
