#include <QTest>
//#include "test_instructions.h"
//#include "testparser.h"
#include "testassembler.h"

int main(int argc, char *argv[])
{
    int status = 0;
    // {
    //     TestInstructions t1;
    //     status |= QTest::qExec(&t1, argc, argv);
    // }
    // {
    //     TestParser t2;
    //     status |= QTest::qExec(&t2, argc, argv);
    // }
    {
        TestAssembler t3;
        status |= QTest::qExec(&t3, argc, argv);
    }
    return status;
}
