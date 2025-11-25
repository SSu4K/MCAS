#include <QTest>
#include "testinstructions.h"
//#include "testparser.h"
#include "testassembler.h"
#include "testdisassembler.h"

int main(int argc, char *argv[])
{
    int status = 0;
    {
        TestInstructions t1;
        status |= QTest::qExec(&t1, argc, argv);
    }
    {
        TestAssembler t3;
        status |= QTest::qExec(&t3, argc, argv);
    }
    {
        TestDisassembler t4;
        status |= QTest::qExec(&t4, argc, argv);
    }
    return status;
}
